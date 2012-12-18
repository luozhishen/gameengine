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
		if(!GetParam("ServerUrl")) SetParam("ServerUrl", "http://192.168.0.96/Atlas/action/");
		if(!GetParam("ConnectType")) SetParam("ConnectType", "http");
	}

	CSGClientApp::~CSGClientApp()
	{
	}

	Atlas::CClient* CSGClientApp::NewClient()
	{
		return ATLAS_NEW CSGClient(this);
	}

}
