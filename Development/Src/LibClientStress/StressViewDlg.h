#ifndef __STRESS_VIEW_DLAILOG_H__
#define __STRESS_VIEW_DLAILOG_H__

class CStructEditView;

struct COUNTER_INFO
{
	Zion::String name;
	Zion::String casename;
	int mode; // 0=sum; 1=average
	Zion::String variable;
	_U32 offset;
	_U32 total_u32, count;
	_F32 total_f32;
	wxUIntProperty*		prop_u32;
	wxFloatProperty*	prop_f32;
};

class CStressViewDlg : public wxDialog
{
	DECLARE_EVENT_TABLE()

public:
	CStressViewDlg(wxWindow* pParent);
	virtual ~CStressViewDlg();

	bool LoadCounterConfig();
	void UpdateCounter();

	void OnTimer(wxTimerEvent& event);
	void OnClientSelected(wxTreeEvent& event);

private:
	wxTimer m_Timer;
	wxTreeCtrl* m_pClientTree;
	CStructEditView* m_pConfig;
	CStructEditView* m_pStatus;
	_U32 m_Index;
	Zion::String m_CaseName;

	wxPropertyGrid* m_pGlobal;
	Zion::Map<Zion::String, COUNTER_INFO> m_Counters;
};

#endif // __STRESS_VIEW_DLAILOG_H__
