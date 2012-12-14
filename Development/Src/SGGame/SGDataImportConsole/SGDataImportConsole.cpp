// SGDataImportConsole.cpp : Defines the entry point for the console application.
//

#include <tchar.h>

#include <vector>
#include <string>

#include <tinyxml.h>

#include "../../../External/TinyXml/tinystr.cpp"
#include "../../../External/TinyXml/tinyxml.cpp"
#include "../../../External/TinyXml/tinyxmlerror.cpp"
#include "../../../External/TinyXml/tinyxmlparser.cpp"

#include <wx/wx.h>
#include <wx/string.h>
#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/config.h>


#include "SGCommon.h"
#include "SGGameDDL.h"
#include <AtlasBase.h>
#include <AtlasCommon.h>
#include <AtlasDefines.h>
#include <AtlasUUID.h>
#include <DDL.h>
#include <DDLProxy.h>
#include <DDLStub.h>
#include <DDLReflect.h>
#include <ImportConfig.h>

#include "../../LibEditor/ImportConfig.cpp"
#include "../../LibEditor/OLEAutoExcelWrapper.cpp"
#include "../../LibEditor/UtilString.cpp"

#define EXCEL_PATH "D:\\SG-document\\"
#define XML_PATH "E:\\Work\\Atlas\\SGGame\\data_import.xml"

struct Sheet_info
{
	std::string strFile;
	std::string strType;
	std::string strSheetName;
	int nStartLine;
	std::vector<std::string> vecKeys;
	std::map<std::string, std::string> mapColumns; //<ddl_col, excel_col>
};

std::map<std::string, std::map<std::string, std::string>> g_FieldMaps;
std::string g_strExcelPath;
std::vector<Sheet_info> sheets;

bool read_xml()
{	
	TiXmlDocument xmldoc;
	std::string strXmlFile = Atlas::AtlasGameDir();
	strXmlFile += "\\data_import.xml";
	if(!xmldoc.LoadFile(strXmlFile.c_str()))
		return false;

	TiXmlElement* pNode = xmldoc.RootElement();
	
	g_strExcelPath = pNode->Attribute("input_path");
		
	TiXmlElement* pSheetNode = pNode->FirstChildElement();//Sheet
	int nDefaultStartLine = 1;
	do 
	{
		Sheet_info sheet;
		sheet.strFile = pSheetNode->Attribute("file"); 
		sheet.strType = pSheetNode->Attribute("type");
		sheet.strSheetName = pSheetNode->Attribute("sheet_name"); 
		std::string strKeys = pSheetNode->Attribute("key");
		const char* pStartLine = pSheetNode->Attribute("start_line", &nDefaultStartLine);
		if(pStartLine)
			sheet.nStartLine = atoi(pStartLine);
		else
			sheet.nStartLine = 1;

		Atlas::StringSplit(strKeys, sheet.vecKeys);

		TiXmlElement* pColNode = pSheetNode->FirstChildElement();

		do
		{
			std::string strColumns = pColNode->Attribute("name");
			std::string strRealName = pColNode->Attribute("real_name"); 
			sheet.mapColumns[strColumns] = strRealName;
			
			TiXmlElement* pFieldNode = pColNode->FirstChildElement();//Field
			std::map<std::string, std::string> fieldMap;

			while(pFieldNode) 
			{
				std::string strEnumName = pFieldNode->Attribute("name"); 
				std::string strEnumValue = pFieldNode->Attribute("value");

				fieldMap[strEnumName] = strEnumValue;

				pFieldNode = pFieldNode->NextSiblingElement();
			} 

			if(!fieldMap.empty())
			{
				std::string strField = sheet.strType;
				strField += ".";
				//strField += strColumns;
				strField += strRealName;

				g_FieldMaps[strField] = fieldMap;
			}

			pColNode = pColNode->NextSiblingElement();
		}
		while(pColNode);

		sheets.push_back(sheet);
		pSheetNode = pSheetNode->NextSiblingElement();
	} while (pSheetNode);

	return true;
}

bool import_data()
{
	Atlas::CContentExcelImportManager importMgr;

	for(std::map<std::string, std::map<std::string, std::string>>::iterator it = g_FieldMaps.begin(); it != g_FieldMaps.end(); ++it)
	{
		importMgr.SetFieldMap(it->first.c_str(), it->second);
	}

	for(std::vector<Sheet_info>::iterator it = sheets.begin(); it != sheets.end(); ++it)
	{
		wxString strPath = wxString::FromUTF8(g_strExcelPath.c_str());
		strPath += wxString::FromUTF8(it->strFile.c_str());
		importMgr.Load(strPath);
		
		const DDLReflect::STRUCT_INFO* pInfo = Atlas::ContentObject::GetType(it->strType.c_str());
		importMgr.PrepareProcess(pInfo, it->vecKeys);
		importMgr.ProcessSheet(it->strSheetName.c_str(), it->mapColumns);

		importMgr.Clear(true);
	}

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	Atlas::InitContentObjects();
	//read xml
	if(!read_xml())
	{
		return 0;
	}

	//reuse class ImportConfig
	if(!import_data())
	{
		return 0;
	}
	
	//save
	if(Atlas::ContentObject::SaveContent()) return 0;
	wxLogDebug(wxT("Save content failed"));

	return 0;
}

