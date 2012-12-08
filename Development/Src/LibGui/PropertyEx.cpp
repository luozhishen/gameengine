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
#include "RefSelDlg.h"

#include "PropertyEx.h"

wxObjectRefProperty::wxObjectRefProperty(const wxString& label, const wxString& name, const DDLReflect::FIELD_INFO* finfo, void* data) : CPropertyEx<wxString, wxLongStringProperty>(label, name, finfo, data, wxT(""))
{
	UpdateValue();
}

bool wxObjectRefProperty::OnButtonClick(wxPropertyGrid* propGrid, wxString& strValue)
{
	CRefSelDlg dlg(propGrid, wxString::FromUTF8(m_finfo->ref_type));
	if(dlg.ShowModal()==wxID_OK)
	{
		AUuidFromString((const char*)dlg.GetSelect().ToUTF8(), *((A_UUID*)m_data));
		UpdateValue();
	}

	return true;
}

void wxObjectRefProperty::UpdateValue()
{
	const A_CONTENT_OBJECT* obj = Atlas::ContentObject::Query(*((A_UUID*)m_data));
	if(obj)
	{
		char name[100];
		AUuidToString(*((A_UUID*)m_data), name);
		wxString value;
		value.Printf(wxT("name %s uuid %s"), wxString::FromUTF8(obj->name._Value), wxString::FromUTF8(name));
		SetValueFromString(value);
	}
	else
	{
		SetValueFromString(wxT("empty"));
	}
}

wxArrayProperty::wxArrayProperty(const wxString& label, const wxString& name, const DDLReflect::FIELD_INFO* finfo, void* data) : CPropertyEx<_U32, wxUIntProperty>(label, name, finfo, data, *((_U32*)data))
{
	memcpy(&m_static_finfo, finfo, sizeof(*finfo));
	m_static_finfo.type &= DDLReflect::TYPE_MASK;
}

bool wxArrayProperty::ModifyValue(const wxVariant& value)
{
	_U32 count = (_U32)value.GetInteger();
	if(count>=(_U32)m_finfo->alen)
	{
		SetValueFromString(wxString::Format(wxT("%d"), *((_U32*)m_data)));
		return false;
	}
	*((_U32*)m_data) = count;
	for(_U32 i=0; i<(_U32)m_finfo->alen; i++)
	{
		m_items[i]->Hide(i>=count);
	}
	
	SetValueFromString(wxString::Format(wxT("%d"), count));
	return true;
}
