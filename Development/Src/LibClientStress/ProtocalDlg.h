#ifndef __PROTOCAL_DLG_INCLUDE__
#define __PROTOCAL_DLG_INCLUDE__

class CProtocalObject;

class CProtocalDialog : public wxDialog
{
	DECLARE_EVENT_TABLE()

public:
	CProtocalDialog(wxWindow* pParent);
	virtual ~CProtocalDialog();

	void OnClassActived(wxTreeEvent& event);
	void OnFunctionSelected(wxListEvent& event);

private:
	wxTreeItemId m_C2S_id, m_S2C_id;
	wxTreeCtrl* m_pClassTree;
	wxListCtrl* m_pFunctionView;
	wxListCtrl* m_pArgementView;
	CProtocalObject* m_pCurrentClass;
};

#endif
