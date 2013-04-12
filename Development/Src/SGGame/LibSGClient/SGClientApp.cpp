#include <AtlasBase.h>
#include <AtlasCommon.h>
#include <AtlasClient.h>
#include <AtlasClientApp.h>

#include "SGCommon.h"
#include "SGClientApp.h"
#include "SGClient.h"

namespace Atlas
{

	CSGClientApp::CSGClientApp(bool bThread) : CClientApp("SGGame", bThread)
	{
		if(!GetParam("ServerUrl")) SetParam("ServerUrl", "http://192.168.0.96/Atlas/action/%s.php");
		//if(!GetParam("ServerUrl")) SetParam("ServerUrl", "http://192.168.0.96/Atlas_Test/action/%s.php");
		//if(!GetParam("ServerUrl")) SetParam("ServerUrl", "http://58.247.138.117/Atlas/action/%s.php");
		if(!GetParam("ConnectType")) SetParam("ConnectType", "http");
	}

	CSGClientApp::~CSGClientApp()
	{
	}

	Atlas::CClient* CSGClientApp::NewClient()
	{
		Atlas::CClient* pClient = ATLAS_NEW CSGClient(this);
		if(!pClient) return NULL;
		pClient->InitializeComponents();
		return pClient;
	}

}
