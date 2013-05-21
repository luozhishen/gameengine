#ifndef _CASE_CONFIG_DLG_H__
#define _CASE_CONFIG_DLG_H__

class CCaseConfigDlg : public wxDialog
{
	DECLARE_EVENT_TABLE()

public:
	CCaseConfigDlg(wxWindow* pParent);
	virtual ~CCaseConfigDlg();

	void ChangeCase(const char* name, bool bForce=false);

	void OnCaseSelect(wxCommandEvent& event);
	void OnConfirm(wxCommandEvent& event);

	const Zion::String& GetCaseName() { return m_CaseName; }
	const DDLReflect::STRUCT_INFO* GetCaseType() { return m_pCaseType; }
	_U8* GetCaseData() { return m_pCaseData; }

private:
	wxComboBox* m_pCaseList;
	wxTextCtrl* m_pConfigText;
	Zion::Map<Zion::String, int> m_CaseMap;
	Zion::String m_CaseName;
	const DDLReflect::STRUCT_INFO* m_pCaseType;
	_U8* m_pCaseData;
};

#endif
