#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/config.h>
#include <wx/utils.h>
#include <wx/combo.h>
#include <wx/statline.h>

#include <AtlasBase.h>
#include <AtlasCommon.h>

#include "GenerateObjectDlg.h"
#include "ContentDataView.h"
#include "StructEditView.h"

#include <string.h>

enum
{
	ID_SEARCH_TEXT = wxID_HIGHEST + 1,
	ID_SEARCH_BUTTON,
	ID_OBJECT_TYPE,
	ID_OBJECT_ADD,
	ID_OBJECT_REMOVE,
	ID_OBJECT_REFLUSH,
	ID_OBJECT_SAVE,
	ID_OBJECT_UNDO,
	ID_OBJECT_LIST,
	ID_OBJECT_INFO,
};

BEGIN_EVENT_TABLE(CContentDataView, wxPanel)
	EVT_BUTTON(ID_SEARCH_BUTTON, CContentDataView::OnSearch)
	EVT_TEXT_ENTER(ID_SEARCH_TEXT, CContentDataView::OnSearch)
	EVT_BUTTON(ID_OBJECT_ADD, CContentDataView::OnObjectAdd)
	EVT_BUTTON(ID_OBJECT_SAVE, CContentDataView::OnObjectSave)
	EVT_BUTTON(ID_OBJECT_UNDO, CContentDataView::OnObjectUndo)
	EVT_BUTTON(ID_OBJECT_REMOVE, CContentDataView::OnObjectRemove)
	EVT_BUTTON(ID_OBJECT_REFLUSH, CContentDataView::OnObjectReflush)
	EVT_LIST_ITEM_SELECTED(ID_OBJECT_LIST, CContentDataView::OnObjectActived)
	EVT_MENU(wxID_NEW, CContentDataView::OnMenuNew)
	EVT_MENU(wxID_COPY, CContentDataView::OnMenuCopy)
	EVT_MENU(wxID_PASTE, CContentDataView::OnMenuPaste)
	EVT_MENU(wxID_DELETE, CContentDataView::OnMenuDel)
	EVT_MENU(wxID_SELECTALL, CContentDataView::OnMenuSelAll)
	EVT_MENU(wxID_SAVE, CContentDataView::OnMenuSave)
	EVT_CONTEXT_MENU(CContentDataView::OnContextMenu)
	EVT_IDLE(CContentDataView::OnIdle)
END_EVENT_TABLE()

