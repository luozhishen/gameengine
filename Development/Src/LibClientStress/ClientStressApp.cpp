#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/config.h>

#include <ZionBase.h>
#include <ZionCommon.h>
#include <ZionClient.h>
#include <ZionClientApp.h>

#include <StressClient.h>
#include <StressLoader.h>

#include "ClientStressApp.h"
#include "ClientStressFrame.h"
#include "ClientLogView.h"
#include "ClientCaseView.h"

CClientStressApp* g_ClientStressApp = NULL;

CClientStressApp::CClientStressApp()
{
	g_ClientStressApp = this;
	chdir(Zion::GetExeDir());
}

CClientStressApp::~CClientStressApp()
{
}

bool CClientStressApp::OnInit()
{
	Zion::CClientApp::GetDefault()->InitApp();

	wxImage::AddHandler(ZION_NEW wxPNGHandler);

	SetVendorName(wxT("Zion"));
	SetAppName(wxT("ClientStress"));

	wxConfigBase *pConfig = wxConfigBase::Get();

	CClientStressFrame* pMainFrame;
	pMainFrame = ZION_NEW CClientStressFrame;
	pMainFrame->Show(true);
	SetTopWindow(pMainFrame);

	return true;
}

int CClientStressApp::OnExit()
{
	Zion::CClientApp::GetDefault()->FiniApp();

	delete wxConfigBase::Set(NULL);
	return 0;
}

void CClientStressApp::InitClientView(CClientStressFrame* pFrame, wxWindow* pParent)
{
	pFrame->AddView(ZION_NEW CClientLogView(pFrame, pParent));
	pFrame->AddView(ZION_NEW CClientCaseView(pFrame, pParent));
}
