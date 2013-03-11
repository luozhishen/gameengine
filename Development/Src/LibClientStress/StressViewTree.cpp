#include "StressViewTree.h"
#include "StressViewDlg.h"

BEGIN_EVENT_TABLE(CStressViewTree, wxTreeCtrl)
	EVT_TREE_ITEM_ACTIVATED(ID_CLIENT_TREE, CStressViewTree::OnItemActivated)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(CStressViewTree, wxTreeCtrl)

CStressViewTree::CStressViewTree(wxWindow* pParent, wxWindowID id, wxPoint point, wxSize size) : wxTreeCtrl(pParent, id, point, size, wxTR_HIDE_ROOT | wxTR_DEFAULT_STYLE)
{
	
}


CStressViewTree::~CStressViewTree()
{
}

void CStressViewTree::OnItemActivated(wxTreeEvent& event)
{
	wxTreeItemId itemId = event.GetItem();
    StreesViewItemData *item = (StreesViewItemData *)GetItemData(itemId);

    if (item != NULL)
    {
		m_pParent->DisplayInfo(item->GetUid(), item->GetCaseName());
    }
}

void CStressViewTree::SetParentWindow(CStressViewDlg* pParent)
{
	m_pParent = (CStressViewDlg*)pParent;
}

StreesViewItemData::StreesViewItemData(_U32 uid)
{
	m_uid = uid;
	m_strCaseName.clear();
}

StreesViewItemData::StreesViewItemData(Atlas::String& strItem, _U32 uid)
{
	m_strCaseName = strItem;
	m_uid = uid;
}

_U32 StreesViewItemData::GetUid()
{
	return m_uid;
}

Atlas::String& StreesViewItemData::GetCaseName()
{
	return m_strCaseName;
}