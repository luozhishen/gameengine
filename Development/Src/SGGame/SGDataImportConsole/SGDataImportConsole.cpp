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

#define EXCEL_PATH "D:\\SG-document\\"
#define XML_PATH "E:\\Work\\Atlas\\SGGame\\data_import.xml"

struct Sheet_info
{
	std::string strFile;
	std::string strType;
	std::vector<std::string> vecKeys;

	std::vector<std::string> vecColumns;
};

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
	//pNode = pNode->FirstChildElement();//Config 
		
	TiXmlElement* pSheetNode = pNode->FirstChildElement();//Sheet
	do 
	{
		Sheet_info sheet;
		sheet.strFile = pSheetNode->Attribute("file");
		sheet.strType = pSheetNode->Attribute("type");
		std::string strKeys = pSheetNode->Attribute("key");
		Atlas::StringSplit(strKeys, sheet.vecKeys);

		TiXmlElement* pColNode = pSheetNode->FirstChildElement();

		do
		{
			std::string strColumns = pColNode->Attribute("name");
			sheet.vecColumns.push_back(strColumns);

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
	
	for(std::vector<Sheet_info>::iterator it = sheets.begin(); it != sheets.end(); ++it)
	{
		wxString strPath = wxString::FromUTF8(g_strExcelPath.c_str());
		strPath += wxString::FromUTF8(it->strFile.c_str());
		importMgr.Load(strPath);
		
		std::vector<wxString> vecSheets;
		importMgr.GetSheets(vecSheets);

		for(std::vector<wxString>::iterator it_sheet = vecSheets.begin(); it_sheet != vecSheets.end(); ++it_sheet)
		{
			const DDLReflect::STRUCT_INFO* pInfo = Atlas::ContentObject::GetType(it->strType.c_str());
			importMgr.PrepareProcess(pInfo, it->vecKeys);
			importMgr.ProcessSheet(it_sheet->mb_str().data());
		}

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

