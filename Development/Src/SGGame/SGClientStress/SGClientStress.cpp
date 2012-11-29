#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/config.h>

#include <AtlasBase.h>
#include <AtlasCommon.h>
#include <AtlasClient.h>
#include <AtlasClientApp.h>

#include <ClientStressApp.h>
#include <StressManager.h>
#include <StressCase.h>

#include "SGCommon.h"
#include "SGClientApp.h"

class CSGClientStressApp : public CClientStressApp
{
public:
	CSGClientStressApp() : m_App(false, 20)
	{
	}

	~CSGClientStressApp()
	{
	}

	virtual void InitClientView(CClientStressFrame* pFrame, wxWindow* pParent)
	{
		CClientStressApp::InitClientView(pFrame, pParent);
	}

	virtual void InitCase()
	{
	}

	Atlas::CSGClientApp m_App;
};

IMPLEMENT_APP(CSGClientStressApp)
