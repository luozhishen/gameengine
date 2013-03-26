#include <wx/wx.h>
#include <wx/string.h>

#include <AtlasBase.h>
#include <AtlasCommon.h>

#include <fstream>
#include "ExcelImportor.h"
#include "OLEAutoExcelWrapper.h"

struct CONTENT_EXECEL_FIELDINFO
{
	Atlas::String column;
	Atlas::String field;
	Atlas::String defval;
	bool notempty;
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
		if(!type.isString())
		{
			m_errmsg = Atlas::StringFormat("template [%s] type invalid value", names[t].c_str());
			return false;
		}
		if(!clear.isBool())
		{
			m_errmsg = Atlas::StringFormat("template [%s] clear invalid value", names[t].c_str());
			return false;
		}
		if(!titleline.isInt() || titleline.asInt()<0)
		{
			m_errmsg = Atlas::StringFormat("template [%s] title invalid value", names[t].c_str());
			return false;
		}
		if(!startline.isInt() || startline.asInt()<=0)
		{
			m_errmsg = Atlas::StringFormat("template [%s] start invalid value", names[t].c_str());
			return false;
		}
		if(!fields.isArray())
		{
			m_errmsg = Atlas::StringFormat("template [%s] fields invalid value", names[t].c_str());
			return false;
		}

		if(m_tmpl_map.find(names[t])!=m_tmpl_map.end())
		{
			m_errmsg = Atlas::StringFormat("duplicate template [%s]", names[t].c_str());
			return false;
		}

		Atlas::Set<Atlas::String> pkey;
		if(!Atlas::ContentObject::GetTypePrimaryKey(type.asCString(), pkey))
		{
			m_errmsg = Atlas::StringFormat("template [%s] error in GetTypePrimaryKey(%s)", names[t].c_str(), type.asCString());
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
				m_errmsg = Atlas::StringFormat("template [%s] fields[%d] invalid value", names[t].c_str(), i);
				return false;
			}

			Json::Value col = item.get("column",		Json::Value());
			Json::Value fld = item.get("field",		Json::Value());
			Json::Value def = item.get("default",	Json::Value(""));
			Json::Value notempty = item.get("notempty", Json::Value(false));
			Json::Value enu = item.get("enum",		Json::Value(""));
			if(!col.isString() || !fld.isString() || !def.isString() || !notempty.isBool() || !enu.isString())
			{
				m_errmsg = Atlas::StringFormat("template [%s] fields[%d] invalid value", names[t].c_str(), i);
			}

			if(DDLReflect::GetStructFieldOffset(tmpl.info, fld.asCString())==(_U32)-1)
			{
				m_errmsg = Atlas::StringFormat("template [%s] fields[%d] error in GetStructFieldData(%s, %s)", names[t].c_str(), i, tmpl.info->name, fld.asCString());
				return false;
			}

			Atlas::Map<Atlas::String, CONTENT_EXECEL_FIELDINFO>::iterator fi;
			for(fi=tmpl.m_fields.begin(); fi!=tmpl.m_fields.end(); fi++)
			{
				if(fi->second.column==col.asString())
				{
					m_errmsg = Atlas::StringFormat("template [%s] fields[%d] column[%s] already existed", names[t].c_str(), i, col.asCString());
					return false;
				}
				if(fi->second.field==fld.asString())
				{
					m_errmsg = Atlas::StringFormat("template [%s] fields[%d] field[%s] already existed", names[t].c_str(), i, fld.asCString());
					return false;
				}
			}

			CONTENT_EXECEL_FIELDINFO fieldinfo;
			fieldinfo.column = col.asString();
			fieldinfo.field = fld.asString();
			fieldinfo.defval = def.asString();
			fieldinfo.notempty = notempty.asBool();
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
					m_errmsg = Atlas::StringFormat("template [%s] fields[%d] enum[%s] not found", names[t].c_str(), i, enu.asCString());
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
			m_errmsg = Atlas::StringFormat("primary key not existed");
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
	if(m_tmpl_map.find(_tmpl)==m_tmpl_map.end())
	{
		m_errmsg = Atlas::StringFormat("template [%s] not found", _tmpl);
		return false;
	}

	CONTENT_EXCEL_TEMPLATE& tmpl = *m_tmpl_map[_tmpl];
	Atlas::String sUUID;
	Atlas::Set<A_UUID> oldobjs;

	if(tmpl.clear_data)
	{
		const A_CONTENT_OBJECT* it = Atlas::ContentObject::FindFirst(tmpl.info, true);
		while(it)
		{
			oldobjs.insert(it->uuid);
			it = Atlas::ContentObject::FindNext(tmpl.info, true, it);
		}
	}

	Atlas::Map<Atlas::String, CONTENT_EXECEL_FIELDINFO*> field_map;
	if(tmpl.title_line==0)
	{
		Atlas::Map<Atlas::String, CONTENT_EXECEL_FIELDINFO>::iterator fi;
		for(fi=tmpl.m_fields.begin(); fi!=tmpl.m_fields.end(); fi++)
		{
			field_map[fi->second.column] = &fi->second;
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

			if(val.empty() && !i->second->defval.empty())
			{
				val = i->second->defval;
			}

			if(val.empty() && i->second->notempty)
			{
				m_errmsg = Atlas::StringFormat("value cannot empty %s", Atlas::StringFormat("%s%d", i->first.c_str(), row).c_str());
				return false;
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
				bExit = false;
			}
			else
			{
				if(!val.empty()) bExit = false;
				val_map[i->second->field] = val;
			}

		}
		if(bError) return false;
		if(bExit) break;

		A_CONTENT_OBJECT* obj = (A_CONTENT_OBJECT*)DDLReflect::CreateObject(tmpl.info);
		if(!obj)
		{
			m_errmsg = "malloc error";
			return false;
		}

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
					DDLReflect::DestoryObject(obj);
					return false;
				}
			}
		}

		Atlas::String pkey;
		if(!Atlas::ContentObject::GenContentObjectUniqueId(type_id, obj, pkey))
		{
			m_errmsg = "error in GenContentObjectUniqueId";
			DDLReflect::DestoryObject(obj);
			return false;
		}

		A_CONTENT_OBJECT* old_obj = (A_CONTENT_OBJECT*)Atlas::ContentObject::QueryByUniqueId(tmpl.info, pkey.c_str());
		if(old_obj)
		{
			const DDLReflect::STRUCT_INFO* info = Atlas::ContentObject::GetObjectType(old_obj->uuid);
			if(info!=tmpl.info)
			{
				m_errmsg = Atlas::StringFormat("content object type [%s - %s] not match uniqueid:{%s}", tmpl.info->name, info?info->name:"unknown", pkey.c_str()), 
				DDLReflect::DestoryObject(obj);
				return false;
			}
			old_obj = Atlas::ContentObject::Modify(old_obj->uuid, tmpl.info);
			ATLAS_ASSERT(old_obj);
			obj->uuid = old_obj->uuid;
			if(oldobjs.find(obj->uuid)!=oldobjs.end()) oldobjs.erase(obj->uuid);
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
					DDLReflect::DestoryObject(obj);
					return false;
				}
			}
		}

		memcpy(old_obj, obj, tmpl.info->size);
		DDLReflect::DestoryObject(obj);
	}

	Atlas::Set<A_UUID>::iterator it;
	for(it=oldobjs.begin(); it!=oldobjs.end(); it++)
	{
		Atlas::ContentObject::DeleteObject(*it);
	}

	return true;
}
