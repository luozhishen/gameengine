#ifndef __ATLAS_OBJECT_DEFINE_VIEW__
#define __ATLAS_OBJECT_DEFINE_VIEW__

class wxPropertyGrid;

class CObjectDefineView : public wxPanel
{
	DECLARE_EVENT_TABLE()

public:
	CObjectDefineView(wxWindow* pParent);
	virtual ~CObjectDefineView();

	void Add(const DDLReflect::STRUCT_INFO* root, Zion::Map<int, const DDLReflect::STRUCT_INFO*>& list);

	void OnObjectActived(wxTreeEvent& event);

protected:
	_U16 GetObjectId(const char* name);
	void GetObjectTitle(const DDLReflect::STRUCT_INFO* info, wxString& title);
	void AddObject(const DDLReflect::STRUCT_INFO* info);
	void ShowObject(const DDLReflect::STRUCT_INFO* info);

private:
	Zion::Map<Zion::String, wxTreeItemId> m_mapItems;
	wxTextCtrl*		m_pSearchText;
	wxTreeCtrl*		m_pTree;
	wxTreeItemId	m_TreeRoot;
	wxPropertyGrid*	m_pInfo;
};

#endif
