#ifndef __ZION_EDITOR_FRAME__
#define __ZION_EDITOR_FRAME__

class CContentDataView;

class CEditorFrame: public wxFrame
{
	DECLARE_EVENT_TABLE()

public:
	CEditorFrame();
	virtual ~CEditorFrame();

	void InitMenu();
	void InitClient();

	void OnFrameQuit(wxCloseEvent& event);
	void OnFileMenu(wxCommandEvent& event);
	void OnToolMenu(wxCommandEvent& event);
	void OnHelpMenu(wxCommandEvent& event);

	void OnSize(wxSizeEvent& event);
	void OnShow(wxShowEvent& event);

private:
	bool SaveContent(bool exit=true);
	CContentDataView* m_pContentDataView;

	struct
	{
		int x, y, w, h, m;
	} m_FrameData;
};

#endif
