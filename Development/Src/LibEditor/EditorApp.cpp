#include <ZionBase.h>

#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/config.h>
#include <wx/utils.h>

#include "EditorApp.h"
#include "EditorFrame.h"

#include <ZionBase.h>
#include <ZionCommon.h>

CEditorApp::CEditorApp()
{
	chdir(Zion::GetExeDir());
}

CEditorApp::~CEditorApp()
{
}

bool CEditorApp::OnInit()
{
	Zion::InitDDLStub();
	Zion::InitContentObjects();
	Zion::InitLiveObjects();
	Zion::ContentObject::LoadContent(NULL, true);

	wxImage::AddHandler(ZION_NEW wxPNGHandler);

	SetVendorName(wxT("Zion"));
	SetAppName(wxT("Editor"));

	wxConfigBase *pConfig = wxConfigBase::Get();
	pConfig->SetRecordDefaults();

	CEditorFrame* pMainFrame;
	pMainFrame = ZION_NEW CEditorFrame;
	pMainFrame->Show(true);
	SetTopWindow(pMainFrame);

	return true;
}

int CEditorApp::OnExit()
{
	delete wxConfigBase::Set(NULL);
	return 0;
}
