#include <ZionDefines.h>

#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/config.h>
#include <wx/utils.h>
#include <wx/clipbrd.h>

#include <ZionBase.h>
#include <ZionCommon.h>

#include "EditorFrame.h"
#include "ObjectDefineView.h"
#include "GenerateObjectDlg.h"
#include "ContentDataView.h"
#include "ImportDlg.h"

enum
{
	ID_QUIT = wxID_HIGHEST + 1,
	ID_RELOAD,
	ID_SAVE,
	ID_COOK_SAVE,
	ID_COOK_LOAD,
	ID_IMPORT,
	ID_GEN_DESKEY,
	ID_BUILD_INDEX,
	ID_ABOUT,
};

BEGIN_EVENT_TABLE(CEditorFrame, wxFrame)
	EVT_CLOSE(CEditorFrame::OnFrameQuit)

	EVT_MENU(ID_RELOAD,			CEditorFrame::OnFileMenu)
	EVT_MENU(ID_SAVE,			CEditorFrame::OnFileMenu)
	EVT_MENU(ID_QUIT,			CEditorFrame::OnFileMenu)

	EVT_MENU(ID_COOK_SAVE,		CEditorFrame::OnToolMenu)
	EVT_MENU(ID_COOK_LOAD,		CEditorFrame::OnToolMenu)
	EVT_MENU(ID_IMPORT,			CEditorFrame::OnToolMenu)
	EVT_MENU(ID_GEN_DESKEY,		CEditorFrame::OnToolMenu)
	EVT_MENU(ID_BUILD_INDEX,	CEditorFrame::OnToolMenu)

	EVT_MENU(ID_ABOUT,			CEditorFrame::OnHelpMenu)

	EVT_SIZE(CEditorFrame::OnSize)
	EVT_SHOW(CEditorFrame::OnShow)
END_EVENT_TABLE()

CEditorFrame::CEditorFrame() : wxFrame(NULL, wxID_ANY, wxT("Zion Editor - "))
{
	wxString title = GetTitle();
	title = title + wxString::FromUTF8(Zion::ZionGameName());
	SetTitle(title);

	// menu
	InitMenu();

	// status bar
	CreateStatusBar();

	// create client
	InitClient();

	wxConfigBase *pConfig = wxConfigBase::Get();
	if(pConfig)
	{
		pConfig->SetPath(wxString::FromUTF8("/")+wxString::FromUTF8(Zion::ZionGameName()));
		m_FrameData.x = pConfig->Read(wxT("x"), 50);
		m_FrameData.y = pConfig->Read(wxT("y"), 50);
		m_FrameData.w = pConfig->Read(wxT("w"), 350);
		m_FrameData.h = pConfig->Read(wxT("h"), 200);
		Move(m_FrameData.x, m_FrameData.y);
		SetClientSize(m_FrameData.w, m_FrameData.h);
		m_FrameData.m = pConfig->Read(wxT("m"), (long)0);
	}
}

CEditorFrame::~CEditorFrame()
{
	wxConfigBase *pConfig = wxConfigBase::Get();
	if(pConfig)
	{
		pConfig->SetPath(wxString::FromUTF8("/")+wxString::FromUTF8(Zion::ZionGameName()));
		pConfig->Write(wxT("x"), (long)m_FrameData.x);
		pConfig->Write(wxT("y"), (long)m_FrameData.y);
		pConfig->Write(wxT("w"), (long)m_FrameData.w);
		pConfig->Write(wxT("h"), (long)m_FrameData.h);
		pConfig->Write(wxT("m"), (long)m_FrameData.m);
	}
}

