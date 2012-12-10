#include <AtlasBase.h>
#include <AtlasServer.h>

class CLoginServerApp : public Atlas::CServerApp
{
public:
	CLoginServerApp()
	{
	}

	virtual void InitApp()
	{
		Atlas::CServerApp::InitApp();
		m_pLoginServer = ATLAS_NEW Atlas::CLoginServer(this);
		m_pLoginServer->Start();
	}

	virtual void FiniApp()
	{
		m_pLoginServer->Stop();
		Atlas::CServerApp::FiniApp();
	}

private:
	Atlas::CLoginServer* m_pLoginServer;
};

int main(int argc, char* argv[])
{
	CLoginServerApp LoginApp;
	if(argc >= 3)
	{
		LoginApp.SetListenAddrPort(argv[1]);
		LoginApp.SetRPCAddrPort(argv[2]);
		LoginApp.SetClusterRpcEP(argv[3]);
		Atlas::SLog("Login Server Start port[%s] rpc[%s] cluster[%s]", argv[1], argv[2], argv[3]);
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
