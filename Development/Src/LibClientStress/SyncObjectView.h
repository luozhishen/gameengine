#ifndef __SYNCOBJECT_VIEW_INCLUDE__
#define __SYNCOBJECT_VIEW_INCLUDE__

class CSyncObjectView: public CStressFrameView
{
	DECLARE_EVENT_TABLE()

public:
	CSyncObjectView(CClientStressFrame* pFrame, wxWindow* pParent);
	virtual ~CSyncObjectView();

	void OnListSelected(wxTreeEvent& event);

private:
	wxSplitterWindow*	m_pSplitter;
	wxTreeCtrl*			m_pList;
	wxTextCtrl*			m_pInfo;
};

#endif
