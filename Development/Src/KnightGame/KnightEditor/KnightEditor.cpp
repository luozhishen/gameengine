#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/config.h>

#include <AtlasBase.h>
#include <AtlasCommon.h>

#include <EditorApp.h>

class CKnightEditorApp : public CEditorApp
{
public:

	virtual bool OnInit()
	{
		if(!CEditorApp::OnInit()) return false;
		Atlas::String path = Atlas::StringFormat("%s../../../../%s/", Atlas::AtlasGameDir(), Atlas::AtlasGameName());
		if(_access(path.c_str(), 0)!=-1)
		{
			Atlas::AtlasSetGameDir(path.c_str());
		}
		return true;
	}
};

IMPLEMENT_APP(CKnightEditorApp)
