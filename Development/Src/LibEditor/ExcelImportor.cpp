#include <wx/wx.h>
#include <wx/string.h>

#include <AtlasBase.h>
#include <AtlasCommon.h>

#include <fstream>
#include "ExcelImportor.h"
#include "OLEAutoExcelWrapper.h"

struct CONTENT_EXECEL_FIELDINFO
{
	Atlas::String colum;
	Atlas::String field;
	CONTENT_EXCEL_ENUM* _enum;
};

struct CONTENT_EXCEL_TEMPLATE
{
	Atlas::String name;
	const DDLReflect::STRUCT_INFO* info;
	unsigned int title_line;
	unsigned int start_line;
	bool clear_data;
	Atlas::Map<Atlas::String, CONTENT_EXECEL_FIELDINFO> m_fields;
};

CContentExcelImportor::CContentExcelImportor()
{
}

CContentExcelImportor::~CContentExcelImportor()
{
	ClearTemplateDefine();
}

void CContentExcelImportor::GetTemplateList(Atlas::Vector<Atlas::String>& list)
{
	list.clear();

	Atlas::Map<Atlas::String, CONTENT_EXCEL_TEMPLATE*>::iterator i2;
	for(i2=m_tmpl_map.begin(); i2!=m_tmpl_map.end(); i2++)
	{
		list.push_back(i2->second->name.c_str());
	}
}

void CContentExcelImportor::ClearTemplateDefine()
{
	Atlas::Map<Atlas::String, CONTENT_EXCEL_ENUM*>::iterator i1;
	for(i1=m_enum_map.begin(); i1!=m_enum_map.end(); i1++)
	{
		delete i1->second;
	}
	m_enum_map.clear();

	Atlas::Map<Atlas::String, CONTENT_EXCEL_TEMPLATE*>::iterator i2;
	for(i2=m_tmpl_map.begin(); i2!=m_tmpl_map.end(); i2++)
	{
		delete i2->second;
	}
	m_tmpl_map.clear();
}

