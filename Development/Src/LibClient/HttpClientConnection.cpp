#include <AtlasBase.h>
#include "AtlasCommon.h"
#include "AtlasClient.h"
#include "AtlasClientApp.h"
#include "HttpClientConnection.h"

namespace Atlas
{

	CHttpClientConnection::CHttpClientConnection(CClient* pClient) : CClientConnectionBase(pClient)
	{
		m_pLoginRequest = NULL;
		m_pNotifyRequest = NULL;
		m_pCurrentRequest = NULL;
	}

	CHttpClientConnection::~CHttpClientConnection()
	{
		ATLAS_ASSERT(!m_pLoginRequest);
		ATLAS_ASSERT(!m_pNotifyRequest);
		ATLAS_ASSERT(!m_pCurrentRequest);
	}

	void CHttpClientConnection::Tick()
	{
		if(m_pLoginRequest)
		{
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
						}
					}
				}
			}
			if(MORequestStatus(m_pLoginRequest)==MOREQUESTSTATE_FAILED)
			{
				ATLAS_ASSERT(m_nState==CClient::STATE_LOGINING);
				SetErrorCode(CClient::ERRCODE_NETWORK);
				m_nState = CClient::STATE_FAILED;
				CLIENT_LOG(GetClient(), "http_connection : http failed");
			}
			MORequestDestory(m_pLoginRequest);
			m_pLoginRequest = NULL;

			if(m_nState==CClient::STATE_FAILED) GetClient()->GetClientApp()->QueueLoginFailed(GetClient());
			if(m_nState==CClient::STATE_LOGINED) GetClient()->GetClientApp()->QueueLoginDone(GetClient());
		}

		if(m_pCurrentRequest && MORequestStatus(m_pCurrentRequest)!=MOREQUESTSTATE_PENDING)
		{
			if(MORequestStatus(m_pCurrentRequest)==MOREQUESTSTATE_DONE)
			{
				m_SendQueue.pop_front();

				int ret = MOClientGetResultCode(m_pCurrentRequest);
				if(ret!=MOERROR_NOERROR)
				{
					CLIENT_LOG(GetClient(), "http_connection : login failed, return code = %d", ret);
				}
				else
				{
					const char* result = MOClientGetResultString(m_pCurrentRequest);
					Json::Value root;
					Json::Reader reader;
					if(!reader.parse(result, result+strlen(result), root) || !root.isMember("response") || !root["response"].isArray())
					{
						CLIENT_LOG(GetClient(), "http_connection : invalid json, %s", result);
					}
					else
					{
						Json::Value& _array = root["response"];
						Json::Value _default;
						for(Json::Value::UInt i=0; i<_array.size(); i++)
						{
							Json::Value& elm = _array[i];
							bool error = false;

							if(!elm.isObject()) error = true;
							if(!elm.isMember("method") || !elm["method"].isString()) error = true;
							if(!elm.isMember("message") || !elm["message"].isObject()) error = true;
							if(error)
							{
								Json::FastWriter writer;
								std::string json = writer.write(elm);
								CLIENT_LOG(GetClient(), "http_connection : invalid data, (%d) %s", json.c_str());
								continue;
							}

							const DDLReflect::CLASS_INFO* cls;
							_U16 fid;
							if(!GetClientFunctionStub(elm["method"].asCString(), cls, fid))
							{
								CLIENT_LOG(GetClient(), "http_connection : invalid method name %s", elm["method"].asCString());
								error = true;
								continue;
							}

							_U32 len = 300*1024;
							_U8* data = (_U8*)ATLAS_ALLOC(len);
							if(!DDLReflect::Json2Call(&cls->finfos[fid], elm["message"], len, data))
							{
								Json::FastWriter writer;
								std::string json = writer.write(elm["message"]);
								CLIENT_LOG(GetClient(), "http_connection : invalid method data, (%d) %s", json.c_str());
								error = true;
							}
							else
							{
								GetClient()->GetClientApp()->QueueData(GetClient(), cls->iid, fid, len, data);
							}

							ATLAS_FREE(data);
						}
					}
				}
			}
			else
			{
				CLIENT_LOG(GetClient(), "http_connection : http failed");
			}
			MORequestDestory(m_pCurrentRequest);
			m_pCurrentRequest = NULL;
		}

		if(!m_pCurrentRequest && !m_SendQueue.empty())
		{
			std::map<std::string, std::string> params;
			params["session_key"] = m_SessionKey;
			params["request"] = m_SendQueue.front();
			std::string url = StringFormat("%srequest.php", m_BaseUrl.c_str());
			m_pCurrentRequest = MORequestString(url.c_str(), params);
		}
	}

	bool CHttpClientConnection::Login(const char* pUrl, _U32 nUID, const char* pToken)
	{
		ATLAS_ASSERT(m_nState==CClient::STATE_NA || m_nState==CClient::STATE_FAILED);
		ATLAS_ASSERT(!m_pLoginRequest);
		ATLAS_ASSERT(!m_pNotifyRequest);
		ATLAS_ASSERT(!m_pCurrentRequest);
		if(m_nState!=CClient::STATE_NA && m_nState!=CClient::STATE_FAILED) return false;
		if(m_pLoginRequest) return false;
		if(m_pNotifyRequest) return false;
		if(m_pCurrentRequest) return false;

		std::map<std::string, std::string> params;
		params["token"] = pToken;
		std::string url = StringFormat("%slogin.php", pUrl);
		m_pLoginRequest = MORequestString(url.c_str(), params);
		if(!m_pLoginRequest)
		{
			m_nState = CClient::STATE_FAILED;
			m_nErrCode = CClient::ERRCODE_NETWORK;
			return false;
		}

		m_BaseUrl = pUrl;
		m_nState = CClient::STATE_LOGINING;
		m_nErrCode = CClient::ERRCODE_SUCCESSED;
		return true;
	}

	void CHttpClientConnection::Logout()
	{
		if(m_nState==CClient::STATE_LOGINED)
		{
			m_nState = CClient::STATE_NA;
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
		std::string json = writer.write(root);
		m_SendQueue.push_back(json);
	}

}
