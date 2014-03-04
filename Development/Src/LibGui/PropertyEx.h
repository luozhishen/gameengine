#ifndef _PROPERTY_EX_H__
#define _PROPERTY_EX_H__

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

class wxArrayProperty;

template<class T, class BASE>
class CPropertyEx : public BASE
{
public:
	CPropertyEx(const wxString& label, const wxString& name, const DDLReflect::FIELD_INFO* finfo, void* data, const T& value) : BASE(label, name, value)
	{
		m_data = data;
		m_finfo = finfo;
		SetClientData((ClientDataType)m_finfo);
	}

	virtual ~CPropertyEx()
	{
		SetClientData(NULL);
	}

	virtual bool ModifyValue(const wxVariant& value)
	{
		switch(m_finfo->type&DDLReflect::TYPE_MASK)
		{
		case DDLReflect::TYPE_U8:
			*((_U8*)m_data) = (_U8)value.GetInteger();
			break;
		case DDLReflect::TYPE_U16:	
			*((_U16*)m_data) = (_U16)value.GetInteger();
			break;
		case DDLReflect::TYPE_U32:	
			*((_U32*)m_data) = (_U32)value.GetInteger();
			break;
		case DDLReflect::TYPE_U64:	
			*((_U64*)m_data) = (_U64)value.GetInteger();
			break;
		case DDLReflect::TYPE_S8:
			*((_S8*)m_data) = (_S8)value.GetInteger();
			break;
		case DDLReflect::TYPE_S16:
			*((_S16*)m_data) = (_S16)value.GetInteger();
			break;
		case DDLReflect::TYPE_S32:	
			*((_S32*)m_data) = (_S32)value.GetInteger();
			break;
		case DDLReflect::TYPE_S64:
			*((_S64*)m_data) = (_S64)value.GetInteger();
			break;
		case DDLReflect::TYPE_F32:	
			*((_F32*)m_data) = (_F32)value.GetDouble();
			break;
		case DDLReflect::TYPE_F64:
			*((_F64*)m_data) = (_F64)value.GetDouble();
			break;
		case DDLReflect::TYPE_STRING:
			{
				std::string val = (const char*)value.GetString().ToUTF8();
				if((size_t)m_finfo->slen<=val.size())
				{
					ZION_ASSERT(0);
					return false;
				}
				strcpy((char*)m_data, val.c_str());
			}
			break;
		case DDLReflect::TYPE_UUID:
			{
				std::string val = (const char*)value.GetString().ToUTF8();
				if(!AUuidFromString(val.c_str(), *((A_UUID*)m_data)))
				{
					ZION_ASSERT(0);
					return false;
				}
			}
			break;
		case DDLReflect::TYPE_STRUCT:
			if(strcmp(m_finfo->sinfo->name, "A_CONFUSED_U32")==0)
			{
				Zion::CNUM_SET(*((A_CONFUSED_U32*)m_data), (_U32)value.GetInteger());
				break;
			}
			if(strcmp(m_finfo->sinfo->name, "A_CONFUSED_S32")==0)
			{
				Zion::CNUM_SET(*((A_CONFUSED_S32*)m_data), (_S32)value.GetInteger());
				break;
			}
			if(strcmp(m_finfo->sinfo->name, "A_CONFUSED_F32")==0)
			{
				Zion::CNUM_SET(*((A_CONFUSED_F32*)m_data), (_F32)value.GetDouble());
				break;
			}
		default:
			ZION_ASSERT(0);
			return false;
		}
		return true;
	}

	void* m_data;
	const DDLReflect::FIELD_INFO* m_finfo;
};

class wxObjectRefProperty : public CPropertyEx<wxString, wxLongStringProperty>
{
public:
	wxObjectRefProperty(const wxString& label, const wxString& name, const DDLReflect::FIELD_INFO* finfo, void* data);

	virtual bool OnButtonClick(wxPropertyGrid* propGrid, wxString& strValue);

	void UpdateValue();
};

class wxArrayProperty : public CPropertyEx<_U32, wxUIntProperty>
{
public:
	wxArrayProperty(const wxString& label, const wxString& name, const DDLReflect::FIELD_INFO* finfo, void* data);

	virtual bool ModifyValue(const wxVariant& value);

	DDLReflect::FIELD_INFO m_static_finfo;
	Zion::Array<wxPGProperty*> m_items;
};

#endif //_PROPERTY_EX_H__
