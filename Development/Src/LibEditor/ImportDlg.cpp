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
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

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

	wxPanel* panel4 = ATLAS_NEW wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = ATLAS_NEW wxBoxSizer( wxHORIZONTAL );

	wxStaticText* staticTextKey = ATLAS_NEW wxStaticText( panel4, wxID_ANY, wxT("Key"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	staticTextKey->Wrap( -1 );
	staticTextKey->SetMinSize( wxSize( 50,-1 ) );

	bSizer4->Add( staticTextKey, 0, wxALL, 5 );

	wxBoxSizer* bSizer8;
	bSizer8 = ATLAS_NEW wxBoxSizer( wxVERTICAL );

	m_textCtrlKey = ATLAS_NEW wxTextCtrl( panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_textCtrlKey, 1, wxALL|wxEXPAND, 5 );

	wxStaticText* staticTextExample = ATLAS_NEW wxStaticText( panel4, wxID_ANY, wxT("input example: column1,column2,column3"), wxDefaultPosition, wxDefaultSize, 0 );
	staticTextExample->Wrap( -1 );
	bSizer8->Add( staticTextExample, 0, wxALL, 5 );

	bSizer4->Add( bSizer8, 1, wxEXPAND, 5 );

	panel4->SetSizer( bSizer4 );
	panel4->Layout();
	bSizer4->Fit( panel4 );
	m_mainSizer->Add( panel4, 0, wxEXPAND | wxALL, 5 );

	wxStaticLine *m_staticline1 = ATLAS_NEW wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_mainSizer->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	m_btnOK = ATLAS_NEW wxButton( this, wxID_OK, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	m_mainSizer->Add( m_btnOK, 0, wxALL|wxALIGN_RIGHT, 5 );
	SetSizer( m_mainSizer );
	Layout();

	InitCombox();
}

CImportDlg::~CImportDlg()
{
}

void CImportDlg::InitCombox()
{
	Atlas::Map<Atlas::String, const DDLReflect::STRUCT_INFO*> m_mapTypes;

	Atlas::Vector<const DDLReflect::STRUCT_INFO*> list;
	Atlas::ContentObject::GetTypeList(list);
	Atlas::Vector<const DDLReflect::STRUCT_INFO*>::iterator i;
	for(i=list.begin(); i!=list.end(); i++)
	{
		const DDLReflect::STRUCT_INFO* p = *i;
		if(m_mapTypes.find(p->name)==m_mapTypes.end())
		{
			m_mapTypes[p->name] = p;
		}
	}

	Atlas::Map<Atlas::String, const DDLReflect::STRUCT_INFO*>::iterator in;
	int nTypeIndex = -1;
	for(in=m_mapTypes.begin(); in!=m_mapTypes.end(); in++)
	{
		if(in->first=="A_CONTENT_OBJECT")
		{
			continue;
		}

		if(nTypeIndex==-1)
		{
			nTypeIndex = m_cbType->Insert(wxString::FromUTF8(in->first.c_str()), m_cbType->GetCount());
		}
		else
		{
			m_cbType->Insert(wxString::FromUTF8(in->first.c_str()), m_cbType->GetCount());
		}
	}

	m_cbType->SetSelection(nTypeIndex);
}

void CImportDlg::OnFilePicker(wxFileDirPickerEvent& event)
{
/*
	m_checkList->Clear();
	wxString strFilePath = m_pFilePicker->GetPath();
	m_pImportManger->Load(strFilePath);

	Atlas::Vector<wxString> vSheets;
	m_pImportManger->GetSheets(vSheets);
	for(unsigned int i = 0; i < vSheets.size(); ++i)
	{
		wxString val(vSheets[i].c_str(), wxMBConvUTF8());
		m_checkList->InsertItems(1, &val, i);
	}
*/
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

bool CImportDlg::GetKeyCols(Atlas::Vector<Atlas::String>& vec)
{
	wxString strKeyCol = m_textCtrlKey->GetValue().Trim();
	Atlas::String strKeyColCopy = (const char*)strKeyCol.ToUTF8();
	Atlas::StringStream ss(strKeyColCopy);
	Atlas::String item;
	while(std::getline(ss, item, ',')) {
		vec.push_back(item);
	}
	return CheckKeyCols(vec);
}

void CImportDlg::EndModal(int retCode)
{
	if(retCode!=wxID_OK) return wxDialog::EndModal(retCode);

	if(ProcessImport())
	{
		wxDialog::EndModal(retCode);
	}
	else
	{
		/*
		wxString strErr = wxString::FromUTF8(m_pImportManger->GetErrorMsg());
		if(!strErr.IsEmpty()) wxMessageBox(strErr);
		*/
	}
}

bool CImportDlg::ProcessImport()
{
	wxString strPath = m_pFilePicker->GetPath();
	if(strPath.empty())
	{
		wxMessageBox(wxT("invaild excel file path"));
		return false;
	}

	wxString strType = m_cbType->GetStringSelection();
	if(strType.empty())
	{
		wxMessageBox(wxT("invaild type"));
		return false;
	}

	if(!Atlas::ContentObject::SaveContent())
	{
		wxMessageBox(wxT("Failed to save content"));
		return false;
	}

	Atlas::Vector<Atlas::String> vCol;
	if(!GetKeyCols(vCol))
	{	
		wxMessageBox(wxT("input key Column repeat!"));
		return false;	
	}

	if(vCol.empty())
	{
		wxMessageBox(wxT("input key Column invalidate!"));
		return false;
	}

	Atlas::Vector<Atlas::String> vSheets;
	if(!GetSelectSheets(vSheets))
	{
		return false;
	}

	if(vSheets.empty())
	{
		wxMessageBox(wxT("no sheet select!"));
		return false;
	}

	/*
	m_pImportManger->Load(strPath);

	const DDLReflect::STRUCT_INFO* pInfo = NULL;
	for(unsigned int i = 0; i < vSheets.size(); ++i)
	{
		pInfo = Atlas::ContentObject::GetType(m_cbType->GetStringSelection().mb_str().data());
		if(!m_pImportManger->PrepareProcess(pInfo, vCol))
		{
			return false;
		}
		
		if(!m_pImportManger->ProcessSheet(vSheets[i]))
		{
			return false;
		}
	}

	wxString strUpdateInfo = wxString::FromUTF8(m_pImportManger->GetImportInfoMsg());
	wxMessageBox(strUpdateInfo, wxT("Import Info"));
*/
	return true;
}

bool CImportDlg::CheckKeyCols( Atlas::Vector<Atlas::String>& vec )
{
	for(size_t i = 0; i < vec.size(); ++i)
	{
		int nTime = 0;
		for(size_t j = i; j < vec.size(); ++j)
		{
			if(vec[i]==vec[j])
			{
				++nTime;
			}

			if(nTime > 1)
			{
				wxString strError;
				strError.Printf(wxT("column:%s repeat"), vec[i]);
				wxLogDebug(strError);
				vec.clear();
				return false;
			}
		}
	}

	return true;
}
