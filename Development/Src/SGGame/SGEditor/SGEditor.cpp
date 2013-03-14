#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/config.h>

#include <AtlasBase.h>
#include <AtlasCommon.h>

#include <EditorApp.h>
#include <io.h>

class CSGEditorApp : public CEditorApp
{
public:

	virtual bool OnInit()
	{
		Atlas::String path = Atlas::StringFormat("%s../../../../%s/", Atlas::AtlasGameDir(), Atlas::AtlasGameName());
		if(_access(path.c_str(), 0)!=-1)
		{
			Atlas::AtlasSetGameDir(path.c_str());
		}
		return CEditorApp::OnInit();
	}
};

IMPLEMENT_APP(CSGEditorApp)
