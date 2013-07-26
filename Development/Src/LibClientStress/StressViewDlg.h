#ifndef __STRESS_VIEW_DLAILOG_H__
#define __STRESS_VIEW_DLAILOG_H__

class CStructEditView;

class CStressViewDlg : public wxDialog
{
	DECLARE_EVENT_TABLE()

public:
	CStressViewDlg(wxWindow* pParent);
	virtual ~CStressViewDlg();

	void OnTimer(wxTimerEvent& event);
	void OnSelect(wxTreeEvent& event);

private:
	void InitClients();

private:
	wxTimer m_Timer;
	wxTreeCtrl* m_pClientTree;
	CStructEditView* m_pConfig;
	CStructEditView* m_pStatus;
	_U32 m_Index;
	Zion::String m_CaseName;
};

#endif // __STRESS_VIEW_DLAILOG_H__
