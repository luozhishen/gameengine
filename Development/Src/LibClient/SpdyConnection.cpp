#include <ZionBase.h>
#include "ZionClient.h"
#include "ZionClientApp.h"
#include "ClientConnection.h"
#include "SpdyConnection.h"

namespace Zion
{

	CSpdyConnection::CSpdyConnection(CClient* pClient) : CClientConnectionBase(pClient)
	{
	}

	CSpdyConnection::~CSpdyConnection()
	{
	}

}