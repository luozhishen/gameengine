#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/config.h>

#include <ZionBase.h>
#include <ZionCommon.h>
#include <ZionClient.h>
#include <ZionClientApp.h>
#include <mosdk.h>
#include <HttpConnection.h>

#include <StressClient.h>
#include <StressLoader.h>
#include <StressManager.h>
#include <StructEditView.h>

#include "ClientStressApp.h"
#include "ClientStressFrame.h"
#include "ClientLogView.h"
#include "ClientCaseView.h"
#include "ClientDataView.h"

#include <process.h>
static volatile int tick_thread_state = 0;
static void tick_thread_proc(void* data)
{
	Zion::Array<_U32> clients;

	while(tick_thread_state==0)
	{
		Zion::CClientApp::GetDefault()->Tick();

		Zion::CStressManager::Get().GetClients(clients);
		for(size_t i=0; i<clients.size(); i++)
		{
			Zion::CStressClient* pClient = Zion::CStressManager::Get().GetClient(clients[i]);
			if(pClient)
			{
				pClient->GetClient()->Tick();
			}
		}

		Zion::CStressManager::Get().UpdateAll();

		Sleep(0);
	}

	tick_thread_state = 2;
}

CClientStressApp* g_ClientStressApp = NULL;

CClientStressApp::CClientStressApp()
{
	g_ClientStressApp = this;
	chdir(Zion::GetExeDir());
}

CClientStressApp::~CClientStressApp()
{
}

#include "StressViewDlg.h"

bool CClientStressApp::OnInit()
{
	wxImage::AddHandler(ZION_NEW wxPNGHandler);

	SetVendorName(wxT("Zion"));
	SetAppName(wxT("ClientStress"));

	if(__argc>1)
	{
		InitCase();

		Zion::String file = Zion::StringFormat("%sConfig/StressTemplate.json", Zion::GetHomeDirectory());
		Zion::CStressLoader loader;
		if(!loader.LoadTemplate(file.c_str()))
		{
			return false;
		}

		Zion::CClientApp::GetDefault()->InitApp();
		for(int i=1; i<__argc; i++)
		{
			if(*__argv[i]!='-' || *(__argv[i]+1)=='\0')
			{
				wxMessageBox(wxT("invalid parameter"));
				return false;
			}

			Zion::Array<Zion::String> rs;
			Zion::StringSplit(Zion::String(__argv[i]+2), ':', rs);
			if(rs.size()!=2)
			{
				wxMessageBox(wxT("invalid parameter"));
				return false;
			}

			if(*(__argv[i]+1)=='C')
			{
				for(int i=atoi(rs[1].c_str()); i>0; i--)
				{
					if(loader.CreateClient(rs[0].c_str())==(_U32)-1)
					{
						wxMessageBox(wxT("invalid case name"));
						return false;
					}
				}
			}

			if(*(__argv[i]+1)=='D')
			{
				Zion::CClientApp::GetDefault()->SetParam(rs[0].c_str(), rs[1].c_str());
			}
		}

		_beginthread(tick_thread_proc, 0, NULL);

		CStressViewDlg dlg(NULL);
		dlg.ShowModal();

		tick_thread_state = 1;
		while(tick_thread_state!=2) Sleep(0);

		Zion::CStressManager::Get().DisconnectAll();

		Zion::CClientApp::GetDefault()->FiniApp();
		return false;
	}

	Zion::CClientApp::GetDefault()->InitApp();
	wxConfigBase *pConfig = wxConfigBase::Get();
	CClientStressFrame* pMainFrame;
	pMainFrame = ZION_NEW CClientStressFrame;
	pMainFrame->Show(true);
	SetTopWindow(pMainFrame);

	return true;
}

int CClientStressApp::OnExit()
{
	Zion::CClientApp::GetDefault()->FiniApp();

	delete wxConfigBase::Set(NULL);
	return 0;
}

void CClientStressApp::InitClientView(CClientStressFrame* pFrame, wxWindow* pParent)
{
	pFrame->AddView(ZION_NEW CClientLogView(pFrame, pParent));
	pFrame->AddView(ZION_NEW CClientCaseView(pFrame, pParent));
	pFrame->AddView(ZION_NEW CClientDataView(pFrame, pParent));
}
