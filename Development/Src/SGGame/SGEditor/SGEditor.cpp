#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/config.h>

#include <ZionBase.h>
#include <ZionCommon.h>

#include <EditorApp.h>
#include <io.h>

class CSGEditorApp : public CEditorApp
{
public:

	virtual bool OnInit()
	{
		Zion::String path = Zion::StringFormat("%s../../../../%s/", Zion::AtlasGameDir(), Zion::AtlasGameName());
		if(_access(path.c_str(), 0)!=-1)
		{
			Zion::AtlasSetGameDir(path.c_str());
		}
		return CEditorApp::OnInit();
	}
};

IMPLEMENT_APP(CSGEditorApp)
