#ifndef __SGGAME_CLIENT__
#define __SGGAME_CLIENT__

#include <SGGameDDL.h>

namespace Zion
{

	class CSGClient : public CClient
	{
	public:
		CSGClient(CClientApp* pClientApp, _U32 recvsize=6*1024);
		virtual ~CSGClient();

		virtual void OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data);

		#include "SGClient_AutoGen.h"

		DDLProxy::SGGAME_C2S<CSGClient, DDL::TMemoryWriter<1024>> c2s;
		DDLStub::SGGAME_S2C<CSGClient, CSGClient> s2c;
	};

}

#endif
