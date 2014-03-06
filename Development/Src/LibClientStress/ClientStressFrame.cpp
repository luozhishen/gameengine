#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/app.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/config.h>
#include <wx/utils.h>
#include <wx/combo.h>
#include <wx/numdlg.h>

#include <ZionBase.h>
#include <ZionCommon.h>
#include <mosdk.h>
#include <ZionClient.h>
#include <ZionClientApp.h>
#include <StressClient.h>
#include <StressCase.h>
#include <StressManager.h>
#include <StressLoader.h>
#include <StructEditView.h>
#include <HttpConnection.h>

#include "ClientStressFrame.h"
#include "ClientStressApp.h"
#include "ProtocalDlg.h"
#include "CaseConfigDlg.h"
#include "StressViewDlg.h"
#include "ServerParamDlg.h"

#include "CmdHistory.h"

enum
{
	ID_QUIT = wxID_HIGHEST + 1,
	ID_ABOUT,
	ID_DEBUG_ENABLE,
	ID_PROTOCAL,
	ID_DOCMD,
	ID_CMDTEXT,
	ID_ADDONE,
	ID_ADDFIVE,
	ID_ADDTEN,
	ID_ADDFIFTY,
	ID_SELECT_ALL,
	ID_LOGIN,
	ID_LOGOUT,
	ID_TIMER,
	ID_CLIENT_LIST,
	ID_CASE_LIST,
	ID_CASE_ADD,
	ID_STRESS_VIEW,
	ID_RELOAD_TEMPLATE,
	ID_SVR_PARAM_DLG,
};

BEGIN_EVENT_TABLE(CClientStressFrame, wxFrame)
	EVT_CHECKBOX(ID_DEBUG_ENABLE,	CClientStressFrame::OnDebugEnable)
	EVT_MENU(ID_PROTOCAL,			CClientStressFrame::OnProtocal)
	EVT_MENU(ID_QUIT,				CClientStressFrame::OnQuit)
	EVT_MENU(ID_ABOUT,				CClientStressFrame::OnAbout)
	EVT_BUTTON(ID_DOCMD,			CClientStressFrame::OnDoCmd)
	EVT_TEXT_ENTER(ID_CMDTEXT,		CClientStressFrame::OnCmdEnter)
	EVT_MENU(ID_ADDONE,				CClientStressFrame::OnAddClient)
	EVT_MENU(ID_ADDFIVE,			CClientStressFrame::OnAddClient)
	EVT_MENU(ID_ADDTEN,				CClientStressFrame::OnAddClient)
	EVT_MENU(ID_ADDFIFTY,			CClientStressFrame::OnAddClient)
	EVT_MENU(ID_LOGIN,				CClientStressFrame::OnLogin)
	EVT_MENU(ID_LOGOUT,				CClientStressFrame::OnLogout)
	EVT_MENU(ID_CASE_ADD,			CClientStressFrame::OnAddCase)
	EVT_MENU(ID_SELECT_ALL,			CClientStressFrame::OnSelectAll)
	EVT_LISTBOX(ID_CLIENT_LIST,		CClientStressFrame::OnClientSelected)
	EVT_MENU(ID_STRESS_VIEW,		CClientStressFrame::OnStressView)
	EVT_MENU(ID_RELOAD_TEMPLATE,	CClientStressFrame::OnReloadTemplate)
	EVT_MENU(ID_SVR_PARAM_DLG,		CClientStressFrame::OnOpenSvrParamDlg)
	EVT_TIMER(ID_TIMER,				CClientStressFrame::OnTimer)
	EVT_SIZE(CClientStressFrame::OnSize)
	EVT_SHOW(CClientStressFrame::OnShow)
END_EVENT_TABLE()

extern CClientStressApp* g_ClientStressApp;

