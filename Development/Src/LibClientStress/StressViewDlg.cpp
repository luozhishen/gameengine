#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/config.h>
#include <wx/utils.h>

#include <AtlasBase.h>
#include <StressManager.h>
#include <StressCase.h>
#include <StressClient.h>

#include "StressViewDlg.h"
#include "StressViewTree.h"

BEGIN_EVENT_TABLE(CStressViewDlg, wxDialog)
END_EVENT_TABLE()


CStressViewDlg::CStressViewDlg(wxWindow* pParent) : wxDialog(pParent, wxID_ANY, wxString(wxT("Stress View")), wxDefaultPosition, wxSize(pParent->GetSize()), wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER)
{
	wxNotebook* pViewTab = ATLAS_NEW wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP);
	wxPanel* pClientPanel = ATLAS_NEW wxPanel(pViewTab);

	m_pClientTree = ATLAS_NEW CStressViewTree(pClientPanel, ID_CLIENT_TREE, wxDefaultPosition, wxSize(180, -1));
	((CStressViewTree*)m_pClientTree)->SetParentWindow(this);
	
	m_pClientInfo = ATLAS_NEW wxTextCtrl(pClientPanel, wxID_ANY);
	m_pClientCase = ATLAS_NEW wxTextCtrl(pClientPanel, wxID_ANY);
	m_pClientData = ATLAS_NEW wxTextCtrl(pClientPanel, wxID_ANY);	

	wxBoxSizer* pClientTop = ATLAS_NEW wxBoxSizer(wxHORIZONTAL);
	pClientTop->Add(ATLAS_NEW wxStaticText(pClientPanel, wxID_ANY, wxT("Client")), 0, wxALIGN_CENTER|wxALL, 5);
	pClientTop->Add(m_pClientInfo, 1, wxGROW|wxALIGN_CENTER|wxALL, 5);
	pClientTop->Add(ATLAS_NEW wxStaticText(pClientPanel, wxID_ANY, wxT("Case")), 0, wxALIGN_CENTER|wxALL, 5);
	pClientTop->Add(m_pClientCase, 1, wxGROW|wxALIGN_CENTER|wxALL, 5);
	
	wxBoxSizer* pClientRight = ATLAS_NEW wxBoxSizer(wxVERTICAL);
	pClientRight->Add(pClientTop, 0, wxGROW|wxALIGN_CENTER);
	pClientRight->Add(m_pClientData, 1, wxGROW|wxALIGN_CENTER|wxALL, 5);
	
	wxBoxSizer* pClientRoot = ATLAS_NEW wxBoxSizer(wxHORIZONTAL);
	pClientRoot->Add(m_pClientTree, 0, wxGROW|wxALIGN_CENTER|wxALL, 5);
	pClientRoot->Add(pClientRight, 1, wxGROW|wxALIGN_CENTER);
	pClientPanel->SetSizer(pClientRoot);
	
	pViewTab->AddPage(pClientPanel, wxT("Clients"));

	wxBoxSizer* pSizerRoot = ATLAS_NEW wxBoxSizer(wxHORIZONTAL);
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
	Atlas::CStressManager& stressMgr = Atlas::CStressManager::Get();
	std::vector<_U32> clients;
	stressMgr.GetClients(clients);

	if(clients.empty())
	{
		return;
	}

	wxTreeItemId rootItem = m_pClientTree->AddRoot(wxT("Clients"));

	for(_U32 i = 0; i < clients.size(); ++i)
	{
		Atlas::CStressClient* pClient = stressMgr.GetClient(clients[i]);
		if(pClient==NULL)
			continue;

		wxTreeItemId clientItem = m_pClientTree->AppendItem(rootItem, wxString::FromUTF8(pClient->GetTitle().c_str()), -1, -1);
		StreesViewItemData* data = ATLAS_NEW StreesViewItemData(pClient->GetIndex());
		//data.SetId(clientItem);
		m_pClientTree->SetItemData(clientItem, (wxTreeItemData*)data);

		std::set<Atlas::CStressCase*> cases;
		pClient->GetStressCases(cases);

		if(cases.empty())
			continue;

		std::set<Atlas::CStressCase*>::iterator it = cases.begin();
		for( ; it != cases.end(); ++it)
		{
			std::string strCaseName = (*it)->GetName();

			wxTreeItemId caseItem = m_pClientTree->AppendItem(clientItem, wxString::FromUTF8((*it)->GetName().c_str()), -1, -1);
			StreesViewItemData* castData = ATLAS_NEW StreesViewItemData(strCaseName, pClient->GetIndex());
			//castData.SetId(caseItem);
			m_pClientTree->SetItemData(caseItem, (wxTreeItemData*)castData);
		}
	}
}

void CStressViewDlg::DisplayInfo(_U32 uid, std::string& strCaseName)
{	
	m_pClientInfo->Clear();
	m_pClientCase->Clear();
	m_pClientData->Clear();

	Atlas::CStressManager& stressMgr = Atlas::CStressManager::Get();
	Atlas::CStressClient* pClient = stressMgr.GetClient(uid);
	if(!pClient)
	{
		return;
	}	

	(*m_pClientInfo)<<wxString::FromUTF8(pClient->GetTitle().c_str());

	Atlas::CStressCase* pCase = pClient->GetStressCase(strCaseName.c_str());
	if(!pCase) return;
		
	(*m_pClientCase)<<wxString::FromUTF8(pCase->GetName().c_str());
	(*m_pClientData)<<wxString::FromUTF8(pCase->GetInfo().c_str());
}