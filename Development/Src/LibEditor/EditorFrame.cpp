#include <AtlasDefines.h>

#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/config.h>
#include <wx/utils.h>

#include <AtlasBase.h>
#include <AtlasCommon.h>

#include "EditorFrame.h"
#include "ObjectDefineView.h"
#include "GenerateObjectDlg.h"
#include "ContentDataView.h"
#include "ImportDlg.h"

enum
{
	ID_QUIT = wxID_HIGHEST + 1,
	ID_DEBUG_MODEL,
	ID_ABOUT,
	ID_SAVE,
	ID_SAVE_FORCE,
	ID_IMPORT,
};

BEGIN_EVENT_TABLE(CEditorFrame, wxFrame)
	EVT_CLOSE(CEditorFrame::OnFrameQuit)
	EVT_MENU(ID_SAVE, CEditorFrame::OnSave)
	EVT_MENU(ID_SAVE_FORCE, CEditorFrame::OnSave)
	EVT_MENU(ID_IMPORT, CEditorFrame::OnImport)
	EVT_MENU(ID_QUIT, CEditorFrame::OnQuit)
	EVT_MENU(ID_ABOUT, CEditorFrame::OnAbout)
	EVT_MENU(ID_DEBUG_MODEL, CEditorFrame::OnDebug)
	EVT_SIZE(CEditorFrame::OnSize)
	EVT_SHOW(CEditorFrame::OnShow)
END_EVENT_TABLE()

CEditorFrame::CEditorFrame() : wxFrame(NULL, wxID_ANY, wxT("Atlas Editor - "))
{
	wxString title = GetTitle();
	title = title + wxString::FromUTF8(Atlas::AtlasGameName());
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
		pConfig->SetPath(wxString::FromUTF8("/")+wxString::FromUTF8(Atlas::AtlasGameName()));
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
	// save the frame position
	wxConfigBase *pConfig = wxConfigBase::Get();
	if(pConfig)
	{
		pConfig->SetPath(wxString::FromUTF8("/")+wxString::FromUTF8(Atlas::AtlasGameName()));
		pConfig->Write(wxT("x"), (long)m_FrameData.x);
		pConfig->Write(wxT("y"), (long)m_FrameData.y);
		pConfig->Write(wxT("w"), (long)m_FrameData.w);
		pConfig->Write(wxT("h"), (long)m_FrameData.h);
		pConfig->Write(wxT("m"), (long)m_FrameData.m);
	}
}

void CEditorFrame::InitMenu()
{
	SetMenuBar(ATLAS_NEW wxMenuBar);
	GetMenuBar()->Append(ATLAS_NEW wxMenu, wxT("&File"));
	GetMenuBar()->GetMenu(0)->Append(ID_SAVE, wxT("&Save Content\tAlt-S"), wxT("Save content to file"));
	GetMenuBar()->GetMenu(0)->Append(ID_SAVE_FORCE, wxT("&Save Content(force)\tAlt-F"), wxT("Force save content to file"));
	GetMenuBar()->GetMenu(0)->AppendSeparator();
	GetMenuBar()->GetMenu(0)->Append(ID_IMPORT, wxT("&Import From Excel...\tAlt-I"), wxT("Import from excel"));
	GetMenuBar()->GetMenu(0)->AppendSeparator();
	GetMenuBar()->GetMenu(0)->Append(ID_QUIT, wxT("E&xit\tAlt-X"), wxT("Exit the program"));
	GetMenuBar()->Append(ATLAS_NEW wxMenu, wxT("&Help"));
	GetMenuBar()->GetMenu(1)->Append(ID_DEBUG_MODEL, wxT("&Debug"), wxT("Show Debug Text"));
	GetMenuBar()->GetMenu(1)->Append(ID_ABOUT, wxT("&About"), wxT("Show About Dailog"));
}

