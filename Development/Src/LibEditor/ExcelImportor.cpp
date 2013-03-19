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
	if(!enums.isObject() || !tmpls.isObject())
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

	names = tmpls.getMemberNames();
	for(size_t t=0; t<names.size(); t++)
	{
		Json::Value jtmpl = tmpls.get(names[t], Json::Value());
		if(!jtmpl.isObject()) return false;
		Json::Value type = jtmpl.get("type", Json::Value());
		Json::Value clear = jtmpl.get("clear", Json::Value());
		Json::Value titleline = jtmpl.get("title", Json::Value());
		Json::Value startline = jtmpl.get("start", Json::Value());
		Json::Value fields = jtmpl.get("fields", Json::Value());
		if(!type.isString()) return false;
		if(!clear.isBool()) return false;
		if(!titleline.isInt() || !startline.isInt() || titleline.asInt()<0 || startline.asInt()<=0) return false;
		if(!fields.isArray()) return false;

		if(m_tmpl_map.find(names[t])!=m_tmpl_map.end())
		{
			m_errmsg = Atlas::StringFormat("duplicate template [%s]", names[t].c_str());
			return false;
		}

		Atlas::Set<Atlas::String> pkey;
		if(!Atlas::ContentObject::GetTypePrimaryKey(type.asCString(), pkey))
		{
			m_errmsg = Atlas::StringFormat("error in GetTypePrimaryKey(%s)", type.asCString());
			return false;
		}

		m_tmpl_map[names[t]] = new CONTENT_EXCEL_TEMPLATE;
		CONTENT_EXCEL_TEMPLATE& tmpl = *m_tmpl_map[names[t]];
		tmpl.name = names[t];
		tmpl.info = Atlas::ContentObject::GetType(type.asCString());
		tmpl.clear_data = clear.asBool();
		tmpl.title_line = titleline.asUInt();
		tmpl.start_line = startline.asUInt();

		for(int i=0; i<(int)fields.size(); i++)
		{
			Json::Value item = fields[i];
			if(!item.isObject())
			{
				m_errmsg = Atlas::StringFormat("error in GetTypePrimaryKey(%s)", type.asCString());
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
				m_errmsg = Atlas::StringFormat("error in GetTypePrimaryKey(%s)", type.asCString());
				return false;
			}

			Atlas::Map<Atlas::String, CONTENT_EXECEL_FIELDINFO>::iterator fi;
			for(fi=tmpl.m_fields.begin(); fi!=tmpl.m_fields.end(); fi++)
			{
				if(fi->second.colum==col.asString())
				{
					m_errmsg = Atlas::StringFormat("error in GetTypePrimaryKey(%s)", type.asCString());
					return false;
				}
				if(fi->second.field==fld.asString())
				{
					m_errmsg = Atlas::StringFormat("error in GetTypePrimaryKey(%s)", type.asCString());
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
					m_errmsg = Atlas::StringFormat("error in GetTypePrimaryKey(%s)", type.asCString());
					return false;
				}
				fieldinfo._enum = i->second;
			}

			if(pkey.find(fld.asString())!=pkey.end())
			{
				pkey.erase(fld.asCString());
			}

			tmpl.m_fields[col.asString()] = fieldinfo;
		}

		if(!pkey.empty())
		{
			m_errmsg = Atlas::StringFormat("error in GetTypePrimaryKey(%s)", type.asCString());
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
		return Atlas::StringFormat("%c", 'A'+col);
	}
}

Atlas::String GetCellName(unsigned int col, unsigned int row)
{
	if(col>=26)
	{
		return Atlas::StringFormat("%c%c%d", 'A'+col/26-1, 'A'+col%26, row+1);
	}
	else
	{
		return Atlas::StringFormat("%c%d", 'A'+col, row+1);
	}
}

