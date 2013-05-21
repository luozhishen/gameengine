#include <ZionBase.h>
#include <ZionCommon.h>
#include <ZionClient.h>
#include <ZionClientApp.h>

#include "SGCommon.h"
#include "SGClientApp.h"
#include "SGClient.h"

namespace Zion
{

	CSGClientApp::CSGClientApp(bool bThread) : CClientApp("SGGame", bThread)
	{
		if(!GetParam("ServerUrl")) SetParam("ServerUrl", "http://192.168.0.96/Zion/action/%s.php");
		if(!GetParam("ConnectType")) SetParam("ConnectType", "http");
	}

	CSGClientApp::~CSGClientApp()
	{
	}

	Zion::CClient* CSGClientApp::NewClient()
	{
		Zion::CClient* pClient = ATLAS_NEW CSGClient(this);
		if(!pClient) return NULL;
		pClient->InitializeComponents();
		return pClient;
	}

}
