#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/config.h>
#include <wx/utils.h>

#include <AtlasBase.h>
#include <AtlasCommon.h>
#include <AtlasClient.h>

#include "ClientStressFrame.h"
#include "ProtocalView.h"

#include <StressClient.h>

enum
{
	ID_CTRL_ENABLE = wxID_HIGHEST + 1,
	ID_CTRL_VERTICAL,
	ID_CTRL_CLEAR,
	ID_CTRL_LIST,
	ID_CTRL_INFO,
};

BEGIN_EVENT_TABLE(CProtocalView, CStressFrameView)
	EVT_CHECKBOX(ID_CTRL_ENABLE, CProtocalView::OnClickEnable)
	EVT_CHECKBOX(ID_CTRL_VERTICAL, CProtocalView::OnClickVertical)
	EVT_BUTTON(ID_CTRL_CLEAR, CProtocalView::OnClickClear)
	EVT_LIST_ITEM_SELECTED(ID_CTRL_LIST, CProtocalView::OnListSelected)
END_EVENT_TABLE()

CProtocalView::CProtocalView(CClientStressFrame* pFrame, wxWindow* pParent) : CStressFrameView(pFrame, pParent, wxT("Protocal"))
{
	wxBoxSizer* pSizer1 = ATLAS_NEW wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* pSizer2 = ATLAS_NEW wxBoxSizer(wxVERTICAL);
	m_pVertical	= ATLAS_NEW wxCheckBox(this, ID_CTRL_VERTICAL, wxT("Vertial"), wxDefaultPosition, wxSize(70, -1));
	m_pEnable	= ATLAS_NEW wxCheckBox(this, ID_CTRL_ENABLE, wxT("Enable"), wxDefaultPosition, wxSize(70, -1));
	m_pClear	= ATLAS_NEW wxButton(this, ID_CTRL_CLEAR, wxT("Clear"));

	pSizer1->Add(m_pVertical, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	pSizer1->Add(m_pEnable, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	pSizer1->Add(m_pClear, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_pSplitter = ATLAS_NEW wxSplitterWindow(this);
	m_pList = ATLAS_NEW wxListCtrl(m_pSplitter, wxID_ANY, wxDefaultPosition, wxSize(180, 180), wxBK_TOP);
	m_pInfo = ATLAS_NEW wxTextCtrl(m_pSplitter, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	m_pSplitter->SplitVertically(m_pList, m_pInfo, 100);

	pSizer2->Add(pSizer1, 0, wxGROW|wxALIGN_CENTER_VERTICAL);
	pSizer2->Add(m_pSplitter, 1, wxGROW|wxALIGN_CENTER_VERTICAL);
	SetSizer(pSizer2);
}

CProtocalView::~CProtocalView()
{
}

void CProtocalView::OnClickEnable(wxCommandEvent& event)
{
}

void CProtocalView::OnClickVertical(wxCommandEvent& event)
{
	if(m_pVertical->GetValue())
	{
		m_pSplitter->SetSplitMode(wxSPLIT_HORIZONTAL);
	}
	else
	{
		m_pSplitter->SetSplitMode(wxSPLIT_VERTICAL);
	}
	m_pSplitter->UpdateSize();
}

void CProtocalView::OnClickClear(wxCommandEvent& event)
{
}

void CProtocalView::OnListSelected(wxListEvent& event)
{
}
