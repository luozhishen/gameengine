#include <AtlasBase.h>
#include "AtlasCommon.h"
#include "AtlasClient.h"
#include "AtlasClientApp.h"
#include "HttpClientConnection.h"

namespace Atlas
{

	CHttpClientConnection::CHttpClientConnection(CClient* pClient) : CClientConnectionBase(pClient)
	{
		m_bPullRequest = false;
		m_pLoginRequest = NULL;
		m_pLogoutRequest = NULL;
		m_pPullRequest = NULL;
		m_pCurrentRequest = NULL;
		m_nHttpState = STATE_RUNNING;
		m_nRequestSeq = 0;
		m_nPullSeq = 0;
		m_bInLogout = false;
	}

	CHttpClientConnection::~CHttpClientConnection()
	{
		ATLAS_ASSERT(!m_pLoginRequest);
		ATLAS_ASSERT(!m_pPullRequest);
		ATLAS_ASSERT(!m_pCurrentRequest);
	}

	void CHttpClientConnection::Tick()
	{
		if(m_pLoginRequest)
		{
			ProcessLoginRequest();
			return;
		}

		if(m_nState==CClient::STATE_LOGINED)
		{
			ProcessQueueRequest();
			ProcessPullRequest();
			ProcessLogoutRequest();

			if(!m_bInLogout && !m_pCurrentRequest && !m_SendQueue.empty() && m_nHttpState!=STATE_PAUSE)
			{
				SendRequest();
			}
		}
	}

	bool CHttpClientConnection::Login(const char* pUrl, const char* pToken)
	{
		ATLAS_ASSERT(m_nState==CClient::STATE_NA || m_nState==CClient::STATE_FAILED);
		ATLAS_ASSERT(!m_pLoginRequest);
		ATLAS_ASSERT(!m_pPullRequest);
		ATLAS_ASSERT(!m_pCurrentRequest);
		if(m_nState!=CClient::STATE_NA && m_nState!=CClient::STATE_FAILED) return false;
		if(m_pLoginRequest) return false;
		if(m_pPullRequest) return false;
		if(m_pCurrentRequest) return false;

		Atlas::Map<Atlas::String, Atlas::String> params;
		params["token"] = pToken;
		String url = StringFormat(pUrl, "login");
		m_pLoginRequest = MORequestString(url.c_str(), params);
		if(!m_pLoginRequest)
		{
			m_nState = CClient::STATE_FAILED;
			m_nErrCode = CClient::ERRCODE_NETWORK;
			GetClient()->GetClientApp()->QueueLoginFailed(GetClient());
			return false;
		}

		m_BaseUrl = pUrl;
		m_nState = CClient::STATE_LOGINING;
		m_nErrCode = CClient::ERRCODE_SUCCESSED;
		m_nHttpState = STATE_RUNNING;
		m_SendQueue.clear();
		return true;
	}

	void CHttpClientConnection::Logout()
	{
		if(m_nState==CClient::STATE_LOGINED && !m_bInLogout)
		{
			m_bInLogout = true;
			m_nLogoutRetry = 1;
			String url = StringFormat(m_BaseUrl.c_str(), "logout");
			Atlas::Map<Atlas::String, Atlas::String> params;
			params["session_key"] = m_SessionKey;
			m_pLogoutRequest = MORequestString(url.c_str(), params);
		}
	}