bool CContentExcelImportor::LoadTemplateDefine(const char* filename)
{
	ClearTemplateDefine();

	Json::Value root;
	Json::Reader reader;
	std::ifstream f(filename, std::ifstream::binary);
	if(!f.is_open() || !reader.parse(f, root, false))
	{
		m_errmsg = "can't open file or parse failed";
		return false;
	}

	Json::Value enums = root.get("enums", Json::Value());
	Json::Value tmpls = root.get("templates", Json::Value());
	if(!enums.isArray() || !tmpls.isArray())
	{
		m_errmsg = "enums or templates not found";
		return false;
	}

	Json::Value::Members names = enums.getMemberNames();
	for(size_t ei=0; ei<names.size(); ei++)
	{
		Json::Value _enum = enums.get(names[ei], Json::Value());
		if(!_enum.isObject())
		{
			m_errmsg = Atlas::StringFormat("enum [%s] not object", names[ei].c_str());
			return false;
		}

		if(m_enum_map.find(names[ei])!=m_enum_map.end())
		{
			m_errmsg = Atlas::StringFormat("enum [%s] duplicate", names[ei].c_str());
			return false;
		}

		m_enum_map[names[ei]] = new CONTENT_EXCEL_ENUM;
		CONTENT_EXCEL_ENUM& _map = *m_enum_map[names[ei]];

		Json::Value::Members keys = _enum.getMemberNames();
		for(size_t i=0; i<keys.size(); i++)
		{
			Json::Value val = _enum.get(keys[i], Json::Value());
			if(!val.isString())
			{
				m_errmsg = Atlas::StringFormat("enum [%s] item %s invalid type", names[ei].c_str(), keys[i].c_str());
				return false;
			}
			if(_map.find(keys[i])!=_map.end())
			{
				m_errmsg = Atlas::StringFormat("enum [%s] item %s duplicate", names[ei].c_str(), keys[i].c_str());
				return false;
			}
			_map[keys[i]] = val.asString();
		}
	}

	for(int t=0; t<(int)tmpls.size(); t++)
	{
		Json::Value jtmpl = tmpls[t];
		if(!jtmpl.isObject()) return false;
		Json::Value name = jtmpl.get("name", Json::Value());
		Json::Value type = jtmpl.get("type", Json::Value());
		Json::Value clear = jtmpl.get("clear", Json::Value());
		Json::Value titleline = jtmpl.get("title", Json::Value());
		Json::Value startline = jtmpl.get("start", Json::Value());
		Json::Value fields = jtmpl.get("fields", Json::Value());
		if(!name.isString()) return false;
		if(!type.isString()) return false;
		if(!type.isBool()) return false;
		if(!titleline.isUInt() || !startline.isUInt() || startline.asUInt()==0) return false;
		if(!fields.isArray()) return false;

		if(m_tmpl_map.find(name.asString())!=m_tmpl_map.end())
		{
			m_errmsg = Atlas::StringFormat("duplicate template [%s]", name.asCString());
			return false;
		}

		Atlas::Set<Atlas::String> pkey;
		if(!Atlas::ContentObject::GetTypePrimaryKey(type.asCString(), pkey))
		{
			m_errmsg = Atlas::StringFormat("error in GetTypePrimaryKey(%s)", type.asCString());
			return false;
		}

		m_tmpl_map[name.asCString()] = new CONTENT_EXCEL_TEMPLATE;
		CONTENT_EXCEL_TEMPLATE& tmpl = *m_tmpl_map[name.asCString()];
		tmpl.name = name.asCString();
		tmpl.info = Atlas::ContentObject::GetType(type.asCString());
		tmpl.clear_data = clear.asBool();
		tmpl.title_line = titleline.asUInt();
		tmpl.start_line = startline.asUInt();

		for(int i=0; i<(int)fields.size(); i++)
		{
			Json::Value item = fields[i];
			if(!item.isObject())
			{
				return false;
			}

			Json::Value col = item.get("c", Json::Value());
			Json::Value fld = item.get("f", Json::Value());
			Json::Value enu = item.get("e", Json::Value(""));
			if(!col.isString() || !fld.isString() || !enu.isString()) return false;

			DDLReflect::FIELD_INFO finfo;
			const void* fdata;
			if(!DDLReflect::GetStructFieldInfo(tmpl.info, fld.asCString(), (const void*)NULL, finfo, fdata))
			{
				return false;
			}

			Atlas::Map<Atlas::String, CONTENT_EXECEL_FIELDINFO>::iterator fi;
			for(fi=tmpl.m_fields.begin(); fi!=tmpl.m_fields.end(); fi++)
			{
				if(fi->second.colum==col.asCString())
				{
					return false;
				}
				if(fi->second.field==fld.asCString())
				{
					return false;
				}
			}

			CONTENT_EXECEL_FIELDINFO fieldinfo;
			fieldinfo.colum = col.asCString();
			fieldinfo.field = fld.asCString();
			if(enu.asString().empty())
			{
				fieldinfo._enum = NULL;
			}
			else
			{
				Atlas::Map<Atlas::String, CONTENT_EXCEL_ENUM*>::iterator i;
				i = m_enum_map.find(enu.asCString());
				if(i==m_enum_map.end())
				{
					return false;
				}
				fieldinfo._enum = i->second;
			}

			if(pkey.find(fld.asCString())!=pkey.end())
			{
				pkey.erase(fld.asCString());
			}

			tmpl.m_fields[col.asCString()] = fieldinfo;
		}

		if(!pkey.empty())
		{
			return false;
		}
	}

	return true;
}

Atlas::String GetColumnName(unsigned int col)
{
	if(col>=26)
	{
		return Atlas::StringFormat("%c%c", 'A'+col/26-1, 'A'+col%26);
	}
	else
	{
		return Atlas::StringFormat("%c:%d", 'A'+col);
	}
}

Atlas::String GetCellName(unsigned int col, unsigned int row)
{
	if(col>=26)
	{
		return Atlas::StringFormat("%c%c:%d", 'A'+col/26-1, 'A'+col%26, row+1);
	}
	else
	{
		return Atlas::StringFormat("%c:%d", 'A'+col, row+1);
	}
}

