#include <AtlasBase.h>
#include <AtlasCommon.h>
#include <AtlasServer.h>

#include "SGCommon.h"
#include "SGNode.h"

class CNodeServerApp : public Atlas::CServerApp
{
public:
	CNodeServerApp()
	{
	}

	virtual void InitApp()
	{
		Atlas::CServerApp::InitApp();
		m_pNodeServer = ATLAS_NEW Atlas::CSGNodeServer(this);
		m_pNodeServer->Start();
	}

	virtual void FiniApp()
	{
		m_pNodeServer->Stop();
		Atlas::CServerApp::FiniApp();
	}

private:
	Atlas::CSGNodeServer* m_pNodeServer;
};

int main(int argc, char* argv[])
{
	CNodeServerApp NodeServerApp;
	if(argc >= 1)
	{
		NodeServerApp.SetRPCAddrPort(argv[1]);
		Atlas::SLog("Node Server Start rpc[%s]", argv[1]);
	}
	else
	{
		NodeServerApp.SetRPCAddrPort("127.0.0.1:1983");
	}

	NodeServerApp.Run();
	return 0;
}
