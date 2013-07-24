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

	CSGClient::CSGClient(CClientApp* pClientApp, _U32 recvsize) : CClient(pClientApp, recvsize)
	{
	}	

	CSGClient::~CSGClient()
	{

	}

}
