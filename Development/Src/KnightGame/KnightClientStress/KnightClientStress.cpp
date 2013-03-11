#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/config.h>

#include <AtlasBase.h>
#include <AtlasCommon.h>
#include <AtlasClient.h>
#include <AtlasClientApp.h>

#include <ClientStressApp.h>
#include <StressManager.h>
#include <StressCase.h>

#include "KnightCommon.h"
#include "KnightClientApp.h"

class CKnightClientStressApp : public CClientStressApp
{
public:
	CKnightClientStressApp() : m_App(false)
	{
	}

	~CKnightClientStressApp()
	{
	}

	virtual void InitClientView(CClientStressFrame* pFrame, wxWindow* pParent)
	{
		CClientStressApp::InitClientView(pFrame, pParent);
	}

	virtual void InitCase()
	{
	}

	Atlas::CKnightClientApp m_App;
};

IMPLEMENT_APP(CKnightClientStressApp)
