#include <AtlasBase.h>
#include <AtlasCommon.h>
#include <AtlasClient.h>
#include <AtlasClientApp.h>

#include "SGCommon.h"
#include "SGClientApp.h"
#include "SGClient.h"

namespace Atlas
{

	CSGClientApp::CSGClientApp(bool bThread) : CClientApp(bThread)
	{
	}

	CSGClientApp::~CSGClientApp()
	{
	}

	Atlas::CClient* CSGClientApp::NewClient()
	{
		return ATLAS_NEW CSGClient(this);
	}

}