CClientStressFrame::CClientStressFrame() : wxFrame(NULL, wxID_ANY, wxT("Client Stress - ")), m_StressLoader(this)
{
	wxString title = GetTitle();
	title = title + wxString::FromUTF8(Zion::ZionGameName());
	SetTitle(title);

	m_nCurrentIndex = (_U32)-1;
	m_nViewCount = 0;
	m_Timer.SetOwner(this, ID_TIMER);

	m_pCmdHistory = ZION_NEW Zion::CmdHistory(Zion::ZionGameDir());

	// create client
	InitClient();
	
	g_ClientStressApp->InitCase();
	g_ClientStressApp->InitClientView(this, m_pTabView);

	// menu
	InitMenu();

	// status bar
	CreateStatusBar();

	InitToolBar();

	LoadStressTemplate();

	wxConfigBase *pConfig = wxConfigBase::Get();
	if(pConfig)
	{
		pConfig->SetPath(wxString(wxT("/"))+wxString::FromUTF8(Zion::ZionGameName()));
		m_FrameData.x = pConfig->Read(wxT("x"), 50);
		m_FrameData.y = pConfig->Read(wxT("y"), 50);
		m_FrameData.w = pConfig->Read(wxT("w"), 350);
		m_FrameData.h = pConfig->Read(wxT("h"), 200);
		Move(m_FrameData.x, m_FrameData.y);
		SetClientSize(m_FrameData.w, m_FrameData.h);
		m_FrameData.m = pConfig->Read(wxT("m"), (long)0);
	}

	MOEnableDebug(false);
	m_Timer.Start(100);
}

CClientStressFrame::~CClientStressFrame()
{
	Zion::CStressManager::Get().DisconnectAll();

	wxConfigBase *pConfig = wxConfigBase::Get();
	if(pConfig)
	{
		pConfig->SetPath(wxString::FromUTF8("/")+wxString::FromUTF8(Zion::ZionGameName()));
		pConfig->Write(wxT("x"), (long)m_FrameData.x);
		pConfig->Write(wxT("y"), (long)m_FrameData.y);
		pConfig->Write(wxT("w"), (long)m_FrameData.w);
		pConfig->Write(wxT("h"), (long)m_FrameData.h);
		pConfig->Write(wxT("m"), (long)m_FrameData.m);
	}

	if(m_pCmdHistory)
	{
		delete m_pCmdHistory;
		m_pCmdHistory = NULL;
	}
}

void CClientStressFrame::InitMenu()
{
	SetMenuBar(ZION_NEW wxMenuBar);
	GetMenuBar()->Append(ZION_NEW wxMenu, wxT("&File"));
	GetMenuBar()->GetMenu(0)->Append(ID_PROTOCAL, wxT("Protocal"), wxT("Show protocal dailog"));
	GetMenuBar()->GetMenu(0)->Append(ID_SVR_PARAM_DLG, wxT("Server Params"), wxT("Set Server Params"));
	GetMenuBar()->GetMenu(0)->Append(ID_QUIT, wxT("E&xit\tAlt-X"), wxT("Exit the program"));
	GetMenuBar()->Append(ZION_NEW wxMenu, wxT("&Help"));
	GetMenuBar()->GetMenu(1)->Append(ID_ABOUT, wxT("&About"), wxT("Show About Dailog"));
}