CContentDataView::CContentDataView(wxWindow* pParent) : wxPanel(pParent)
{
	m_nSelectIndex = -1;
	m_pCurInfo = NULL;
	m_pCurData = NULL;

	wxBoxSizer* pSizerTop = ATLAS_NEW wxBoxSizer(wxHORIZONTAL);
	wxSplitterWindow* pSplitter = ATLAS_NEW wxSplitterWindow(this);

	// left
	wxPanel* pPanel1 = ATLAS_NEW wxPanel(pSplitter);
	wxBoxSizer* pSizerLeft = ATLAS_NEW wxBoxSizer(wxVERTICAL);

	wxBitmap bmpAdd;
	wxBitmap bmpRemove;
	wxBitmap bmpReflush;
	bmpAdd.LoadFile(wxT("../wxRes/AtlasEditor/Add.png"),			wxBITMAP_TYPE_PNG);
	bmpRemove.LoadFile(wxT("../wxRes/AtlasEditor/Remove.png"),		wxBITMAP_TYPE_PNG);
	bmpReflush.LoadFile(wxT("../wxRes/AtlasEditor/Reflush.png"),	wxBITMAP_TYPE_PNG);
	
	m_pObjectType = ATLAS_NEW wxComboBox(this, ID_OBJECT_TYPE, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0, NULL, wxCB_DROPDOWN|wxCB_READONLY);
	m_pSearchText = ATLAS_NEW wxTextCtrl(this, ID_SEARCH_TEXT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	wxButton* pSearchBtn = ATLAS_NEW wxButton(this, ID_SEARCH_BUTTON, wxT("Search"));

	pSizerTop->Add(m_pObjectType, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	pSizerTop->Add(m_pSearchText, 1, wxGROW|wxALIGN_LEFT|wxALL, 5);
	pSizerTop->Add(pSearchBtn, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_pList = ATLAS_NEW wxListCtrl(pPanel1, ID_OBJECT_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL);
	m_pSearchResult = ATLAS_NEW wxStaticText(pPanel1, wxID_ANY, wxT("NOT FOUND"));
	pSizerLeft->Add(m_pSearchResult, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	pSizerLeft->Add(m_pList, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL);
	pPanel1->SetSizer(pSizerLeft);
	
	// right
	wxPanel* pPanel2 = ATLAS_NEW wxPanel(pSplitter);
	wxBoxSizer* pSizer11 = ATLAS_NEW wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* pSizer13 = ATLAS_NEW wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* pSizerRight = ATLAS_NEW wxBoxSizer(wxVERTICAL);

	wxBitmap bmpSave;
	wxBitmap bmpUndo;
	bmpSave.LoadFile(wxT("../wxRes/AtlasEditor/Save.bmp"),			wxBITMAP_TYPE_BMP);
	bmpUndo.LoadFile(wxT("../wxRes/AtlasEditor/Undo.png"),			wxBITMAP_TYPE_PNG);

	m_pObjectName = ATLAS_NEW wxTextCtrl(pPanel2, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	m_pInfo = ATLAS_NEW CStructEditView(pPanel2, this);
	pSizerRight->Add(ATLAS_NEW wxStaticText(pPanel2, wxID_ANY, wxT("Object Edit")), 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	pSizer11->Add(m_pObjectName, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0);
	pSizer11->Add(ATLAS_NEW wxBitmapButton(pPanel2, ID_OBJECT_SAVE, bmpSave), 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0);
	pSizer11->Add(ATLAS_NEW wxBitmapButton(pPanel2, ID_OBJECT_REMOVE, bmpRemove), 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0);
	pSizer11->Add(ATLAS_NEW wxBitmapButton(pPanel2, ID_OBJECT_UNDO, bmpUndo), 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0);

	pSizerRight->Add(pSizer13, 0, wxGROW|wxALIGN_CENTER_VERTICAL);
	pSizerRight->AddSpacer(5);
	pSizerRight->Add(pSizer11, 0, wxGROW|wxALIGN_CENTER_VERTICAL);
	pSizerRight->AddSpacer(5);
	pSizerRight->Add(m_pInfo, 1, wxGROW|wxALIGN_CENTER_VERTICAL);

	pPanel2->SetSizer(pSizerRight);

	// main
	pSplitter->SplitVertically(pPanel1, pPanel2, 300);
	wxBoxSizer* pSizerAll = ATLAS_NEW wxBoxSizer(wxVERTICAL);
	pSizerAll->Add(ATLAS_NEW wxStaticText(this, wxID_ANY, wxT("Search")), 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxUP, 5);
	pSizerAll->Add(pSizerTop, 0, wxGROW|wxALIGN_CENTER_VERTICAL);
	pSizerAll->Add(ATLAS_NEW wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), 0, wxGROW|wxALIGN_CENTER_VERTICAL); 
	pSizerAll->Add(pSplitter, 1, wxGROW|wxALIGN_CENTER_VERTICAL);
	SetSizer(pSizerAll);

	Atlas::Vector<const DDLReflect::STRUCT_INFO*> list;
	Atlas::ContentObject::GetTypeList(list);
	Atlas::Vector<const DDLReflect::STRUCT_INFO*>::iterator i;
	for(i=list.begin(); i!=list.end(); i++)
	{
		const DDLReflect::STRUCT_INFO* p = *i;
		while(p)
		{
			if(m_mapTypes.find(p->name)==m_mapTypes.end())
			{
				m_mapTypes[p->name] = p;
			}
			p = p->parent;
		}
	}
	Atlas::Map<Atlas::String, const DDLReflect::STRUCT_INFO*>::iterator in;
	int nTypeIndex = -1;
	for(in=m_mapTypes.begin(); in!=m_mapTypes.end(); in++)
	{
		if(in->first=="A_CONTENT_OBJECT")
		{
			nTypeIndex = m_pObjectType->Insert(wxString::FromUTF8(in->first.c_str()), m_pObjectType->GetCount());
		}
		else
		{
			m_pObjectType->Insert(wxString::FromUTF8(in->first.c_str()), m_pObjectType->GetCount());
		}
	}
	m_pObjectType->SetSelection(nTypeIndex);

	m_pList->InsertColumn(0, wxT("Name"), 0, 60);
	m_pList->InsertColumn(1, wxT("Type"), 0, 60);
	m_pList->InsertColumn(2, wxT("UUID"), 0, 200);

	FlashList();
}

CContentDataView::~CContentDataView()
{
}

void CContentDataView::OnSearch(wxCommandEvent& event)
{
	FlashList();
}

void CContentDataView::OnObjectActived(wxListEvent& event)
{
	long nSelectIndex = event.GetIndex();
	if(nSelectIndex==m_nSelectIndex) return;
	if(!CheckModify()) return;

	wxUIntPtr pData = m_pList->GetItemData(nSelectIndex);
	A_UUID& uuid = *(A_UUID*)pData;
	const DDLReflect::STRUCT_INFO* info = Atlas::ContentObject::GetObjectType(uuid);
	const A_CONTENT_OBJECT* object = Atlas::ContentObject::QueryByUUID(uuid, info);
	SetCurrentObject(nSelectIndex, info, object);
}

void CContentDataView::OnObjectAdd(wxCommandEvent& event)
{
	if(!CheckModify()) return;

	if(m_dlgGenerateObject.ShowModal()!=wxID_OK) return;
	const DDLReflect::STRUCT_INFO* info = Atlas::ContentObject::GetType(m_dlgGenerateObject.GetType().ToUTF8());
	A_UUID uuid;
	A_CONTENT_OBJECT* pObject = Atlas::ContentObject::Create(info, uuid);
	if(!pObject) return;

	pObject->name = (const char*)m_dlgGenerateObject.GetName().ToUTF8();
	SetCurrentObject(-1, info, pObject);
}

void CContentDataView::OnObjectSave(wxCommandEvent& event)
{
	if(!m_pCurInfo || !m_pCurData) return;

	A_CONTENT_OBJECT* data = Atlas::ContentObject::Modify(m_pCurData->uuid, m_pCurInfo);
	if(!m_pInfo->Get(m_pCurInfo, data))
	{
		wxMessageBox(wxT("invalid data or no select item"));
		return;
	}
}

void CContentDataView::OnObjectUndo(wxCommandEvent& event)
{
	if(!m_pCurInfo || !m_pCurData) return;
	m_pInfo->Set(m_pCurInfo, m_pCurData);
}

void CContentDataView::OnObjectRemove(wxCommandEvent& event)
{
	if(!m_pCurData) return;

	RemoveObject(m_pCurData->uuid);
	SetCurrentObjectNULL();
}

void CContentDataView::OnObjectReflush(wxCommandEvent& event)
{
	if(m_pCurInfo && m_pCurData)
	{
		m_pInfo->Set(m_pCurInfo, m_pCurData);
	}
}

void CContentDataView::OnIdle(wxIdleEvent& event)
{
	if(m_nSelectIndex>=0)
	{
		m_pList->SetItemState(m_nSelectIndex, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	}
	else
	{
		if(m_pList->GetSelectedItemCount())
		{
			m_pList->SetItemState(0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
			m_pList->SetItemState(0, wxLIST_STATE_SELECTED, 0);
		}
	}
}

void CContentDataView::OnMenuNew(wxCommandEvent& event)
{
	OnObjectAdd(event);
}

void CContentDataView::OnMenuCopy(wxCommandEvent& event)
{
	if(!m_pCurData) return;
	m_copyUUID = m_pCurData->uuid;
}

void CContentDataView::OnMenuPaste(wxCommandEvent& event)
{
	const A_CONTENT_OBJECT* pSrc = Atlas::ContentObject::QueryByUUID(m_copyUUID);
	if(!pSrc)
	{
		wxMessageBox(wxT("no copy instance exists"));
		return;
	}

	const DDLReflect::STRUCT_INFO* pInfo = Atlas::ContentObject::GetObjectType(m_copyUUID);
	if(!pInfo) return;

	A_UUID uuid;
	A_CONTENT_OBJECT* pDest = Atlas::ContentObject::Create(pInfo, uuid);
	if(!pDest)
	{
		wxMessageBox(wxT("no copy instance exists"));
		return;
	}

	memcpy(pDest, pSrc, pInfo->size);
	pDest->uuid = uuid;
}

void CContentDataView::OnMenuDel(wxCommandEvent& event)
{
	OnObjectRemove(event);
}

void CContentDataView::OnMenuSelAll(wxCommandEvent& event)
{
}

void CContentDataView::OnMenuSave(wxCommandEvent& event)
{
}

void CContentDataView::OnContextMenu(wxContextMenuEvent& event)
{
	if(event.GetEventObject() == m_pList)
	{
		wxMenu menu;
		menu.Append(wxID_NEW, wxT("N&ew"));
		menu.AppendSeparator();
		menu.Append(wxID_COPY, wxT("C&opy"));
		menu.Append(wxID_PASTE, wxT("P&aste"));
		menu.AppendSeparator();
		menu.Append(wxID_DELETE, wxT("D&el"));

		if(m_pList->GetSelectedItemCount()==0)
		{
			menu.Enable(wxID_COPY,false);
			menu.Enable(wxID_DELETE,false);
		}

		wxPoint pos = event.GetPosition();
		pos = m_pList->ScreenToClient(pos);
		m_pList->PopupMenu(&menu, pos.x, pos.y);
	}
}

void CContentDataView::UpdateItemName(wxString& strItemValue, A_UUID& uuid)
{
	for(long i = 0; i < m_pList->GetItemCount(); ++i)
	{
		wxUIntPtr pData = m_pList->GetItemData(i);
		A_UUID& item_uuid = *(A_UUID*)pData;
		if(item_uuid == uuid)
		{
			m_pList->SetItem(i, 0, strItemValue);
		}
	}
}

bool CContentDataView::CheckModify()
{
	if(!m_pCurInfo || !m_pCurData) return true;
	ATLAS_ASSERT(m_pCurInfo && m_pCurData);

	m_pInfo->Updata();

	A_CONTENT_OBJECT* obj = (A_CONTENT_OBJECT*)ATLAS_ALLOC(m_pCurInfo->size);
	if(!m_pInfo->Get(m_pCurInfo, obj))
	{
		ATLAS_FREE(obj);
		return false;
	}

	if(memcmp(m_pCurData, obj, (size_t)m_pCurInfo->size)==0)
	{
		ATLAS_FREE(obj);
		return true;
	}
	ATLAS_FREE(obj);

	if(wxMessageBox(wxT("save data?"), wxT("!!!"), wxYES_NO)!=wxYES) return true;

	A_CONTENT_OBJECT* data = Atlas::ContentObject::Modify(m_pCurData->uuid, m_pCurInfo);
	if(!m_pInfo->Get(m_pCurInfo, data))
	{
		wxMessageBox(wxT("xxxx"));
		return false;
	}

	return true;
}

void CContentDataView::AppendObject(const DDLReflect::STRUCT_INFO* info, const A_CONTENT_OBJECT* data)
{
	wxString strName(data->name._Value, wxMBConvUTF8());
	if(strName.empty()) strName = wxT("{NAME_EMPTY}");
	char szUUID[100];
	AUuidToString(data->uuid, szUUID);

	int n = m_pList->InsertItem(m_pList->GetItemCount(), strName);
	m_pList->SetItem(n, 1, wxString::FromUTF8(info->name));
	m_pList->SetItem(n, 2, wxString::FromUTF8(szUUID));
	m_pList->SetItemPtrData(n, (wxUIntPtr)&data->uuid);
}

void CContentDataView::RemoveObject(const A_UUID& uuid)
{
	for(long i = 0; i < m_pList->GetItemCount(); ++i)
	{
		wxUIntPtr pData = m_pList->GetItemData(i);
		A_UUID& item_uuid = *(A_UUID*)pData;
		if(item_uuid == uuid)
		{
			m_pList->DeleteItem(i);
		}
	}
	
	Atlas::ContentObject::Delete(uuid);
}

void CContentDataView::SelectObject(const A_UUID& uuid)
{
	for(long i = 0; i < m_pList->GetItemCount(); ++i)
	{
		wxUIntPtr pData = m_pList->GetItemData(i);
		A_UUID& item_uuid = *(A_UUID*)pData;
		if(item_uuid == uuid)
		{
			m_pList->SetItemState(i, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		}
	}
}

void CContentDataView::SetCurrentObjectNULL()
{
	m_nSelectIndex = -1;
	m_pCurInfo = NULL;
	m_pCurData = NULL;
	m_pInfo->Clear();
	m_pObjectName->SetValue(wxT(""));
}

void CContentDataView::SetCurrentObject(long nSelectObject, const DDLReflect::STRUCT_INFO* info, const A_CONTENT_OBJECT* data)
{
	char uuid[100];
	wxString name;
	name.Printf(wxT("%s %s UUID:"), wxString::FromUTF8(info->name), wxString::FromUTF8(data->name._Value));
	AUuidToString(data->uuid, uuid);
	name += wxString::FromUTF8(uuid);
	m_pObjectName->SetLabel(name);

	m_nSelectIndex = nSelectObject;
	m_pCurInfo = info;
	m_pCurData = data;

	m_pInfo->Set(info, data);
}

void CContentDataView::FlashList()
{
	m_pList->DeleteAllItems();
	SetCurrentObjectNULL();

	const DDLReflect::STRUCT_INFO* info = m_mapTypes[(const char*)m_pObjectType->GetValue().ToUTF8()];
	const A_CONTENT_OBJECT* object = Atlas::ContentObject::FindFirst(info, false);
	while(object)
	{
		AppendObject(Atlas::ContentObject::GetObjectType(object->uuid), object);
		object = Atlas::ContentObject::FindNext(info, false, object);
	}
}
