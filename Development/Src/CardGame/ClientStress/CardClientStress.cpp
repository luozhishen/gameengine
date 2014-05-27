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

#include "CardCommon.h"
#include "CardClientApp.h"
#include "CardStressCases.h"

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
		Zion::CStressManager::Get().RegisterCase(&m_CaseEnterGame);
	}

	Zion::CCardClientApp m_App;
	Zion::StressCases::CLogin m_CaseLogin;
	Zion::StressCases::CEnterGame m_CaseEnterGame;
};

IMPLEMENT_APP(CCardClientStressApp)
