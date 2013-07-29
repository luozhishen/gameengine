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

	_U32 total_u32;
	_U32 last_u32;
	_U32 last_time;

	_F32 total_f32;
	_U32 count;

	long prop;
};

class CStressViewDlg : public wxDialog
{
	DECLARE_EVENT_TABLE()

public:
	CStressViewDlg(wxWindow* pParent);
	virtual ~CStressViewDlg();

	bool LoadCounterConfig();
	void UpdateCounter();
	void ClearCounter();

	void OnTimer(wxTimerEvent& event);
	void OnClientSelected(wxTreeEvent& event);

private:
	wxTimer m_Timer;
	wxTreeCtrl* m_pClientTree;
	CStructEditView* m_pConfig;
	CStructEditView* m_pStatus;
	_U32 m_Index;
	Zion::String m_CaseName;

	wxListCtrl* m_pGlobal;
//	wxPropertyGrid* m_pGlobal;
	Zion::Map<Zion::String, COUNTER_INFO> m_Counters;
};

#endif // __STRESS_VIEW_DLAILOG_H__
