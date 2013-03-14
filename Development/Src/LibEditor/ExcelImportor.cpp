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
	if(!f.is_open()) return true;
	if(!reader.parse(f, root, false)) return false;
	Json::Value enums = root.get("enums", Json::Value());
	Json::Value tmpls = root.get("templates", Json::Value());
	if(!enums.isArray() || !tmpls.isArray()) return false;

	for(int e=0; e<(int)enums.size(); e++)
	{
		Json::Value _enum = enums[e];
		if(!_enum.isObject()) return false;
		Json::Value name = _enum.get("name", Json::Value());
		if(!name.isString()) return false;
		Json::Value items = _enum.get("map", Json::Value());
		if(!items.isArray()) return false;

		if(m_enum_map.find(name.asString().c_str())!=m_enum_map.end()) return false;
		m_enum_map[name.asString().c_str()] = new CONTENT_EXCEL_ENUM;
		CONTENT_EXCEL_ENUM& _map = *m_enum_map[name.asString().c_str()];

		for(int i=0; i<(int)items.size(); i++)
		{
			Json::Value item = items[i];
			if(!item.isObject()) return false;
			Json::Value key = item.get("k", Json::Value());
			Json::Value val = item.get("v", Json::Value());
			if(!key.isString() || !val.isString()) return false;
			if(_map.find(key.asString().c_str())!=_map.end()) return false;
			_map[key.asString().c_str()] = val.asString().c_str();
		}
	}

	for(int t=0; t<(int)tmpls.size(); t++)
	{
		Json::Value jtmpl = tmpls[t];
		if(!jtmpl.isObject()) return false;
		Json::Value name = jtmpl.get("name", Json::Value());
		if(!name.isString()) return false;
		Json::Value type = jtmpl.get("type", Json::Value());
		if(!type.isString()) return false;
		Json::Value clear = jtmpl.get("clear", Json::Value());
		if(!type.isBool()) return false;
		Json::Value titleline = jtmpl.get("title", Json::Value());
		Json::Value startline = jtmpl.get("start", Json::Value());
		if(!titleline.isInt() || !startline.isInt()) return false;
		Json::Value fields = jtmpl.get("fields", Json::Value());
		if(!fields.isArray()) return false;

		Atlas::Set<Atlas::String> pkey;
		if(!Atlas::ContentObject::GetTypePrimaryKey(type.asCString(), pkey)) return false;
		if(m_tmpl_map.find(name.asCString())!=m_tmpl_map.end()) return false;

		m_tmpl_map[name.asCString()] = new CONTENT_EXCEL_TEMPLATE;
		CONTENT_EXCEL_TEMPLATE& tmpl = *m_tmpl_map[name.asCString()];
		tmpl.name = name.asCString();
		tmpl.info = Atlas::ContentObject::GetType(type.asCString());
		tmpl.clear_data = clear.asBool();
		tmpl.title_line = (unsigned int)titleline.asInt();
		tmpl.start_line = (unsigned int)startline.asInt();

		for(int i=0; i<(int)fields.size(); i++)
		{
			Json::Value item = fields[i];
			if(!item.isObject()) return false;

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
				if(fi->second.colum==col.asCString()) return false;
				if(fi->second.field==fld.asCString()) return false;
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
				if(i==m_enum_map.end()) return false;
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
	if(tmpl.title_line<0)
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
			if(!excel->GetCellValue(GetCellName(0, tmpl.title_line), ColName))
			{
				return false;
			}
			if(tmpl.m_fields.find(ColName)==tmpl.m_fields.end()) continue;
			Atlas::String FieldName = tmpl.m_fields[ColName].field;
			if(field_map.find(FieldName)!=field_map.end())
			{
				return false;
			}

			field_map[FieldName] = GetColumnName(col);
		}

		if(field_map.size()!=tmpl.m_fields.size())
		{
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
			return false;
		}
		memset(obj, 0, tmpl.info->size);

		Atlas::Map<Atlas::String, Atlas::String>::iterator i;
		for(i=field_map.begin(); i!=field_map.end(); i++)
		{
			Atlas::String val;
			if(!excel->GetCellValue(Atlas::StringFormat("%s:%d", i->second.c_str(), i), val))
			{
				free(obj);
				return false;
			}
			if(!DDLReflect::StructParamFromString(tmpl.info, i->first.c_str(), obj, val.c_str()))
			{
				free(obj);
				return false;
			}
		}

		Atlas::String pkey;
		if(!Atlas::ContentObject::GenContentObjectUniqueId(type_id, obj, pkey))
		{
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
			old_obj = Atlas::ContentObject::Create(tmpl.info, obj->uuid);
			ATLAS_ASSERT(old_obj);
		}

		memcpy(old_obj, obj, tmpl.info->size);
		free(obj);
	}

	return true;
}
