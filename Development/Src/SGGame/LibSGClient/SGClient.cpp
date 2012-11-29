#include <AtlasBase.h>
#include <AtlasClient.h>
#include <AtlasCommon.h>

#include "SGCommon.h"
#include "SGClient.h"

namespace Atlas
{
	
	CSGClient::CSGClient(CClientApp* pClientApp, _U32 recvsize) : CClient(pClientApp, recvsize)
	{
	}

	CSGClient::~CSGClient()
	{
	}

}
