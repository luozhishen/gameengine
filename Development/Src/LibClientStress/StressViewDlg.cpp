#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/config.h>
#include <wx/utils.h>

#include <ZionBase.h>
#include <StressManager.h>
#include <StressCase.h>
#include <StressClient.h>
#include <StructEditView.h>

#include "StressViewDlg.h"
#include <fstream>

class StreesViewItemData : public wxTreeItemData
{
public:
	StreesViewItemData(_U32 uid, Zion::String name="")
	{
		m_uid = uid;
		m_name = name;
	}

	_U32 m_uid;
	Zion::String m_name;
};

enum
{
	ID_LEFT_TREE = wxID_HIGHEST + 1,
	ID_CASE_TREE,
	ID_SVD_TIMER,
};

BEGIN_EVENT_TABLE(CStressViewDlg, wxDialog)
	EVT_TREE_SEL_CHANGED(ID_LEFT_TREE, CStressViewDlg::OnClientSelected)
	EVT_TIMER(ID_SVD_TIMER, CStressViewDlg::OnTimer)
END_EVENT_TABLE()

#include <ZionCommon.h>
#include <ZionClientApp.h>

CStressViewDlg::CStressViewDlg(wxWindow* pParent) : wxDialog(pParent, wxID_ANY, wxString(wxT("Stress View")), wxDefaultPosition, wxSize(700, 500), wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER)
{
	m_Timer.SetOwner(this, ID_SVD_TIMER);

	wxString txt;
	txt.Printf(wxT("%s for %s [UID=%s]"), GetTitle(), wxString::FromUTF8(Zion::GetAppName()), wxString::FromUTF8(Zion::CClientApp::GetDefault()->GetParam("UidBase", "0")));
	SetTitle(txt);

	wxNotebook* pViewTab = ZION_NEW wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP);
	wxPanel* pClientPanel = ZION_NEW wxPanel(pViewTab);

	m_pClientTree = ZION_NEW wxTreeCtrl(pClientPanel, ID_LEFT_TREE, wxDefaultPosition, wxSize(180, -1));
	m_pConfig = ZION_NEW CStructEditView(pClientPanel);
	m_pStatus = ZION_NEW CStructEditView(pClientPanel);

	wxBoxSizer* pSizer1 = ZION_NEW wxBoxSizer(wxVERTICAL);
	pSizer1->Add(m_pConfig, 1, wxGROW|wxALIGN_CENTER|wxALL, 5);
	pSizer1->Add(m_pStatus, 1, wxGROW|wxALIGN_CENTER|wxALL, 5);
	
	wxBoxSizer* pClientRoot = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	pClientRoot->Add(m_pClientTree, 0, wxGROW|wxALIGN_CENTER|wxALL, 5);
	pClientRoot->Add(pSizer1, 1, wxGROW|wxALIGN_CENTER);
	pClientPanel->SetSizer(pClientRoot);

	wxPropertyGrid* pGrid = ZION_NEW wxPropertyGrid(pViewTab, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_AUTO_SORT|wxPG_SPLITTER_AUTO_CENTER|wxPG_DEFAULT_STYLE);
	
	Zion::Map<Zion::String, Zion::String> params = Zion::CClientApp::GetDefault()->GetParams();
	Zion::Map<Zion::String, Zion::String>::iterator it = params.begin();

	for(int i = 0; it != params.end(); ++it, ++i)
	{
		wxPGProperty* propGrid = ZION_NEW wxStringProperty(wxString::FromUTF8(it->first.c_str()), wxPG_LABEL, wxString::FromUTF8(it->second.c_str()));
		pGrid->SetPropertyReadOnly(pGrid->Append(propGrid));
	}

	m_pGlobal = ZION_NEW wxListCtrl(pViewTab, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	m_pGlobal->InsertColumn(0, wxT("Name"));
	m_pGlobal->InsertColumn(1, wxT("Value"));
	m_pGlobal->InsertColumn(2, wxT("Per second"));

	pViewTab->AddPage(pClientPanel,	wxT("Clients Info"));
	pViewTab->AddPage(pGrid,			wxT("Global Config"));
	pViewTab->AddPage(m_pGlobal,		wxT("Cases Info"));

	wxBoxSizer* pSizerRoot = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	pSizerRoot->Add(pViewTab, 1, wxGROW|wxALIGN_CENTER|wxALL, 5);
	SetSizer(pSizerRoot);

	// init client view
	Zion::CStressManager& stressMgr = Zion::CStressManager::Get();
	Zion::Array<_U32> clients;
	stressMgr.GetClients(clients);
	if(!clients.empty())
	{
		wxTreeItemId rootItem = m_pClientTree->AddRoot(wxT("Clients"));
		for(_U32 i=0; i<clients.size(); ++i)
		{
			Zion::CStressClient* pClient = stressMgr.GetClient(clients[i]);
			if(pClient==NULL) continue;

			wxTreeItemId clientItem = m_pClientTree->AppendItem(rootItem, wxString::FromUTF8(pClient->GetTitle().c_str()), -1, -1);
			m_pClientTree->SetItemData(clientItem, ZION_NEW StreesViewItemData(pClient->GetIndex()));

			Zion::Set<Zion::CStressCase*> cases;
			pClient->GetStressCases(cases);
			if(cases.empty()) continue;

			Zion::Set<Zion::CStressCase*>::iterator it;
			for(it=cases.begin(); it!=cases.end(); ++it)
			{
				wxTreeItemId caseItem = m_pClientTree->AppendItem(clientItem, wxString::FromUTF8((*it)->GetName().c_str()), -1, -1);
				StreesViewItemData* castData = ZION_NEW StreesViewItemData(pClient->GetIndex(), (*it)->GetName());
				m_pClientTree->SetItemData(caseItem, (wxTreeItemData*)castData);
			}
		}
	}

	// init case
	if(!LoadCounterConfig())
	{
		wxMessageBox(wxT("load stress counter config"));
	}
	UpdateCounter();

	m_Timer.Start(100);
}

