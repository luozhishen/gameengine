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


#include <StructEditView.h>
#include "CaseConfigDlg.h"

enum
{
	ID_CASELIST = wxID_HIGHEST + 1,
	ID_CASETEXT,
};

BEGIN_EVENT_TABLE(CCaseConfigDlg, wxDialog)
	EVT_COMBOBOX(ID_CASETEXT,	CCaseConfigDlg::OnCaseSelect)
	EVT_BUTTON(wxID_OK,			CCaseConfigDlg::OnConfirm)
END_EVENT_TABLE()

CCaseConfigDlg::CCaseConfigDlg(wxWindow* pParent) : wxDialog(pParent, wxID_ANY, wxT("Case Config"), wxDefaultPosition, wxSize(300, 300), wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER)
{
	m_pCaseType = NULL;
	m_pCaseData = NULL;

	m_pCaseList = ZION_NEW wxComboBox(this, ID_CASELIST, wxT(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY);
	m_pDataView = ZION_NEW CStructEditView(this);
	m_pCaseList->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(CCaseConfigDlg::OnCaseSelect), NULL, this); 

	wxBoxSizer* pSizer2 = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	pSizer2->AddStretchSpacer();
	pSizer2->Add(ZION_NEW wxButton(this, wxID_OK, wxT("Confirm")), 0, wxALIGN_RIGHT | wxALL, 5);
	pSizer2->Add(ZION_NEW wxButton(this, wxID_CANCEL, wxT("Cancel")), 0, wxALIGN_RIGHT | wxALL, 5);
	wxBoxSizer* pSizer1 = ZION_NEW wxBoxSizer(wxVERTICAL);
	pSizer1->Add(m_pCaseList, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	pSizer1->Add(m_pDataView, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);	pSizer1->Add(pSizer2, 0, wxGROW|wxALIGN_CENTER_VERTICAL);
	SetSizer(pSizer1);

	Zion::Array<Zion::String> cases;
	Zion::CStressManager::Get().GetCases(cases);
	Zion::Array<Zion::String>::iterator i;
	for(i=cases.begin(); i!=cases.end(); i++)
	{
		m_CaseMap[*i] = m_pCaseList->Append(wxString::FromUTF8((*i).c_str()));
	}

	if(cases.size()>0)
	{
		ChangeCase(cases[0].c_str());
	}
	else
	{
		m_pCaseType = NULL;
	}
}

CCaseConfigDlg::~CCaseConfigDlg()
{
	if(m_pCaseData)
	{
		ZION_FREE(m_pCaseData);
		m_pCaseData = NULL;
	}
}

void CCaseConfigDlg::ChangeCase(const char* name, bool bForce)
{
	Zion::Map<Zion::String, int>::iterator i;
	i = m_CaseMap.find(name);
	if(i==m_CaseMap.end()) return;


	m_pCaseList->SetSelection(i->second);
	m_pDataView->Set(NULL, NULL);

	m_CaseName = name;
	m_pCaseType = Zion::CStressManager::Get().GetCaseConfigType(name);
	if(m_pCaseData)
	{
		ZION_FREE(m_pCaseData);
		m_pCaseData = NULL;
	}

	if(m_pCaseType)
	{
		m_pCaseData = (_U8*)ZION_ALLOC(m_pCaseType->size);
		Zion::CStressManager::Get().GetCaseConfigDefault(name, m_pCaseData, m_pCaseType->size);
		m_pDataView->Set(m_pCaseType, m_pCaseData);
	}

	if(bForce) m_pCaseList->Disable();
}

void CCaseConfigDlg::OnCaseSelect(wxCommandEvent& event)
{
	ChangeCase((const char*)m_pCaseList->GetValue().ToUTF8());
}

void CCaseConfigDlg::OnConfirm(wxCommandEvent& event)
{
	if(m_pCaseList->GetValue()==wxT("")) return;

	if(m_pCaseData)
	{
		ZION_FREE(m_pCaseData);
		m_pCaseData = NULL;
	}

	if(m_pCaseType)
	{
		if(!m_pCaseData)
		{
			m_pCaseData = (_U8*)ZION_ALLOC(m_pCaseType->size);
		}
		m_pDataView->Get(m_pCaseType, m_pCaseData);
	}

	EndDialog(wxID_OK);
}
