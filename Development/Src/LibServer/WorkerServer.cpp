#include <ZionBase.h>
#include <AsyncSockIO.h>
#include <AsyncRPC.h>

#include "ServerApp.h"
#include "ServerBase.h"
#include "WorkerServer.h"

#include "ServerRpc.s.h"
#include "ServerRpc.c.h"

namespace Zion
{

	CWorkerServer::CWorkerServer(CServerApp* pServerApp) : CServerBase(pServerApp)
	{
	}

	CWorkerServer::~CWorkerServer()
	{
	}

}

void WRPC_DoRequest(Zion::HCLIENT hClient, _U64 tid, _U16 code, _U32 len, const _U8* data)
{
}
