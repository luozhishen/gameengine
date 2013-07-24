#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/config.h>

#include <ZionBase.h>
#include <ZionCommon.h>
#include <ZionClient.h>
#include <ZionClientApp.h>

#include <ClientStressApp.h>
#include <StressManager.h>
#include <StressCase.h>
#include <StressCases.h>

#include "SGCommon.h"
#include "SGClientApp.h"

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
	}

	virtual void InitCase()
	{
		Zion::CStressManager::Get().RegisterCase(&m_CaseLogin);
	}

	Zion::CSGClientApp m_App;
	Zion::StressCases::CLogin m_CaseLogin;
};

IMPLEMENT_APP(CSGClientStressApp)
