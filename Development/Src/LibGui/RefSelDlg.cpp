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

#include "RefSelDlg.h"

enum
{
	ID_REF_TEXT = wxID_HIGHEST + 1,
	ID_REF_LIST,
};

BEGIN_EVENT_TABLE(CRefSelDlg, wxDialog)
	EVT_LIST_ITEM_SELECTED(ID_REF_LIST, CRefSelDlg::OnListSelect)
	EVT_BUTTON(wxID_OK,			CRefSelDlg::OnConfirm)
	EVT_TEXT(ID_REF_TEXT, CRefSelDlg::OnTextInput)
END_EVENT_TABLE()

CRefSelDlg::CRefSelDlg(wxWindow* pParent, wxString wxRefType) : wxDialog(pParent, wxID_ANY, wxT("Ref Select"), wxDefaultPosition, wxSize(600, 300), wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER)
{
	m_pTextCtrl = ZION_NEW wxTextCtrl(this, ID_REF_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT);

	wxBoxSizer* pSizer2 = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	pSizer2->AddStretchSpacer();
	pSizer2->Add(ZION_NEW wxButton(this, wxID_OK, wxT("Confirm")), 0, wxALIGN_RIGHT | wxALL, 5);
	pSizer2->Add(ZION_NEW wxButton(this, wxID_CANCEL, wxT("Cancel")), 0, wxALIGN_RIGHT | wxALL, 5);

	wxBoxSizer* pSizer1 = ZION_NEW wxBoxSizer(wxVERTICAL);
	pSizer1->Add(m_pTextCtrl, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pUUIDList = ZION_NEW wxListCtrl(this, ID_REF_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	pSizer1->Add(m_pUUIDList, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	pSizer1->Add(pSizer2, 0, wxGROW|wxALIGN_CENTER_VERTICAL);
	SetSizer(pSizer1);

	m_strRefType = wxRefType;
	m_pUUIDList->InsertColumn(0, wxT("Name"), 0, 120);
	m_pUUIDList->InsertColumn(1, wxT("UUID"), 0, 800);

	AppendItem(m_strRefType);
}

CRefSelDlg::~CRefSelDlg()
{
}

void CRefSelDlg::AppendItem(wxString& wxRefType, wxString strInput)
{
	Zion::Array<const DDLReflect::STRUCT_INFO*> list;
	Zion::ContentObject::GetTypeList(list);
	Zion::Array<const DDLReflect::STRUCT_INFO*>::iterator i;
	const DDLReflect::STRUCT_INFO* info = NULL;
	for(i=list.begin(); i!=list.end(); i++)
	{
		info = *i;
		while(info)
		{
			if(wxRefType==wxString::FromUTF8(info->name))
			{
				break;
			}

			info = info->parent;
		}

		if(info && wxRefType.c_str()==wxString::FromUTF8(info->name))
		{
			break;
		}
	}

	if(info == NULL) return;

	const A_CONTENT_OBJECT* object = Zion::ContentObject::FindFirst(info, true);
	while(object)
	{
		wxString strNameValue(object->name._Value, wxMBConvUTF8());
		if(!strInput.empty() && strNameValue.find(strInput) == wxNOT_FOUND)
		{
			continue;
		}

		long item_id = m_pUUIDList->InsertItem(m_pUUIDList->GetItemCount(), wxString::FromUTF8(object->name._Value));

		char szUUID[128];
		AUuidToString(object->uuid, szUUID);
		m_pUUIDList->SetItem(item_id, 1, wxString::FromUTF8(szUUID));

		wxUIntPtr itemData = (wxUIntPtr)(&(object->uuid));
		m_pUUIDList->SetItemPtrData(item_id, itemData);

		object = Zion::ContentObject::FindNext(info, true, object);
	}
}

void CRefSelDlg::OnConfirm(wxCommandEvent& event)
{
	EndDialog(wxID_OK);
}

void CRefSelDlg::OnListSelect(wxListEvent& event)
{
	_U32 index = (_U32)event.GetItem().GetId();
	
	if(index < (_U32)m_pUUIDList->GetItemCount())
	{
		wxUIntPtr itemdata = m_pUUIDList->GetItemData(index);		

		char uuid[1000];
		AUuidToString(*((A_UUID*)itemdata), uuid);
		m_strSel = wxString(uuid, wxMBConvUTF8());
	}
}

void CRefSelDlg::OnTextInput(wxCommandEvent& event)
{
	wxTextCtrl *text = (wxTextCtrl *)event.GetEventObject();
	wxString strValue = text->GetValue();

	m_pUUIDList->DeleteAllItems();
	AppendItem(m_strRefType, strValue);
}