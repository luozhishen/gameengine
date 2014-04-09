#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/config.h>
#include <wx/utils.h>

#include <ZionBase.h>
#include "StructEditView.h"

enum
{
	ID_OBJECT_NAME = wxID_HIGHEST + 1,
	ID_OBJECT_TEXT,
	ID_OBJECT_GRID,
};

BEGIN_EVENT_TABLE(CStructEditView, wxPanel)
	EVT_PG_CHANGED(ID_OBJECT_GRID, CStructEditView::OnPropertyGridChange)
END_EVENT_TABLE()

CStructEditView::CStructEditView(wxWindow* pParent) : wxPanel(pParent)
{
	wxBoxSizer* m_pSizer;
	m_pSizer = ZION_NEW wxBoxSizer(wxVERTICAL);
	m_pPropGrid = ZION_NEW wxPropertyGrid(this, ID_OBJECT_GRID, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE );
	m_pPropGrid->SetExtraStyle( wxPG_EX_HELP_AS_TOOLTIPS );
	m_pPropGrid->SetColumnProportion(0, 30);
	m_pPropGrid->SetColumnProportion(1, 70);
	m_pSizer->Add(m_pPropGrid, 1, wxGROW|wxALIGN_CENTER_VERTICAL);
	SetSizer(m_pSizer);
	m_pInfo = NULL;
	m_pData = NULL;
}

CStructEditView::~CStructEditView()
{
	if(m_pData && m_bDataFree)
	{
		DDLReflect::DestoryObject(m_pInfo, m_pData);
		m_pData = NULL;
	}
}

void CStructEditView::Clear()
{
	m_pPropGrid->Clear();
	if(m_pData && m_bDataFree) DDLReflect::DestoryObject(m_pInfo, m_pData);
	m_pInfo = NULL;
	m_pData = NULL;
}

void CStructEditView::Updata()
{
	m_pPropGrid->UnfocusEditor();
	m_pPropGrid->UpdateWindowUI();
	m_pPropGrid->Update();
}

bool CStructEditView::Set(const DDLReflect::STRUCT_INFO* info, const void* data)
{
	Clear();

	if(info!=NULL && data!=NULL)
	{
		m_bDataFree = true;
		m_pData = DDLReflect::CreateObject(info);
		memcpy(m_pData, data, (size_t)info->size);
		m_pInfo = info;
		InitPropGrid(info, m_pData);
	}

	return true;
}

bool CStructEditView::Set(const DDLReflect::STRUCT_INFO* info, void* data)
{
	Clear();

	if(info!=NULL && data!=NULL)
	{
		m_bDataFree = false;
		m_pData = data;
		m_pInfo = info;
		InitPropGrid(info, m_pData);
	}

	return true;
}

bool CStructEditView::Get(const DDLReflect::STRUCT_INFO* info, void* data)
{
	if(m_pInfo!=info) return false;
	memcpy(data, m_pData, (size_t)info->size);
	return true;
}

void CStructEditView::OnPropertyGridChange(wxPropertyGridEvent &event)
{
	wxPGProperty* prop = event.GetProperty();
	wxVariant value = prop->GetValue();
	const DDLReflect::FIELD_INFO* finfo = (const DDLReflect::FIELD_INFO*)prop->GetClientData();

	if(finfo->type&DDLReflect::TYPE_ARRAY)
	{
		((wxArrayProperty*)prop)->ModifyValue(value);
	}
	else
	{
		switch(finfo->type&DDLReflect::TYPE_MASK)
		{
		case DDLReflect::TYPE_U8:		
			((CPropertyEx<_U8, wxUIntProperty>*)prop)->ModifyValue(value);
			break;
		case DDLReflect::TYPE_U16:	
			((CPropertyEx<_U16, wxUIntProperty>*)prop)->ModifyValue(value);
			break;
		case DDLReflect::TYPE_U32:	
			((CPropertyEx<_U32, wxUIntProperty>*)prop)->ModifyValue(value);
			break;
		case DDLReflect::TYPE_U64:	
			((CPropertyEx<_U64, wxUIntProperty>*)prop)->ModifyValue(value);
			break;
		case DDLReflect::TYPE_S8:
			((CPropertyEx<_S8, wxIntProperty>*)prop)->ModifyValue(value);
			break;
		case DDLReflect::TYPE_S16:
			((CPropertyEx<_S16, wxIntProperty>*)prop)->ModifyValue(value);
			break;
		case DDLReflect::TYPE_S32:	
			((CPropertyEx<_S32, wxIntProperty>*)prop)->ModifyValue(value);
			break;
		case DDLReflect::TYPE_S64:
			((CPropertyEx<_S64, wxIntProperty>*)prop)->ModifyValue(value);
			break;
		case DDLReflect::TYPE_F32:	
			((CPropertyEx<_F32, wxFloatProperty>*)prop)->ModifyValue(value);
			break;
		case DDLReflect::TYPE_F64:
			((CPropertyEx<_F64, wxFloatProperty>*)prop)->ModifyValue(value);
			break;
		case DDLReflect::TYPE_STRING:
			((CPropertyEx<wxString, wxStringProperty>*)prop)->ModifyValue(value);
			break;
		case DDLReflect::TYPE_UUID:
			((CPropertyEx<wxString, wxStringProperty>*)prop)->ModifyValue(value);
			break;
		case DDLReflect::TYPE_STRUCT:
			if(strcmp(finfo->sinfo->name, "A_CONFUSED_U32")==0)
			{
				((CPropertyEx<_U32, wxUIntProperty>*)prop)->ModifyValue(value);
				break;
			}
			if(strcmp(finfo->sinfo->name, "A_CONFUSED_S32")==0)
			{
				((CPropertyEx<_F32, wxIntProperty>*)prop)->ModifyValue(value);
				break;
			}
			if(strcmp(finfo->sinfo->name, "A_CONFUSED_F32")==0)
			{
				((CPropertyEx<_F32, wxFloatProperty>*)prop)->ModifyValue(value);
				break;
			}
		default:
			ZION_ASSERT(0);
			return;
		}
	}
}

