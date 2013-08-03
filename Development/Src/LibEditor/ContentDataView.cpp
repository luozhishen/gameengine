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

#include <ZionBase.h>
#include <ZionCommon.h>

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

	wxBoxSizer* pSizerTop = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	wxSplitterWindow* pSplitter = ZION_NEW wxSplitterWindow(this);

	// left
	wxPanel* pPanel1 = ZION_NEW wxPanel(pSplitter);
	wxBoxSizer* pSizerLeft = ZION_NEW wxBoxSizer(wxVERTICAL);

	wxBitmap bmpAdd;
	wxBitmap bmpRemove;
	wxBitmap bmpReflush;
	bmpAdd.LoadFile(wxT("../wxRes/Editor/Add.png"),			wxBITMAP_TYPE_PNG);
	bmpRemove.LoadFile(wxT("../wxRes/Editor/Remove.png"),		wxBITMAP_TYPE_PNG);
	bmpReflush.LoadFile(wxT("../wxRes/Editor/Reflush.png"),	wxBITMAP_TYPE_PNG);
	
	m_pObjectType = ZION_NEW wxComboBox(this, ID_OBJECT_TYPE, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0, NULL, wxCB_DROPDOWN|wxCB_READONLY);
	m_pSearchText = ZION_NEW wxTextCtrl(this, ID_SEARCH_TEXT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	wxButton* pSearchBtn = ZION_NEW wxButton(this, ID_SEARCH_BUTTON, wxT("Search"));

	pSizerTop->Add(m_pObjectType, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	pSizerTop->Add(m_pSearchText, 1, wxGROW|wxALIGN_LEFT|wxALL, 5);
	pSizerTop->Add(pSearchBtn, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_pList = ZION_NEW wxListCtrl(pPanel1, ID_OBJECT_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL);
	m_pSearchResult = ZION_NEW wxStaticText(pPanel1, wxID_ANY, wxT("NOT FOUND"));
	pSizerLeft->Add(m_pSearchResult, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	pSizerLeft->Add(m_pList, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL);
	pPanel1->SetSizer(pSizerLeft);
	
	// right
	wxPanel* pPanel2 = ZION_NEW wxPanel(pSplitter);
	wxBoxSizer* pSizer11 = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* pSizer13 = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* pSizerRight = ZION_NEW wxBoxSizer(wxVERTICAL);

	wxBitmap bmpSave;
	wxBitmap bmpUndo;
	bmpSave.LoadFile(wxT("../wxRes/Editor/Save.bmp"),			wxBITMAP_TYPE_BMP);
	bmpUndo.LoadFile(wxT("../wxRes/Editor/Undo.png"),			wxBITMAP_TYPE_PNG);

	m_pObjectName = ZION_NEW wxTextCtrl(pPanel2, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	m_pInfo = ZION_NEW CStructEditView(pPanel2);
	pSizerRight->Add(ZION_NEW wxStaticText(pPanel2, wxID_ANY, wxT("Object Edit")), 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	pSizer11->Add(m_pObjectName, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0);
	pSizer11->Add(ZION_NEW wxBitmapButton(pPanel2, ID_OBJECT_SAVE, bmpSave), 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0);
	pSizer11->Add(ZION_NEW wxBitmapButton(pPanel2, ID_OBJECT_REMOVE, bmpRemove), 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0);
	pSizer11->Add(ZION_NEW wxBitmapButton(pPanel2, ID_OBJECT_UNDO, bmpUndo), 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0);

	pSizerRight->Add(pSizer13, 0, wxGROW|wxALIGN_CENTER_VERTICAL);
	pSizerRight->AddSpacer(5);
	pSizerRight->Add(pSizer11, 0, wxGROW|wxALIGN_CENTER_VERTICAL);
	pSizerRight->AddSpacer(5);
	pSizerRight->Add(m_pInfo, 1, wxGROW|wxALIGN_CENTER_VERTICAL);

	pPanel2->SetSizer(pSizerRight);

	// main
	pSplitter->SplitVertically(pPanel1, pPanel2, 300);
	wxBoxSizer* pSizerAll = ZION_NEW wxBoxSizer(wxVERTICAL);
	pSizerAll->Add(ZION_NEW wxStaticText(this, wxID_ANY, wxT("Search")), 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxUP, 5);
	pSizerAll->Add(pSizerTop, 0, wxGROW|wxALIGN_CENTER_VERTICAL);
	pSizerAll->Add(ZION_NEW wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), 0, wxGROW|wxALIGN_CENTER_VERTICAL); 
	pSizerAll->Add(pSplitter, 1, wxGROW|wxALIGN_CENTER_VERTICAL);
	SetSizer(pSizerAll);

	Zion::Array<const DDLReflect::STRUCT_INFO*> list;
	Zion::ContentObject::GetTypeList(list);
	Zion::Array<const DDLReflect::STRUCT_INFO*>::iterator i;
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
	Zion::Map<Zion::String, const DDLReflect::STRUCT_INFO*>::iterator in;
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
	if(!CheckModify(false)) return;

	wxUIntPtr pData = m_pList->GetItemData(nSelectIndex);
	A_UUID& uuid = *(A_UUID*)pData;
	const DDLReflect::STRUCT_INFO* info = Zion::ContentObject::GetObjectType(uuid);
	const A_CONTENT_OBJECT* object = Zion::ContentObject::QueryByUUID(uuid, info);
	SetCurrentObject(nSelectIndex, info, object);
}

void CContentDataView::OnObjectAdd(wxCommandEvent& event)
{
	if(!CheckModify(false)) return;

	if(m_dlgGenerateObject.ShowModal()!=wxID_OK) return;
	const DDLReflect::STRUCT_INFO* info = Zion::ContentObject::GetType(m_dlgGenerateObject.GetType().ToUTF8());
	A_UUID uuid;
	A_CONTENT_OBJECT* pObject = Zion::ContentObject::CreateObject(info, uuid);
	if(!pObject) return;

	pObject->_name = (const char*)m_dlgGenerateObject.GetName().ToUTF8();
	SetCurrentObject(-1, info, pObject);
}

void CContentDataView::OnObjectSave(wxCommandEvent& event)
{
	if(!m_pCurInfo || !m_pCurData) return;

	A_CONTENT_OBJECT* data = Zion::ContentObject::Modify(m_pCurData->_uuid, m_pCurInfo);
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

	RemoveObject(m_pCurData->_uuid);
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
	m_copyUUID = m_pCurData->_uuid;
}

void CContentDataView::OnMenuPaste(wxCommandEvent& event)
{
	const A_CONTENT_OBJECT* pSrc = Zion::ContentObject::QueryByUUID(m_copyUUID);
	if(!pSrc)
	{
		wxMessageBox(wxT("no copy instance exists"));
		return;
	}

	const DDLReflect::STRUCT_INFO* pInfo = Zion::ContentObject::GetObjectType(m_copyUUID);
	if(!pInfo) return;

	A_UUID uuid;
	A_CONTENT_OBJECT* pDest = Zion::ContentObject::CreateObject(pInfo, uuid);
	if(!pDest)
	{
		wxMessageBox(wxT("no copy instance exists"));
		return;
	}

	memcpy(pDest, pSrc, pInfo->size);
	pDest->_uuid = uuid;
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

bool CContentDataView::CheckModify(bool bClear)
{
	if(!m_pCurInfo || !m_pCurData) return true;
	ZION_ASSERT(m_pCurInfo && m_pCurData);

	m_pInfo->Updata();

	A_CONTENT_OBJECT* obj = (A_CONTENT_OBJECT*)DDLReflect::CreateObject(m_pCurInfo);
	if(!m_pInfo->Get(m_pCurInfo, obj))
	{
		DDLReflect::DestoryObject(obj);
		return false;
	}

	if(memcmp(m_pCurData, obj, (size_t)m_pCurInfo->size)==0)
	{
		DDLReflect::DestoryObject(obj);
	}
	else
	{
		DDLReflect::DestoryObject(obj);

		if(wxMessageBox(wxT("save data?"), wxT("!!!"), wxYES_NO)!=wxYES)
		{
			return false;
		}

		A_CONTENT_OBJECT* data = Zion::ContentObject::Modify(m_pCurData->_uuid, m_pCurInfo);
		if(!m_pInfo->Get(m_pCurInfo, data))
		{
			wxMessageBox(wxT("error in ContentObject::Modify"));
			return false;
		}
	}

	if(bClear)
	{
		m_pList->ClearAll();
		SetCurrentObjectNULL();
	}

	return true;
}

void CContentDataView::AppendObject(const DDLReflect::STRUCT_INFO* info, const A_CONTENT_OBJECT* data)
{
	char szUUID[100];
	AUuidToString(data->_uuid, szUUID);
	int n = m_pList->InsertItem(m_pList->GetItemCount(), wxString::FromUTF8(data->_name._Value));
	m_pList->SetItem(n, 1, wxString::FromUTF8(info->name));
	m_pList->SetItem(n, 2, wxString::FromUTF8(szUUID));
	m_pList->SetItemPtrData(n, (wxUIntPtr)&data->_uuid);
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
	
	Zion::ContentObject::DeleteObject(uuid);
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
	name.Printf(wxT("%s %s UUID:"), wxString::FromUTF8(info->name), wxString::FromUTF8(data->_name._Value));
	AUuidToString(data->_uuid, uuid);
	name += wxString::FromUTF8(uuid);
	m_pObjectName->SetLabel(name);

	m_nSelectIndex = nSelectObject;
	m_pCurInfo = info;
	m_pCurData = data;

	m_pInfo->Set(info, data);
}

struct SEARCH_CONDITION
{
	Zion::String name;
	Zion::String value;
	const char* opt;
};
static char* opts[] = { "!=", "<=", ">=", "=", "<", ">" };

void CContentDataView::FlashList()
{
	m_pList->DeleteAllItems();
	SetCurrentObjectNULL();

	const DDLReflect::STRUCT_INFO* info = m_mapTypes[(const char*)m_pObjectType->GetValue().ToUTF8()];

	Zion::Array<SEARCH_CONDITION> conds;
	Zion::Array<Zion::String> condstrs;
	Zion::StringSplit((const char*)m_pSearchText->GetValue().ToUTF8(), ' ', condstrs);
	if(condstrs.size()>0)
	{
		for(size_t i=0; i<condstrs.size(); i++)
		{
			Zion::String& condstr = condstrs[i];
			size_t c;
			for(c=0; c<sizeof(opts)/sizeof(opts[0]); c++)
			{
				size_t offset = condstr.find(opts[c]);
				if(offset==Zion::String::npos) continue;

				SEARCH_CONDITION cond;
				cond.name = condstr.substr(0, offset);
				cond.value = condstr.substr(offset+strlen(opts[c]));
				cond.opt = opts[c];

				DDLReflect::FIELD_INFO finfo;
				if(DDLReflect::GetStructFieldOffset(info, cond.name.c_str(), &finfo)==(_U32)-1)
				{
					c = sizeof(opts)/sizeof(opts[0]);
					break;
				}

				if(finfo.type!=DDLReflect::TYPE_STRING && cond.value.empty())
				{
					c = sizeof(opts)/sizeof(opts[0]);
					break;
				}

				switch(finfo.type)
				{
				case DDLReflect::TYPE_STRING:
					if(strcmp(cond.opt, "!=")!=0 && strcmp(cond.opt, "=")!=0)
					{
						c = sizeof(opts)/sizeof(opts[0]);
					}
				case DDLReflect::TYPE_U8:
				case DDLReflect::TYPE_U16:
				case DDLReflect::TYPE_U32:
				case DDLReflect::TYPE_U64:
				case DDLReflect::TYPE_S8:
				case DDLReflect::TYPE_S16:
				case DDLReflect::TYPE_S32:
				case DDLReflect::TYPE_S64:
				case DDLReflect::TYPE_F32:
				case DDLReflect::TYPE_F64:
					break;
				default:
					c = sizeof(opts)/sizeof(opts[0]);
					break;
				}
				if(c==sizeof(opts)/sizeof(opts[0]))
				{
					break;
				}

				conds.push_back(cond);
				break;
			}
			if(c==sizeof(opts)/sizeof(opts[0]))
			{
				wxMessageBox(wxT("invalid input"), wxT("Error"));
				return;
			}
		}
	}

	const A_CONTENT_OBJECT* object = Zion::ContentObject::FindFirst(info, false);
	unsigned int count = 0;
	while(object)
	{
		bool skip = false;
		for(size_t c=0; c<conds.size() && !skip; c++)
		{
			Zion::String val;
			DDLReflect::FIELD_INFO finfo;
			if(!DDLReflect::StructParamToString(info, conds[c].name.c_str(), object, val, &finfo))
			{
				wxMessageBox(wxT("error in StructParamToString"), wxT("Error"));
				return;
			}

			if(strcmp(conds[c].opt, "!=")==0)
			{
				if(finfo.type==DDLReflect::TYPE_STRING)
				{
					if(val==conds[c].value) skip = true;
				}
				else if(finfo.type>=DDLReflect::TYPE_U8 && finfo.type<=DDLReflect::TYPE_S64)
				{
					if(atoi(val.c_str())==atoi(conds[c].value.c_str())) skip = true;
				}
				else if(finfo.type>=DDLReflect::TYPE_F32 && finfo.type<=DDLReflect::TYPE_F64)
				{
					if(atof(val.c_str())==atof(conds[c].value.c_str())) skip = true;
				}
			}
			else if(strcmp(conds[c].opt, "=")==0)
			{
				if(finfo.type==DDLReflect::TYPE_STRING)
				{
					if(val!=conds[c].value) skip = true;
				}
				else if(finfo.type>=DDLReflect::TYPE_U8 && finfo.type<=DDLReflect::TYPE_S64)
				{
					if(atoi(val.c_str())!=atoi(conds[c].value.c_str())) skip = true;
				}
				else if(finfo.type>=DDLReflect::TYPE_F32 && finfo.type<=DDLReflect::TYPE_F64)
				{
					if(atof(val.c_str())!=atof(conds[c].value.c_str())) skip = true;
				}
			}
			else if(strcmp(conds[c].opt, "<=")==0)
			{
				if(finfo.type>=DDLReflect::TYPE_U8 && finfo.type<=DDLReflect::TYPE_S64)
				{
					if(atoi(val.c_str())>atoi(conds[c].value.c_str())) skip = true;
				}
				else if(finfo.type>=DDLReflect::TYPE_F32 && finfo.type<=DDLReflect::TYPE_F64)
				{
					if(atof(val.c_str())>atof(conds[c].value.c_str())) skip = true;
				}
			}
			else if(strcmp(conds[c].opt, ">=")==0)
			{
				if(finfo.type>=DDLReflect::TYPE_U8 && finfo.type<=DDLReflect::TYPE_S64)
				{
					if(atoi(val.c_str())<atoi(conds[c].value.c_str())) skip = true;
				}
				else if(finfo.type>=DDLReflect::TYPE_F32 && finfo.type<=DDLReflect::TYPE_F64)
				{
					if(atof(val.c_str())<atof(conds[c].value.c_str())) skip = true;
				}
			}
			else if(strcmp(conds[c].opt, "<")==0)
			{
				if(finfo.type>=DDLReflect::TYPE_U8 && finfo.type<=DDLReflect::TYPE_S64)
				{
					if(atoi(val.c_str())>=atoi(conds[c].value.c_str())) skip = true;
				}
				else if(finfo.type>=DDLReflect::TYPE_F32 && finfo.type<=DDLReflect::TYPE_F64)
				{
					if(atof(val.c_str())>=atof(conds[c].value.c_str())) skip = true;
				}
			}
			else if(strcmp(conds[c].opt, ">")==0)
			{
				if(finfo.type>=DDLReflect::TYPE_U8 && finfo.type<=DDLReflect::TYPE_S64)
				{
					if(atoi(val.c_str())<=atoi(conds[c].value.c_str())) skip = true;
				}
				else if(finfo.type>=DDLReflect::TYPE_F32 && finfo.type<=DDLReflect::TYPE_F64)
				{
					if(atof(val.c_str())<=atof(conds[c].value.c_str())) skip = true;
				}
			}
		}
		if(!skip)
		{
			AppendObject(Zion::ContentObject::GetObjectType(object->_uuid), object);
			count++;
		}

		object = Zion::ContentObject::FindNext(info, false, object);
	}

	if(count>0)
	{
		m_pSearchResult->SetLabel(wxString::Format(wxT("Found %d Object"), count));
	}
	else
	{
		m_pSearchResult->SetLabel(wxT("Not found"));
	}
}
