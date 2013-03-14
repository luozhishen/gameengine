#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/combobox.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/filepicker.h>
#include <wx/checklst.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/statline.h>

#include <sstream>  
#include <string>
#include <map>

#include <AtlasBase.h>
#include <AtlasCommon.h>

#include "ImportDlg.h"
#include "ExcelImportor.h"
#include "OLEAutoExcelWrapper.h"

enum
{
	ID_COMBO = wxID_HIGHEST + 1,
	ID_CHECKLIST,
	ID_CHECKBOX_SELECT_ALL,
	ID_FILEPICER,
};

BEGIN_EVENT_TABLE(CImportDlg, wxDialog)
	EVT_FILEPICKER_CHANGED(ID_FILEPICER, CImportDlg::OnFilePicker)
	EVT_CHECKBOX(ID_CHECKBOX_SELECT_ALL, CImportDlg::OnSelectAll)
END_EVENT_TABLE()

CImportDlg::CImportDlg(wxWindow* parent) : wxDialog(parent, wxID_ANY, wxT("Import Excel"), wxDefaultPosition, wxSize(400, 400), wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER)
{
	m_pImporter = new CContentExcelImportor();
	m_pExcel = new COLEAutoExcelWrapper();

	InitClient();
}

CImportDlg::~CImportDlg()
{
	delete m_pImporter;
	delete m_pExcel;
}

bool CImportDlg::LoadTemplateDefine(const char* filename)
{
	if(!m_pImporter->LoadTemplateDefine(filename))
	{
		wxMessageBox(wxT("Failed to load template define file"), wxT("Error"));
		return false;
	}

	Atlas::Vector<Atlas::String> list;
	m_pImporter->GetTemplateList(list);

	for(size_t i=0; list.size(); i++)
	{
		m_cbType->Insert(wxString::FromUTF8(list[i].c_str()), m_cbType->GetCount());
	}

	if(list.size()>0) m_cbType->SetSelection(0);
	return true;
}

