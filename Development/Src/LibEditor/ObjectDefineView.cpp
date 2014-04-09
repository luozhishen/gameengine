#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/config.h>
#include <wx/utils.h>

#include <ZionBase.h>
#include <ZionCommon.h>

#include "ObjectDefineView.h"
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include "PropertyEx.h"

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
	wxBoxSizer* pSizer1 = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* pSizer2 = ZION_NEW wxBoxSizer(wxVERTICAL);
	m_pSearchText = ZION_NEW wxTextCtrl(this, ID_SEARCH_TEXT, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	pSizer1->Add(m_pSearchText, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	pSizer1->Add(ZION_NEW wxButton(this, ID_SEARCH_BUTTON, wxT("Search")), 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxSplitterWindow* pSplitter = ZION_NEW wxSplitterWindow(this);
	m_pTree = ZION_NEW wxTreeCtrl(pSplitter, ID_OBJECT_TREE, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT | wxTR_HIDE_ROOT);
	m_pInfo = ZION_NEW wxPropertyGrid(pSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE );

	pSplitter->SplitVertically(m_pTree, m_pInfo, 200);

	pSizer2->Add(pSizer1, 0, wxGROW|wxALIGN_CENTER_VERTICAL);
	pSizer2->Add(pSplitter, 1, wxGROW|wxALIGN_CENTER_VERTICAL);
	SetSizer(pSizer2);

	m_TreeRoot = m_pTree->AddRoot(wxT("ROOT"));
}

CObjectDefineView::~CObjectDefineView()
{
}

void CObjectDefineView::Add(const DDLReflect::STRUCT_INFO* root, Zion::Map<int, const DDLReflect::STRUCT_INFO*>& list)
{
	wxString title;
	GetObjectTitle(root, title);
	wxTreeItemId tid = m_pTree->AppendItem(m_TreeRoot, title, -1, -1, ZION_NEW CObjectData(GetObjectId(root->name), root));
	m_mapItems[root->name] = tid;

	Zion::Map<int, const DDLReflect::STRUCT_INFO*>::iterator i;
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
	_U16 id = Zion::ContentObject::GetTypeId(name);
	if(id==(_U16)-1) id = Zion::LiveObject::GetTypeId(name);
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

	Zion::Map<Zion::String, wxTreeItemId>::iterator i;
	i = m_mapItems.find(info->parent->name);
	wxString title;
	GetObjectTitle(info, title);
	wxTreeItemId tid = m_pTree->AppendItem(i->second, title, -1, -1, ZION_NEW CObjectData(GetObjectId(info->name), info));
	m_mapItems[info->name] = tid;
}

bool StructParamType(const DDLReflect::FIELD_INFO* finfo, Zion::String& type)
{
	char name[100];
	switch(finfo->type&DDLReflect::TYPE_MASK)
	{
	case DDLReflect::TYPE_U8:		sprintf(name, "%s", "_U8"); break;
	case DDLReflect::TYPE_U16:		sprintf(name, "%s", "_U16"); break;
	case DDLReflect::TYPE_U32:		sprintf(name, "%s", "_U32"); break;
	case DDLReflect::TYPE_U64:		sprintf(name, "%s", "_U64"); break;
	case DDLReflect::TYPE_S8:		sprintf(name, "%s", "_S8"); break;
	case DDLReflect::TYPE_S16:		sprintf(name, "%s", "_S16"); break;
	case DDLReflect::TYPE_S32:		sprintf(name, "%s", "_S32"); break;
	case DDLReflect::TYPE_S64:		sprintf(name, "%s", "_S64"); break;
	case DDLReflect::TYPE_F32:		sprintf(name, "%s", "_F32"); break;
	case DDLReflect::TYPE_F64:		sprintf(name, "%s", "_F64"); break;
	case DDLReflect::TYPE_STRING:	sprintf(name, "string<%d>", finfo->slen); break;
	case DDLReflect::TYPE_UUID:		sprintf(name, "%s", "A_UUID"); break;
	case DDLReflect::TYPE_UUID_REF:	sprintf(name, "content_ref<%s>", finfo->ref_type); break;
	case DDLReflect::TYPE_STRUCT:	sprintf(name, "%s", finfo->sinfo->name); break;
	default: return false;
	}
	if((finfo->type&DDLReflect::TYPE_ARRAY)==0)
	{
		type = name;
	}
	else
	{
		type = Zion::StringFormat("array<%s, %d>", name, finfo->alen);
	}
	return true;
}

void AddStruct(wxPropertyGrid* pGrid, const DDLReflect::STRUCT_INFO* info)
{
	if(info->parent)
	{
		AddStruct(pGrid, info->parent);
	}

	wxString name;
	name.Printf(wxT("{%s}"), wxString::FromUTF8(info->name));
	wxPGProperty* prop = ZION_NEW wxPropertyCategory(name);
	prop->ChangeFlag(wxPG_PROP_READONLY, true);
	wxPGProperty* prop_id = pGrid->Append(prop);
	for(_U16 i=0; i<info->fcount; i++)
	{
		Zion::String type;
		StructParamType(info->finfos+i, type);
		name = wxString::FromUTF8(info->finfos[i].name);
		prop = ZION_NEW wxStringProperty(name, name, wxString::FromUTF8(type.c_str()));
		prop->ChangeFlag(wxPG_PROP_READONLY, true);
		pGrid->AppendIn(prop_id, prop);
	}
}

void CObjectDefineView::ShowObject(const DDLReflect::STRUCT_INFO* info)
{
	m_pInfo->Clear();
	AddStruct(m_pInfo, info);
}
