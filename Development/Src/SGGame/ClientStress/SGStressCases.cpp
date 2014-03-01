#include <ZionBase.h>
#include <ZionCommon.h>
#include <ZionClient.h>

#include <StressCase.h>
#include <StressClient.h>

#include <SGClient.h>
#include "SGStressCases.h"

namespace Zion
{
	namespace StressCases
	{

		CEcho::CEcho() : TSGStressCase<STRESSCASE_ECHO_CONFIG, STRESSCASE_ECHO_STATUS>("Echo")
		{
		}
		
		CEcho::~CEcho()
		{
		}

		void CEcho::OnTick()
		{
			if(GetClient()->GetState()==CClient::STATE_LOGINED)
			{
				GetClient()->c2s.Ping();
			}
		}
		
		CStressCase* CEcho::Create()
		{
			return ZION_NEW CEcho();
		}

		CEnterServer::CEnterServer() : TSGStressCase<STRESSCASE_ENTERSERVER_CONFIG, STRESSCASE_ENTERSERVER_STATUS>("EnterServer")
		{
		}

		CEnterServer::~CEnterServer()
		{
		}

		void CEnterServer::OnAttach()
		{
			GetClient()->_OnLoginDone.connect(this, &CEnterServer::OnLoginDone);
			GetClient()->_OnGetServerListResult.connect(this, &CEnterServer::OnGetServerListResult);
		}

		void CEnterServer::OnDetach()
		{
			GetClient()->_OnLoginDone.disconnect(this);
			GetClient()->_OnGetServerListResult.disconnect(this);
		}

		void CEnterServer::OnTick()
		{
		}

		void CEnterServer::OnLoginDone()
		{
			GetClient()->c2s.GetServerList();
		}

		void CEnterServer::OnGetServerListResult(const SG_SERVER_INFO* infos, _U32 count, _U32 last_server)
		{
			if(count==0)
			{
				CLIENT_LOG(GetClient(), "server list return empty");
				GetClient()->Logout();
				return;
			}

			GetClient()->c2s.EnterServer(infos[0].server_id);
			GetClient()->c2s.QueryAvatar();
		}

		void CEnterServer::OnQueryAvatarFailed(_U32 errcode)
		{
			GetClient()->c2s.CreateAvatar("aaa", 10);
			GetClient()->c2s.QueryAvatar();
		}

		void CEnterServer::OnQueryAvatarResult(const SG_PLAYER& player)
		{
			GetClient()->c2s.EnterGame();
		}

		CStressCase* CEnterServer::Create()
		{
			return ZION_NEW CEnterServer();
		}

	}
}
