#include <AtlasBase.h>
#include <AtlasServer.h>

#include "SGCluster.h"
#include "SGCommon.h"

class CClusterServerApp : public Atlas::CServerApp
{
public:
	CClusterServerApp()
	{
	}

	virtual void InitApp()
	{
		Atlas::CServerApp::InitApp();
		m_pClusterServer = ATLAS_NEW Atlas::CSGClusterServer(this);
		m_pClusterServer->Start();
	}

	virtual void FiniApp()
	{
		m_pClusterServer->Stop();
		Atlas::CServerApp::FiniApp();
	}

private:
	Atlas::CSGClusterServer* m_pClusterServer;
};

int main(int argc, char* argv[])
{
	CClusterServerApp ClusterServerApp;
	if(argc>1)
	{
		ClusterServerApp.SetRPCAddrPort(argv[1]);
		Atlas::SLog("Cluster Server Start rpc[%s]", argv[1]);
	}
	else
	{
		ClusterServerApp.SetRPCAddrPort("127.0.0.1:1982");
	}

	ClusterServerApp.Run();
	return 0;
}