void CEditorFrame::InitMenu()
{
	SetMenuBar(ZION_NEW wxMenuBar);
	GetMenuBar()->Append(ZION_NEW wxMenu, wxT("&File"));
	GetMenuBar()->GetMenu(0)->Append(ID_RELOAD, wxT("&Reload Content\tAlt-F"), wxT("Discard current modify"));
	GetMenuBar()->GetMenu(0)->Append(ID_SAVE, wxT("&Save Content\tAlt-S"), wxT("Save content to file"));
	GetMenuBar()->GetMenu(0)->AppendSeparator();
	GetMenuBar()->GetMenu(0)->Append(ID_QUIT, wxT("E&xit\tAlt-X"), wxT("Exit the program"));

	GetMenuBar()->Append(ZION_NEW wxMenu, wxT("&Tools"));
	GetMenuBar()->GetMenu(1)->Append(ID_COOK_SAVE, wxT("Save Cook Data"), wxT("Import from excel"));
	GetMenuBar()->GetMenu(1)->Append(ID_COOK_LOAD, wxT("Load Cook Data"), wxT("Import from excel"));
	GetMenuBar()->GetMenu(1)->AppendSeparator();

	GetMenuBar()->GetMenu(1)->Append(ID_IMPORT, wxT("&Import From Excel...\tAlt-I"), wxT("Import from excel"));
	GetMenuBar()->GetMenu(1)->Append(ID_GEN_DESKEY, wxT("&Generate DESKEY..."), wxT("Generate DESKEY"));
	GetMenuBar()->GetMenu(1)->Append(ID_BUILD_INDEX, wxT("&Build Index"), wxT("Build index for Content Object"));

	GetMenuBar()->Append(ZION_NEW wxMenu, wxT("&Help"));
	GetMenuBar()->GetMenu(2)->Append(ID_ABOUT, wxT("&About"), wxT("Show About Dailog"));
}

void CEditorFrame::InitClient()
{
	wxNotebook* pTab = ZION_NEW wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP);

	Zion::Map<int, const DDLReflect::STRUCT_INFO*> content;
	Zion::Map<int, const DDLReflect::STRUCT_INFO*> live;
	Zion::Array<const DDLReflect::STRUCT_INFO*> list;
	Zion::Array<const DDLReflect::STRUCT_INFO*>::iterator i;

	Zion::ContentObject::GetTypeList(list);
	for(i=list.begin(); i!=list.end(); i++)
	{
		content[Zion::ContentObject::GetTypeId((*i)->name)] = *i;
	}
	Zion::LiveObject::GetTypeList(list);
	for(i=list.begin(); i!=list.end(); i++)
	{
		live[Zion::LiveObject::GetTypeId((*i)->name)] = *i;
	}

	CObjectDefineView* pView = ZION_NEW CObjectDefineView(pTab);
	pTab->AddPage(pView, wxT("Object Define"));
	m_pContentDataView = ZION_NEW CContentDataView(pTab);
	pTab->AddPage(m_pContentDataView, wxT("Content Editor"));

	pView->Add(DDLReflect::GetStruct<A_CONTENT_OBJECT>(), content);
	pView->Add(DDLReflect::GetStruct<A_LIVE_OBJECT>(), live);
}

void CEditorFrame::OnFrameQuit(wxCloseEvent& event)
{
	if(SaveContent())
	{
		event.Skip();
	}
}

void CEditorFrame::OnFileMenu(wxCommandEvent& event)
{
	switch(event.GetId())
	{
	case ID_RELOAD:
		if(m_pContentDataView->CheckModify(true))
		{
			if(!Zion::ContentObject::LoadContent(NULL, false))
			{
				wxMessageBox(wxT("Failed to load content"), wxT("Error"));
			}
			else
			{
				wxMessageBox(wxT("Load content completed."), wxT("Notify"));
			}
		}
		break;
	case ID_SAVE:
		SaveContent(false);
		break;
	case ID_QUIT:
		Close(true);
		break;
	}
}

#include <des64.h>

