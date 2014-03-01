#include <ZionBase.h>
#include <ZionCommon.h>
#include <ZionClient.h>
#include <ZionClientApp.h>

#include "CardCommon.h"
#include "CardClientApp.h"
#include "CardClient.h"

namespace Zion
{

	CCardClientApp::CCardClientApp(bool bThread) : CClientApp("CardGame", bThread)
	{
		if(!GetParam("ServerUrl")) SetParam("ServerUrl", "http://127.0.0.1/zion-web/CardGame/%s.php");
		if(!GetParam("ConnectType")) SetParam("ConnectType", "http");
	}

	CCardClientApp::~CCardClientApp()
	{
	}

	Zion::CClient* CCardClientApp::NewClient()
	{
		Zion::CClient* pClient = ZION_NEW CCardClient(this);
		if(!pClient) return NULL;
		pClient->InitializeComponents();
		return pClient;
	}

}
