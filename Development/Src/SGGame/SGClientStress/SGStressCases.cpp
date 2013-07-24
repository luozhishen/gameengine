#include <ZionBase.h>
#include <ZionCommon.h>
#include <ZionClient.h>

#include <StressCase.h>
#include <StressClient.h>

#include "SGStressCases.h"
#include <SGGameDDL.h>

namespace Zion
{
	namespace StressCases
	{

		CEcho::CEcho() : CStressCase("echo")
		{
		}
		
		CEcho::~CEcho()
		{
		}

		void CEcho::OnTick()
		{
			if(GetClient()->GetState()==CClient::STATE_LOGINED)
			{
				DDLProxy::SGGAME_C2S<CClient, DDL::TMemoryWriter<1024>> c2s(GetClient());
				c2s.Ping();
			}
		}
		
		CStressCase* CEcho::Create()
		{
			return ZION_NEW CEcho();
		}

	}
}
