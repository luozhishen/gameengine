#include <ZionBase.h>
#include <ZionServer.h>

class CSessionServerApp : public Zion::CServerApp
{
public:
	CSessionServerApp()
	{
	}

	virtual void InitApp()
	{
		Zion::CServerApp::InitApp();
		m_pSessionServer = ZION_NEW Zion::CSessionServer(this);
		m_pSessionServer->Start();
	}

	virtual void FiniApp()
	{
		m_pSessionServer->Stop();
		Zion::CServerApp::FiniApp();
	}

private:
	Zion::CSessionServer* m_pSessionServer;
};

int main(int argc, char* argv[])
{
	CSessionServerApp SessionApp;
	if(argc>3)
	{
		SessionApp.SetListenAddrPort(argv[1]);
		SessionApp.SetRPCAddrPort(argv[2]);
		SessionApp.SetClusterRpcEP(argv[3]);
		Zion::SLog("Session Server Start port[%s] rpc[%s] cluster[%s]", argv[1], argv[2], argv[3]);
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
