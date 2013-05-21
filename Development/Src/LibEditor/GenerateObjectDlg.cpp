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

CGenerateObjectDlg::CGenerateObjectDlg() : wxDialog(NULL, wxID_ANY, wxT("Create Content Object"), wxDefaultPosition, wxSize(365, 200), wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER)
{
	SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bMainSizer;
	bMainSizer = ATLAS_NEW wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerTop;
	bSizerTop = ATLAS_NEW wxBoxSizer( wxVERTICAL );

	wxStaticText* staticText2 = ATLAS_NEW wxStaticText( this, wxID_ANY, wxT("Choose Content Type"), wxDefaultPosition, wxDefaultSize, 0 );
	staticText2->Wrap( -1 );
	bSizerTop->Add( staticText2, 0, wxALL, 5 );

	wxBoxSizer* bSizer32;
	bSizer32 = ATLAS_NEW wxBoxSizer( wxHORIZONTAL );

	m_comboBoxType = ATLAS_NEW wxComboBox( this, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL,  wxCB_READONLY); 
	bSizer32->Add( m_comboBoxType, 1, wxALL, 5 );
	bSizerTop->Add( bSizer32, 1, wxEXPAND, 5 );

	bMainSizer->Add( bSizerTop, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizerMid;
	bSizerMid = ATLAS_NEW wxBoxSizer( wxVERTICAL );

	wxStaticText* staticText1 = ATLAS_NEW wxStaticText( this, wxID_ANY, wxT("Input content name"), wxDefaultPosition, wxDefaultSize, 0 );
	staticText1->Wrap( -1 );
	bSizerMid->Add( staticText1, 1, wxALL|wxALIGN_BOTTOM, 5 );

	bMainSizer->Add( bSizerMid, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer31;
	bSizer31 = ATLAS_NEW wxBoxSizer( wxHORIZONTAL );

	m_textCtrlName = ATLAS_NEW wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( m_textCtrlName, 1, wxALL, 5 );
	bSizerMid->Add( bSizer31, 1, wxEXPAND, 5 );

	wxStaticLine* m_staticline1 = ATLAS_NEW wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bMainSizer->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	wxStdDialogButtonSizer* sdbSizer1 = ATLAS_NEW wxStdDialogButtonSizer();
	m_sdbSizerOK = ATLAS_NEW wxButton( this, wxID_OK );
	sdbSizer1->AddButton( m_sdbSizerOK );
	m_sdbSizerCancel = ATLAS_NEW wxButton( this, wxID_CANCEL );
	sdbSizer1->AddButton( m_sdbSizerCancel );
	sdbSizer1->Realize();
	sdbSizer1->SetMinSize(400, 33);

	bMainSizer->Add( sdbSizer1, 0, wxEXPAND, 5 );

	this->SetSizer( bMainSizer );
	this->Layout();

	InitCombox();
}


CGenerateObjectDlg::~CGenerateObjectDlg()
{
}

void CGenerateObjectDlg::InitCombox()
{
	Zion::Map<Zion::String, const DDLReflect::STRUCT_INFO*> mapTypes;

	Zion::Vector<const DDLReflect::STRUCT_INFO*> list;
	Zion::ContentObject::GetTypeList(list);
	Zion::Vector<const DDLReflect::STRUCT_INFO*>::iterator i;
	for(i=list.begin(); i!=list.end(); i++)
	{
		m_comboBoxType->Insert(wxString::FromUTF8((*i)->name), m_comboBoxType->GetCount());
	}

	m_comboBoxType->SetSelection(0);
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
