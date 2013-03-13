#ifndef _IMPORT_DLG_H__
#define _IMPORT_DLG_H__

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
	bool GetSelectSheets(Atlas::Vector<Atlas::String>& vSheets);
	bool GetKeyCols(Atlas::Vector<Atlas::String>& vec);
	void OnSelectAll(wxCommandEvent& event);
	bool CheckKeyCols(Atlas::Vector<Atlas::String>& vec);

protected:
	wxComboBox* m_cbType;
	wxFilePickerCtrl* m_pFilePicker;
	wxCheckListBox* m_checkList;
	wxCheckBox* m_checkBoxSelAll;
	wxTextCtrl* m_textCtrlKey;
	wxButton* m_btnOK;
};

#endif //_IMPORT_DLG_H__