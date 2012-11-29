#ifndef __PROTOCAL_VIEW_INCLUDE__
#define __PROTOCAL_VIEW_INCLUDE__

class CProtocalView: public CStressFrameView
{
	DECLARE_EVENT_TABLE()

public:
	CProtocalView(CClientStressFrame* pFrame, wxWindow* pParent);
	virtual ~CProtocalView();

	void OnClickEnable(wxCommandEvent& event);
	void OnClickVertical(wxCommandEvent& event);
	void OnClickClear(wxCommandEvent& event);
	void OnListSelected(wxListEvent& event);

private:
	wxCheckBox*			m_pEnable;
	wxCheckBox*			m_pVertical;
	wxButton*			m_pClear;
	wxSplitterWindow*	m_pSplitter;
	wxListCtrl*			m_pList;
	wxTextCtrl*			m_pInfo;
};

#endif
