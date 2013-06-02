#include <ZionBase.h>
#include <ZionServer.h>

class CLoginServerApp : public Zion::CServerApp
{
public:
	CLoginServerApp()
	{
	}

	virtual void InitApp()
	{
		Zion::CServerApp::InitApp();
		m_pLoginServer = ZION_NEW Zion::CLoginServer(this);
		m_pLoginServer->Start();
	}

	virtual void FiniApp()
	{
		m_pLoginServer->Stop();
		Zion::CServerApp::FiniApp();
	}

private:
	Zion::CLoginServer* m_pLoginServer;
};

int main(int argc, char* argv[])
{
	CLoginServerApp LoginApp;
	if(argc>3)
	{
		LoginApp.SetListenAddrPort(argv[1]);
		LoginApp.SetRPCAddrPort(argv[2]);
		LoginApp.SetClusterRpcEP(argv[3]);
		Zion::SLog("Login Server Start port[%s] rpc[%s] cluster[%s]", argv[1], argv[2], argv[3]);
	}
	else
	{
		LoginApp.SetListenAddrPort("127.0.0.1:1978");
		LoginApp.SetRPCAddrPort("127.0.0.1:1979");
		LoginApp.SetClusterRpcEP("127.0.0.1:1982");
	}
	
	LoginApp.Run();

	return 0;
}
