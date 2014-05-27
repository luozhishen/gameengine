#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/combobox.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/filepicker.h>
#include <wx/checklst.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/statline.h>

#include <ZionBase.h>
#include <ZionCommon.h>

#include "GenerateObjectDlg.h"


BEGIN_EVENT_TABLE(CGenerateObjectDlg, wxDialog)
	EVT_BUTTON(wxID_OK, CGenerateObjectDlg::OnOK)
END_EVENT_TABLE()

CGenerateObjectDlg::CGenerateObjectDlg() : wxDialog(NULL, wxID_ANY, wxT("Create Content Object"), wxDefaultPosition, wxSize(wxDefaultSize.x, 230), wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER)
{

	wxBoxSizer* bMainSizer;
	bMainSizer = ZION_NEW wxBoxSizer(wxVERTICAL);

	wxStaticText* staticText2 = ZION_NEW wxStaticText(this, wxID_ANY, wxT("Choose Content Type"), wxDefaultPosition, wxDefaultSize, 0);
	staticText2->Wrap(-1);
	bMainSizer->Add(staticText2, 0, wxGROW|wxALL|wxALIGN_CENTER_VERTICAL, 5);
	m_comboBoxType = ZION_NEW wxComboBox(this, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL,  wxCB_READONLY); 
	bMainSizer->Add(m_comboBoxType, 0, wxGROW|wxALL|wxALIGN_CENTER_VERTICAL, 5);
	wxStaticText* staticText1 = ZION_NEW wxStaticText(this, wxID_ANY, wxT("Input content name"), wxDefaultPosition, wxDefaultSize, 0);
	staticText1->Wrap(-1);
	bMainSizer->Add(staticText1, 0, wxGROW|wxALL|wxALIGN_CENTER_VERTICAL, 5);
	m_textCtrlName = ZION_NEW wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bMainSizer->Add(m_textCtrlName, 0, wxGROW|wxALL|wxALIGN_CENTER_VERTICAL, 5);
	wxStaticLine* m_staticline1 = ZION_NEW wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	bMainSizer->Add(m_staticline1, 0, wxGROW|wxALL|wxALIGN_CENTER_VERTICAL, 5);

	wxStdDialogButtonSizer* sdbSizer1 = ZION_NEW wxStdDialogButtonSizer();
	m_sdbSizerOK = ZION_NEW wxButton(this, wxID_OK);
	sdbSizer1->AddButton(m_sdbSizerOK);
	m_sdbSizerCancel = ZION_NEW wxButton(this, wxID_CANCEL);
	sdbSizer1->AddButton(m_sdbSizerCancel);
	sdbSizer1->Realize();
	bMainSizer->Add(sdbSizer1, 0, wxGROW|wxALL|wxALIGN_CENTER_VERTICAL, 5);

	SetSizer(bMainSizer);

	Zion::Map<Zion::String, const DDLReflect::STRUCT_INFO*> mapTypes;

	Zion::Array<const DDLReflect::STRUCT_INFO*> list;
	Zion::ContentObject::GetTypeList(list);
	Zion::Array<const DDLReflect::STRUCT_INFO*>::iterator i;
	for(i=list.begin(); i!=list.end(); i++)
	{
		m_comboBoxType->Insert(wxString::FromUTF8((*i)->name), m_comboBoxType->GetCount());
	}

	m_comboBoxType->SetSelection(0);
}


CGenerateObjectDlg::~CGenerateObjectDlg()
{
}

wxString CGenerateObjectDlg::GetName()
{
	wxString strRet = m_textCtrlName->GetValue();
	return strRet;
}

wxString CGenerateObjectDlg::GetType()
{
	wxString strRet = m_comboBoxType->GetStringSelection();
	return strRet;
}

int CGenerateObjectDlg::ShowModal()
{
	m_textCtrlName->SetValue(wxT(""));
	return wxDialog::ShowModal();
}

void CGenerateObjectDlg::OnOK(wxCommandEvent& event)
{
	wxString strRet = m_textCtrlName->GetValue().Trim();
	if(strRet.length()<=0)
	{
		wxMessageBox(wxT("please input object name"));
		return;
	}
	event.Skip();
}
