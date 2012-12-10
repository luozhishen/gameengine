#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/config.h>
#include <wx/utils.h>

#include <AtlasBase.h>
#include <AtlasCommon.h>

#include "ObjectDefineView.h"

#include <sstream>

enum
{
	ID_SEARCH_TEXT = wxID_HIGHEST + 1,
	ID_SEARCH_BUTTON,
	ID_OBJECT_TREE,
	ID_OBJECT_INFO,
};

BEGIN_EVENT_TABLE(CObjectDefineView, wxPanel)
	EVT_TREE_SEL_CHANGED(ID_OBJECT_TREE, CObjectDefineView::OnObjectActived)
END_EVENT_TABLE()

class CObjectData : public wxTreeItemData
{
public:
	CObjectData(int id, const DDLReflect::STRUCT_INFO* info) { m_nId = id; m_pInfo = info; }

	int m_nId;
	const DDLReflect::STRUCT_INFO* m_pInfo;
};

CObjectDefineView::CObjectDefineView(wxWindow* pParent) : wxPanel(pParent)
{
	wxBoxSizer* pSizer1 = ATLAS_NEW wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* pSizer2 = ATLAS_NEW wxBoxSizer(wxVERTICAL);
	m_pSearchText = ATLAS_NEW wxTextCtrl(this, ID_SEARCH_TEXT, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	pSizer1->Add(m_pSearchText, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	pSizer1->Add(ATLAS_NEW wxButton(this, ID_SEARCH_BUTTON, wxT("Search")), 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxSplitterWindow* pSplitter = ATLAS_NEW wxSplitterWindow(this);
	m_pTree = ATLAS_NEW wxTreeCtrl(pSplitter, ID_OBJECT_TREE, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT | wxTR_HIDE_ROOT);
	m_pInfo = ATLAS_NEW wxTextCtrl(pSplitter, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY);
	pSplitter->SplitVertically(m_pTree, m_pInfo, 200);

	pSizer2->Add(pSizer1, 0, wxGROW|wxALIGN_CENTER_VERTICAL);
	pSizer2->Add(pSplitter, 1, wxGROW|wxALIGN_CENTER_VERTICAL);
	SetSizer(pSizer2);

	m_TreeRoot = m_pTree->AddRoot(wxT("ROOT"));
}

CObjectDefineView::~CObjectDefineView()
{
}

void CObjectDefineView::Add(const DDLReflect::STRUCT_INFO* root, std::map<int, const DDLReflect::STRUCT_INFO*>& list)
{
	wxString title;
	GetObjectTitle(root, title);
	wxTreeItemId tid = m_pTree->AppendItem(m_TreeRoot, title, -1, -1, ATLAS_NEW CObjectData(GetObjectId(root->name), root));
	m_mapItems[root->name] = tid;

	std::map<int, const DDLReflect::STRUCT_INFO*>::iterator i;
	for(i=list.begin(); i!=list.end(); i++)
	{
		AddObject(i->second);
	}
}

void CObjectDefineView::OnObjectActived(wxTreeEvent& event)
{
	CObjectData* pData = (CObjectData*)m_pTree->GetItemData(event.GetItem());
	if(!pData) return;
	ShowObject(pData->m_pInfo);
}

_U16 CObjectDefineView::GetObjectId(const char* name)
{
	_U16 id = Atlas::ContentObject::GetTypeId(name);
	if(id==(_U16)-1) id = Atlas::LiveObject::GetTypeId(name);
	return id;
}

void CObjectDefineView::GetObjectTitle(const DDLReflect::STRUCT_INFO* info, wxString& title)
{
	_U16 id = GetObjectId(info->name);
	wxString name(info->name, wxMBConvUTF8());
	if(id==(_U16)-1)
	{
		title.Printf(wxT("%s : *"), name);
	}
	else
	{
		title.Printf(wxT("%s : H%x"), name, id);
	}
}

void CObjectDefineView::AddObject(const DDLReflect::STRUCT_INFO* info)
{
	if(m_mapItems.find(info->name)!=m_mapItems.end()) return;

	if(m_mapItems.find(info->parent->name)==m_mapItems.end())
	{
		AddObject(info->parent);
	}

	std::map<std::string, wxTreeItemId>::iterator i;
	i = m_mapItems.find(info->parent->name);
	wxString title;
	GetObjectTitle(info, title);
	wxTreeItemId tid = m_pTree->AppendItem(i->second, title, -1, -1, ATLAS_NEW CObjectData(GetObjectId(info->name), info));
	m_mapItems[info->name] = tid;
}

void CObjectDefineView::ShowObject(const DDLReflect::STRUCT_INFO* info)
{
	wxMBConvUTF8 conv;
	wxString s;
	s += wxT("struct ");
	s += wxString(info->name, conv);
	if(info->parent)
	{
		s += wxT(" : ");
		s += wxString(info->parent->name, conv);
	}
	s += wxT("\n{\n");
	
	for(_U16 i=0; i<info->fcount; i++)
	{
		std::string type;
		DDLReflect::StructParamType(info, i, type);
		s += wxT("\t");
		s += wxString(type.c_str(), conv);
		s += wxT(" ");
		s += wxString(info->finfos[i].name, conv);
		s += wxT(";\n");
	}
	s += wxT("};\n");

	m_pInfo->SetValue(s);
}
