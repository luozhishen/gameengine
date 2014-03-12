#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/app.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/config.h>
#include <wx/utils.h>

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
	OnClear();
}

void CClientDataView::OnSaveClick(wxCommandEvent& event)
{
	if(GetCurrentClient()==(_U32)-1) return;
}

void CClientDataView::OnCancelClick(wxCommandEvent& event)
{
	if(GetCurrentClient()==(_U32)-1) return;
}

void CClientDataView::OnSwitchTo(_U32 index)
{
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
}

void CClientDataView::OnSyncClose(_U32 nIndex)
{
}

void CClientDataView::OnObjectCreate(_U32 nIndex, const A_UUID& _uuid)
{
}

void CClientDataView::OnObjectUpdate(_U32 nIndex, const A_UUID& _uuid)
{
}

void CClientDataView::OnObjectDelete(_U32 nIndex, const A_UUID& _uuid)
{
}