void CEditorFrame::InitClient()
{
	wxNotebook* pTab = ATLAS_NEW wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP);

	std::map<int, const DDLReflect::STRUCT_INFO*> content;
	std::map<int, const DDLReflect::STRUCT_INFO*> live;
	std::map<int, const DDLReflect::STRUCT_INFO*> sync;
	{
		std::vector<const DDLReflect::STRUCT_INFO*> list;
		Atlas::ContentObject::GetTypeList(list);
		std::vector<const DDLReflect::STRUCT_INFO*>::iterator i;
		for(i=list.begin(); i!=list.end(); i++)
		{
			content[Atlas::ContentObject::GetTypeId((*i)->name)] = *i;
		}
	}
	{
		std::vector<const DDLReflect::STRUCT_INFO*> list;
		Atlas::LiveObject::GetTypeList(list);
		std::vector<const DDLReflect::STRUCT_INFO*>::iterator i;
		for(i=list.begin(); i!=list.end(); i++)
		{
			live[Atlas::LiveObject::GetTypeId((*i)->name)] = *i;
		}
	}
	{
		std::vector<const DDLReflect::STRUCT_INFO*> list;
		Atlas::Sync::GetTypeList(list);
		std::vector<const DDLReflect::STRUCT_INFO*>::iterator i;
		for(i=list.begin(); i!=list.end(); i++)
		{
			sync[Atlas::Sync::GetTypeId((*i)->name)] = *i;
		}
	}

	CObjectDefineView* pView = ATLAS_NEW CObjectDefineView(pTab);
	pTab->AddPage(pView, wxT("Object Define"));
	m_pContentDataView = ATLAS_NEW CContentDataView(pTab);
	pTab->AddPage(m_pContentDataView, wxT("Content Editor"));

	pView->Add(DDLReflect::GetStruct<A_CONTENT_OBJECT>(), content);
	pView->Add(DDLReflect::GetStruct<A_LIVE_OBJECT>(), live);
	pView->Add(DDLReflect::GetStruct<A_SYNC_OBJECT>(), sync);
}

void CEditorFrame::OnFrameQuit(wxCloseEvent& event)
{
	if(SaveContent()) event.Skip();
}

void CEditorFrame::OnSave(wxCommandEvent& event)
{
	if(event.GetId()==ID_SAVE_FORCE)
	{
		if(Atlas::ContentObject::SaveContent()) return;
		wxMessageBox(wxT("Save content failed"), wxT("!!!"));
	}
	else
	{
		SaveContent(false);
	}
}

void CEditorFrame::OnImport(wxCommandEvent& event)
{
	CImportDlg dlg(this);
	dlg.ShowModal();
}

void CEditorFrame::OnQuit(wxCommandEvent&)
{
	Close(true);
}

void CEditorFrame::OnAbout(wxCommandEvent&)
{
	wxString txt;
	txt.Printf(wxT("Atlas Editor for %s\n(C) 2011-2012 Epic Game China"), wxString::FromUTF8(Atlas::AtlasGameName()));
	wxMessageBox(txt, wxT("About"), wxICON_INFORMATION|wxOK);
}

void CEditorFrame::OnDebug(wxCommandEvent& event)
{
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
	if(!m_pContentDataView->CheckModify()) return false;

	if(exit)
	{
		std::map<std::string, bool> list;
		Atlas::ContentObject::GetContentFileList(list);
		bool modify = false;
		std::map<std::string, bool>::iterator i;
		for(i=list.begin(); i!=list.end(); i++)
		{
			if(i->second)
			{
				modify = true;
				break;
			}
		}
		if(!modify) return true;

		int ret = wxMessageBox(wxT("The file has not been saved... save...?"), wxT("Please confirm"), wxICON_QUESTION | wxYES_NO | wxCANCEL);
		if(ret==wxCANCEL) return false;
		if(ret==wxNO) return true;
	}

	if(Atlas::ContentObject::SaveContent()) return true;
	wxMessageBox(wxT("Save content failed"), wxT("!!!"));
	return false;
}
