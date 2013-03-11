#include <AtlasBase.h>
#include <AtlasClient.h>
#include <AtlasCommon.h>

#include "KnightCommon.h"
#include "KnightClient.h"
#include <time.h>

namespace Atlas
{

	CKnightClient::CKnightClient(CClientApp* pClientApp, _U32 recvsize) : CClient(pClientApp, recvsize)
	{
	}	

	CKnightClient::~CKnightClient()
	{

	}

	CKnightClientCallback::~CKnightClientCallback()
	{
	}
}
