#include <AtlasBase.h>
#include <AtlasServer.h>

class CSessionServerApp : public Atlas::CServerApp
{
public:
	CSessionServerApp()
	{
	}

	virtual void InitApp()
	{
		Atlas::CServerApp::InitApp();
		m_pSessionServer = ATLAS_NEW Atlas::CSessionServer(this);
		m_pSessionServer->Start();
	}

	virtual void FiniApp()
	{
		m_pSessionServer->Stop();
		Atlas::CServerApp::FiniApp();
	}

private:
	Atlas::CSessionServer* m_pSessionServer;
};

int main(int argc, char* argv[])
{
	CSessionServerApp SessionApp;
	if(argc>3)
	{
		SessionApp.SetListenAddrPort(argv[1]);
		SessionApp.SetRPCAddrPort(argv[2]);
		SessionApp.SetClusterRpcEP(argv[3]);
		Atlas::SLog("Session Server Start port[%s] rpc[%s] cluster[%s]", argv[1], argv[2], argv[3]);
	}
	else
	{
		SessionApp.SetListenAddrPort("127.0.0.1:1980");
		SessionApp.SetRPCAddrPort("127.0.0.1:1981");
		SessionApp.SetClusterRpcEP("127.0.0.1:1982");
	}
	
	SessionApp.Run();
	return 0;
}
