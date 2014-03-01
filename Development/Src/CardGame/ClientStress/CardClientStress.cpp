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

#include "CardStressCases.h"

#include "CardCommon.h"
#include "CardClientApp.h"

class CCardClientStressApp : public CClientStressApp
{
public:
	CCardClientStressApp() : m_App(false)
	{
	}

	~CCardClientStressApp()
	{
	}

	virtual void InitClientView(CClientStressFrame* pFrame, wxWindow* pParent)
	{
		CClientStressApp::InitClientView(pFrame, pParent);
	}

	virtual void InitCase()
	{
		Zion::CStressManager::Get().RegisterCase(&m_CaseLogin);
		Zion::CStressManager::Get().RegisterCase(&m_CaseEcho);
		Zion::CStressManager::Get().RegisterCase(&m_CaseEnterServer);
	}

	Zion::CCardClientApp m_App;
	Zion::StressCases::CLogin m_CaseLogin;
	Zion::StressCases::CEcho m_CaseEcho;
	Zion::StressCases::CEnterServer m_CaseEnterServer;
};

IMPLEMENT_APP(CCardClientStressApp)
