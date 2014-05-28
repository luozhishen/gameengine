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

#include <ZionBase.h>
#include <ZionCommon.h>

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
	m_pImportor = new CContentExcelImportor();
	m_pExcel = new COLEAutoExcelWrapper();

	InitClient();
}

CImportDlg::~CImportDlg()
{
	delete m_pImportor;
	delete m_pExcel;
}

bool CImportDlg::LoadTemplateDefine(const char* filename)
{
	if(!m_pImportor->LoadTemplateDefine(filename))
	{
		wxMessageBox(wxString::FromUTF8(m_pImportor->GetErrorInfo()), wxT("Error"));
		return false;
	}

	Zion::Array<Zion::String> list;
	m_pImportor->GetTemplateList(list);
	if(list.empty())
	{
		wxMessageBox(wxT("template not found"), wxT("Error"));
		return false;
	}

	for(size_t i=0; i<list.size(); i++)
	{
		m_cbType->Insert(wxString::FromUTF8(list[i].c_str()), m_cbType->GetCount());
	}

	if(list.size()>0) m_cbType->SetSelection(0);
	return true;
}

void CImportDlg::InitClient()
{
	wxBoxSizer* pMainSizer = ZION_NEW wxBoxSizer(wxVERTICAL);

	// type line
	wxBoxSizer* pLineSizer = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	pLineSizer->Add(
		ZION_NEW wxStaticText(this, wxID_ANY, wxT("Type"), wxDefaultPosition, wxSize(60,-1), 0),
		0, wxLEFT|wxRIGHT, 5);
	m_cbType = ZION_NEW wxComboBox(this, ID_COMBO, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	pLineSizer->Add(
		m_cbType,
		1, wxLEFT|wxRIGHT, 5);
	pMainSizer->Add(pLineSizer, 0, wxEXPAND | wxALL, 5);

	// execel file
	pLineSizer = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	pLineSizer->Add(
		ZION_NEW wxStaticText(this, wxID_ANY, wxT("Execl File"),
		wxDefaultPosition, wxSize(60,-1), 0), 0, wxLEFT|wxRIGHT, 5);
	m_pFilePicker = ZION_NEW wxFilePickerCtrl(this, ID_FILEPICER, wxEmptyString, wxT("Select a Excel File"), wxT("excel files (*.xlsx) | *.xlsx"), wxDefaultPosition, wxDefaultSize);
	pLineSizer->Add(m_pFilePicker, 1, wxLEFT|wxRIGHT, 5);
	pMainSizer->Add(pLineSizer, 0, wxEXPAND | wxLEFT|wxRIGHT, 5);

	// clear data & exact match
	pLineSizer = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	pLineSizer->AddSpacer(70);
	m_pClearData = ZION_NEW wxCheckBox(this, wxID_ANY, wxT("Clear Data"));
	pLineSizer->Add(m_pClearData, 0, wxLEFT|wxRIGHT, 5);
	pMainSizer->Add(pLineSizer, 0, wxLEFT|wxRIGHT, 5);
	pLineSizer = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	pLineSizer->AddSpacer(70);
	m_pExactMatch = ZION_NEW wxCheckBox(this, wxID_ANY, wxT("Exact Match"));
	pLineSizer->Add(m_pExactMatch, 0, wxLEFT|wxRIGHT, 5);
	pMainSizer->Add(pLineSizer, 0, wxLEFT|wxRIGHT, 5);

	// sheet list
	pLineSizer = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	pLineSizer->Add(ZION_NEW wxStaticText(this, wxID_ANY, wxT("Sheet"), wxDefaultPosition, wxSize(60,-1), 0), 0, wxLEFT|wxRIGHT, 5);
	wxArrayString m_checkListChoices;
	m_checkList = ZION_NEW wxCheckListBox(this, ID_CHECKLIST, wxDefaultPosition, wxSize(wxDefaultSize.x, 200), m_checkListChoices, wxLB_MULTIPLE);
	pLineSizer->Add(m_checkList, 1, wxLEFT|wxRIGHT, 5);
	pMainSizer->Add(pLineSizer, 0, wxEXPAND | wxALL, 5);

	// select all
	pLineSizer = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	pLineSizer->AddSpacer(70);
	m_checkBoxSelAll = ZION_NEW wxCheckBox(this, ID_CHECKBOX_SELECT_ALL, wxT("Select All Sheet"), wxDefaultPosition, wxDefaultSize, 0);
	pLineSizer->Add(m_checkBoxSelAll, 0, wxLEFT|wxRIGHT, 5);
	pMainSizer->Add(pLineSizer, 0, wxLEFT|wxRIGHT, 5);

	// static line
	pMainSizer->Add(
		ZION_NEW wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL),
		0, wxEXPAND | wxALL, 5 );

	// botton
	pMainSizer->Add(
		ZION_NEW wxButton( this, wxID_OK, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0),
		0, wxLEFT|wxRIGHT|wxALIGN_RIGHT, 10);

	SetSizer(pMainSizer);
}

void CImportDlg::OnFilePicker(wxFileDirPickerEvent& event)
{
	m_checkList->Clear();

	Zion::String strFilePath = (const char*)m_pFilePicker->GetPath().ToUTF8();
	if(!m_pExcel->Open(strFilePath, false))
	{
		wxMessageBox(wxT("Failed to open excel file"), wxT("Error"));
		return;
	}

	Zion::Array<Zion::String> vSheets;
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

bool CImportDlg::GetSelectSheets(Zion::Array<Zion::String>& vSheets)
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
	if(!Zion::ContentObject::BuildIndex())
	{
		wxMessageBox(wxString::FromUTF8(Zion::ContentObject::BuildIndexGetErrorMsg().c_str()), wxT("Build Index Error"));
		return false;
	}

	if(!Zion::ContentObject::SaveContent())
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

	Zion::Array<Zion::String> sheets;
	if(!GetSelectSheets(sheets) || sheets.empty())
	{
		wxMessageBox(wxT("no sheet selected!"));
		return false;
	}

	size_t i;
	for(i=0; i<sheets.size(); i++)
	{
		if(!m_pExcel->SetActiveSheet(sheets[i]))
		{
			wxMessageBox(wxString::Format(wxT("error in SetActiveSheet(%s)"), wxString::FromUTF8(sheets[i].c_str()).c_str()), wxT("Error"));
			break;
		}

		if(!m_pImportor->ImportSheet((const char*)strType.ToUTF8(), m_pExcel))
		{
			wxMessageBox(wxString::FromUTF8(m_pImportor->GetErrorInfo()), wxT("Error"));
			break;
		}
	}
	if(i!=sheets.size())
	{
		Zion::ContentObject::LoadContent(NULL, true);
	}

	return true;
}
