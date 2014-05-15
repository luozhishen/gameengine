#include <ZionBase.h>
#include <ZionCommon.h>
#include <ZionClient.h>

#include <time.h>
#include "CardCommon.h"
#include "CardClient.h"

namespace Zion
{

	CCardClient::CCardClient(CClientApp* pClientApp, _U32 recvsize) : CClient(pClientApp, recvsize), c2s_base(this), c2s_game(this)
	{
		RegisterStub(&s2c_base);
		RegisterStub(&s2c_game);
	}	

	CCardClient::~CCardClient()
	{

	}

}
