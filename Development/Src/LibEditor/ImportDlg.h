#ifndef _IMPORT_DLG_H__
#define _IMPORT_DLG_H__

#include "ImportConfig.h"

class wxFilePickerCtrl;
class wxFileDirPickerEvent;

class CImportDlg : public wxDialog
{
public:
	CImportDlg(wxWindow* parent);
	virtual ~CImportDlg();

protected:
	DECLARE_EVENT_TABLE()
	void InitCombox();
	void OnFilePicker(wxFileDirPickerEvent& event);
	virtual void  EndModal(int retCode);
	bool ProcessImport();

protected:
	bool GetSelectSheets(std::vector<std::string>& vSheets);
	bool GetKeyCols(std::vector<std::string>& vec);
	void OnSelectAll(wxCommandEvent& event);
	bool CheckKeyCols(std::vector<std::string>& vec);

protected:
	wxComboBox* m_cbType;
	wxFilePickerCtrl* m_pFilePicker;
	wxCheckListBox* m_checkList;
	wxCheckBox* m_checkBoxSelAll;
	wxTextCtrl* m_textCtrlKey;
	wxButton* m_btnOK;

	Atlas::CContentExcelImportManager* m_pImportManger;
};

#endif //_IMPORT_DLG_H__