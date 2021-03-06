#ifndef _IMPORT_DLG_H__
#define _IMPORT_DLG_H__

class wxFilePickerCtrl;
class wxFileDirPickerEvent;
class CContentExcelImportor;

class CImportDlg : public wxDialog
{
public:
	CImportDlg(wxWindow* parent);
	virtual ~CImportDlg();

	bool LoadTemplateDefine(const char* filename);

protected:
	DECLARE_EVENT_TABLE()
	void InitClient();
	void OnFilePicker(wxFileDirPickerEvent& event);
	virtual void EndModal(int retCode);
	bool ProcessImport();

	bool GetSelectSheets(Zion::Array<Zion::String>& vSheets);
	void OnSelectAll(wxCommandEvent& event);
	bool CheckKeyCols(Zion::Array<Zion::String>& vec);

public:
	wxComboBox* m_cbType;
	wxFilePickerCtrl* m_pFilePicker;
	wxCheckBox* m_pClearData;
	wxCheckListBox* m_checkList;
	wxCheckBox* m_checkBoxSelAll;
	CContentExcelImportor* m_pImportor;
};

#endif //_IMPORT_DLG_H__