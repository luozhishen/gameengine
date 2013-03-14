#include <AtlasBase.h>

#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/config.h>
#include <wx/utils.h>

#include "EditorApp.h"
#include "EditorFrame.h"

#include <AtlasBase.h>
#include <AtlasCommon.h>

CEditorApp::CEditorApp()
{
	chdir(Atlas::GetExeDir());
}

CEditorApp::~CEditorApp()
{
}

bool CEditorApp::OnInit()
{
	Atlas::InitDDLStub();
	Atlas::InitContentObjects();
	Atlas::InitLiveObjects();
	Atlas::ContentObject::LoadContent();

	wxImage::AddHandler(ATLAS_NEW wxPNGHandler);

	SetVendorName(wxT("Atlas"));
	SetAppName(wxT("Editor"));

	wxConfigBase *pConfig = wxConfigBase::Get();
	pConfig->SetRecordDefaults();

	CEditorFrame* pMainFrame;
	pMainFrame = ATLAS_NEW CEditorFrame;
	pMainFrame->Show(true);
	SetTopWindow(pMainFrame);

	return true;
}

int CEditorApp::OnExit()
{
	Atlas::ContentObject::SaveContent();

	delete wxConfigBase::Set(NULL);
	return 0;
}
