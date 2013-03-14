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
	int m_nTitleLine;
	int m_nStartLine;
	Atlas::Vector<CONTENT_EXECEL_FIELDINFO> m_fields;
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
		if(!Atlas::ContentObject::GetType(type.asCString())) return false;
		Json::Value titleline = jtmpl.get("title", Json::Value());
		Json::Value startline = jtmpl.get("start", Json::Value());
		if(!titleline.isInt() || !startline.isInt()) return false;
		Json::Value fields = jtmpl.get("fields", Json::Value());
		if(!fields.isArray()) return false;

		if(m_tmpl_map.find(name.asCString())!=m_tmpl_map.end()) return false;

		m_tmpl_map[name.asCString()] = new CONTENT_EXCEL_TEMPLATE;
		CONTENT_EXCEL_TEMPLATE& tmpl = *m_tmpl_map[name.asCString()];
		tmpl.name = name.asCString();
		tmpl.info = Atlas::ContentObject::GetType(type.asCString());
		tmpl.m_nTitleLine = titleline.asInt();
		tmpl.m_nStartLine = startline.asInt();

		for(int i=0; i<(int)fields.size(); i++)
		{
			Json::Value item = fields[i];
			if(!item.isObject()) return false;
			Json::Value col = item.get("c", Json::Value());
			Json::Value fld = item.get("f", Json::Value());
			Json::Value enu = item.get("e", Json::Value(""));
			if(!col.isString() || !fld.isString() || !enu.isString()) return false;
			for(size_t f=0; f<tmpl.m_fields.size(); f++)
			{
				if(tmpl.m_fields[f].colum==col.asCString()) return false;
				if(tmpl.m_fields[f].field==fld.asCString()) return false;
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
			tmpl.m_fields.push_back(fieldinfo);
		}
	}

	return true;
}