static bool PrimaryKeyIsUUID(const char* name)
{
	Atlas::Set<Atlas::String> keys;
	if(!Atlas::ContentObject::GetTypePrimaryKey(name, keys)) ATLAS_ASSERT(0);
	return keys.size()==1 && (*keys.begin())=="uuid";
}

bool CContentExcelImportor::ImportSheet(const char* _tmpl, COLEAutoExcelWrapper* excel)
{
	if(m_tmpl_map.find(_tmpl)==m_tmpl_map.end()) return false;
	CONTENT_EXCEL_TEMPLATE& tmpl = *m_tmpl_map[_tmpl];

	Atlas::Map<Atlas::String, Atlas::String> field_map;
	if(tmpl.title_line==0)
	{
		Atlas::Map<Atlas::String, CONTENT_EXECEL_FIELDINFO>::iterator fi;
		for(fi=tmpl.m_fields.begin(); fi!=tmpl.m_fields.end(); fi++)
		{
			field_map[fi->second.field] = fi->second.colum;
		}
	}
	else
	{
		for(unsigned int col=0; col<200; col++)
		{
			Atlas::String ColName;
			if(!excel->GetCellValue(GetCellName(0, tmpl.title_line-1), ColName))
			{
				m_errmsg = "error in GetCellValue";
				return false;
			}
			if(tmpl.m_fields.find(ColName)==tmpl.m_fields.end()) continue;
			Atlas::String FieldName = tmpl.m_fields[ColName].field;
			if(field_map.find(FieldName)!=field_map.end())
			{
				m_errmsg = Atlas::StringFormat("field duplicate %s", FieldName.c_str());
				return false;
			}

			field_map[FieldName] = GetColumnName(col);
		}

		if(field_map.size()!=tmpl.m_fields.size())
		{
			m_errmsg = "field count not match template field count";
			return false;
		}
	}

	bool bUUID = PrimaryKeyIsUUID(tmpl.info->name);
	_U16 type_id = Atlas::ContentObject::GetTypeId(tmpl.info->name);

	for(unsigned int row=tmpl.start_line-1; row<65536; row++)
	{
		A_CONTENT_OBJECT* obj = (A_CONTENT_OBJECT*)malloc(tmpl.info->size);
		if(!obj)
		{
			m_errmsg = "malloc error";
			return false;
		}
		memset(obj, 0, tmpl.info->size);

		Atlas::Map<Atlas::String, Atlas::String>::iterator i;
		for(i=field_map.begin(); i!=field_map.end(); i++)
		{
			Atlas::String val;
			if(!excel->GetCellValue(Atlas::StringFormat("%s:%d", i->second.c_str(), i), val))
			{
				m_errmsg = "error in GetCellValue";
				free(obj);
				return false;
			}
			if(!DDLReflect::StructParamFromString(tmpl.info, i->first.c_str(), obj, val.c_str()))
			{
				m_errmsg = Atlas::StringFormat("error in StructParamFromString(%s, %s, %s)", tmpl.info->name, i->first.c_str(), val.c_str());
				free(obj);
				return false;
			}
		}

		Atlas::String pkey;
		if(!Atlas::ContentObject::GenContentObjectUniqueId(type_id, obj, pkey))
		{
			m_errmsg = "error in GenContentObjectUniqueId";
			free(obj);
			return false;
		}

		A_CONTENT_OBJECT* old_obj = (A_CONTENT_OBJECT*)Atlas::ContentObject::QueryByUniqueId(tmpl.info, pkey.c_str());
		if(old_obj)
		{
			old_obj = Atlas::ContentObject::Modify(old_obj->uuid, tmpl.info);
			ATLAS_ASSERT(old_obj);
			obj->uuid = old_obj->uuid;
		}
		else
		{
			old_obj = Atlas::ContentObject::CreateObject(tmpl.info, obj->uuid);
			ATLAS_ASSERT(old_obj);
			if(bUUID)
			{
				Atlas::String range = Atlas::StringFormat("%s:%d", field_map["uuid"].c_str(), row);
				char suuid[60];
				AUuidToString(old_obj->uuid, suuid);
				excel->SetCellValue(range, Atlas::String(suuid));
			}
		}

		memcpy(old_obj, obj, tmpl.info->size);
		free(obj);
	}

	return true;
}
