#include <AtlasBase.h>
#include <AtlasCommon.h>
#include <AtlasClient.h>
#include "ExLobbyLoginCase.h"
#include <StressClient.h>

		//_OnLoginDone;
		//_OnLoginFailed;
		//_OnDisconnected;

namespace Atlas
{
	CExLobbyLoginCase::CExLobbyLoginCase() : CStressCase("ExLobbyLoginCase")
	{
		m_nSuccTimes = 0;
		m_nFailedTimes = 0;
	}

	CExLobbyLoginCase::~CExLobbyLoginCase()
	{
	}

	void CExLobbyLoginCase::OnAttach()
	{
		GetClient()->_OnConnectFailed.connect(this, &CExLobbyLoginCase::OnConnectFailed);
		GetClient()->_OnConnect.connect(this, &CExLobbyLoginCase::OnConnect);
		GetClient()->_OnLoginDone.connect(this, &CExLobbyLoginCase::OnLoginDone);
		GetClient()->_OnDisconnect.connect(this, &CExLobbyLoginCase::OnDisconnect);
	}

	void CExLobbyLoginCase::OnTick()
	{
		switch(GetClient()->GetClientState())
		{
		case Atlas::CClient::CLIENT_NA:
				GetStressClient()->Login();
			break;
		case Atlas::CClient::CLIENT_LOGINING:
			break;
		case Atlas::CClient::CLIENT_LOGINED:
				GetStressClient()->Logout();
			break;
		case Atlas::CClient::CLIENT_AUTH_FAILED:
			{
				OutputDebugString("Auth_failed\n");
				GetStressClient()->Login();
			}
			break;
		}
	}

	void CExLobbyLoginCase::OnDetach()
	{

	}

	void CExLobbyLoginCase::OnConnectFailed()
	{
		m_nFailedTimes++;
	}

	void CExLobbyLoginCase::OnConnect()
	{	
		m_nSuccTimes++;
	}

	void CExLobbyLoginCase::OnLoginDone()
	{
	}

	void CExLobbyLoginCase::OnDisconnect()
	{
	}

	CStressCase* CExLobbyLoginCase::Create()
	{
		return new CExLobbyLoginCase();
	}

	void CExLobbyLoginCase::_GetInfo(std::string& info)
	{
		char szInfo[100];
		sprintf(szInfo, "%s\t succ %u | failed %u", GetName().c_str(), m_nSuccTimes, m_nFailedTimes);
		info = szInfo;
	}

}