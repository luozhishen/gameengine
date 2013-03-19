#ifndef __ATLAS_OBJECT_DATA_VIEW__
#define __ATLAS_OBJECT_DATA_VIEW__

class CStructEditView;

class CContentDataView : public wxPanel
{
	DECLARE_EVENT_TABLE()

public:
	CContentDataView(wxWindow* pParent);
	virtual ~CContentDataView();

	void OnSearch(wxCommandEvent& event);
	void OnObjectActived(wxListEvent& event);
	void OnObjectAdd(wxCommandEvent& event);
	void OnObjectSave(wxCommandEvent& event);
	void OnObjectUndo(wxCommandEvent& event);
	void OnObjectRemove(wxCommandEvent& event);
	void OnObjectReflush(wxCommandEvent& event);
	void OnIdle(wxIdleEvent& event);
	void OnContextMenu(wxContextMenuEvent& event);

	void UpdateItemName(wxString& strItemValue, A_UUID& uuid);
	bool CheckModify(bool bClear);

protected:
	void AppendObject(const DDLReflect::STRUCT_INFO* info, const A_CONTENT_OBJECT* data);
	void RemoveObject(const A_UUID& uuid);
	void SelectObject(const A_UUID& uuid);
	void SetCurrentObjectNULL();
	void SetCurrentObject(long nSelectObject, const DDLReflect::STRUCT_INFO* info, const A_CONTENT_OBJECT* data);
	void FlashList();

	void OnMenuNew(wxCommandEvent& (event));
	void OnMenuCopy(wxCommandEvent& (event));
	void OnMenuPaste(wxCommandEvent& (event));
	void OnMenuDel(wxCommandEvent& (event));
	void OnMenuSelAll(wxCommandEvent& (event));
	void OnMenuSave(wxCommandEvent& (event));

private:
	CGenerateObjectDlg m_dlgGenerateObject;

	wxStaticText* m_pSearchResult;

	wxTextCtrl* m_pSearchText;
	wxComboBox* m_pObjectType;
	wxBitmapButton* m_pAddObject;
	wxListCtrl* m_pList;
	wxTextCtrl* m_pObjectName;
	CStructEditView* m_pInfo;

	long m_nSelectIndex;
	const DDLReflect::STRUCT_INFO* m_pCurInfo;
	const A_CONTENT_OBJECT* m_pCurData;
	Atlas::Map<Atlas::String, const DDLReflect::STRUCT_INFO*> m_mapTypes;

	A_UUID m_copyUUID;
};

#endif