bool CContentExcelImportor::ImportSheet(const char* _tmpl, COLEAutoExcelWrapper* excel)
{
	if(m_tmpl_map.find(_tmpl)==m_tmpl_map.end()) return false;
	CONTENT_EXCEL_TEMPLATE& tmpl = *m_tmpl_map[_tmpl];

	Atlas::String sUUID;

	Atlas::Map<Atlas::String, CONTENT_EXECEL_FIELDINFO*> field_map;
	if(tmpl.title_line==0)
	{
		Atlas::Map<Atlas::String, CONTENT_EXECEL_FIELDINFO>::iterator fi;
		for(fi=tmpl.m_fields.begin(); fi!=tmpl.m_fields.end(); fi++)
		{
			field_map[fi->second.colum] = &fi->second;
		}
	}
	else
	{
		for(unsigned int col=0; col<200; col++)
		{
			Atlas::String ColName;
			if(!excel->GetCellValue(GetCellName(col, tmpl.title_line-1), ColName))
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

			field_map[GetColumnName(col)] = &tmpl.m_fields[ColName];

			if(tmpl.m_fields[ColName].field=="uuid")
			{
				sUUID = GetColumnName(col);
			}
		}

		if(field_map.size()!=tmpl.m_fields.size())
		{
			m_errmsg = "field count not match template field count";
			return false;
		}
	}

	_U16 type_id = Atlas::ContentObject::GetTypeId(tmpl.info->name);

	for(unsigned int row=tmpl.start_line; row<65536; row++)
	{
		Atlas::Map<Atlas::String, Atlas::String> val_map;
		bool bExit = true, bError = false;

		Atlas::Map<Atlas::String, CONTENT_EXECEL_FIELDINFO*>::iterator i;
		for(i=field_map.begin(); i!=field_map.end(); i++)
		{
			Atlas::String val;
			if(!excel->GetCellValue(Atlas::StringFormat("%s%d", i->first.c_str(), row), val))
			{
				m_errmsg = "error in GetCellValue";
				return false;
			}
			if(!val.empty())
			{
				bExit = false;
			}

			if(i->second->_enum)
			{
				if(i->second->_enum->find(val)!=i->second->_enum->end())
				{
					val_map[i->second->field] = (*i->second->_enum)[val];
				}
				else
				{
					if(i->second->_enum->find("$$default$$")!=i->second->_enum->end())
					{
						val_map[i->second->field] = (*i->second->_enum)["$$default$$"];
					}
					else
					{
						m_errmsg = Atlas::StringFormat("invalid value %s", Atlas::StringFormat("%s%d", i->first.c_str(), row).c_str());
						bError = true;
					}
				}
			}
			else
			{
				val_map[i->second->field] = val;
			}

		}
		if(bExit) break;
		if(bError) return false;

		A_CONTENT_OBJECT* obj = (A_CONTENT_OBJECT*)malloc(tmpl.info->size);
		if(!obj)
		{
			m_errmsg = "malloc error";
			return false;
		}
		memset(obj, 0, tmpl.info->size);

		Atlas::Map<Atlas::String, Atlas::String>::iterator i1;
		for(i1=val_map.begin(); i1!=val_map.end(); i1++)
		{
			if(i1->first=="uuid" && i1->second.empty())
			{
			}
			else
			{
				if(!DDLReflect::StructParamFromString(tmpl.info, i1->first.c_str(), obj, i1->second.c_str()))
				{
					m_errmsg = Atlas::StringFormat("error in StructParamFromString(%s, %s, %s)", tmpl.info->name, i1->first.c_str(), i1->second.c_str());
					free(obj);
					return false;
				}
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
			if(!sUUID.empty())
			{
				Atlas::String range = Atlas::StringFormat("%s%d", sUUID.c_str(), row);
				char suuid[60];
				AUuidToString(old_obj->uuid, suuid);
				if(!excel->SetCellValue(range, Atlas::String(suuid)))
				{
					m_errmsg = "error in SetCellValue";
					free(obj);
					return false;
				}
			}
		}

		memcpy(old_obj, obj, tmpl.info->size);
		free(obj);
	}

	return true;
}
