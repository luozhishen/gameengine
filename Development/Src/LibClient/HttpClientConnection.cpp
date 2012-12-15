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
	}

	CHttpClientConnection::~CHttpClientConnection()
	{
		ATLAS_ASSERT(!m_pLoginRequest);
	}

	void CHttpClientConnection::Tick()
	{
	}

	bool CHttpClientConnection::Login(const SOCK_ADDR& sa, _U32 nUID, const char* pToken)
	{
		return false;
	}

	void CHttpClientConnection::Logout()
	{
	}

	void CHttpClientConnection::SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		const DDLReflect::CLASS_INFO* classinfo;
		bool bRet = GetClientFunctionStub(iid, fid, classinfo);
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