CStressViewDlg::~CStressViewDlg()
{
	m_pClientTree->DeleteAllItems();
}

bool CStressViewDlg::LoadCounterConfig()
{
	Zion::String file = Zion::StringFormat("%sConfig/StressCounter.json", Zion::GetHomeDirectory());

	Json::Value root;
	std::ifstream f(file.c_str(), std::ifstream::binary);
	if(!f.is_open()) return false;
	Json::Reader reader;
	if(!reader.parse(f, root, false)) return false;

	Json::Value::Members ms = root.getMemberNames();
	Json::Value::Members::iterator i;
	for(i=ms.begin(); i!=ms.end(); i++)
	{
		Json::Value tpl = root.get(*i, Json::Value());
		if(!tpl.isObject()) return false;

		Json::Value _case = tpl.get("case", Json::Value());
		Json::Value _mode = tpl.get("mode", Json::Value());
		Json::Value _varb = tpl.get("variable", Json::Value());
		if(!_case.isString() || !_mode.isString() || !_varb.isString())
		{
			return false;
		}

		COUNTER_INFO cinfo;
		cinfo.name = *i;
		cinfo.casename = _case.asString().c_str();
		const DDLReflect::STRUCT_INFO* info = Zion::CStressManager::Get().GetCaseStatusType(cinfo.casename.c_str());
		if(!info) return false;
		DDLReflect::FIELD_INFO finfo;
		cinfo.offset = DDLReflect::GetStructFieldOffset(info, _varb.asString().c_str(), &finfo);
		if(cinfo.offset==(_U32)-1) return false;
		if(_mode.asString()=="sum")
		{
			if(finfo.type!=DDLReflect::TYPE_U32) return false;
			cinfo.mode = 0;
			cinfo.prop = m_pGlobal->InsertItem(m_pGlobal->GetItemCount(), wxString::FromUTF8(cinfo.name.c_str()));
			cinfo.last_u32 = 0;
			cinfo.last_time = GetTickCount();
		}
		else if(_mode.asString()=="average")
		{
			if(finfo.type!=DDLReflect::TYPE_F32) return false;
			cinfo.mode = 1;
			cinfo.prop = m_pGlobal->InsertItem(m_pGlobal->GetItemCount(), wxString::FromUTF8(cinfo.name.c_str()));
		}
		else
		{
			return false;
		}
		m_Counters[cinfo.name] = cinfo;
	}

	return true;
}