void CEditorFrame::OnToolMenu(wxCommandEvent& event)
{
	switch(event.GetId())
	{
	case ID_COOK_SAVE:
		if(m_pContentDataView->CheckModify(false))
		{
			if(!Zion::ContentObject::SaveContentToBinaryFile(Zion::StringFormat("%s/Content/CookedData.xxx", Zion::ZionGameDir()).c_str(), "e80cb90fe7042fd9"))
			{
				wxMessageBox(wxT("error in SaveContentToBinaryFile"), wxT("Error"));
			}
			else
			{
				wxMessageBox(wxT("Save contont to binary file completed."), wxT("Notify"));
			}
		}
		break;
	case ID_COOK_LOAD:
		if(m_pContentDataView->CheckModify(true))
		{
			Zion::ContentObject::ClearContents();
			if(!Zion::ContentObject::LoadContentFromBinaryFile(Zion::StringFormat("%s/Content/CookedData.xxx", Zion::ZionGameDir()).c_str(), "e80cb90fe7042fd9"))
			{
				wxMessageBox(wxT("error in LoadContentFromBinaryFile"), wxT("Error"));
			}
			else
			{
				wxMessageBox(wxT("Load contont from binary file completed."), wxT("Notify"));
			}
		}
		break;
	case ID_IMPORT:
		if(m_pContentDataView->CheckModify(true))
		{
			CImportDlg dlg(this);
			Zion::String path = Zion::StringFormat("%s%s", Zion::ZionGameDir(), "Config/ContentTemplate.json");
			if(dlg.LoadTemplateDefine(path.c_str()))
			{
				dlg.ShowModal();
			}
		}
		break;
	case ID_GEN_DESKEY:
		{
			DES_KEY key1;
			char s1[20];
			DES_GenKey(key1);
			DES_KeyToString(key1, s1);
			wxOpenClipboard();
			wxEmptyClipboard();
			wxSetClipboardData(wxDataFormat(wxDF_TEXT), s1);
			wxCloseClipboard();
		}
		break;
	case ID_BUILD_INDEX:
		if(!Zion::ContentObject::BuildIndex())
		{
			wxMessageBox(wxString::FromUTF8(Zion::ContentObject::BuildIndexGetErrorMsg().c_str()), wxT("BUILD INDEX ERROR"));
		}
		else
		{
			wxMessageBox(wxT("Build index completed."), wxT("Notify"));
		}
	}
}

void CEditorFrame::OnHelpMenu(wxCommandEvent&)
{
	wxString txt;
	txt.Printf(wxT("Zion Editor for %s\n(C) 2011-2012 Epic Game China"), wxString::FromUTF8(Zion::ZionGameName()));
	wxMessageBox(txt, wxT("About"), wxICON_INFORMATION|wxOK);
}

void CEditorFrame::OnSize(wxSizeEvent& event)
{
	if(IsMaximized())
	{
		m_FrameData.m = 1;
	}
	else
	{
		m_FrameData.m = 0;
		GetClientSize(&m_FrameData.w, &m_FrameData.h);
		GetPosition(&m_FrameData.x, &m_FrameData.y);
	}
	event.Skip();
}

void CEditorFrame::OnShow(wxShowEvent& event)
{
	if(!IsMaximized() && m_FrameData.m)
	{
		Maximize();
	}
	event.Skip();
}

bool CEditorFrame::SaveContent(bool exit)
{
	if(!m_pContentDataView->CheckModify(false))
	{
		return false;
	}

	if(exit)
	{
		if(!Zion::ContentObject::IsContentDirty())
		{
			return true;
		}

		int ret = wxMessageBox(wxT("The file has not been saved... save...?"), wxT("Please confirm"), wxICON_QUESTION | wxYES_NO | wxCANCEL);
		if(ret==wxCANCEL) return false;
		if(ret==wxNO) return true;
	}

	if(!Zion::ContentObject::SaveContent(NULL, true))
	{
		wxMessageBox(wxT("Save content failed"), wxT("!!!"));
		return false;
	}

	return false;
}
