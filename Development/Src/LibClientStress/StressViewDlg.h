#ifndef __STRESS_VIEW_DLAILOG_H__
#define __STRESS_VIEW_DLAILOG_H__

class CStressViewDlg : public wxDialog
{
	DECLARE_EVENT_TABLE()

public:
	CStressViewDlg(wxWindow* pParent);
	virtual ~CStressViewDlg();

	void DisplayInfo(_U32 uid, std::string& strCaseName);

private:
	void InitClients();

private:
	wxTreeCtrl* m_pClientTree;
	wxTextCtrl* m_pClientInfo;
	wxTextCtrl* m_pClientCase;
	wxTextCtrl* m_pClientData;
};

#endif // __STRESS_VIEW_DLAILOG_H__
