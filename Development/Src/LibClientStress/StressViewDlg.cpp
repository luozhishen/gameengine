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
#include "StressViewTree.h"

BEGIN_EVENT_TABLE(CStressViewDlg, wxDialog)
END_EVENT_TABLE()

CStressViewDlg::CStressViewDlg(wxWindow* pParent) : wxDialog(pParent, wxID_ANY, wxString(wxT("Stress View")), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER)
{
	wxNotebook* pViewTab = ZION_NEW wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP);
	wxPanel* pClientPanel = ZION_NEW wxPanel(pViewTab);

	m_pClientTree = ZION_NEW CStressViewTree(pClientPanel, ID_CLIENT_TREE, wxDefaultPosition, wxSize(180, -1));
	((CStressViewTree*)m_pClientTree)->SetParentWindow(this);
	
	m_pClientInfo = ZION_NEW wxTextCtrl(pClientPanel, wxID_ANY);
	m_pClientCase = ZION_NEW wxTextCtrl(pClientPanel, wxID_ANY);
	m_pClientData = ZION_NEW wxTextCtrl(pClientPanel, wxID_ANY);	

	wxBoxSizer* pClientTop = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	pClientTop->Add(ZION_NEW wxStaticText(pClientPanel, wxID_ANY, wxT("Client")), 0, wxALIGN_CENTER|wxALL, 5);
	pClientTop->Add(m_pClientInfo, 1, wxGROW|wxALIGN_CENTER|wxALL, 5);
	pClientTop->Add(ZION_NEW wxStaticText(pClientPanel, wxID_ANY, wxT("Case")), 0, wxALIGN_CENTER|wxALL, 5);
	pClientTop->Add(m_pClientCase, 1, wxGROW|wxALIGN_CENTER|wxALL, 5);
	
	wxBoxSizer* pClientRight = ZION_NEW wxBoxSizer(wxVERTICAL);
	pClientRight->Add(pClientTop, 0, wxGROW|wxALIGN_CENTER);
	pClientRight->Add(m_pClientData, 1, wxGROW|wxALIGN_CENTER|wxALL, 5);
	
	wxBoxSizer* pClientRoot = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	pClientRoot->Add(m_pClientTree, 0, wxGROW|wxALIGN_CENTER|wxALL, 5);
	pClientRoot->Add(pClientRight, 1, wxGROW|wxALIGN_CENTER);
	pClientPanel->SetSizer(pClientRoot);
	
	pViewTab->AddPage(pClientPanel, wxT("Clients"));

	wxBoxSizer* pSizerRoot = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	pSizerRoot->Add(pViewTab, 1, wxGROW|wxALIGN_CENTER|wxALL, 5);
	SetSizer(pSizerRoot);

	InitClients();
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

	if(clients.empty())
	{
		return;
	}

	wxTreeItemId rootItem = m_pClientTree->AddRoot(wxT("Clients"));

	for(_U32 i = 0; i < clients.size(); ++i)
	{
		Zion::CStressClient* pClient = stressMgr.GetClient(clients[i]);
		if(pClient == NULL)
			continue;

		wxTreeItemId clientItem = m_pClientTree->AppendItem(rootItem, wxString::FromUTF8(pClient->GetTitle().c_str()), -1, -1);
		StreesViewItemData* data = ZION_NEW StreesViewItemData(pClient->GetIndex());
		//data.SetId(clientItem);
		m_pClientTree->SetItemData(clientItem, (wxTreeItemData*)data);

		Zion::Set<Zion::CStressCase*> cases;
		pClient->GetStressCases(cases);

		if(cases.empty())
			continue;

		Zion::Set<Zion::CStressCase*>::iterator it = cases.begin();
		for( ; it != cases.end(); ++it)
		{
			Zion::String strCaseName = (*it)->GetName();

			wxTreeItemId caseItem = m_pClientTree->AppendItem(clientItem, wxString::FromUTF8((*it)->GetName().c_str()), -1, -1);
			StreesViewItemData* castData = ZION_NEW StreesViewItemData(strCaseName, pClient->GetIndex());
			//castData.SetId(caseItem);
			m_pClientTree->SetItemData(caseItem, (wxTreeItemData*)castData);
		}
	}
}

void CStressViewDlg::DisplayInfo(_U32 uid, Zion::String& strCaseName)
{	
	m_pClientInfo->Clear();
	m_pClientCase->Clear();
	m_pClientData->Clear();

	Zion::CStressManager& stressMgr = Zion::CStressManager::Get();
	Zion::CStressClient* pClient = stressMgr.GetClient(uid);
	if(!pClient)
	{
		return;
	}	

	(*m_pClientInfo)<<wxString::FromUTF8(pClient->GetTitle().c_str());

	Zion::CStressCase* pCase = pClient->GetStressCase(strCaseName.c_str());
	if(!pCase) return;
		
	(*m_pClientCase)<<wxString::FromUTF8(pCase->GetName().c_str());
	(*m_pClientData)<<wxString::FromUTF8(pCase->GetStatusInfo().c_str());
}