#ifndef __CARDGAME_CLIENT__
#define __CARDGAME_CLIENT__

#include <CardGameDDL.h>

namespace Zion
{

	class CCardClient : public CClient
	{
	public:
		CCardClient(CClientApp* pClientApp, _U32 recvsize=6*1024);
		virtual ~CCardClient();

		#include "CardClient_AutoGen.h"

		DDLProxy::CARDGAME_C2S<CCardClient, DDL::TMemoryWriter<1024>> c2s;
	};

}

#endif