void CStructEditView::InitPropGrid(const DDLReflect::STRUCT_INFO* info, const void* data)
{
	if(info->parent) InitPropGrid(info->parent, data);
	if(info->fcount==0) return;

	wxString name;
	name.Printf(wxT("{%s}"), wxString::FromUTF8(info->name));
	wxPGProperty* prop = ZION_NEW wxPropertyCategory(name);
	prop->ChangeFlag(wxPG_PROP_READONLY, true);
	wxPGProperty* prop_id = m_pPropGrid->Append(prop);

	for(_U32 i = 0; i < info->fcount; ++i)
	{
		const DDLReflect::FIELD_INFO* finfo = &info->finfos[i];
		if(info->finfos[i].type&DDLReflect::TYPE_ARRAY)
		{
			CreateArray(prop_id, wxString::FromUTF8(finfo->name),&info->finfos[i], (char*)data+finfo->offset);
		}
		else
		{
			CreateProperty(prop_id, wxString::FromUTF8(finfo->name), &info->finfos[i], (char*)data+finfo->offset);
		}
	}
}

void CStructEditView::CreateStruct(wxPGProperty* id, const wxString& name, const DDLReflect::STRUCT_INFO* info, void* data)
{
	if(info->parent) CreateStruct(id, name, info->parent, data);

	for(_U16 i=0; i<info->fcount; i++)
	{
		const DDLReflect::FIELD_INFO* finfo = &info->finfos[i];
		if(finfo->type&DDLReflect::TYPE_ARRAY)
		{
			CreateArray(id, name+wxT(".")+wxString::FromUTF8(finfo->name), finfo, (char*)data+finfo->offset);
		}
		else
		{
			CreateProperty(id, name+wxT(".")+wxString::FromUTF8(finfo->name), finfo, (char*)data+finfo->offset);
		}
	}
}

void CStructEditView::CreateArray(wxPGProperty* id, const wxString& name, const DDLReflect::FIELD_INFO* finfo, void* data)
{
	ZION_ASSERT(finfo->type|DDLReflect::TYPE_ARRAY);

	_U32& count = *((_U32*)data);
	wxString label(finfo->name, wxMBConvUTF8());
	wxArrayProperty* prop = ZION_NEW wxArrayProperty(label+wxString::Format(wxT("[%d]"), finfo->alen), name, finfo, data);//wxPG_LABEL, wxString::Format(wxT("array %d"), count));
	prop->ChangeFlag(wxPG_PROP_COLLAPSED, true);
	wxPGProperty* prog_id = m_pPropGrid->AppendIn(id, prop);
	for(_U32 i=0; i<(_U32)finfo->alen; i++)
	{
		wxPGProperty* item;
		item = CreateProperty(prog_id, wxString::Format(wxT("%s[%d]"), name, i), finfo, (char*)data+finfo->prefix+finfo->elen*i, i);
		prop->m_items.push_back(item);
		if(i>=*((_U32*)data)) item->Hide(true);
	}
}

