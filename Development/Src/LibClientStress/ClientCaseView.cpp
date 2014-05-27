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
#include <mosdk.h>
#include <HttpConnection.h>

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
#include "ClientCaseView.h"

enum
{
	ID_CASEVIEW = wxID_HIGHEST + 1,
	ID_CASETIMER,
	ID_APPLY,
	ID_RELOAD
};

BEGIN_EVENT_TABLE(CClientCaseView, CStressFrameView)
	EVT_TIMER(ID_CASETIMER,	CClientCaseView::OnTimer)
	EVT_LIST_ITEM_FOCUSED(ID_CASEVIEW, CClientCaseView::OnClientSelected)
END_EVENT_TABLE()

CClientCaseView::CClientCaseView( CClientStressFrame* pFrame, wxWindow* pParent ) : CStressFrameView(pFrame, pParent, wxT("Case Status")), m_pCurrentClient(NULL)
{
	m_pCurrentCase = NULL;

	wxBoxSizer* pSizer1 = ZION_NEW wxBoxSizer(wxVERTICAL);
	m_pConfig = ZION_NEW CStructEditView(this);
	m_pStatus = ZION_NEW CStructEditView(this);
	pSizer1->Add(m_pConfig, 1, wxGROW|wxALIGN_CENTER_VERTICAL);
	pSizer1->Add(m_pStatus, 1, wxGROW|wxALIGN_CENTER_VERTICAL);

	m_pListCtrl = ZION_NEW wxListCtrl(this, ID_CASEVIEW, wxDefaultPosition, wxSize(100, 10), wxLC_LIST|wxLC_SINGLE_SEL);

	wxBoxSizer* pSizer = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	pSizer->Add(m_pListCtrl, 0, wxGROW|wxALIGN_CENTER_VERTICAL);
	pSizer->Add(pSizer1, 1, wxGROW|wxALIGN_CENTER_VERTICAL);

	SetSizer(pSizer);

	m_StressTimer.SetOwner(this, ID_CASETIMER);
	m_StressTimer.Start(1000);
}

CClientCaseView::~CClientCaseView()
{
}

void CClientCaseView::Flush(bool full)
{
	if(!m_pCurrentClient) return;

	if(full)
	{
		m_pListCtrl->DeleteAllItems();
		SelectCase(NULL);

		Zion::Set<Zion::CStressCase*> cases;
		m_pCurrentClient->GetStressCases(cases);
		Zion::Set<Zion::CStressCase*>::iterator i;
		for(i=cases.begin(); i!=cases.end(); i++)
		{
			long n = m_pListCtrl->InsertItem(m_pListCtrl->GetItemCount(), wxString::FromUTF8((*i)->GetName().c_str()));

			if((*i)->GetName()==m_CurrentCaseName)
			{
				wxListItem item;
				item.m_itemId = n;
				item.m_mask = wxLIST_MASK_STATE;
				item.m_state = wxLIST_STATE_SELECTED|wxLIST_STATE_FOCUSED;
				item.m_stateMask = wxLIST_STATE_SELECTED|wxLIST_STATE_FOCUSED;
				m_pListCtrl->SetItem(item);
			}
		}

		SelectCase(m_CurrentCaseName.c_str());
	}
	else
	{
		if(m_pCurrentCase && m_pCurrentCase->GetStatusType())
		{
			m_pStatus->Set(m_pCurrentCase->GetStatusType(), m_pCurrentCase->GetStatusData());
		}
	}
}

void CClientCaseView::OnSwitchTo(_U32 index)
{
	m_pCurrentClient = Zion::CStressManager::Get().GetClient(index);
	m_pCurrentCase = NULL;
	Flush(true);
}

void CClientCaseView::OnClear()
{
	m_pListCtrl->DeleteAllItems();
	m_pCurrentClient = NULL;
}

void CClientCaseView::OnNewCase(_U32 index, Zion::CStressCase* pCase)
{
	Flush(true);
}

void CClientCaseView::SelectCase(const char* name)
{
	if(m_pCurrentCase!=NULL && name!=NULL && m_pCurrentCase->GetName()==name) return;

	m_pConfig->Clear();
	m_pStatus->Clear();

	if(!name) return;
	m_pCurrentCase = m_pCurrentClient->GetStressCase(name);
	if(!m_pCurrentCase) return;
	m_CurrentCaseName = name;

	if(m_pCurrentCase->GetConfigType())
	{
		m_pConfig->Set(m_pCurrentCase->GetConfigType(), (void*)m_pCurrentCase->GetConfigData());
	}

	if(m_pCurrentCase->GetStatusType())
	{
		m_pStatus->Set(m_pCurrentCase->GetStatusType(), m_pCurrentCase->GetStatusData());
	}
}

void CClientCaseView::OnTimer(wxTimerEvent& event)
{
	Flush(false);
}

void CClientCaseView::OnClientSelected(wxListEvent& event)
{
	SelectCase((const char*)event.GetItem().GetText().ToUTF8());
}

void OnCaseSelected(wxListEvent& event)
{
}
