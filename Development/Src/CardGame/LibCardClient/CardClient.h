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

		virtual void OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data);

		#include "CardClient_AutoGen.h"

		DDLProxy::CARDGAME_C2S<CCardClient, DDL::TMemoryWriter<1024>> c2s;
		DDLStub::CARDGAME_S2C<CCardClient, CCardClient> s2c;
	};

}

#endif