wxPGProperty* CStructEditView::CreateProperty(wxPGProperty* id, const wxString& name, const DDLReflect::FIELD_INFO* finfo, void* data, _U32 index)
{
	wxString strLabel;
	wxPGProperty* prop = NULL;

	if(finfo->type&DDLReflect::TYPE_ARRAY)
	{
		strLabel.Printf(wxT("[%d]"), index);
	}
	else
	{
		strLabel = wxString::FromUTF8(finfo->name);
	}

	wxPGProperty* prog_id = 0;
	bool has_subitem = false;
	switch(finfo->type&DDLReflect::TYPE_MASK)
	{
	case DDLReflect::TYPE_U8:
		prop = ZION_NEW CPropertyEx<_U8, wxUIntProperty>(strLabel, name, finfo, data, *((const _U8*)data));
		prog_id = m_pPropGrid->AppendIn(id, prop);
		break;
	case DDLReflect::TYPE_U16:
		prop = ZION_NEW CPropertyEx<_U16, wxUIntProperty>(strLabel, name, finfo, data, *((const _U16*)data));
		prog_id = m_pPropGrid->AppendIn(id, prop);
		break;
	case DDLReflect::TYPE_U32:
		prop = ZION_NEW CPropertyEx<_U32, wxUIntProperty>(strLabel, name, finfo, data, *((const _U32*)data));
		prog_id = m_pPropGrid->AppendIn(id, prop);
		break;
	case DDLReflect::TYPE_U64:
		prop = ZION_NEW CPropertyEx<_U64, wxUIntProperty>(strLabel, name, finfo, data, *((const _U64*)data));
		prog_id = m_pPropGrid->AppendIn(id, prop);
		break;
	case DDLReflect::TYPE_S8:
		prop = ZION_NEW CPropertyEx<_S8, wxIntProperty>(strLabel, name, finfo, data, *((const _S8*)data));
		prog_id = m_pPropGrid->AppendIn(id, prop);
		break;
	case DDLReflect::TYPE_S16:
		prop = ZION_NEW CPropertyEx<_S16, wxIntProperty>(strLabel, name, finfo, data, *((const _S16*)data));
		prog_id = m_pPropGrid->AppendIn(id, prop);
		break;
	case DDLReflect::TYPE_S32:
		prop = ZION_NEW CPropertyEx<_S32, wxIntProperty>(strLabel, name, finfo, data, *((const _S32*)data));
		prog_id = m_pPropGrid->AppendIn(id, prop);
		break;
	case DDLReflect::TYPE_S64:
		prop = ZION_NEW CPropertyEx<_S64, wxIntProperty>(strLabel, name, finfo, data, *((const _S64*)data));
		prog_id = m_pPropGrid->AppendIn(id, prop);
		break;
	case DDLReflect::TYPE_F32:
		prop = ZION_NEW CPropertyEx<_F32, wxFloatProperty>(strLabel, name, finfo, data, *((const _F32*)data));
		prog_id = m_pPropGrid->AppendIn(id, prop);
		break;
	case DDLReflect::TYPE_F64:
		prop = ZION_NEW CPropertyEx<_F64, wxFloatProperty>(strLabel, name, finfo, data, *((const _F64*)data));
		prog_id = m_pPropGrid->AppendIn(id, prop);
		break;
	case DDLReflect::TYPE_STRING:
		prop = ZION_NEW CPropertyEx<wxString, wxStringProperty>(strLabel, name, finfo, data, wxString((const char*)data, wxMBConvUTF8()));
		prog_id = m_pPropGrid->AppendIn(id, prop);
		break;
	case DDLReflect::TYPE_UUID:
		{
			char value[100];
			AUuidToString(*((A_UUID*)data), value);
			prop = ZION_NEW CPropertyEx<wxString, wxStringProperty>(strLabel, name, finfo, data, wxString(value, wxMBConvUTF8()));
			prog_id = m_pPropGrid->AppendIn(id, prop);
		}
		break;
	case DDLReflect::TYPE_STRUCT:
		if(strcmp(finfo->sinfo->name, "A_CONFUSED_U32")==0)
		{
			prop = ZION_NEW CPropertyEx<_U32, wxUIntProperty>(strLabel, name, finfo, data, Zion::CNUM_GET(*((const A_CONFUSED_U32*)data)));
			prog_id = m_pPropGrid->AppendIn(id, prop);
			break;
		}
		if(strcmp(finfo->sinfo->name, "A_CONFUSED_S32")==0)
		{
			prop = ZION_NEW CPropertyEx<_S32, wxIntProperty>(strLabel, name, finfo, data, Zion::CNUM_GET(*((const A_CONFUSED_S32*)data)));
			prog_id = m_pPropGrid->AppendIn(id, prop);
			break;
		}
		if(strcmp(finfo->sinfo->name, "A_CONFUSED_F32")==0)
		{
			prop = ZION_NEW CPropertyEx<_F32, wxFloatProperty>(strLabel, name, finfo, data, Zion::CNUM_GET(*((const A_CONFUSED_F32*)data)));
			prog_id = m_pPropGrid->AppendIn(id, prop);
			break;
		}
		{
			wxString value(wxT("struct "));
			value += wxString(finfo->sinfo->name, wxMBConvUTF8());
			prop = ZION_NEW CPropertyEx<wxString, wxStringProperty>(strLabel, name, finfo, data, value);
			prop->ChangeFlag(wxPG_PROP_READONLY, true);
			prop->ChangeFlag(wxPG_PROP_COLLAPSED, true);
			prog_id = m_pPropGrid->AppendIn(id, prop);
			has_subitem = true;
		}
		break;
	case DDLReflect::TYPE_UUID_REF:
		prop = ZION_NEW wxObjectRefProperty(strLabel, name, finfo, data);
		prog_id = m_pPropGrid->AppendIn(id, prop);
		break;
	default:
		ZION_ASSERT(0);
		return NULL;
	}

	if((finfo->flags&DDLReflect::FLAG_READONLY)!=0)
	{
		prop->ChangeFlag(wxPG_PROP_READONLY, true);
	}
	
	if(has_subitem)
	{
		CreateStruct(prog_id, name, finfo->sinfo, data);
	}

	return prop;
}
