#include <AtlasBase.h>
#include <AtlasCommon.h>
#include <AtlasClient.h>
#include <AtlasClientApp.h>

#include "KnightCommon.h"
#include "KnightClientApp.h"
#include "KnightClient.h"

namespace Atlas
{

	CKnightClientApp::CKnightClientApp(bool bThread) : CClientApp("KnightGame", bThread)
	{
		if(!GetParam("ServerUrl")) SetParam("ServerUrl", "http://192.168.4.42/Atlas/action/");
		if(!GetParam("ConnectType")) SetParam("ConnectType", "http");
	}

	CKnightClientApp::~CKnightClientApp()
	{
	}

	Atlas::CClient* CKnightClientApp::NewClient()
	{
		Atlas::CClient* pClient = ATLAS_NEW CKnightClient(this);
		if(!pClient) return NULL;
		pClient->InitializeComponents();
		return pClient;
	}

}