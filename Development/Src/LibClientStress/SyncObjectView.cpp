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
#include "SyncObjectView.h"

#include <StressClient.h>

enum
{
	ID_CTRL_LIST,
	ID_CTRL_INFO,
};

BEGIN_EVENT_TABLE(CSyncObjectView, CStressFrameView)
	EVT_TREE_SEL_CHANGED(ID_CTRL_LIST, CSyncObjectView::OnListSelected)
END_EVENT_TABLE()

CSyncObjectView::CSyncObjectView(CClientStressFrame* pFrame, wxWindow* pParent) : CStressFrameView(pFrame, pParent, wxT("SyncDomain"))
{
	wxBoxSizer* pSizer1 = ATLAS_NEW wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* pSizer2 = ATLAS_NEW wxBoxSizer(wxVERTICAL);

	m_pSplitter = ATLAS_NEW wxSplitterWindow(this);
	m_pList = ATLAS_NEW wxTreeCtrl(m_pSplitter, wxID_ANY, wxDefaultPosition, wxSize(180, 180), wxBK_TOP);
	m_pInfo = ATLAS_NEW wxTextCtrl(m_pSplitter, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	m_pSplitter->SplitVertically(m_pList, m_pInfo, 100);

	pSizer2->Add(pSizer1, 0, wxGROW|wxALIGN_CENTER_VERTICAL);
	pSizer2->Add(m_pSplitter, 1, wxGROW|wxALIGN_CENTER_VERTICAL);
	SetSizer(pSizer2);
}

CSyncObjectView::~CSyncObjectView()
{
}

void CSyncObjectView::OnListSelected(wxTreeEvent& event)
{
}
