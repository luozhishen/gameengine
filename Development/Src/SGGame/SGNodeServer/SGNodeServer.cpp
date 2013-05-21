#include <ZionBase.h>
#include <ZionCommon.h>
#include <ZionServer.h>

#include "SGCommon.h"
#include "SGNode.h"

class CNodeServerApp : public Zion::CServerApp
{
public:
	CNodeServerApp()
	{
	}

	virtual void InitApp()
	{
		Zion::CServerApp::InitApp();
		m_pNodeServer = ATLAS_NEW Zion::CSGNodeServer(this);
		m_pNodeServer->Start();
	}

	virtual void FiniApp()
	{
		m_pNodeServer->Stop();
		Zion::CServerApp::FiniApp();
	}

private:
	Zion::CSGNodeServer* m_pNodeServer;
};

int main(int argc, char* argv[])
{
	CNodeServerApp NodeServerApp;
	if(argc>1)
	{
		NodeServerApp.SetRPCAddrPort(argv[1]);
		Zion::SLog("Node Server Start rpc[%s]", argv[1]);
	}
	else
	{
		NodeServerApp.SetRPCAddrPort("127.0.0.1:1983");
	}

	NodeServerApp.Run();
	return 0;
}
