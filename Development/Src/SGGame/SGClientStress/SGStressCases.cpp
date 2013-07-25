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

		void CEnterServer::OnTick()
		{
		}

		CStressCase* CEnterServer::Create()
		{
			return ZION_NEW CEnterServer();
		}

	}
}
