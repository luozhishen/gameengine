#include <wx/wx.h>
#include <wx/string.h>

#include <ZionBase.h>
#include <ZionCommon.h>

#include <fstream>
#include "ExcelImportor.h"
#include "OLEAutoExcelWrapper.h"

struct EXCEL_FIELDINFO
{
	Zion::String column;
	Zion::String field;
	Zion::String defval;
	bool notempty;
	EXCEL_ENUM* _enum;
};

struct EXCEL_TEMPLATE
{
	Zion::String name;
	const DDLReflect::STRUCT_INFO* info;
	unsigned int title_line;
	unsigned int start_line;
	Zion::Map<Zion::String, EXCEL_FIELDINFO> m_fields;
};

CContentExcelImportor::CContentExcelImportor()
{
}

CContentExcelImportor::~CContentExcelImportor()
{
	ClearTemplateDefine();
}

void CContentExcelImportor::GetTemplateList(Zion::Array<Zion::String>& list)
{
	list.clear();

	Zion::Map<Zion::String, EXCEL_TEMPLATE*>::iterator i2;
	for(i2=m_tmpl_map.begin(); i2!=m_tmpl_map.end(); i2++)
	{
		list.push_back(i2->second->name.c_str());
	}
}

bool CContentExcelImportor::GetSheetNames(const char* filename, Zion::Array<Zion::String>& sheets)
{
	COLEAutoExcelWrapper* xls = ZION_NEW COLEAutoExcelWrapper();
	if(xls->Open(filename, false))
	{
		if(xls->GetExcelSheets(sheets))
		{
			return true;
		}
		xls->Quit();
	}
	return false;
}

