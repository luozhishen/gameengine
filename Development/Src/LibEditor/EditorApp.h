#ifndef __ATLAS_EDITOR_APP__
#define __ATLAS_EDITOR_APP__

class CEditorApp : public wxApp
{
public:
	CEditorApp();
	~CEditorApp();

	virtual bool OnInit();
	virtual int OnExit();
};

#endif
