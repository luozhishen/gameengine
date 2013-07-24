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

enum
{
	ID_CASEVIEW = wxID_HIGHEST + 1,
	ID_CASETIMER,
};

BEGIN_EVENT_TABLE(CClientCaseView, CStressFrameView)
	EVT_TIMER(ID_CASETIMER,	CClientCaseView::OnTimer)
END_EVENT_TABLE()

CClientCaseView::CClientCaseView( CClientStressFrame* pFrame, wxWindow* pParent ) : CStressFrameView(pFrame, pParent, wxT("Case Status")), m_pCurrentClient(NULL)
{
	m_pListCtrl = ZION_NEW wxListCtrl(this, ID_CASEVIEW, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	m_pListCtrl->SetBackgroundColour(*wxWHITE);
	m_pListCtrl->InsertColumn(0, wxT("name"), 0, 200);
	m_pListCtrl->InsertColumn(1, wxT("status"), 0, 600);

	wxBoxSizer* pSizer = ZION_NEW wxBoxSizer(wxVERTICAL);
	pSizer->Add(m_pListCtrl, 1, wxGROW|wxALIGN_CENTER_VERTICAL);
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
			m_pListCtrl->SetItem(n, 1, wxString::FromUTF8((*i)->GetStatusInfo().c_str()));
		}
	}
	else
	{
		for(int i=0; i<m_pListCtrl->GetItemCount(); i++)
		{
			Zion::CStressCase* pCase = m_pCurrentClient->GetStressCase((const char*)m_pListCtrl->GetItemText(i).ToUTF8());
			if(!pCase) continue;
			m_pListCtrl->SetItem(i, 1, wxString::FromUTF8(pCase->GetStatusInfo().c_str()));
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
