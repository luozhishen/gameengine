#include "ServerParamDlg.h"
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/grid.h>
#include <wx/wx.h>

#include <AtlasBase.h>
#include <AtlasClientApp.h>

#include "StressLoader.h"

enum
{
	ID_PROPERTY_GRID = wxID_HIGHEST + 1,
	ID_NEW_LINE,
	ID_DELETE_LINE,
};

BEGIN_EVENT_TABLE(ServerParamDlg, wxDialog)
	EVT_BUTTON(wxID_OK,			ServerParamDlg::OnConfirm)
	EVT_BUTTON(ID_NEW_LINE,		ServerParamDlg::OnNewLine)
	EVT_BUTTON(ID_DELETE_LINE,	ServerParamDlg::OnDeleteLine)
END_EVENT_TABLE()

ServerParamDlg::ServerParamDlg() : wxDialog(NULL, wxID_ANY, wxString(wxT("Server param Config")), wxDefaultPosition, wxSize(400, 300), wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER)
{
	wxBoxSizer* pSizer2 = ATLAS_NEW wxBoxSizer(wxHORIZONTAL);
	pSizer2->AddStretchSpacer();
	pSizer2->Add(ATLAS_NEW wxButton(this, ID_NEW_LINE, wxT("New")), 0, wxALIGN_RIGHT | wxALL, 5);
	pSizer2->Add(ATLAS_NEW wxButton(this, ID_DELETE_LINE, wxT("Delete")), 0, wxALIGN_RIGHT | wxALL, 5);

	pSizer2->Add(ATLAS_NEW wxButton(this, wxID_OK, wxT("Save")), 0, wxALIGN_RIGHT | wxALL, 5);
	pSizer2->Add(ATLAS_NEW wxButton(this, wxID_CANCEL, wxT("Cancel")), 0, wxALIGN_RIGHT | wxALL, 5);

	wxBoxSizer* pSizer1 = ATLAS_NEW wxBoxSizer(wxVERTICAL);
	m_pPropGrid = ATLAS_NEW wxPropertyGrid(this, ID_PROPERTY_GRID, wxDefaultPosition, wxDefaultSize, 
            wxPG_AUTO_SORT | wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE );
	
	std::map<std::string, std::string> params = Atlas::CClientApp::GetDefault()->GetParams();
	std::map<std::string, std::string>::iterator it = params.begin();

	for(int i = 0; it != params.end(); ++it, ++i)
	{
		wxPGProperty* propGrid = ATLAS_NEW wxStringProperty(wxString::FromUTF8(it->first.c_str()), wxPG_LABEL, wxString::FromUTF8(it->second.c_str()));
		m_pPropGrid->Append(propGrid);
	}	
	
	pSizer1->Add(m_pPropGrid, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	pSizer1->Add(pSizer2, 0, wxGROW|wxALIGN_BOTTOM);
	SetSizer(pSizer1);
}

ServerParamDlg::~ServerParamDlg()
{

}

void ServerParamDlg::OnConfirm(wxCommandEvent& event)
{
	Atlas::CStressLoader loader;
	wxFileDialog dlg(this, wxT("save script file"), wxT(""), wxT(""), wxT("xml files (*.xml) | *.xml"), wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if(dlg.ShowModal()==wxID_CANCEL)
	{
		return;
	}

	wxString strPath = dlg.GetPath();
	wxPropertyGridIterator it;
	for(it = m_pPropGrid->GetIterator(); !it.AtEnd(); ++it)
	{
		wxPGProperty* p = *it;
		wxString strKey = p->GetName();
		wxString strValue = p->GetValueAsString();

		Atlas::CClientApp::GetDefault()->SetParam((char*)strKey.c_str(), (char*)strValue.c_str());
	}
	
	loader.LoadScript((const char*)strPath.c_str());
	loader.SaveSvrParam();

	this->EndModal(wxID_OK);
}

void ServerParamDlg::OnNewLine(wxCommandEvent& event)
{
	wxString strValue;
	wxTextEntryDialog Dialog(this, wxT("Input New Config Name"), wxT("Please enter a string"), strValue);
	if(Dialog.ShowModal()==wxID_OK)
	{
		strValue = Dialog.GetValue();
		wxPGProperty* propGrid = ATLAS_NEW wxStringProperty(strValue, wxPG_LABEL, wxT(""));
		m_pPropGrid->Append(propGrid);
		m_pPropGrid->RefreshGrid();
	}
}

void ServerParamDlg::OnDeleteLine(wxCommandEvent& event)
{
	wxPGProperty* p = m_pPropGrid->GetSelectedProperty();
	if(p)
	{
		m_pPropGrid->DeleteProperty(p);
	}
}
