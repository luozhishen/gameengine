#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/app.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/config.h>
#include <wx/utils.h>
#include <wx/wxchar.h>
#include <wx/window.h>

#include <AtlasBase.h>
#include <AtlasCommon.h>
#include <AtlasClient.h>

#include <StressClient.h>
#include <StressCase.h>
#include <StressManager.h>

#include "ClientStressApp.h"
#include "ClientStressFrame.h"
#include "ClientCaseView.h"

#include "SGCommon.h"
#include "SGClient.h"

#include "ClientComponentDataView.h"
#include "SGClient.h"
#include "SGClientPlayerComponent.h"
#include "SGClientSoldierComponent.h"
#include "SGClientGeneralComponent.h"
#include "SGCommon.h"
#include "SGClient.h"

enum
{
	ID_CASEVIEW = wxID_HIGHEST + 1,
	ID_DATA_TEXT,
	ID_DATA_TIMER,
};

BEGIN_EVENT_TABLE(CClientComponentDataView, CStressFrameView)
	EVT_TIMER(ID_DATA_TIMER, CClientComponentDataView::OnTimer)
END_EVENT_TABLE()

CClientComponentDataView::CClientComponentDataView(CClientStressFrame* pFrame, wxWindow* pParent) : CStressFrameView(pFrame, pParent, wxT("Component Data")), m_pCurrentClient(NULL)
{
	m_pTextCtrl = ATLAS_NEW wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);

	wxBoxSizer* pSizer = ATLAS_NEW wxBoxSizer(wxVERTICAL);
	pSizer->Add(m_pTextCtrl, 1, wxGROW|wxALIGN_CENTER_VERTICAL);
	SetSizer(pSizer);

	m_StressTimer.SetOwner(this, ID_DATA_TIMER);
	m_StressTimer.Start(1000);
}


CClientComponentDataView::~CClientComponentDataView()
{

}


void CClientComponentDataView::Flush(bool full)
{
	if(!m_pCurrentClient) return;

	Atlas::CSGClient* pClient =  (Atlas::CSGClient*)m_pCurrentClient->GetClient();
	Atlas::CSGClientPlayerComponent* pPlayer = pClient->GetPlayerComponent();
	const SG_PLAYER* pInfo = pPlayer->GetPlayerInfo();

	wxString strInfo;
	strInfo += wxT("nick:");
	strInfo += wxString::FromUTF8(pInfo->nick._Value);	strInfo += wxT("\n");

	strInfo += wxT("rmb:");
	strInfo << pInfo->rmb;	strInfo += wxT("\n");

	strInfo += wxT("gold:");
	strInfo << pInfo->gold;	strInfo += wxT("\n");

	strInfo += wxT("sex:");
	strInfo << pInfo->sex;	strInfo += wxT("\n");

	strInfo += wxT("uuid:");
	char szInfo[128];
	AUuidToString(pInfo->uuid, szInfo);
	strInfo += wxString::FromUTF8(szInfo);	strInfo += wxT("\n");
	
	strInfo += wxT("generals:\n");
	strInfo += wxT("{\n");
	for(_U32 i = 0; i < pInfo->equip_generals._Count; ++i)
	{
		strInfo << wxT("[") << i << wxT("]") << pInfo->equip_generals._Array[i] << wxT("\n");	
	}
	strInfo += wxT("\n}\n");


	strInfo += wxT("equip_soldiers:\n");
	strInfo += wxT("{\n");
	for(_U32 i = 0; i < pInfo->equip_generals._Count; ++i)
	{
		strInfo << wxT("[") << i << wxT("]") << pInfo->equip_soldiers._Array[i] << wxT("\n");	
	}
	strInfo += wxT("\n}\n");

	m_pTextCtrl->SetValue(strInfo);
}

void CClientComponentDataView::OnSwitchTo(_U32 index)
{
	m_pCurrentClient = Atlas::CStressManager::Get().GetClient(index);
	Flush(true);

}

void CClientComponentDataView::OnClear()
{
	m_pTextCtrl->Clear();
	m_pCurrentClient = NULL;
}

void CClientComponentDataView::OnNewCase(_U32 index, Atlas::CStressCase* pCase)
{
	Flush(true);
}

void CClientComponentDataView::OnTimer(wxTimerEvent& event)
{
	Flush(false);
}