	void CHttpClientConnection::SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		const DDLReflect::CLASS_INFO* classinfo;
		bool bRet = GetServerFunctionStub(iid, fid, classinfo);
		ATLAS_ASSERT(bRet);
		if(!bRet) return;
		Json::Value root(Json::objectValue);
		Json::Value message(Json::objectValue);
		bRet = Call2Json(classinfo->finfos+fid, len, data, message);
		ATLAS_ASSERT(bRet);
		if(!bRet) return;
		root["method"] = StringFormat("%s.%s", classinfo->name, classinfo->finfos[fid].name);
		root["message"] = message;
		Json::FastWriter writer;
		String json = writer.write(root);
		CLIENT_LOG(GetClient(), "send request : %s", json.c_str());
		m_SendQueue.push_back(json);
	}

	void CHttpClientConnection::Retry()
	{
		if(m_nHttpState==STATE_PAUSE) m_nHttpState = STATE_RETRY;
	}

	void CHttpClientConnection::Cancel()
	{
		if(m_nHttpState!=STATE_PAUSE) return;
		m_nHttpState = STATE_RETRY;
		m_SendQueue.pop_front();
	}

	void CHttpClientConnection::SetStateCallback(STATE_CALLBACK callback)
	{
		m_StateCallback = callback;
	}

	void CHttpClientConnection::ProcessLoginRequest()
	{
		ATLAS_ASSERT(m_pLoginRequest);

		if(MORequestStatus(m_pLoginRequest)==MOREQUESTSTATE_PENDING) return;
		if(MORequestStatus(m_pLoginRequest)==MOREQUESTSTATE_DONE)
		{
			int ret = MOClientGetResultCode(m_pLoginRequest);
			if(ret!=MOERROR_NOERROR)
			{
				SetErrorCode(ret==MOERROR_AUTH_FAILED?CClient::ERRCODE_AUTH_FAILED:CClient::ERRCODE_UNKOWN);
				m_nState = CClient::STATE_FAILED;
				CLIENT_LOG(GetClient(), "http_connection : login failed, return code = %d", ret);
			}
			else
			{
				const char* result = MOClientGetResultString(m_pLoginRequest);
				Json::Value root;
				Json::Reader reader;
				if(!reader.parse(result, root))
				{
					SetErrorCode(CClient::ERRCODE_UNKOWN);
					m_nState = CClient::STATE_FAILED;
					CLIENT_LOG(GetClient(), "http_connection : failed to parse json, %s", result);
				}
				else
				{
					if(!root.isMember("session_key") || !root["session_key"].isString())
					{
						SetErrorCode(CClient::ERRCODE_UNKOWN);
						m_nState = CClient::STATE_FAILED;
						CLIENT_LOG(GetClient(), "http_connection : invalid session key in response, %s", result);
					}
					else
					{
						Json::Value session_key = root["session_key"];
						m_SessionKey = session_key.asString();
						m_nState = CClient::STATE_LOGINED;
						m_nHttpState = STATE_RUNNING;
					}
				}
			}
		}
		if(MORequestStatus(m_pLoginRequest)==MOREQUESTSTATE_FAILED)
		{
			ATLAS_ASSERT(m_nState==CClient::STATE_LOGINING);
			SetErrorCode(CClient::ERRCODE_NETWORK);
			m_nState = CClient::STATE_FAILED;
			CLIENT_LOG(GetClient(), "http_connection : http failed for login");
		}
		MORequestDestory(m_pLoginRequest);
		m_pLoginRequest = NULL;

		if(m_nState==CClient::STATE_FAILED)
		{
			GetClient()->GetClientApp()->QueueLoginFailed(GetClient());
		}
		if(m_nState==CClient::STATE_LOGINED)
		{
			m_nRequestSeq = 1;
			m_nPullSeq = 1;
			GetClient()->GetClientApp()->QueueLoginDone(GetClient());
		}
	}

	void CHttpClientConnection::ProcessLogoutRequest()
	{
		if(!m_bInLogout) return;

		if(m_pLogoutRequest)
		{
			if(MORequestStatus(m_pLogoutRequest)==MOREQUESTSTATE_PENDING)
			{
				return;
			}

			if(MORequestStatus(m_pLogoutRequest)!=MOREQUESTSTATE_DONE)
			{
				MORequestDestory(m_pLogoutRequest);
				m_pLogoutRequest = NULL;
				CLIENT_LOG(GetClient(), "http_connection : http failed for logout");

				if(m_nLogoutRetry<3)
				{
					String url = StringFormat(m_BaseUrl.c_str(), "logout");
					Atlas::Map<Atlas::String, Atlas::String> params;
					params["session_key"] = m_SessionKey;
					m_pLogoutRequest = MORequestString(url.c_str(), params);
					m_nLogoutRetry++;
					return;
				}
			}
			else
			{
				MORequestDestory(m_pLogoutRequest);
				m_pLogoutRequest = NULL;
			}

		}

		if(m_pPullRequest==NULL && m_pCurrentRequest==NULL && m_pLogoutRequest==NULL)
		{
			m_nState = CClient::STATE_NA;
			m_bInLogout = false;
			m_SessionKey = "";
			m_SendQueue.clear();
			GetClient()->GetClientApp()->QueueDisconnected(GetClient());
		}
	}

	void CHttpClientConnection::ProcessQueueRequest()
	{
		if(m_pCurrentRequest && MORequestStatus(m_pCurrentRequest)!=MOREQUESTSTATE_PENDING)
		{
			if(MORequestStatus(m_pCurrentRequest)==MOREQUESTSTATE_PENDING)
			{
				return;
			}

			if(MORequestStatus(m_pCurrentRequest)==MOREQUESTSTATE_FAILED)
			{
				CLIENT_LOG(GetClient(), "http_connection : http failed");
				MORequestDestory(m_pCurrentRequest);
				m_pCurrentRequest = NULL;

				if(m_StateCallback)
				{
					m_nHttpState = STATE_PAUSE;
					m_StateCallback(m_nHttpState);
				}
				return;
			}

			ATLAS_ASSERT(MORequestStatus(m_pCurrentRequest)==MOREQUESTSTATE_DONE);

			if(ProcessRequest(m_pCurrentRequest)==MOERROR_NOERROR)
			{
				m_SendQueue.pop_front();
				m_nRequestSeq++;
				if(m_nHttpState==STATE_RETRY)
				{
					m_nHttpState = STATE_RUNNING;
					m_StateCallback(m_nHttpState);
				}
			}
			MORequestDestory(m_pCurrentRequest);
			m_pCurrentRequest = NULL;
		}
	}

	void CHttpClientConnection::ProcessPullRequest()
	{
		if(!m_pPullRequest)
		{
			if(m_bInLogout || !m_bPullRequest) return;

			Atlas::Map<Atlas::String, Atlas::String> params;
			params["session_key"] = m_SessionKey;
			params["seq"] = Atlas::StringFormat("%d", m_nPullSeq);
			String url = StringFormat(m_BaseUrl.c_str(), "pull");
			m_pPullRequest = MORequestString(url.c_str(), params);
			if(!m_pPullRequest) return;
		}

		if(MORequestStatus(m_pPullRequest)==MOREQUESTSTATE_PENDING)
		{
			return;
		}

		if(MORequestStatus(m_pPullRequest)!=MOREQUESTSTATE_DONE)
		{
			CLIENT_LOG(GetClient(), "http_connection : http failed for pull");
			MORequestDestory(m_pPullRequest);
			m_pPullRequest = NULL;
			return;
		}

		int ret = ProcessRequest(m_pPullRequest);
		MORequestDestory(m_pPullRequest);
		m_pPullRequest = NULL;
		if(ret==MOERROR_NOERROR)
		{
			m_nPullSeq++;
		}
	}

	int CHttpClientConnection::ProcessRequest(MOREQUEST* request)
	{
		int ret = MOClientGetResultCode(request);
		if(ret!=MOERROR_NOERROR)
		{
			return ret;
		}

		const char* result = MOClientGetResultString(request);
		CLIENT_LOG(GetClient(), "recv response: %s", result);
		if(*result=='\0')
		{
			return MOERROR_NOERROR;
		}

		Json::Value root;
		Json::Reader reader;
		if(!reader.parse(result, result+strlen(result), root) || !root.isMember("response") || !root["response"].isArray())
		{
			CLIENT_LOG(GetClient(), "http_connection : invalid json, %s", result);
			return MOERROR_UNKNOWN;
		}

		Json::Value& _array = root["response"];
		Json::Value _default;
		Json::Value::UInt i;
		for(i=0; i<_array.size(); i++)
		{
			Json::Value& elm = _array[i];

			if(		!elm.isObject()
				||	!elm.isMember("method") || !elm["method"].isString()
				||	!elm.isMember("message") || !elm["message"].isObject())
			{
				Json::FastWriter writer;
				String json = writer.write(elm);
				CLIENT_LOG(GetClient(), "http_connection : invalid data, (%d) %s", json.c_str());
				break;
			}

			const DDLReflect::CLASS_INFO* cls;
			_U16 fid;
			if(!GetClientFunctionStub(elm["method"].asCString(), cls, fid))
			{
				CLIENT_LOG(GetClient(), "http_connection : invalid method name %s", elm["method"].asCString());
				break;
			}

			_U32 len = 300*1024;
			_U8* data = (_U8*)ATLAS_ALLOC(len);
			if(!DDLReflect::Json2Call(&cls->finfos[fid], elm["message"], len, data))
			{
				Json::FastWriter writer;
				String json = writer.write(elm["message"]);
				CLIENT_LOG(GetClient(), "http_connection : invalid method data, (%d) %s", fid, json.c_str());
				ATLAS_FREE(data);
				break;
			}

			GetClient()->GetClientApp()->QueueData(GetClient(), cls->iid, fid, len, data);
			ATLAS_FREE(data);
		}
		return (i==_array.size())?MOERROR_NOERROR:MOERROR_UNKNOWN;
	}

	void CHttpClientConnection::SendRequest()
	{
		Atlas::Map<Atlas::String, Atlas::String> params;
		params["session_key"] = m_SessionKey;
		params["request"] = m_SendQueue.front();
		params["seq"] = Atlas::StringFormat("%d", m_nRequestSeq);
		String url = StringFormat(m_BaseUrl.c_str(), "request");
		m_pCurrentRequest = MORequestString(url.c_str(), params);
	}

}
