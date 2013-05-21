#ifndef _CLIENT_CASE_VIEW_H__
#define _CLIENT_CASE_VIEW_H__

class CClientCaseView : public CStressFrameView
{
	DECLARE_EVENT_TABLE()

public:
	CClientCaseView(CClientStressFrame* pFrame, wxWindow* pParent);
	~CClientCaseView();

	void Flush(bool full);

	virtual void OnSwitchTo(_U32 index);
	virtual void OnClear();
	virtual void OnNewCase(_U32 index, Zion::CStressCase* pCase);

	void OnTimer(wxTimerEvent& event);

private:
	wxListCtrl* m_pListCtrl;
	Zion::CStressClient* m_pCurrentClient;
	wxTimer m_StressTimer;

};

#endif // _CLIENT_CASE_VIEW_H__