void CStressViewDlg::UpdateCounter()
{
	Zion::Map<Zion::String, COUNTER_INFO>::iterator i;
	for(i=m_Counters.begin(); i!=m_Counters.end(); i++)
	{
		i->second.total_u32 = 0;
		i->second.total_f32 = 0.0f;
		i->second.count = 0;
	}

	Zion::CStressManager& stressMgr = Zion::CStressManager::Get();
	Zion::Array<_U32> clients;
	stressMgr.GetClients(clients);
	if(clients.empty()) return;
	for(_U32 cid=0; cid<clients.size(); ++cid)
	{
		Zion::CStressClient* pClient = stressMgr.GetClient(clients[cid]);
		if(pClient==NULL) continue;

		for(i=m_Counters.begin(); i!=m_Counters.end(); i++)
		{
			Zion::CStressCase* pCase = pClient->GetStressCase(i->second.casename.c_str());
			if(pCase==NULL) continue;
			if(i->second.mode==0)
			{
				i->second.total_u32 += *((const _U32*)(pCase->GetStatusData()+i->second.offset));
			}
			else
			{
				i->second.total_f32 += *((const _F32*)(pCase->GetStatusData()+i->second.offset));
				i->second.count += 1;
			}
		}
	}

	for(i=m_Counters.begin(); i!=m_Counters.end(); i++)
	{
		if(i->second.mode==0)
		{
			m_pGlobal->SetItem(i->second.prop, 1, wxString::Format(wxT("%u"), i->second.total_u32));

			_U32 curtime = GetTickCount(), deltatime;;
			if(curtime<i->second.last_time)
			{
				deltatime = curtime + (((_U32)-1) - i->second.last_time);
			}
			else
			{
				deltatime = curtime - i->second.last_time;
			}
			if(deltatime>5000)
			{
				_F32 v = ((_F32)(i->second.total_u32) - (_F32)(i->second.last_u32)) / deltatime * 1000 / 5;
				m_pGlobal->SetItem(i->second.prop, 2, wxString::Format(wxT("%f"), v));
				i->second.last_time = curtime;
				i->second.last_u32 = i->second.total_u32;
			}
		}
		else
		{
			m_pGlobal->SetItem(i->second.prop, 1, wxString::Format(wxT("%f"), i->second.total_f32/i->second.count));
		}
	}
}

void CStressViewDlg::ClearCounter()
{
	m_Counters.clear();
}

void CStressViewDlg::OnTimer(wxTimerEvent& event)
{
	UpdateCounter();

	Zion::CStressClient* client = Zion::CStressManager::Get().GetClient(m_Index);
	if(!client) return;

	Zion::CStressCase* scase = client->GetStressCase(m_CaseName.c_str());
	if(!scase) return;

	if(scase->GetStatusType())
	{
		m_pStatus->Set(scase->GetStatusType(), (void*)scase->GetStatusData());
	}

	wxTreeItemId root = m_pClientTree->GetRootItem();
	if(!root.IsOk()) return;

	m_pClientTree->SetDoubleBuffered(true);

	wxTreeItemIdValue cookie;
	wxTreeItemId id = m_pClientTree->GetFirstChild(root, cookie);
	while(id.IsOk())
	{
		StreesViewItemData* data = (StreesViewItemData*)m_pClientTree->GetItemData(id);
		if(data)
		{
			Zion::CStressClient* client = Zion::CStressManager::Get().GetClient(data->m_uid);
			if(!client) return;
			m_pClientTree->SetItemText(id, wxString::FromUTF8(client->GetTitle().c_str()));
		}
		id = m_pClientTree->GetNextChild(root, cookie);
	}
}

void CStressViewDlg::OnClientSelected(wxTreeEvent& event)
{
	StreesViewItemData* data = (StreesViewItemData*)m_pClientTree->GetItemData(event.GetItem());
	if(data)
	{
		m_Index = data->m_uid;
		m_CaseName = data->m_name;

		Zion::CStressClient* client = Zion::CStressManager::Get().GetClient(m_Index);
		if(client)
		{
			Zion::CStressCase* scase = client->GetStressCase(m_CaseName.c_str());
			if(scase)
			{
				if(scase->GetConfigType())
				{
					m_pConfig->Set(scase->GetConfigType(), (void*)scase->GetConfig());
				}
				else
				{
					m_pConfig->Clear();
				}
				if(scase->GetStatusType())
				{
					m_pStatus->Set(scase->GetStatusType(), (void*)scase->GetStatusData());
				}
				else
				{
					m_pStatus->Clear();
				}
				return;
			}
		}
	}
	else
	{
		m_Index = (_U32)-1;
		m_CaseName = "";
	}

	m_pConfig->Clear();
	m_pStatus->Clear();
}
