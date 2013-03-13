#ifndef __ATLAS_EDITOR_FRAME__
#define __ATLAS_EDITOR_FRAME__

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
	void OnSave(wxCommandEvent& event);
	void OnImport(wxCommandEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnGenDESKey(wxCommandEvent& event);
	void OnBuildIndex(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
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
