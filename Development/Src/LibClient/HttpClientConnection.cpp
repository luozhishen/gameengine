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
					}
					else
					{
						if(!root.isMember("session_key"))
						{
							SetErrorCode(CClient::ERRCODE_UNKOWN);
							m_nState = CClient::STATE_FAILED;
						}
						else
						{
							Json::Value session_key = root["session_key"];
							if(!session_key.isString())
							{
								SetErrorCode(CClient::ERRCODE_UNKOWN);
								m_nState = CClient::STATE_FAILED;
							}
							else
							{
								m_SessionKey = session_key.asString();
								m_nState = CClient::STATE_LOGINED;
							}
						}
					}
				}
			}
			if(MORequestStatus(m_pLoginRequest)==MOREQUESTSTATE_FAILED)
			{
				ATLAS_ASSERT(m_nState==CClient::STATE_LOGINING);
				SetErrorCode(CClient::ERRCODE_NETWORK);
				m_nState = CClient::STATE_FAILED;
			}
			MORequestDestory(m_pLoginRequest);
			m_pLoginRequest = NULL;

			if(m_nState==CClient::STATE_FAILED) GetClient()->GetClientApp()->QueueLoginFailed(GetClient());
			if(m_nState==CClient::STATE_LOGINED) GetClient()->GetClientApp()->QueueLoginDone(GetClient());
		}

		if(m_pNotifyRequest)
		{
		}

		if(m_pCurrentRequest && MORequestStatus(m_pCurrentRequest)!=MOREQUESTSTATE_PENDING)
		{
			if(MORequestStatus(m_pCurrentRequest)==MOREQUESTSTATE_DONE)
			{
				m_SendQueue.pop_front();

				int code = MOClientGetResultCode(m_pCurrentRequest);
				if(code==MOERROR_NOERROR)
				{
					const char* result = MOClientGetResultString(m_pCurrentRequest);
					int i = 0;
				}
				else
				{
				}
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
