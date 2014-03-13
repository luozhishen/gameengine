#ifndef _CLIENT_DATA_VIEW_H__
#define _CLIENT_DATA_VIEW_H__

class CStructEditView;
class CClientDataView;

class CClientDataSyncProxy : public sigslot::has_slots<>
{
public:
	CClientDataSyncProxy(CClientDataView* pDataView, _U32 index);

	void OnSyncOpen();
	void OnSyncClose();
	void OnObjectCreate(const A_UUID& _uuid);
	void OnObjectUpdate(const A_UUID& _uuid);
	void OnObjectDelete(const A_UUID& _uuid);

private:
	CClientDataView* m_pDataView;
	_U32 m_nIndex;
};

class CClientDataView : public CStressFrameView
{
	DECLARE_EVENT_TABLE()

public:
	CClientDataView(CClientStressFrame* pFrame, wxWindow* pParent);
	~CClientDataView();

	void OnSyncClick(wxCommandEvent& event);
	void OnNewClick(wxCommandEvent& event);
	void OnSaveClick(wxCommandEvent& event);
	void OnCancelClick(wxCommandEvent& event);
	void OnObjectClick(wxListEvent& event);

	virtual void OnSwitchTo(_U32 index);
	virtual void OnClear();
	virtual void OnNewClient(_U32 index);
	virtual void OnNewCase(_U32 index, Zion::CStressCase* pCase);

	void OnSyncOpen(_U32 nIndex);
	void OnSyncClose(_U32 nIndex);
	void OnObjectCreate(_U32 nIndex, const A_UUID& _uuid);
	void OnObjectUpdate(_U32 nIndex, const A_UUID& _uuid);
	void OnObjectDelete(_U32 nIndex, const A_UUID& _uuid);

	void SetEditObject(const A_UUID* pUUID, const DDLReflect::STRUCT_INFO*	pType, A_LIVE_OBJECT* pData);
	void ClearEditor();

private:
	bool							m_IsObjectNew;
	A_UUID							m_ObjectUUID;
	const DDLReflect::STRUCT_INFO*	m_ObjectType;
	A_LIVE_OBJECT*					m_pObjectData;

	wxListCtrl* m_pDataList;
	wxStaticText* m_pBarText;
	CStructEditView* m_pDataView;
	Zion::Map<_U32, CClientDataSyncProxy*> m_ProxyMap;
};

#endif // _CLIENT_CASE_VIEW_H__