void CContentExcelImportor::ClearTemplateDefine()
{
	Zion::Map<Zion::String, EXCEL_ENUM*>::iterator i1;
	for(i1=m_enum_map.begin(); i1!=m_enum_map.end(); i1++)
	{
		delete i1->second;
	}
	m_enum_map.clear();

	Zion::Map<Zion::String, EXCEL_TEMPLATE*>::iterator i2;
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
			m_errmsg = Zion::StringFormat("enum [%s] not object", names[ei].c_str());
			return false;
		}

		if(m_enum_map.find(names[ei])!=m_enum_map.end())
		{
			m_errmsg = Zion::StringFormat("enum [%s] duplicate", names[ei].c_str());
			return false;
		}

		m_enum_map[names[ei]] = new EXCEL_ENUM;
		EXCEL_ENUM& _map = *m_enum_map[names[ei]];

		Json::Value::Members keys = _enum.getMemberNames();
		for(size_t i=0; i<keys.size(); i++)
		{
			Json::Value val = _enum.get(keys[i], Json::Value());
			if(!val.isString())
			{
				m_errmsg = Zion::StringFormat("enum [%s] item %s invalid type", names[ei].c_str(), keys[i].c_str());
				return false;
			}
			if(_map.find(keys[i])!=_map.end())
			{
				m_errmsg = Zion::StringFormat("enum [%s] item %s duplicate", names[ei].c_str(), keys[i].c_str());
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
		Json::Value titleline = jtmpl.get("title", Json::Value());
		Json::Value startline = jtmpl.get("start", Json::Value());
		Json::Value fields = jtmpl.get("fields", Json::Value());
		if(!type.isString())
		{
			m_errmsg = Zion::StringFormat("template [%s] type invalid value", names[t].c_str());
			return false;
		}
		if(!titleline.isInt() || titleline.asInt()<0)
		{
			m_errmsg = Zion::StringFormat("template [%s] title invalid value", names[t].c_str());
			return false;
		}
		if(!startline.isInt() || startline.asInt()<=0)
		{
			m_errmsg = Zion::StringFormat("template [%s] start invalid value", names[t].c_str());
			return false;
		}
		if(!fields.isArray())
		{
			m_errmsg = Zion::StringFormat("template [%s] fields invalid value", names[t].c_str());
			return false;
		}

		if(m_tmpl_map.find(names[t])!=m_tmpl_map.end())
		{
			m_errmsg = Zion::StringFormat("duplicate template [%s]", names[t].c_str());
			return false;
		}

		Zion::Set<Zion::String> pkey;
		if(!Zion::ContentObject::GetTypePrimaryKey(type.asCString(), pkey))
		{
			m_errmsg = Zion::StringFormat("template [%s] error in GetTypePrimaryKey(%s)", names[t].c_str(), type.asCString());
			return false;
		}

		m_tmpl_map[names[t]] = new EXCEL_TEMPLATE;
		EXCEL_TEMPLATE& tmpl = *m_tmpl_map[names[t]];
		tmpl.name = names[t];
		tmpl.info = Zion::ContentObject::GetType(type.asCString());
		tmpl.title_line = titleline.asUInt();
		tmpl.start_line = startline.asUInt();

		for(int i=0; i<(int)fields.size(); i++)
		{
			Json::Value item = fields[i];
			if(!item.isObject())
			{
				m_errmsg = Zion::StringFormat("template [%s] fields[%d] invalid value", names[t].c_str(), i);
				return false;
			}

			Json::Value col = item.get("column",		Json::Value(""));
			Json::Value fld = item.get("field",		Json::Value());
			Json::Value def = item.get("default",	Json::Value(""));
			Json::Value notempty = item.get("notempty", Json::Value(false));
			Json::Value enu = item.get("enum",		Json::Value(""));
			if(!col.isString() || !fld.isString() || !def.isString() || !notempty.isBool() || !enu.isString())
			{
				m_errmsg = Zion::StringFormat("template [%s] fields[%d] invalid value", names[t].c_str(), i);
			}
			if(DDLReflect::GetStructFieldOffset(tmpl.info, fld.asCString())==(_U32)-1)
			{
				m_errmsg = Zion::StringFormat("template [%s] fields[%d] error in GetStructFieldData(%s, %s)", names[t].c_str(), i, tmpl.info->name, fld.asCString());
				return false;
			}

			Zion::Map<Zion::String, EXCEL_FIELDINFO>::iterator fi;
			for(fi=tmpl.m_fields.begin(); fi!=tmpl.m_fields.end(); fi++)
			{
				if(fi->second.column==col.asString())
				{
					m_errmsg = Zion::StringFormat("template [%s] fields[%d] column[%s] already existed", names[t].c_str(), i, col.asCString());
					return false;
				}
				if(fi->second.field==fld.asString())
				{
					m_errmsg = Zion::StringFormat("template [%s] fields[%d] field[%s] already existed", names[t].c_str(), i, fld.asCString());
					return false;
				}
			}

			EXCEL_FIELDINFO fieldinfo;
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
				Zion::Map<Zion::String, EXCEL_ENUM*>::iterator i;
				i = m_enum_map.find(enu.asCString());
				if(i==m_enum_map.end())
				{
					m_errmsg = Zion::StringFormat("template [%s] fields[%d] enum[%s] not found", names[t].c_str(), i, enu.asCString());
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
			m_errmsg = Zion::StringFormat("primary key not existed");
			return false;
		}
	}

	return true;
}

void CContentExcelImportor::AddTask(const char* tmpl, const char* filename, const char* sheetname, bool clear)
{
	TASK_INFO info;
	info.tmpl = tmpl;
	info.filename = filename;
	info.sheetname = sheetname;
	info.clear = clear;
	m_Tasks.push_back(info);
}

void CContentExcelImportor::ClearTasks()
{
	m_Tasks.clear();
}

bool CContentExcelImportor::RunTasks()
{
	m_pXlsFile = NULL;
	m_DeleteList.clear();

	for(size_t i=0; i<m_Tasks.size(); i++)
	{
		if(!ImportSheet(m_Tasks[i].tmpl.c_str(), m_Tasks[i].filename.c_str(), m_Tasks[i].sheetname.c_str(), m_Tasks[i].clear))
		{
			if(m_pXlsFile)
			{
				m_pXlsFile->Quit();
				ZION_DELETE m_pXlsFile;
			}
			return false;
		}
	}

	Zion::Set<A_UUID>::iterator it;
	for(it=m_DeleteList.begin(); it!=m_DeleteList.end(); it++)
	{
		Zion::ContentObject::DeleteObject(*it);
	}

	if(m_pXlsFile)
	{
		m_pXlsFile->Save();
		m_pXlsFile->Quit();
		ZION_DELETE m_pXlsFile;
	}

	return true;
}

Zion::String GetColumnName(unsigned int col)
{
	if(col>=26)
	{
		return Zion::StringFormat("%c%c", 'A'+col/26-1, 'A'+col%26);
	}
	else
	{
		return Zion::StringFormat("%c", 'A'+col);
	}
}

Zion::String GetCellName(unsigned int col, unsigned int row)
{
	if(col>=26)
	{
		return Zion::StringFormat("%c%c%d", 'A'+col/26-1, 'A'+col%26, row+1);
	}
	else
	{
		return Zion::StringFormat("%c%d", 'A'+col, row+1);
	}
}

bool CContentExcelImportor::ImportSheet(const char* _tmpl, const char* filename, const char* sheetname, bool clear)
{
	if(m_tmpl_map.find(_tmpl)==m_tmpl_map.end())
	{
		m_errmsg = Zion::StringFormat("template [%s] not found", _tmpl);
		return false;
	}

	if(m_pXlsFile==NULL || m_XlsFileName!=filename)
	{
		ZION_DELETE m_pXlsFile;
		m_pXlsFile = NULL;

		m_pXlsFile = new COLEAutoExcelWrapper();
		if(!m_pXlsFile)
		{
			m_errmsg = Zion::StringFormat("can't open xls file. %s", filename);
			return false;
		}
	}

	if(!m_pXlsFile->SetActiveSheet(sheetname))
	{
		m_errmsg = Zion::StringFormat("can't open sheet %s in file %s", sheetname, filename);
		return false;
	}

	EXCEL_TEMPLATE& tmpl = *m_tmpl_map[_tmpl];

	const A_CONTENT_OBJECT* it = Zion::ContentObject::FindFirst(tmpl.info, true);
	while(it)
	{
		if(m_DeleteList.find(it->_uuid)==m_DeleteList.end())
		{
			m_DeleteList.insert(it->_uuid);
		}
		it = Zion::ContentObject::FindNext(tmpl.info, true, it);
	}


	Zion::String sUUID;

	Zion::Map<Zion::String, EXCEL_FIELDINFO*> field_map;
	if(tmpl.title_line==0)
	{
		Zion::Map<Zion::String, EXCEL_FIELDINFO>::iterator fi;
		for(fi=tmpl.m_fields.begin(); fi!=tmpl.m_fields.end(); fi++)
		{
			field_map[fi->second.column] = &fi->second;
		}
	}
	else
	{
		for(unsigned int col=0; col<200; col++)
		{
			Zion::String ColName;
			if(!m_pXlsFile->GetCellValue(GetCellName(col, tmpl.title_line-1), ColName))
			{
				m_errmsg = "error in GetCellValue";
				return false;
			}
			if(tmpl.m_fields.find(ColName)==tmpl.m_fields.end()) continue;
			Zion::String FieldName = tmpl.m_fields[ColName].field;
			if(field_map.find(FieldName)!=field_map.end())
			{
				m_errmsg = Zion::StringFormat("field duplicate %s", FieldName.c_str());
				return false;
			}

			field_map[GetColumnName(col)] = &tmpl.m_fields[ColName];

			if(tmpl.m_fields[ColName].field=="_uuid")
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

	_U16 type_id = Zion::ContentObject::GetTypeId(tmpl.info->name);

	for(unsigned int row=tmpl.start_line; row<65536; row++)
	{
		Zion::Map<Zion::String, Zion::String> val_map;
		bool bExit = true, bError = false;

		Zion::Map<Zion::String, EXCEL_FIELDINFO*>::iterator i;
		for(i=field_map.begin(); i!=field_map.end(); i++)
		{
			Zion::String val;

			// get cell data
			if(!m_pXlsFile->GetCellValue(Zion::StringFormat("%s%d", i->first.c_str(), row), val))
			{
				m_errmsg = "error in GetCellValue";
				return false;
			}

			// if(all colum is empty) then exit
			if(!val.empty()) bExit = false;

			// use default value
			if(val.empty() && !i->second->defval.empty())
			{
				val = i->second->defval;
			}

			// check not empty
			if(val.empty() && i->second->notempty)
			{
				m_errmsg = Zion::StringFormat("value cannot empty %s", Zion::StringFormat("%s%d", i->first.c_str(), row).c_str());
				return false;
			}

			// process enum value
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
						m_errmsg = Zion::StringFormat("invalid value %s", Zion::StringFormat("%s%d", i->first.c_str(), row).c_str());
						bError = true;
					}
				}
			}
			else
			{
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

		// fill object data
		Zion::Map<Zion::String, Zion::String>::iterator i1;
		for(i1=val_map.begin(); i1!=val_map.end(); i1++)
		{
			if(!i1->second.empty())
			{
				if(!DDLReflect::StructParamFromString(tmpl.info, i1->first.c_str(), obj, i1->second.c_str()))
				{
					m_errmsg = Zion::StringFormat("error in StructParamFromString(%s, %s, %s)", tmpl.info->name, i1->first.c_str(), i1->second.c_str());
					DDLReflect::DestoryObject(tmpl.info, obj);
					return false;
				}
			}
		}

		// make content object primary key
		Zion::String pkey;
		if(!Zion::ContentObject::GenContentObjectUniqueId(type_id, obj, pkey))
		{
			m_errmsg = "error in GenContentObjectUniqueId";
			DDLReflect::DestoryObject(tmpl.info, obj);
			return false;
		}

		A_CONTENT_OBJECT* old_obj = (A_CONTENT_OBJECT*)Zion::ContentObject::QueryByUniqueId(tmpl.info, pkey.c_str());
		if(old_obj)
		{
			const DDLReflect::STRUCT_INFO* info = Zion::ContentObject::GetObjectType(old_obj->_uuid);
			if(info!=tmpl.info)
			{
				m_errmsg = Zion::StringFormat("content object type [%s - %s] not match uniqueid:{%s}", tmpl.info->name, info?info->name:"unknown", pkey.c_str()), 
				DDLReflect::DestoryObject(tmpl.info, obj);
				return false;
			}
			old_obj = Zion::ContentObject::Modify(old_obj->_uuid, tmpl.info);
			ZION_ASSERT(old_obj);
			obj->_uuid = old_obj->_uuid;
			if(m_DeleteList.find(obj->_uuid)!=m_DeleteList.end()) m_DeleteList.erase(obj->_uuid);
		}
		else
		{
			old_obj = Zion::ContentObject::CreateObject(tmpl.info);
			ZION_ASSERT(old_obj);
			obj->_uuid = old_obj->_uuid;
			if(!sUUID.empty())
			{
				Zion::String range = Zion::StringFormat("%s%d", sUUID.c_str(), row);
				char suuid[60];
				AUuidToString(old_obj->_uuid, suuid);
				if(!m_pXlsFile->SetCellValue(range, Zion::String(suuid)))
				{
					m_errmsg = "error in SetCellValue";
					DDLReflect::DestoryObject(tmpl.info, obj);
					return false;
				}
			}
		}

		memcpy(old_obj, obj, tmpl.info->size);
		DDLReflect::DestoryObject(tmpl.info, obj);
	}

	return true;
}
