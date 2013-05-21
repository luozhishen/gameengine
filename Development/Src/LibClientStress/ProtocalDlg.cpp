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

#include "ProtocalDlg.h"

enum
{
	ID_CLASS_TREE = wxID_HIGHEST + 1,
	ID_FUNCTION_VIEW,
	ID_ARGEMENT_VIEW,
};

BEGIN_EVENT_TABLE(CProtocalDialog, wxDialog)
	EVT_TREE_SEL_CHANGED(ID_CLASS_TREE, CProtocalDialog::OnClassActived)
	EVT_LIST_ITEM_SELECTED(ID_FUNCTION_VIEW, CProtocalDialog::OnFunctionSelected)
END_EVENT_TABLE()

class CProtocalObject : public wxTreeItemData
{
public:
	CProtocalObject(bool isC2S, const DDLReflect::CLASS_INFO* pInfo)
	{
		m_isC2S = isC2S;
		m_pInfo = pInfo;
	}
	bool m_isC2S;
	const DDLReflect::CLASS_INFO* m_pInfo;
};

CProtocalDialog::CProtocalDialog(wxWindow* pParent) : wxDialog(pParent, wxID_ANY, wxString(wxT("Modal dialog")), wxDefaultPosition, wxSize(300, 300), wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER)
{
	m_pCurrentClass = NULL;

	wxSplitterWindow* pSplitter1 = ATLAS_NEW wxSplitterWindow(this);
	wxSplitterWindow* pSplitter2 = ATLAS_NEW wxSplitterWindow(pSplitter1);

	m_pClassTree = ATLAS_NEW wxTreeCtrl(pSplitter1, ID_CLASS_TREE);
	m_pFunctionView = ATLAS_NEW wxListCtrl(pSplitter2, ID_FUNCTION_VIEW, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_HRULES|wxLC_VRULES);
	m_pArgementView = ATLAS_NEW wxListCtrl(pSplitter2, ID_ARGEMENT_VIEW, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_HRULES|wxLC_VRULES);

	m_pFunctionView->SetAutoLayout(true);
	m_pArgementView->SetAutoLayout(true);

	m_pFunctionView->InsertColumn(0, wxT("Function Name"));
	m_pFunctionView->InsertColumn(1, wxT("Function Argements"));
	m_pArgementView->InsertColumn(0, wxT("Argement Name"));
	m_pArgementView->InsertColumn(1, wxT("Argement Type"));

	pSplitter2->SplitHorizontally(m_pFunctionView, m_pArgementView);
	pSplitter1->SplitVertically(m_pClassTree, pSplitter2);

	wxBoxSizer* pSizer = ATLAS_NEW wxBoxSizer(wxHORIZONTAL);
	pSizer->Add(pSplitter1, 1, wxGROW|wxALIGN_CENTER);

	SetSizer(pSizer);

	m_pClassTree->AddRoot(wxT("Root"));
	m_C2S_id = m_pClassTree->AppendItem(m_pClassTree->GetRootItem(), wxT("C2S"));
	m_S2C_id = m_pClassTree->AppendItem(m_pClassTree->GetRootItem(), wxT("S2C"));

	for(_U32 i=0; i<Zion::GetStubCount(); i++)
	{
		const DDLReflect::CLASS_INFO* pClient = Zion::GetClientStub(i);
		const DDLReflect::CLASS_INFO* pServer = Zion::GetServerStub(i);
		if(pClient)
		{
			m_pClassTree->AppendItem(m_S2C_id, wxString::FromUTF8(pClient->name), -1, -1, ATLAS_NEW CProtocalObject(true, pClient));
		}
		if(pServer)
		{
			m_pClassTree->AppendItem(m_C2S_id, wxString::FromUTF8(pServer->name), -1, -1, ATLAS_NEW CProtocalObject(false, pServer));
		}
	}
}

CProtocalDialog::~CProtocalDialog()
{
}

void CProtocalDialog::OnClassActived(wxTreeEvent& event)
{
	wxTreeItemId id = event.GetItem();
	CProtocalObject* pData = (CProtocalObject*)m_pClassTree->GetItemData(id);
	if(pData)
	{
		if(m_pCurrentClass==pData) return;
		m_pFunctionView->DeleteAllItems();
		m_pArgementView->DeleteAllItems();
		for(_U32 i=0; i<pData->m_pInfo->fcount; i++)
		{
			m_pFunctionView->InsertItem((long)i, wxString::FromUTF8(pData->m_pInfo->finfos[i].name));
			m_pFunctionView->SetItemData((long)i, (long)i);
		}
		m_pCurrentClass = pData;
		return;
	}
	m_pFunctionView->DeleteAllItems();
	m_pArgementView->DeleteAllItems();
}

void CProtocalDialog::OnFunctionSelected(wxListEvent& event)
{
	_U32 index = (_U32)event.GetItem().GetId();
	m_pArgementView->DeleteAllItems();
	if(index<m_pCurrentClass->m_pInfo->fcount)
	{
		const DDLReflect::FUNCTION_INFO* pInfo = &m_pCurrentClass->m_pInfo->finfos[index];
		for(_U32 i=0; i<pInfo->fcount; i++)
		{
			m_pArgementView->InsertItem((long)i, wxString::FromUTF8(pInfo->finfos[i].name));
			m_pArgementView->SetItemData((long)i, (long)i);
		}

	}
}