void CClientStressFrame::InitToolBar()
{
	wxBitmap bmpAddOne;
	wxBitmap bmpAddFive;
	wxBitmap bmpAddTen;
	wxBitmap bmpAddFifty;
	wxBitmap bmpSelectAll;
	wxBitmap bmpLogin;
	wxBitmap bmpLogout;
	wxBitmap bmpAddCase;
	wxBitmap bmpStressView;
	wxBitmap bmpScriptRun;
	bmpAddOne.LoadFile(wxT("../wxRes/ClientStress/AddOne.png"),			wxBITMAP_TYPE_PNG);
	bmpAddFive.LoadFile(wxT("../wxRes/ClientStress/AddFive.png"),		wxBITMAP_TYPE_PNG);
	bmpAddTen.LoadFile(wxT("../wxRes/ClientStress/AddTen.png"),			wxBITMAP_TYPE_PNG);
	bmpAddFifty.LoadFile(wxT("../wxRes/ClientStress/AddFifty.png"),		wxBITMAP_TYPE_PNG);
	bmpSelectAll.LoadFile(wxT("../wxRes/ClientStress/SelectAll.png"),	wxBITMAP_TYPE_PNG);
	bmpLogin.LoadFile(wxT("../wxRes/ClientStress/ClientLogin.png"),		wxBITMAP_TYPE_PNG);
	bmpLogout.LoadFile(wxT("../wxRes/ClientStress/ClientLogout.png"),	wxBITMAP_TYPE_PNG);
	bmpAddCase.LoadFile(wxT("../wxRes/ClientStress/AddCase.png"),		wxBITMAP_TYPE_PNG);
	bmpStressView.LoadFile(wxT("../wxRes/ClientStress/StressView.png"),	wxBITMAP_TYPE_PNG);
	bmpScriptRun.LoadFile(wxT("../wxRes/ClientStress/Run.png"),			wxBITMAP_TYPE_PNG);

	wxToolBar* pToolBar;
	pToolBar = wxFrame::CreateToolBar();//wxTB_FLAT|wxTB_TOP, wxID_ANY);
	m_pCase = ZION_NEW wxComboBox(pToolBar, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY);
	pToolBar->AddControl(m_pCase);
	pToolBar->AddTool(ID_ADDONE,		wxT("Add 1"),		bmpAddOne,		wxT("Add 1 Client"));
	pToolBar->AddTool(ID_ADDFIVE,		wxT("Add 5"),		bmpAddFive,		wxT("Add 5 Client"));
	pToolBar->AddTool(ID_ADDTEN,		wxT("Add 10"),		bmpAddTen,		wxT("Add 10 Client"));
	pToolBar->AddTool(ID_ADDFIFTY,		wxT("Add 50"),		bmpAddFifty,	wxT("Add 50 Client"));
	pToolBar->AddSeparator();
	pToolBar->AddTool(ID_SELECT_ALL,	wxT("Select All"),	bmpSelectAll,	wxT("Select All"));
	pToolBar->AddSeparator();
	pToolBar->AddTool(ID_LOGIN,			wxT("Login"),		bmpLogin,		wxT("Client Login"));
	pToolBar->AddTool(ID_LOGOUT,		wxT("Logout"),		bmpLogout,		wxT("Client Logout"));
	pToolBar->AddSeparator();
	pToolBar->AddTool(ID_CASE_ADD,		wxT("Add Case"),	bmpAddCase,		wxT("Add Case to client"));
	pToolBar->AddTool(ID_STRESS_VIEW,	wxT("Stress View"),	bmpStressView,	wxT("Open stress view dailog"));
	pToolBar->AddTool(ID_RELOAD_TEMPLATE,wxT("Reload Stress Template"),	bmpScriptRun,	wxT("Reload stress template from json"));
	m_pEnableXDebug = ZION_NEW wxCheckBox(pToolBar, ID_DEBUG_ENABLE, wxT("Enable XDebug"));
	pToolBar->AddControl(m_pEnableXDebug);

	pToolBar->Realize();
}

