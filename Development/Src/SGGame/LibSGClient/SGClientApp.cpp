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
		if(!GetParam("ServerUrl")) SetParam("ServerUrl", "http://127.0.0.1:1982/game-api/%s");
		if(!GetParam("ConnectType")) SetParam("ConnectType", "http");
	}

	CSGClientApp::~CSGClientApp()
	{
	}

	Zion::CClient* CSGClientApp::NewClient()
	{
		Zion::CClient* pClient = ZION_NEW CSGClient(this);
		if(!pClient) return NULL;
		pClient->InitializeComponents();
		return pClient;
	}

}
