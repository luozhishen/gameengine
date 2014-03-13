#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/app.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/config.h>
#include <wx/utils.h>
#include <wx/statline.h>

#include <ZionBase.h>
#include <ZionCommon.h>
#include <ZionClient.h>
#include <DataSyncClient.h>

#include <wx/listctrl.h>
#include <wx/wxchar.h>
#include <wx/window.h>

#include <StressClient.h>
#include <StressCase.h>
#include <StressManager.h>
#include <StructEditView.h>
#include <StressLoader.h>

#include "ClientStressApp.h"
#include "ClientStressFrame.h"
#include "ClientDataView.h"

class CNewObjectDlg : public wxDialog
{	
	DECLARE_EVENT_TABLE()
public:
	CNewObjectDlg();
	virtual ~CNewObjectDlg();

	void InitCombox();
	wxString GetType();

    virtual int ShowModal();

	void OnOK(wxCommandEvent& event);

private:
	wxComboBox* m_comboBoxType;
	wxButton* m_sdbSizerOK;
	wxButton* m_sdbSizerCancel;
};

BEGIN_EVENT_TABLE(CNewObjectDlg, wxDialog)
	EVT_BUTTON(wxID_OK, CNewObjectDlg::OnOK)
END_EVENT_TABLE()

CNewObjectDlg::CNewObjectDlg() : wxDialog(NULL, wxID_ANY, wxT("Create Live Object"), wxDefaultPosition, wxSize(365, 150), wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER)
{
//	SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bMainSizer = ZION_NEW wxBoxSizer( wxVERTICAL );

	wxStaticText* staticText2 = ZION_NEW wxStaticText( this, wxID_ANY, wxT("Choose Content Type"), wxDefaultPosition, wxDefaultSize, 0 );
	staticText2->Wrap( -1 );
	bMainSizer->Add( staticText2, 0, wxEXPAND | wxALL, 5 );
	m_comboBoxType = ZION_NEW wxComboBox( this, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL,  wxCB_READONLY); 
	bMainSizer->Add( m_comboBoxType, 0, wxEXPAND | wxALL, 5 );
	wxStaticLine* m_staticline1 = ZION_NEW wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bMainSizer->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	wxStdDialogButtonSizer* sdbSizer1 = ZION_NEW wxStdDialogButtonSizer();
	wxButton* m_sdbSizerOK = ZION_NEW wxButton( this, wxID_OK );
	sdbSizer1->AddButton( m_sdbSizerOK );
	wxButton* m_sdbSizerCancel = ZION_NEW wxButton( this, wxID_CANCEL );
	sdbSizer1->AddButton( m_sdbSizerCancel );
	sdbSizer1->Realize();

	bMainSizer->Add( sdbSizer1, 0, wxEXPAND, 5 );
	SetSizer( bMainSizer );
	Layout();

	InitCombox();
}


CNewObjectDlg::~CNewObjectDlg()
{
}

void CNewObjectDlg::InitCombox()
{
	Zion::Array<const DDLReflect::STRUCT_INFO*> list;
	Zion::LiveObject::GetTypeList(list);
	Zion::Array<const DDLReflect::STRUCT_INFO*>::iterator i;
	for(i=list.begin(); i!=list.end(); i++)
	{
		m_comboBoxType->Insert(wxString::FromUTF8((*i)->name), m_comboBoxType->GetCount());
	}

	m_comboBoxType->SetSelection(0);
}

wxString CNewObjectDlg::GetType()
{
	wxString strRet = m_comboBoxType->GetStringSelection();
	return strRet;
}

int CNewObjectDlg::ShowModal()
{
	return wxDialog::ShowModal();
}

void CNewObjectDlg::OnOK(wxCommandEvent& event)
{
	event.Skip();
}

