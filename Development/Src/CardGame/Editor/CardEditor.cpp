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

class CCardEditorApp : public CEditorApp
{
public:

	virtual bool OnInit()
	{
		Zion::String path = Zion::StringFormat("%s../../../../%s/", Zion::GetHomeDirectory(), Zion::GetAppName());
		if(_access(path.c_str(), 0)!=-1)
		{
			Zion::SetHomeDirectory(path.c_str());
		}
		return CEditorApp::OnInit();
	}
};

IMPLEMENT_APP(CCardEditorApp)
