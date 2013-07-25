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

		DDLProxy::SGGAME_C2S<CClient, DDL::TMemoryWriter<1024>> c2s;
	};

}

#endif
