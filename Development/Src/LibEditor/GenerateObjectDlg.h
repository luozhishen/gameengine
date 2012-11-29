#ifndef _GENERATE_OBJ_DLG_H__
#define _GENERATE_OBJ_DLG_H__

class wxStaticLine;

class CGenerateObjectDlg : public wxDialog
{	
	DECLARE_EVENT_TABLE()
public:
	CGenerateObjectDlg();
	virtual ~CGenerateObjectDlg();

	void InitCombox();
	wxString GetName();
	wxString GetType();

    virtual int ShowModal();

	void OnOK(wxCommandEvent& event);

private:
	wxTextCtrl* m_textCtrlName;
	wxComboBox* m_comboBoxType;
	wxButton* m_sdbSizerOK;
	wxButton* m_sdbSizerCancel;
};

#endif //_GENERATE_OBJ_DLG_H__