#ifndef _STRESS_VIEW_CTRL_H__
#define	_STRESS_VIEW_CTRL_H__

#include <wx/wxprec.h>
#include <wx/wx.h>

#include <wx/treectrl.h>
#include <wx/config.h>
#include <wx/utils.h>

#include <ZionBase.h>

enum
{
	ID_CLIENT_TREE = wxID_HIGHEST + 1,
};

class CStressViewDlg;

class StreesViewItemData : wxTreeItemData
{
public:
	StreesViewItemData(_U32 uid);
	StreesViewItemData(Zion::String& strItem, _U32 uid);
	
	_U32 GetUid();
	Zion::String& GetCaseName();

private:
	_U32 m_uid;
	Zion::String m_strCaseName;
};

class CStressViewTree :	public wxTreeCtrl
{
public:
	CStressViewTree(){}
	CStressViewTree(wxWindow* pParent, wxWindowID id, wxPoint point, wxSize size);
	virtual ~CStressViewTree();
	void SetParentWindow(CStressViewDlg* pParent);
	
	void OnItemActivated(wxTreeEvent& event);

	DECLARE_DYNAMIC_CLASS(CStressViewTree)
    DECLARE_EVENT_TABLE()
private:
	CStressViewDlg* m_pParent;
};


#endif	//_STRESS_VIEW_CTRL_H__

