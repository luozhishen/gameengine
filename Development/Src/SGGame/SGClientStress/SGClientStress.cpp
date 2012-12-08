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
#include "ClientComponentDataView.h"

class CSGClientStressApp : public CClientStressApp
{
public:
	CSGClientStressApp() : m_App(false)
	{
	}

	~CSGClientStressApp()
	{
	}

	virtual void InitClientView(CClientStressFrame* pFrame, wxWindow* pParent)
	{
		CClientStressApp::InitClientView(pFrame, pParent);

		pFrame->AddView(ATLAS_NEW CClientComponentDataView(pFrame, pParent));
	}

	virtual void InitCase()
	{
	}

	Atlas::CSGClientApp m_App;
};

IMPLEMENT_APP(CSGClientStressApp)
