#ifndef __ZION_STRUCT_EDITOR_VIEW__
#define __ZION_STRUCT_EDITOR_VIEW__

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include "PropertyEx.h"

class CStructEditView : public wxPanel
{
	DECLARE_EVENT_TABLE()

public:
	CStructEditView(wxWindow* pParent);
	virtual ~CStructEditView();
		
	void Clear();
	void Updata();
	bool Set(const DDLReflect::STRUCT_INFO* info, const void* data);
	bool Set(const DDLReflect::STRUCT_INFO* info, void* data);
	bool Get(const DDLReflect::STRUCT_INFO* info, void* data);

	void OnPropertyGridChange(wxPropertyGridEvent& event);

private:
	void InitPropGrid(const DDLReflect::STRUCT_INFO* info, const void* data);
	void CreateStruct(wxPGProperty* id, const wxString& name, const DDLReflect::STRUCT_INFO* info, void* data);
	void CreateArray(wxPGProperty* id, const wxString& name, const DDLReflect::FIELD_INFO* finfo, void* data);
	wxPGProperty* CreateProperty(wxPGProperty* id, const wxString& name, const DDLReflect::FIELD_INFO* finfo, void* data, _U32 index=-1);

private:
	wxPropertyGrid* m_pPropGrid;
	const DDLReflect::STRUCT_INFO* m_pInfo;
	void* m_pData;
	bool m_bDataFree;
};

#endif
