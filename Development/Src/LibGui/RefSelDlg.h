#ifndef _REF_SEL_DLG_H__
#define _REF_SEL_DLG_H__

class CRefSelDlg : public wxDialog
{
	DECLARE_EVENT_TABLE()

public:
	CRefSelDlg(wxWindow* pParent, wxString wxRefType);
	virtual ~CRefSelDlg();

	void OnConfirm(wxCommandEvent& event);
	wxString GetSelect() const { return m_strSel; }

protected:
	void AppendItem(wxString& wxRefType, wxString strInput = wxT(""));
	void OnListSelect(wxListEvent& event);
	void OnTextInput(wxCommandEvent& event);

private:
	wxTextCtrl* m_pTextCtrl;
	wxListCtrl* m_pUUIDList;
	wxString m_strSel;
	wxString m_strRefType;
};

#endif //_REF_SEL_DLG_H__
