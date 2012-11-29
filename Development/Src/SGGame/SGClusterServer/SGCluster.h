#ifndef __SGGAME_CLUSTER__
#define __SGGAME_CLUSTER__

#include <ClusterServer.h>

namespace Atlas
{

	class CSGClusterServer : public CClusterServer
	{
	public:
		CSGClusterServer(CServerApp* pServerApp);
		virtual ~CSGClusterServer();

		virtual CClusterClient* NewClusterClient(_U64 nCNDX);
	};

	class CSGClusterClient : public CClusterClient
	{
	public:
		CSGClusterClient(CClusterServer* pServer, _U64 nCNDX);
		virtual ~CSGClusterClient();

		virtual void OnUserLogin();
	};

};

#endif
