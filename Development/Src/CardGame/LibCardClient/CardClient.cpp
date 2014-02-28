#include <ZionBase.h>
#include <ZionClient.h>
#include <ZionCommon.h>

#include <time.h>
#include "CardCommon.h"
#include "CardClient.h"

namespace Zion
{

	CCardClient::CCardClient(CClientApp* pClientApp, _U32 recvsize) : CClient(pClientApp, recvsize), c2s(this)
	{
		RegisterStub(new DDLStub::CARDGAME_S2C<CCardClient>(this));
	}	

	CCardClient::~CCardClient()
	{

	}

}