CClientDataSyncProxy::CClientDataSyncProxy(CClientDataView* pDataView, _U32 index)
{
	m_pDataView = pDataView;
	m_nIndex = index;

	Zion::CStressManager::Get().GetClient(index)->GetClient()->GetDataSync()->_OnSyncOpen.connect(this, &CClientDataSyncProxy::OnSyncOpen);
	Zion::CStressManager::Get().GetClient(index)->GetClient()->GetDataSync()->_OnSyncClose.connect(this, &CClientDataSyncProxy::OnSyncClose);
	Zion::CStressManager::Get().GetClient(index)->GetClient()->GetDataSync()->_OnObjectCreate.connect(this, &CClientDataSyncProxy::OnObjectCreate);
	Zion::CStressManager::Get().GetClient(index)->GetClient()->GetDataSync()->_OnObjectUpdate.connect(this, &CClientDataSyncProxy::OnObjectUpdate);
	Zion::CStressManager::Get().GetClient(index)->GetClient()->GetDataSync()->_OnObjectDelete.connect(this, &CClientDataSyncProxy::OnObjectDelete);
}

void CClientDataSyncProxy::OnSyncOpen()
{
	m_pDataView->OnSyncOpen(m_nIndex);
}

void CClientDataSyncProxy::OnSyncClose()
{
	m_pDataView->OnSyncClose(m_nIndex);
}

void CClientDataSyncProxy::OnObjectCreate(const A_UUID& _uuid)
{
	m_pDataView->OnObjectCreate(m_nIndex, _uuid);
}

void CClientDataSyncProxy::OnObjectUpdate(const A_UUID& _uuid)
{
	m_pDataView->OnObjectUpdate(m_nIndex, _uuid);
}

void CClientDataSyncProxy::OnObjectDelete(const A_UUID& _uuid)
{
	m_pDataView->OnObjectDelete(m_nIndex, _uuid);
}

enum
{
	ID_DATAVIEW = wxID_HIGHEST + 1,
	ID_DATALIST,
	ID_DATA_SYNC,
	ID_DATA_OBJECT_NEW,
	ID_DATA_OBJECT_SAVE,
	ID_DATA_OBJECT_CANCEL,
};

BEGIN_EVENT_TABLE(CClientDataView, CStressFrameView)
	EVT_LIST_ITEM_SELECTED(ID_DATALIST, CClientDataView::OnObjectClick)
	EVT_BUTTON(ID_DATA_SYNC,			CClientDataView::OnSyncClick)
	EVT_BUTTON(ID_DATA_OBJECT_NEW,		CClientDataView::OnNewClick)
	EVT_BUTTON(ID_DATA_OBJECT_SAVE,		CClientDataView::OnSaveClick)
	EVT_BUTTON(ID_DATA_OBJECT_CANCEL,	CClientDataView::OnCancelClick)
END_EVENT_TABLE()

