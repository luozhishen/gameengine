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

#include <wx/listctrl.h>
#include <wx/wxchar.h>
#include <wx/window.h>

#include "ClientStressApp.h"
#include "ClientStressFrame.h"
#include "ClientCaseView.h"

#include <StressClient.h>
#include <StressCase.h>
#include <StressManager.h>
#include <StructEditView.h>

enum
{
	ID_CASEVIEW = wxID_HIGHEST + 1,
	ID_CASETIMER,
	ID_APPLY,
	ID_RELOAD
};

BEGIN_EVENT_TABLE(CClientCaseView, CStressFrameView)
	EVT_TIMER(ID_CASETIMER,	CClientCaseView::OnTimer)
END_EVENT_TABLE()

CClientCaseView::CClientCaseView( CClientStressFrame* pFrame, wxWindow* pParent ) : CStressFrameView(pFrame, pParent, wxT("Case Status")), m_pCurrentClient(NULL)
{

	wxBoxSizer* pSizer1 = ZION_NEW wxBoxSizer(wxVERTICAL);
	m_pConfig = ZION_NEW CStructEditView(this);
	m_pStatus = ZION_NEW CStructEditView(this);
	pSizer1->Add(m_pConfig, 1, wxGROW|wxALIGN_CENTER_VERTICAL);
	pSizer1->Add(m_pStatus, 1, wxGROW|wxALIGN_CENTER_VERTICAL);

	m_pListCtrl = ZION_NEW wxListCtrl(this, ID_CASEVIEW, wxDefaultPosition, wxSize(100, 10));

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

		Zion::Set<Zion::CStressCase*> cases;
		m_pCurrentClient->GetStressCases(cases);
		Zion::Set<Zion::CStressCase*>::iterator i;
		for(i=cases.begin(); i!=cases.end(); i++)
		{
			int n = m_pListCtrl->InsertItem(m_pListCtrl->GetItemCount(), wxString::FromUTF8((*i)->GetName().c_str()));
		}
	}
	else
	{
		for(int i=0; i<m_pListCtrl->GetItemCount(); i++)
		{
			Zion::CStressCase* pCase = m_pCurrentClient->GetStressCase((const char*)m_pListCtrl->GetItemText(i).ToUTF8());
			if(!pCase) continue;
		}
	}
}

void CClientCaseView::OnSwitchTo(_U32 index)
{
	m_pCurrentClient = Zion::CStressManager::Get().GetClient(index);
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

void CClientCaseView::OnTimer(wxTimerEvent& event)
{
	Flush(false);
}
