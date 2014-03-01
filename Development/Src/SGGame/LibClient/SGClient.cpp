#include <ZionBase.h>
#include <ZionClient.h>
#include <ZionCommon.h>

#include <time.h>
#include "SGCommon.h"
#include "SGClient.h"
#include "SGSyncDataManager.h"
#include "SGClientUtil.h"

#include "SGActionStatusCache.h"
#include "SGClientTipsHelper.h"

namespace Zion
{

	CSGClient::CSGClient(CClientApp* pClientApp, _U32 recvsize) : CClient(pClientApp, recvsize), c2s(this), s2c(this)
	{
	}	

	CSGClient::~CSGClient()
	{

	}

	void CSGClient::OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		if(iid==GetServerStubID<SGGAME_C2S>())
		{
			DDL::MemoryReader buf(data, len);
			s2c.Dispatcher(this, fid, buf);
		}
	}

}