CClientDataView::CClientDataView( CClientStressFrame* pFrame, wxWindow* pParent ) : CStressFrameView(pFrame, pParent, wxT("Data View"))
{
	wxSplitterWindow* pSplitter = ZION_NEW wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN);
    pSplitter->SetSashGravity(0);

	m_pDataList = ZION_NEW wxListCtrl(pSplitter, ID_DATALIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL);
	m_pDataList->InsertColumn(0, wxT("UUID"), 0, 100);
	m_pDataList->InsertColumn(1, wxT("Type"), 0, 100);
	m_pDataList->InsertColumn(2, wxT("Value"), 0, wxLIST_AUTOSIZE);
	wxPanel* pBottomPanel = ZION_NEW wxPanel(pSplitter);
	pSplitter->SplitHorizontally(m_pDataList, pBottomPanel, 100);

	wxBoxSizer* pPanelSizer = ZION_NEW wxBoxSizer(wxVERTICAL);
	wxBoxSizer* pBarSizer = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	pBarSizer->Add(ZION_NEW wxButton(pBottomPanel, ID_DATA_SYNC, wxT("Sync")), 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 0);
	pBarSizer->Add(ZION_NEW wxButton(pBottomPanel, ID_DATA_OBJECT_NEW, wxT("New")), 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 0);
	pBarSizer->Add(ZION_NEW wxButton(pBottomPanel, ID_DATA_OBJECT_SAVE, wxT("Save")), 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 0);
	pBarSizer->Add(ZION_NEW wxButton(pBottomPanel, ID_DATA_OBJECT_CANCEL, wxT("Cancel")), 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 0);
	pBarSizer->Add(10, 10);
	m_pBarText = ZION_NEW wxStaticText(pBottomPanel, wxID_ANY, wxT(""));
	pBarSizer->Add(m_pBarText, 1, wxGROW|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 0);

	pPanelSizer->Add(pBarSizer, 0, wxGROW|wxALIGN_CENTER_VERTICAL);
	m_pDataView = ZION_NEW CStructEditView(pBottomPanel);
	pPanelSizer->Add(m_pDataView, 1, wxGROW|wxALIGN_CENTER_VERTICAL);
	pBottomPanel->SetSizer(pPanelSizer);

	pPanelSizer = ZION_NEW wxBoxSizer(wxVERTICAL);
	pPanelSizer->Add(pSplitter, 1, wxGROW|wxALIGN_CENTER_VERTICAL);
	SetSizer(pPanelSizer);

	m_ObjectType = NULL;
	m_pObjectData = NULL;
}

CClientDataView::~CClientDataView()
{
	while(!m_ProxyMap.empty())
	{
		ZION_DELETE m_ProxyMap.begin()->second;
		m_ProxyMap.erase(m_ProxyMap.begin());
	}
}

void CClientDataView::OnSyncClick(wxCommandEvent& event)
{
	_U32 index = GetCurrentClient();
	if(index==(_U32)-1) return;
	Zion::CDataSyncClient* pClient = Zion::CStressManager::Get().GetClient(index)->GetClient()->GetDataSync();
	if(pClient->GetSyncFlag()==(_U32)-1) return;
	if((pClient->GetSyncFlag()&SYNCFLAG_CLIENT)==0) return;
	pClient->Sync();
}

void CClientDataView::OnNewClick(wxCommandEvent& event)
{
	_U32 index = GetCurrentClient();
	if(index==(_U32)-1) return;
	Zion::CDataSyncClient* pClient = Zion::CStressManager::Get().GetClient(index)->GetClient()->GetDataSync();
	if(pClient->GetSyncFlag()==(_U32)-1) return;
	if((pClient->GetSyncFlag()&SYNCFLAG_CLIENT)==0) return;

	CNewObjectDlg Dlg;
	if(Dlg.ShowModal()!=wxID_OK) return;

	const DDLReflect::STRUCT_INFO* pType;
	pType = Zion::LiveObject::GetType((const char*)Dlg.GetType().ToUTF8());

	SetEditObject(NULL, pType, NULL);
}

void CClientDataView::OnSaveClick(wxCommandEvent& event)
{
	_U32 index = GetCurrentClient();
	if(index==(_U32)-1) return;
	Zion::CDataSyncClient* pClient = Zion::CStressManager::Get().GetClient(index)->GetClient()->GetDataSync();

	if(pClient->GetSyncFlag()==(_U32)-1) return;
	if((pClient->GetSyncFlag()&SYNCFLAG_CLIENT)==0) return;
	if(!m_pObjectData) return;
	if(m_IsObjectNew)
	{
		A_LIVE_OBJECT* pData = pClient->CreateObject(m_ObjectType);
		memcpy(pData, m_pObjectData, m_ObjectType->size);
		ClearEditor();
	}
	else
	{
		Zion::LiveData::CObject* pObject = pClient->GetObject(m_ObjectUUID);
		pObject->SetData(m_pObjectData);
	}
}

