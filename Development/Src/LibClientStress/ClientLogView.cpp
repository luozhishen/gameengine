#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/config.h>
#include <wx/utils.h>

#include <ZionBase.h>
#include <ZionCommon.h>
#include <ZionClient.h>

#include <StressClient.h>
#include <StressLoader.h>

#include "ClientStressFrame.h"
#include "ClientLogView.h"

enum
{
	ID_CTRL_ENABLE = wxID_HIGHEST + 1,
	ID_CTRL_AUTO,
	ID_CTRL_CLEAR,
	ID_CTRL_FLUSH,
};

BEGIN_EVENT_TABLE(CClientLogView, CStressFrameView)
	EVT_CHECKBOX(ID_CTRL_ENABLE, CClientLogView::OnClickEnable)
	EVT_BUTTON(ID_CTRL_CLEAR, CClientLogView::OnClickClear)
END_EVENT_TABLE()

CClientLogView::CClientLogView(CClientStressFrame* pFrame, wxWindow* pParent) : CStressFrameView(pFrame, pParent, wxT("Client Log"))
{
	wxBoxSizer* pSizer1 = ZION_NEW wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* pSizer2 = ZION_NEW wxBoxSizer(wxVERTICAL);
	m_pEnable	= ZION_NEW wxCheckBox(this, ID_CTRL_ENABLE, wxT("Enable"), wxDefaultPosition, wxSize(70, -1));
	m_pAutoReflush = ZION_NEW wxCheckBox(this, ID_CTRL_AUTO, wxT("Auto"), wxDefaultPosition, wxSize(70, -1));
	pSizer1->Add(m_pAutoReflush, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	pSizer1->Add(m_pEnable, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	pSizer1->Add(ZION_NEW wxButton(this, ID_CTRL_CLEAR, wxT("Clear")), 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	pSizer1->AddSpacer(5);
	pSizer1->Add(ZION_NEW wxButton(this, ID_CTRL_FLUSH, wxT("Flush")), 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pLog = ZION_NEW wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	pSizer2->Add(pSizer1, 0, wxGROW|wxALIGN_CENTER_VERTICAL);
	pSizer2->Add(m_pLog, 1, wxGROW|wxALIGN_CENTER_VERTICAL);
	SetSizer(pSizer2);
}

CClientLogView::~CClientLogView()
{
}

void CClientLogView::OnClickEnable(wxCommandEvent& event)
{
	CClientLogItem& Item = m_Clients[GetCurrentClient()];
	Item.m_bEnable = m_pEnable->IsChecked()?true:false;
}

void CClientLogView::OnClickClear(wxCommandEvent& event)
{
	if(GetCurrentClient()==-1) return;
	m_pLog->SetValue(wxT(""));
	m_Clients[GetCurrentClient()].Clear();
}

void CClientLogView::Append(_U32 index, const char* msg)
{
	CClientLogItem& Item = m_Clients[index];
	if(!Item.m_bEnable) return;
	Item.Append(msg);
	if(index==GetCurrentClient())
	{
		m_pLog->AppendText(wxString::FromUTF8(msg));
		m_pLog->AppendText(wxT("\n"));
	}
}

void CClientLogView::OnSwitchTo(_U32 index)
{
	CClientLogItem& Item = m_Clients[index];
	m_pEnable->Set3StateValue(Item.m_bEnable?wxCHK_CHECKED:wxCHK_UNCHECKED);
	for(size_t i=0; i<Item.m_nCount; i++)
	{
		m_pLog->AppendText(wxString::FromUTF8(Item.m_Logs[(Item.m_nStart+i)%(Item.m_Logs.size())].c_str()));
		m_pLog->AppendText(wxT("\n"));
	}
}

void CClientLogView::OnClear()
{
	m_pLog->SetValue(wxT(""));
}

void CClientLogView::OnNewClient(_U32 index)
{
	m_Clients[index];
	Zion::CStressClient* pClient = GetFrame()->GetStressClient(index);
	if(!pClient) return;
	pClient->_OnLoginDone.connect(this, &CClientLogView::OnLoginDone);
	pClient->_OnLoginFailed.connect(this, &CClientLogView::OnLoginFailed);
	pClient->_OnData.connect(this, &CClientLogView::OnData);
	pClient->_OnDisconnected.connect(this, &CClientLogView::OnDisconnected);
	pClient->_OnLogMessage.connect(this, &CClientLogView::OnLogMessage);
}

void CClientLogView::OnLoginDone(_U32 index)
{
	Append(index, "login done");
}

void CClientLogView::OnLoginFailed(_U32 index)
{
	Append(index, "login failed");
}

void CClientLogView::OnData(_U32 index, _U16 iid, _U16 fid, _U32 len, const _U8* data)
{
	char msg[10000];
	const DDLReflect::CLASS_INFO* cls;
	if(Zion::GetClientFunctionStub(iid, fid, cls))
	{
		Zion::String json;
		DDLReflect::Call2Json(&cls->finfos[fid], len, data, json);
		sprintf(msg, "ondata : %s.%s %s", cls->name, cls->finfos[fid].name, json.c_str());
	}
	else
	{
		sprintf(msg, "ondata : unknown(%u, %u)", iid, fid);
	}
	Append(index, msg);
}

void CClientLogView::OnDisconnected(_U32 index)
{
	Append(index, "disconnected");
}

void CClientLogView::OnLogMessage(_U32 index, const char* msg)
{
	Append(index, msg);
}