void CImportDlg::InitClient()
{
	SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* m_mainSizer;
	m_mainSizer = ATLAS_NEW wxBoxSizer( wxVERTICAL );

	wxPanel* panel1 = ATLAS_NEW wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1;
	bSizer1 = ATLAS_NEW wxBoxSizer( wxHORIZONTAL );

	wxStaticText* staticTextType = ATLAS_NEW wxStaticText( panel1, wxID_ANY, wxT("Type"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	staticTextType->Wrap( -1 );
	staticTextType->SetMinSize( wxSize( 50,-1 ) );

	bSizer1->Add( staticTextType, 0, wxALL, 5 );

	m_cbType = ATLAS_NEW wxComboBox( panel1, ID_COMBO, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	bSizer1->Add( m_cbType, 1, wxALL, 5 );

	panel1->SetSizer( bSizer1 );
	panel1->Layout();
	bSizer1->Fit( panel1 );
	m_mainSizer->Add( panel1, 0, wxEXPAND | wxALL, 5 );

	wxPanel* panel2 = ATLAS_NEW wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = ATLAS_NEW wxBoxSizer( wxHORIZONTAL );

	wxStaticText* staticTextPath = ATLAS_NEW wxStaticText( panel2, wxID_ANY, wxT("ExcelFile"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	staticTextPath->Wrap( -1 );
	staticTextPath->SetMinSize( wxSize( 50,-1 ) );

	bSizer2->Add( staticTextPath, 0, wxALL, 5 );

	m_pFilePicker = ATLAS_NEW wxFilePickerCtrl( panel2, ID_FILEPICER, wxEmptyString, wxT("Select a Excel File"), wxT("excel files (*.xlsx) | *.xlsx"), wxDefaultPosition, wxDefaultSize );
	bSizer2->Add( m_pFilePicker, 1, wxALL, 5 );

	panel2->SetSizer( bSizer2 );
	panel2->Layout();
	bSizer2->Fit( panel2 );
	m_mainSizer->Add( panel2, 0, wxEXPAND | wxALL, 5 );

	wxPanel* panel3 = ATLAS_NEW wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = ATLAS_NEW wxBoxSizer( wxHORIZONTAL );

	wxStaticText* staticTextSheet = ATLAS_NEW wxStaticText( panel3, wxID_ANY, wxT("Sheet"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	staticTextSheet->Wrap( -1 );
	staticTextSheet->SetMinSize( wxSize( 50,-1 ) );

	bSizer3->Add( staticTextSheet, 0, wxALL, 5 );

	wxBoxSizer* bSizer6;
	bSizer6 = ATLAS_NEW wxBoxSizer( wxVERTICAL );

	wxArrayString m_checkListChoices;
	m_checkList = ATLAS_NEW wxCheckListBox( panel3, ID_CHECKLIST, wxDefaultPosition, wxDefaultSize, m_checkListChoices, wxLB_MULTIPLE );

	bSizer6->Add( m_checkList, 1, wxALL|wxEXPAND, 5 );

	m_checkBoxSelAll = ATLAS_NEW wxCheckBox( panel3, ID_CHECKBOX_SELECT_ALL, wxT("Select All Sheet"), wxDefaultPosition, wxDefaultSize, 0 );

	bSizer6->Add( m_checkBoxSelAll, 0, wxALL, 5 );

	bSizer3->Add( bSizer6, 1, wxEXPAND, 5 );
	
	panel3->SetSizer( bSizer3 );
	panel3->Layout();
	bSizer3->Fit( panel3 );
	m_mainSizer->Add( panel3, 1, wxEXPAND | wxALL, 5 );

	wxStaticLine *m_staticline1 = ATLAS_NEW wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_mainSizer->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	m_btnOK = ATLAS_NEW wxButton( this, wxID_OK, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	m_mainSizer->Add( m_btnOK, 0, wxALL|wxALIGN_RIGHT, 5 );
	SetSizer( m_mainSizer );
	Layout();
}

void CImportDlg::OnFilePicker(wxFileDirPickerEvent& event)
{
	m_checkList->Clear();

	wxString strFilePath = m_pFilePicker->GetPath();
	m_pExcel->SetFilePath((const char*)strFilePath.ToUTF8());
	if(FAILED(m_pExcel->OpenExcelBook(false)))
	{
		wxMessageBox(wxT("Failed to open excel file"), wxT("Error"));
		return;
	}

	Atlas::Vector<Atlas::String> vSheets;
	if(!m_pExcel->GetExcelSheets(vSheets))
	{
		wxMessageBox(wxT("Failed to read excel file"), wxT("Error"));
		return;
	}

	for(unsigned int i = 0; i < vSheets.size(); ++i)
	{
		wxString val = wxString::FromUTF8(vSheets[i].c_str());
		m_checkList->InsertItems(1, &val, i);
	}
}

void CImportDlg::OnSelectAll(wxCommandEvent& event)
{
	bool bFlag = m_checkBoxSelAll->IsChecked();
	for(unsigned int i=0; i<m_checkList->GetCount();++i)
	{
		m_checkList->Check(i, bFlag);
	}
}

bool CImportDlg::GetSelectSheets(Atlas::Vector<Atlas::String>& vSheets)
{
	if(m_checkList->GetCount()==0)
	{
		wxMessageBox(wxT("no Sheet load!!!"));
		return false;
	}

	for(unsigned int i=0; i < m_checkList->GetCount(); ++i)
	{
		if(m_checkList->IsChecked(i))
			vSheets.push_back((const char*)m_checkList->GetString(i).ToUTF8());
	}

	return true;
}

void CImportDlg::EndModal(int retCode)
{
	if(retCode!=wxID_OK) return wxDialog::EndModal(retCode);

	if(ProcessImport())
	{
		wxDialog::EndModal(retCode);
	}
}

bool CImportDlg::ProcessImport()
{
	if(!Atlas::ContentObject::SaveContent())
	{
		wxMessageBox(wxT("Failed to save content"));
		return false;
	}

	wxString strType = m_cbType->GetStringSelection();
	if(strType.empty())
	{
		wxMessageBox(wxT("invaild type"));
		return false;
	}

	Atlas::Vector<Atlas::String> vSheets;
	if(!GetSelectSheets(vSheets) || vSheets.empty())
	{
		wxMessageBox(wxT("no sheet select!"));
		return false;
	}

	{
		/*
		wxString strErr = wxString::FromUTF8(m_pImportManger->GetErrorMsg());
		if(!strErr.IsEmpty()) wxMessageBox(strErr);
		*/
	}


/*
	wxString strUpdateInfo = wxString::FromUTF8(m_pImportManger->GetImportInfoMsg());
	wxMessageBox(strUpdateInfo, wxT("Import Info"));
*/
	return true;
}
