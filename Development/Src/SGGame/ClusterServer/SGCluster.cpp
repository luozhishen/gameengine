#include <ZionBase.h>
#include <ZionServer.h>

#include "SGCluster.h"
#include "SGGameDDL.h"

namespace Zion
{

	CSGClusterServer::CSGClusterServer(CServerApp* pServerApp) : CClusterServer(pServerApp)
	{
	}

	CSGClusterServer::~CSGClusterServer()
	{
	}

	CClusterClient* CSGClusterServer::NewClusterClient(_U64 nCNDX)
	{
		return ZION_NEW CSGClusterClient(this, nCNDX);
	}

	CSGClusterClient::CSGClusterClient(CClusterServer* pServer, _U64 nCNDX) : CClusterClient(pServer, nCNDX)
	{
	}

	CSGClusterClient::~CSGClusterClient()
	{
	}

	void CSGClusterClient::OnUserLogin()
	{
		//NodeSwitch(NODEID_AVATAR, GetLocalRPCServer(), 0, 0, NULL);
		_U32 nUID = GetUID();
		_U32 len = sizeof(nUID);
		_U8* data = ZION_NEW _U8[len];
		memcpy(data, (void*)&nUID, len);
		//NodeSwitch(NODEID_AVATAR, GetRPCServer("127.0.0.1:1983"), 0, len, data);
		NodeSwitch(0, GetRPCServer("127.0.0.1:1983"), 0, len, data);
	}

}
