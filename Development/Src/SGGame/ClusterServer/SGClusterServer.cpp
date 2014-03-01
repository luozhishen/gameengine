#include <ZionBase.h>
#include <ZionServer.h>

#include "SGCluster.h"
#include "SGCommon.h"

class CClusterServerApp : public Zion::CServerApp
{
public:
	CClusterServerApp()
	{
	}

	virtual void InitApp()
	{
		Zion::CServerApp::InitApp();
		m_pClusterServer = ZION_NEW Zion::CSGClusterServer(this);
		m_pClusterServer->Start();
	}

	virtual void FiniApp()
	{
		m_pClusterServer->Stop();
		Zion::CServerApp::FiniApp();
	}

private:
	Zion::CSGClusterServer* m_pClusterServer;
};

int main(int argc, char* argv[])
{
	CClusterServerApp ClusterServerApp;
	if(argc>1)
	{
		ClusterServerApp.SetRPCAddrPort(argv[1]);
		Zion::SLog("Cluster Server Start rpc[%s]", argv[1]);
	}
	else
	{
		ClusterServerApp.SetRPCAddrPort("127.0.0.1:1982");
	}

	ClusterServerApp.Run();
	return 0;
}
