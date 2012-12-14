#include <AtlasBase.h>
#include "AtlasClient.h"
#include "AtlasClientApp.h"
#include "HttpClientConnection.h"

namespace Atlas
{

	CHttpClientConnection::CHttpClientConnection(CClient* pClient) : CClientConnectionBase(pClient)
	{
	}

	CHttpClientConnection::~CHttpClientConnection()
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
	}

}
