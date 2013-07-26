#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/config.h>
#include <wx/utils.h>

#include <ZionBase.h>
#include <StressManager.h>
#include <StressCase.h>
#include <StressClient.h>

#include "StressViewDlg.h"
#include <StructEditView.h>

class StreesViewItemData : public wxTreeItemData
{
public:
	StreesViewItemData(_U32 uid, Zion::String name="")
	{
		m_uid = uid;
		m_name = name;
	}

	_U32 m_uid;
	Zion::String m_name;
};

enum
{
	ID_LEFT_TREE = wxID_HIGHEST + 1,
	ID_SVD_TIMER,
};

BEGIN_EVENT_TABLE(CStressViewDlg, wxDialog)
	EVT_TREE_SEL_CHANGED(ID_LEFT_TREE, CStressViewDlg::OnSelect)
	EVT_TIMER(ID_SVD_TIMER, CStressViewDlg::OnTimer)
END_EVENT_TABLE()

#include <ZionCommon.h>

CStressViewDlg::CStressViewDlg(wxWindow* pParent) : wxDialog(pParent, wxID_ANY, wxString(wxT("Stress View")), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER)
{
	m_Timer.SetOwner(this, ID_SVD_TIMER);

	SetTitle(GetTitle() + wxT(" for ") + wxString::FromUTF8(Zion::ZionGameName()));

	wxNotebook* pViewTab = ZION_NEW wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP);
	wxPanel* pClientPanel = ZION_NEW wxPanel(pViewTab);

	m_pClientTree = ZION_NEW wxTreeCtrl(pClientPanel, ID_LEFT_TREE, wxDefaultPosition, wxSize(180, -1));
	m_pConfig = ZION_NEW CStructEditView(pClientPanel);
	m_pStatus = ZION_NEW CStructEditView(pClientPanel);

	wxBoxSizer* pSizer1 = ZION_NEW wxBoxSizer(wxVERTICAL);
	pSizer1->Add(m_pConfig, 1, wxGROW|wxALIGN_CENTER|wxALL, 5);
	pSizer1->Add(m_pStatus, 1, wxGROW|wxALIGN_CENTER|wxALL, 5);
	
	wxBoxSizer* pClientRoot = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	pClientRoot->Add(m_pClientTree, 0, wxGROW|wxALIGN_CENTER|wxALL, 5);
	pClientRoot->Add(pSizer1, 1, wxGROW|wxALIGN_CENTER);
	pClientPanel->SetSizer(pClientRoot);
	
	pViewTab->AddPage(pClientPanel, wxT("Clients"));

	wxBoxSizer* pSizerRoot = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	pSizerRoot->Add(pViewTab, 1, wxGROW|wxALIGN_CENTER|wxALL, 5);
	SetSizer(pSizerRoot);

	InitClients();

	m_Timer.Start(100);
}

CStressViewDlg::~CStressViewDlg()
{
	m_pClientTree->DeleteAllItems();
}

void CStressViewDlg::InitClients()
{
	Zion::CStressManager& stressMgr = Zion::CStressManager::Get();
	Zion::Array<_U32> clients;
	stressMgr.GetClients(clients);
	if(clients.empty()) return;

	wxTreeItemId rootItem = m_pClientTree->AddRoot(wxT("Clients"));

	for(_U32 i=0; i<clients.size(); ++i)
	{
		Zion::CStressClient* pClient = stressMgr.GetClient(clients[i]);
		if(pClient==NULL) continue;

		wxTreeItemId clientItem = m_pClientTree->AppendItem(rootItem, wxString::FromUTF8(pClient->GetTitle().c_str()), -1, -1);
		m_pClientTree->SetItemData(clientItem, ZION_NEW StreesViewItemData(pClient->GetIndex()));

		Zion::Set<Zion::CStressCase*> cases;
		pClient->GetStressCases(cases);
		if(cases.empty()) continue;

		Zion::Set<Zion::CStressCase*>::iterator it;
		for(it=cases.begin(); it!=cases.end(); ++it)
		{
			wxTreeItemId caseItem = m_pClientTree->AppendItem(clientItem, wxString::FromUTF8((*it)->GetName().c_str()), -1, -1);
			StreesViewItemData* castData = ZION_NEW StreesViewItemData(pClient->GetIndex(), (*it)->GetName());
			m_pClientTree->SetItemData(caseItem, (wxTreeItemData*)castData);
		}
	}
}

void CStressViewDlg::OnTimer(wxTimerEvent& event)
{
	Zion::CStressClient* client = Zion::CStressManager::Get().GetClient(m_Index);
	if(!client) return;

	Zion::CStressCase* scase = client->GetStressCase(m_CaseName.c_str());
	if(!scase) return;

	if(scase->GetStatusType())
	{
		m_pStatus->Set(scase->GetStatusType(), (void*)scase->GetStatusData());
	}

	wxTreeItemId root = m_pClientTree->GetRootItem();
	if(!root.IsOk()) return;

	wxTreeItemIdValue cookie;
	wxTreeItemId id = m_pClientTree->GetFirstChild(root, cookie);
	while(id.IsOk())
	{
		StreesViewItemData* data = (StreesViewItemData*)m_pClientTree->GetItemData(id);
		if(data)
		{
			Zion::CStressClient* client = Zion::CStressManager::Get().GetClient(data->m_uid);
			if(!client) return;
			m_pClientTree->SetItemText(id, wxString::FromUTF8(client->GetTitle().c_str()));
		}
		id = m_pClientTree->GetNextChild(root, cookie);
	}
}

void CStressViewDlg::OnSelect(wxTreeEvent& event)
{
	StreesViewItemData* data = (StreesViewItemData*)m_pClientTree->GetItemData(event.GetItem());
	if(data)
	{
		m_Index = data->m_uid;
		m_CaseName = data->m_name;
	}
	else
	{
		m_pConfig->Clear();
		m_pStatus->Clear();
		m_Index = (_U32)-1;
		m_CaseName = "";
		return;
	}

	Zion::CStressClient* client = Zion::CStressManager::Get().GetClient(m_Index);
	if(!client) return;

	Zion::CStressCase* scase = client->GetStressCase(m_CaseName.c_str());
	if(!scase) return;

	if(scase->GetConfigType())
	{
		m_pConfig->Set(scase->GetConfigType(), (void*)scase->GetConfig());
	}
	if(scase->GetStatusType())
	{
		m_pStatus->Set(scase->GetStatusType(), (void*)scase->GetStatusData());
	}
}