void CClientDataView::OnCancelClick(wxCommandEvent& event)
{
	if(GetCurrentClient()==(_U32)-1) return;
	if(!m_pObjectData) return;
	ClearEditor();
}

void CClientDataView::OnObjectClick(wxListEvent& event)
{
	ClearEditor();

	long nSelectIndex = event.GetIndex();
	wxUIntPtr Ptr = m_pDataList->GetItemData(nSelectIndex);
	Zion::LiveData::CObject* pObject = (Zion::LiveData::CObject*)Ptr;

	SetEditObject(&pObject->GetUUID("_uuid"), pObject->GetStructInfo(), (A_LIVE_OBJECT*)pObject->GetData());
}

void CClientDataView::OnSwitchTo(_U32 index)
{
	ClearEditor();
//	m_pDataList->ClearAll();
	Zion::CDataSyncClient* pClient = Zion::CStressManager::Get().GetClient(index)->GetClient()->GetDataSync();
	if(pClient->GetSyncFlag()==(_U32)-1) return;
	if(!pClient->IsReady()) return;

	Zion::LiveData::CObject* pObject;
	pObject = pClient->GetAccesser().FindFirst();
	while(pObject)
	{
		char suuid[100];
		AUuidToString(pObject->GetUUID("_uuid"), suuid);
		long id = m_pDataList->InsertItem(m_pDataList->GetItemCount(), wxString::FromUTF8(suuid));
		m_pDataList->SetItem(id, 1, wxString::FromUTF8(pObject->GetStructInfo()->name));
		m_pDataList->SetItem(id, 2, wxT(""));
		m_pDataList->SetItemPtrData(id, (wxUIntPtr)pObject);

		pObject = pClient->GetAccesser().FindNext(pObject);
	}

}

void CClientDataView::OnClear()
{
}

void CClientDataView::OnNewClient(_U32 index)
{
	if(m_ProxyMap.find(index)==m_ProxyMap.end())
	{
		CClientDataSyncProxy* pProxy = ZION_NEW CClientDataSyncProxy(this, index);
		m_ProxyMap[index] = pProxy;
	}
}

void CClientDataView::OnNewCase(_U32 index, Zion::CStressCase* pCase)
{
}

void CClientDataView::OnSyncOpen(_U32 nIndex)
{
	OnSwitchTo(GetCurrentClient());
}

void CClientDataView::OnSyncClose(_U32 nIndex)
{
	OnSwitchTo(GetCurrentClient());
}

void CClientDataView::OnObjectCreate(_U32 nIndex, const A_UUID& _uuid)
{
	OnSwitchTo(GetCurrentClient());
}

void CClientDataView::OnObjectUpdate(_U32 nIndex, const A_UUID& _uuid)
{
	OnSwitchTo(GetCurrentClient());
}

void CClientDataView::OnObjectDelete(_U32 nIndex, const A_UUID& _uuid)
{
	OnSwitchTo(GetCurrentClient());
}

void CClientDataView::SetEditObject(const A_UUID* pUUID, const DDLReflect::STRUCT_INFO*	pType, A_LIVE_OBJECT* pData)
{
	ClearEditor();

	if(pUUID)
	{
		m_IsObjectNew = false;
		m_ObjectUUID = *pUUID;
	}
	else
	{
		m_IsObjectNew = true;
	}

	m_ObjectType = pType;
	m_pObjectData = (A_LIVE_OBJECT*)DDLReflect::CreateObject(pType);
	if(pData) memcpy(m_pObjectData, pData, (size_t)pType->size);
	m_pDataView->Set(m_ObjectType, m_pObjectData);
}

void CClientDataView::ClearEditor()
{
	m_pDataView->Clear();
	if(m_pObjectData) DDLReflect::DestoryObject(m_ObjectType, m_pObjectData);
	m_ObjectType = NULL;
	m_pObjectData = NULL;
	m_pBarText->SetLabel(wxT(""));
}
