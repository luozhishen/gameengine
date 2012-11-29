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
	m_pCaseList = ATLAS_NEW wxComboBox(this, ID_CASELIST, wxT(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY);
	m_pConfigText = ATLAS_NEW wxTextCtrl(this, ID_CASETEXT, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

	wxBoxSizer* pSizer2 = ATLAS_NEW wxBoxSizer(wxHORIZONTAL);
	pSizer2->AddStretchSpacer();
	pSizer2->Add(ATLAS_NEW wxButton(this, wxID_OK, wxT("Confirm")), 0, wxALIGN_RIGHT | wxALL, 5);
	pSizer2->Add(ATLAS_NEW wxButton(this, wxID_CANCEL, wxT("Cancel")), 0, wxALIGN_RIGHT | wxALL, 5);
	wxBoxSizer* pSizer1 = ATLAS_NEW wxBoxSizer(wxVERTICAL);
	pSizer1->Add(m_pCaseList, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	pSizer1->Add(m_pConfigText, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	pSizer1->Add(pSizer2, 0, wxGROW|wxALIGN_CENTER_VERTICAL);
	SetSizer(pSizer1);

	std::vector<std::string> cases;
	Atlas::CStressManager::Get().GetCases(cases);
	std::vector<std::string>::iterator i;
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
}

void CCaseConfigDlg::ChangeCase(const char* name, bool bForce)
{
	std::map<std::string, int>::iterator i;
	i = m_CaseMap.find(name);
	if(i==m_CaseMap.end()) return;
	m_pCaseList->SetSelection(i->second);

	m_pCaseType = Atlas::CStressManager::Get().GetCaseConfigType(name);
	if(m_pCaseType)
	{
		_U8* pData = (_U8*)ATLAS_ALLOC(m_pCaseType->size);
		Atlas::CStressManager::Get().GetCaseConfigDefault(name, pData, m_pCaseType->size);
		std::string json;
		DDLReflect::Struct2Json(m_pCaseType, pData, json);
		ATLAS_FREE(pData);
		m_pConfigText->SetValue(wxString::FromUTF8(json.c_str()));
		m_pConfigText->Enable();
	}
	else
	{
		m_pConfigText->SetValue(wxT("NO Config"));
		m_pConfigText->Disable();
	}

	if(bForce) m_pCaseList->Disable();
	m_pCaseData = NULL;
}

void CCaseConfigDlg::OnCaseSelect(wxCommandEvent& event)
{
	if(m_pCaseData)
	{
		ATLAS_FREE(m_pCaseData);
		m_pCaseData = NULL;
	}
}

void CCaseConfigDlg::OnConfirm(wxCommandEvent& event)
{
	if(m_pCaseList->GetValue()==wxT("")) return;

	if(m_pCaseType)
	{
		if(m_pCaseData) ATLAS_FREE(m_pCaseData);

		m_pCaseData = (_U8*)ATLAS_ALLOC(m_pCaseType->size);
		std::string json;
		bool bRet = DDLReflect::Json2Struct(m_pCaseType, *m_pConfigText->GetValue().ToUTF8(), m_pCaseData);
		if(!bRet)
		{
			wxMessageBox(wxT("invalid config data"), wxT("Error"));
			return;
		}
	}
	m_CaseName = *m_pCaseList->GetValue().ToUTF8();
	EndDialog(wxID_OK);
}
