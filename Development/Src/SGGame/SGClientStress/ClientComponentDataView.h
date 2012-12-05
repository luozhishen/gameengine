#ifndef __CLIENT_COMPONENT_DATA_VIEW_H__
#define __CLIENT_COMPONENT_DATA_VIEW_H__

#include "ClientStressFrame.h"

class CClientComponentDataView : public CStressFrameView
{
	
	DECLARE_EVENT_TABLE()
public:
	CClientComponentDataView(CClientStressFrame* pFrame, wxWindow* pParent);
	~CClientComponentDataView();

	void Flush(bool full);

	virtual void OnSwitchTo(_U32 index);
	virtual void OnClear();
	virtual void OnNewCase(_U32 index, Atlas::CStressCase* pCase);

	void OnTimer(wxTimerEvent& event);

private:
	wxTextCtrl* m_pTextCtrl;
	Atlas::CStressClient* m_pCurrentClient;
	wxTimer m_StressTimer;
};

#endif //__CLIENT_COMPONENT_DATA_VIEW_H__