void CClientStressFrame::InitClient()
{
	m_pSplitter = ZION_NEW wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN);
    m_pSplitter->SetSashGravity(0);
	wxNotebook* pClientTab = ZION_NEW wxNotebook(m_pSplitter, wxID_ANY, wxDefaultPosition, wxSize(180,-1), wxBK_TOP);
	m_pClientList = ZION_NEW wxListBox(pClientTab, ID_CLIENT_LIST, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_EXTENDED);
	pClientTab->AddPage(m_pClientList, wxT("Clients"));
	wxPanel* pPanel = ZION_NEW wxPanel(m_pSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	wxBoxSizer* pSizer1 = ZION_NEW wxBoxSizer(wxVERTICAL);
	m_pTabView = ZION_NEW wxNotebook(pPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP);
	wxBoxSizer* pSizer2 = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	m_pCmdText = ZION_NEW wxComboBox( pPanel, ID_CMDTEXT, wxT(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxTE_PROCESS_ENTER|wxCB_DROPDOWN|wxCB_SORT);
	int n = m_pCmdHistory->GetHistoryNum();
	Zion::CmdHistory::CMD_SET& cmds = m_pCmdHistory->GetHistorySet();
	for(int i = 0; i < n; ++i)
	{
		m_pCmdText->Append(wxString::FromUTF8(cmds[i].c_str()));
	}
    m_pCmdButton = ZION_NEW wxButton( pPanel, ID_DOCMD, wxT("Run"), wxDefaultPosition, wxDefaultSize, 0 );
    pSizer2->Add(m_pCmdText, 1, wxGROW|wxALIGN_CENTER_HORIZONTAL|wxUP|wxDOWN, 5 );
    pSizer2->Add(m_pCmdButton, 0, wxALIGN_CENTER|wxALL, 5 );
	pSizer1->Add(m_pTabView, 1, wxGROW|wxALIGN_CENTER_VERTICAL);
	pSizer1->Add(pSizer2, 0, wxGROW|wxALIGN_CENTER_VERTICAL);
	pPanel->SetSizer(pSizer1);
	m_pSplitter->SplitVertically(pClientTab, pPanel, 100);
}

void CClientStressFrame::OnDebugEnable(wxCommandEvent& event)
{
	if(m_pEnableXDebug->IsChecked())
	{
		MOEnableDebug(true);
		Zion::String url = Zion::StringFormat(Zion::CClientApp::GetDefault()->GetParam("ServerUrl", "http://localhost/%s.php"), "config");
		url += "?XDEBUG_SESSION_START=CLIENTSTRESS&KEY=1980114";
		Zion::Map<Zion::String, Zion::String> params;
		MOREQUEST* request = MORequestString(url.c_str(), params);
		while(MORequestStatus(request)==MOREQUESTSTATE_PENDING) SwitchToThread();
		MORequestDestory(request);
	}
	else
	{
		MOEnableDebug(false);
	}
}

void CClientStressFrame::OnProtocal(wxCommandEvent& event)
{
	CProtocalDialog Dialog(this);
	Dialog.ShowModal();
}

void CClientStressFrame::OnQuit(wxCommandEvent&)
{
	Close(true);
}

void CClientStressFrame::OnAbout(wxCommandEvent&)
{
	wxString txt;
	txt.Printf(wxT("Zion Client Stress for %s\n(C) 2011-2012 gamemake@gmail.com"), wxString::FromUTF8(Zion::ZionGameName()));
	wxMessageBox(txt, wxT("About"), wxICON_INFORMATION|wxOK);
}

void CClientStressFrame::OnCmdEnter(wxCommandEvent& event)
{
	OnDoCmd(event);
}

void CClientStressFrame::OnDoCmd(wxCommandEvent& event)
{
	wxString val = m_pCmdText->GetValue();
	val = val.Trim().Trim(false);
	if(val.size()<=0) return;

	wxString cmd = val.BeforeFirst(wxT(' ')).Trim();
	wxString arg = wxT("");
	if(cmd.size()!=val.size())
	{
		arg = val.AfterFirst(wxT(' ')).Trim(false);
	}

	const DDLReflect::CLASS_INFO* cls;
	_U16 fid;
	if(!Zion::GetServerFunctionStub((const char*)cmd.ToUTF8(), cls, fid))
	{
		wxMessageBox(wxT("unknown command"), wxT("error"));
		return;
	}

	Zion::String json((const char*)arg.ToUTF8());
	Zion::Array<Zion::String> args;
	Zion::StringSplit(json, ' ', args);
	if(cls->finfos[fid].fcount!=(_U16)args.size())
	{
		return;
	}
	json = "{";
	for(_U16 a=0; a<cls->finfos[fid].fcount; a++)
	{
		if(a>0)	json += ",";
		json += Zion::StringFormat("\"%s\":", cls->finfos[fid].finfos[a].name);
		json += args[a];
	}
	json += "}";

	if(!ProcessJsonCommand(cls, fid, json)) return;

	Zion::String line = (const char*)val.ToUTF8();
	m_pCmdHistory->AddCmd(line);
	m_pCmdText->Clear();
	int n = m_pCmdHistory->GetHistoryNum();
	Zion::CmdHistory::CMD_SET& cmds = m_pCmdHistory->GetHistorySet();
	for(int i = 0; i < n; ++i)
	{
		wxString tmp(cmds[i].c_str(), wxMBConvUTF8());
		m_pCmdText->AppendString(tmp);
		m_pCmdText->SetValue(val);
	}
}

static void OnHttpCallback(Zion::CHttpConnection* pHttp, Zion::CHttpConnection::STATE state)
{
	if(state==Zion::CHttpConnection::STATE_PAUSE)
	{
		wxMessageBox(wxT("Http return failed"));
		pHttp->Retry();
	}
}

void CClientStressFrame::OnAddClient(wxCommandEvent& event)
{
	int count = 1;
	switch(event.GetId())
	{
	case ID_ADDFIVE:	count = 5;	break;
	case ID_ADDTEN:		count = 10;	break;
	case ID_ADDFIFTY:	count = 50;	break;
	}

	Zion::String name = (const char*)m_pCase->GetValue().ToUTF8();

	for(int i=0; i<count; i++)
	{
		_U32 index;
		if(name=="<EMPTY>")
		{
			index = Zion::CStressManager::Get().NewClient();
			NotifyClientAdd(index);
		}
		else
		{
			index = m_StressLoader.CreateClient(name.c_str());
		}
		Zion::CStressClient* pClient = Zion::CStressManager::Get().GetClient(index);
		if(pClient->GetClient()->GetClientConnectionType()=="http")
		{
			Zion::CHttpConnection* pHttp = (Zion::CHttpConnection*)(pClient->GetClient()->GetClientConnection());
			pHttp->SetStateCallback(&OnHttpCallback);
		}
	}
}

void CClientStressFrame::OnSelectAll(wxCommandEvent& event)
{
	unsigned int i;
	for(i=0; i<m_pClientList->GetCount(); i++)
	{
		m_pClientList->SetSelection((int)i, true);
	}
}

void CClientStressFrame::OnLogin(wxCommandEvent& event)
{
	Zion::Array<_U32> clients;
	GetSelectClients(clients);
	for(size_t i=0; i<clients.size(); i++)
	{
		Zion::CStressClient* pClient = Zion::CStressManager::Get().GetClient(clients[i]);
		ZION_ASSERT(pClient);
		if(!pClient) continue;
		pClient->Login();
	}
	UpdateClientList();
}

void CClientStressFrame::OnLogout(wxCommandEvent& event)
{
	Zion::Array<_U32> clients;
	GetSelectClients(clients);
	for(size_t i=0; i<clients.size(); i++)
	{
		Zion::CStressClient* pClient = Zion::CStressManager::Get().GetClient(clients[i]);
		ZION_ASSERT(pClient);
		if(!pClient) continue;
		pClient->Logout();
	}
}

void CClientStressFrame::OnAddCase(wxCommandEvent& event)
{
	CCaseConfigDlg dlg(this);
	if(dlg.ShowModal()==wxID_CANCEL) return;

	Zion::Array<_U32> clients;
	GetSelectClients(clients);
	for(size_t i=0; i<clients.size(); i++)
	{
		Zion::CStressClient* pClient = Zion::CStressManager::Get().GetClient(clients[i]);
		ZION_ASSERT(pClient);
		if(!pClient) continue;
		Zion::CStressCase* pCase = pClient->NewStressCase(dlg.GetCaseName().c_str());
		if(!pCase) continue;
		if(dlg.GetCaseData()) pCase->SetConfig(dlg.GetCaseData(), dlg.GetCaseType()->size);
		NotifyClientAddCase(pClient->GetIndex(), pCase);
	}
}

void CClientStressFrame::OnClientSelected(wxCommandEvent& event)
{
	Zion::CStressClient* pClient = (Zion::CStressClient*)event.GetClientData();
	if(!pClient) return;
	m_nCurrentIndex = pClient->GetIndex();
	for(int v=0; v<m_nViewCount; v++)
	{
		m_pViews[v]->SwitchClient(m_nCurrentIndex);
	}
}

void CClientStressFrame::OnStressView(wxCommandEvent& event)
{
	CStressViewDlg dlg(this);
	dlg.ShowModal();
}

void CClientStressFrame::OnReloadTemplate(wxCommandEvent& event)
{
	if(!LoadStressTemplate())
	{
		wxMessageBox(wxT("load failed"));
	}
}

void CClientStressFrame::OnOpenSvrParamDlg(wxCommandEvent& event)
{
	ServerParamDlg dlg(this);
	if(dlg.ShowModal() == wxID_OK)
	{
		wxMessageBox(wxT("save succ!"));
	}
}

void CClientStressFrame::OnSize(wxSizeEvent& event)
{
	if(IsMaximized())
	{
		m_FrameData.m = 1;
	}
	else
	{
		m_FrameData.m = 0;
		GetClientSize(&m_FrameData.w, &m_FrameData.h);
		GetPosition(&m_FrameData.x, &m_FrameData.y);
	}
	event.Skip();
}

void CClientStressFrame::OnShow(wxShowEvent& event)
{
	if(!IsMaximized() && m_FrameData.m)
	{
		Maximize();
	}
	event.Skip();
}

void CClientStressFrame::OnTimer(wxTimerEvent& event)
{
	if(Zion::CClientApp::GetDefault()->Tick())
	{
		UpdateClientList();	
	}

	Zion::Array<_U32> clients;
	Zion::CStressManager::Get().GetClients(clients);
	bool EnableXDebug = m_pEnableXDebug->IsChecked();
	for(size_t i=0; i<clients.size(); i++)
	{
		Zion::CStressClient* pClient = Zion::CStressManager::Get().GetClient(clients[i]);
		if(pClient)
		{
			pClient->GetClient()->Tick();
		}
	}

	Zion::CStressManager::Get().UpdateAll();
}

void CClientStressFrame::UpdateClientList()
{
	unsigned int count = m_pClientList->GetCount();
	for(unsigned int i=0; i<count; i++)
	{
		Zion::CStressClient* pClient = (Zion::CStressClient*)m_pClientList->GetClientData(i);
		if(pClient)
		{
			m_pClientList->SetString(i, wxString::FromUTF8(pClient->GetTitle().c_str()));
		}
	}
}

void CClientStressFrame::GetSelectClients(Zion::Array<_U32>& clients)
{
	clients.clear();

	for(unsigned int i=0; i<m_pClientList->GetCount(); i++)
	{
		if(!m_pClientList->IsSelected(i)) continue;
		clients.push_back(((Zion::CStressClient*)m_pClientList->GetClientData(i))->GetIndex());
	}
}

void CClientStressFrame::AddView(CStressFrameView* pView)
{
	m_pTabView->AddPage(pView, pView->GetTabName());
	m_pViews[m_nViewCount++] = pView;
}

Zion::CStressClient* CClientStressFrame::GetStressClient(_U32 id)
{
	return Zion::CStressManager::Get().GetClient(id);
}

void CClientStressFrame::NotifyClientAdd(_U32 index)
{
	m_pClientList->Append(wxT(""), Zion::CStressManager::Get().GetClient(index));
	for(int v=0; v<m_nViewCount; v++)
	{
		m_pViews[v]->OnNewClient(index);
	}
	UpdateClientList();
}

void CClientStressFrame::NotifyClientAddCase(_U32 index, Zion::CStressCase* pCase)
{
	for(int v=0; v<m_nViewCount; v++)
	{
		m_pViews[v]->OnNewCase(index, pCase);
	}
}

bool CClientStressFrame::ProcessJsonCommand(const DDLReflect::CLASS_INFO* classinfo, _U16 fid, const Zion::String& json)
{
	_U8 data[10000];
	_U32 len = (_U32)sizeof(data);
	if(!DDLReflect::Json2Call(classinfo->finfos+fid, json, len, data))
	{
		wxMessageBox(wxT("invalid json"), wxT("error"));
		return false;
	}

	Zion::Array<_U32> clients;
	GetSelectClients(clients);
	if(clients.size()<=0)
	{
		wxMessageBox(wxT("no client selected"), wxT("error"));
		return false;
	}

	Zion::Array<_U32>::iterator i;
	Zion::CStressClient* pClient;

	for(i=clients.begin(); i!=clients.end(); i++)
	{
		pClient = Zion::CStressManager::Get().GetClient(m_nCurrentIndex);
		if(!pClient) continue;
		if(pClient->GetClient()->GetState()!=Zion::CClient::STATE_LOGINED) continue;
		pClient->GetClient()->SendData(classinfo->iid, fid, len, data);
	}

	return true;
}

bool CClientStressFrame::LoadStressTemplate()
{
	Zion::String file = Zion::StringFormat("%sConfig/StressTemplate.json", Zion::ZionGameDir());

	m_pCase->Clear();
	m_pCase->Insert(wxString::FromUTF8("<EMPTY>"), 0);
	m_pCase->SetSelection(0);

	if(!m_StressLoader.LoadTemplate(file.c_str()))
	{
		return false;
	}

	Zion::Array<Zion::String> names = m_StressLoader.GetNames();
	for(size_t i=0; i<names.size(); i++)
	{
		m_pCase->Insert(wxString::FromUTF8(names[i].c_str()), (unsigned int)i+1);
	}

	return true;
}
