// SGDataImportConsole.cpp : Defines the entry point for the console application.
//

#include <tchar.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
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
#include "ImportConfig.h"

#include "../../LibEditor/OLEAutoExcelWrapper.cpp"

std::wstring Utf8ToUtf16( const Atlas::String& sSrc );
std::wstring Utf8ToUtf16( const char* const szSource, const _U32 uiSize );
Atlas::String Utf16ToUtf8( const std::wstring& sSrc );
Atlas::String Utf16ToUtf8( const wchar_t* const szSource, const _U32 uiSize );
Atlas::String MultiByteToUtf8( const Atlas::String& sSrc );
Atlas::String MultiByteToUtf8( const char* const szSource, const _U32 uiSize );
Atlas::String Utf8ToMultiByte( const Atlas::String& sSrc );
Atlas::String Utf8ToMultiByte( const char* const szSource, const _U32 uiSize );
std::wstring ANSIToUnicode(const Atlas::String& str);
Atlas::String UnicodeToANSI(const std::wstring& str);
char* replace(char* szSrc, int len, char szTarget, char szValue);



void display_start();
void display_end();

struct Sheet_info
{
	Atlas::String strFile;
	Atlas::String strType;
	Atlas::String strSheetName;
	int nStartLine;
	Atlas::Vector<Atlas::String> vecKeys;
	Atlas::Map<Atlas::String, Atlas::String> mapColumns; //<ddl_col, excel_col>
};

Atlas::Map<Atlas::String, Atlas::Map<Atlas::String, Atlas::String>> g_FieldMaps;
Atlas::String g_strExcelPath;
Atlas::Vector<Sheet_info> sheets;
bool g_useRelativePath = false;

bool read_xml()
{	
	TiXmlDocument xmldoc;
	Atlas::String strXmlFile = Atlas::AtlasGameDir();
	
	strXmlFile += "\\ClientConfig\\data_import.xml";
	if(!xmldoc.LoadFile(strXmlFile.c_str()))
		return false;

	TiXmlElement* pNode = xmldoc.RootElement();
	g_strExcelPath = pNode->Attribute("input_path");

	if(g_useRelativePath)
	{
		wchar_t szPath[512];
		GetCurrentDirectory(512, szPath);
		Atlas::String strPre = UnicodeToANSI(szPath);
		strPre += "\\";
		g_strExcelPath.insert(0, strPre);
	}

	std::cout<<g_strExcelPath<<"\n";
	//system("pause");

	TiXmlElement* pSheetNode = pNode->FirstChildElement();//Sheet
	int nDefaultStartLine = 1;
	do 
	{
		Sheet_info sheet;
		sheet.strFile = pSheetNode->Attribute("file"); 
		sheet.strType = pSheetNode->Attribute("type");
		sheet.strSheetName = pSheetNode->Attribute("sheet_name"); 
		Atlas::String strKeys = pSheetNode->Attribute("key");
		const char* pStartLine = pSheetNode->Attribute("start_line", &nDefaultStartLine);
		if(pStartLine)
			sheet.nStartLine = atoi(pStartLine);
		else
			sheet.nStartLine = 1;

		Atlas::StringSplit(strKeys, '.', sheet.vecKeys);

		TiXmlElement* pColNode = pSheetNode->FirstChildElement();

		do
		{
			Atlas::String strColumns = pColNode->Attribute("name");
			Atlas::String strRealName = pColNode->Attribute("real_name"); 
			sheet.mapColumns[strColumns] = strRealName;
			
			TiXmlElement* pFieldNode = pColNode->FirstChildElement();//Field
			Atlas::Map<Atlas::String, Atlas::String> fieldMap;

			while(pFieldNode) 
			{
				Atlas::String strEnumName = pFieldNode->Attribute("name"); 
				Atlas::String strEnumValue = pFieldNode->Attribute("value");

				fieldMap[strEnumName] = strEnumValue;

				pFieldNode = pFieldNode->NextSiblingElement();
			} 

			if(!fieldMap.empty())
			{
				Atlas::String strField = sheet.strType;
				strField += ".";
				//strField += strColumns;
				strField += strRealName;

				Atlas::Map<Atlas::String, Atlas::Map<Atlas::String, Atlas::String>>::iterator it_enum_find = g_FieldMaps.find(strField);
				if(it_enum_find == g_FieldMaps.end())
				{
					g_FieldMaps[strField] = fieldMap;
				}
				else
				{
					g_FieldMaps[strField].insert(fieldMap.begin(), fieldMap.end());
				}
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

	for(Atlas::Map<Atlas::String, Atlas::Map<Atlas::String, Atlas::String>>::iterator it = g_FieldMaps.begin(); it != g_FieldMaps.end(); ++it)
	{
		importMgr.SetFieldMap(it->first.c_str(), it->second);
	}
	
	//display_start();

	std::ofstream ofs;
	Atlas::String strLogFile = Atlas::AtlasGameDir();
	strLogFile += "\\ClientConfig\\import_log.txt";
	ofs.open(strLogFile.c_str(), std::ios_base::out|std::ios_base::app);
	assert(ofs.is_open());

	for(Atlas::Vector<Sheet_info>::iterator it = sheets.begin(); it != sheets.end(); ++it)
	{
		wxString strPath = wxString::FromUTF8(g_strExcelPath.c_str());
		strPath += wxString::FromUTF8(it->strFile.c_str());
		importMgr.Load(strPath);
		
		const DDLReflect::STRUCT_INFO* pInfo = Atlas::ContentObject::GetType(it->strType.c_str());
		importMgr.PrepareProcess(pInfo, it->vecKeys);

		if(!importMgr.IsExistSheet(it->strSheetName.c_str()))
		{
			ofs<<"in excel:"<< it->strFile <<"on this sheet:"<<it->strSheetName<<"\n";
			std::cout<<"excel:"<< it->strFile.c_str() 
				<<" sheet:"<< it->strSheetName.c_str() <<"\n";
			continue;
		}

		if(!importMgr.ProcessSheet(it->strSheetName.c_str(), it->mapColumns))
		{
			ofs<<importMgr.GetErrorMsg();
			ofs<<"in excel:"<< it->strFile <<" sheet:"<<it->strSheetName<<"\n";
			std::cout<<"excel:"<< Utf8ToMultiByte(it->strFile.c_str(), (_U32)it->strFile.size()) 
				<<" sheet:"<< Utf8ToMultiByte(it->strSheetName.c_str(), (_U32)it->strSheetName.size()) <<" failed!\n";
		}
		else
		{
			ofs<<"excel:"<< it->strFile <<" sheet:"<< it->strSheetName <<" ok!\n";
			std::cout<<"excel:"<< Utf8ToMultiByte(it->strFile.c_str(), (_U32)it->strFile.size()) <<" sheet:"
				<< Utf8ToMultiByte(it->strSheetName.c_str(), (_U32)it->strSheetName.size())<<" ok!\n";
		}

		importMgr.Clear(true);
	}

	ofs.close();
	//display_end();
	system("PAUSE");

	return true;
}

void display_start()
{
	printf("=================================================\n");
	printf("= ***	 ***	*******	  ***	  ***	*		=\n");
	printf("=*   *  *   *	   *	 *	 *	 *	 *	*		=\n");
	printf("= *	    * 	*	   *	*	  *	*	  *	*		=\n");
	printf("=  *	*		   *	*  *  *	*  *  *	*		=\n");
	printf("=*   *  *   ***    *	*	  *	*	  *	*		=\n");
	printf("=*	 *  *   *	   *	 *	 *	 *	 *	*		=\n");
	printf("= ***	 ***	   *	  ***	  ***	******	=\n");
	printf("=================================================\n");
}

void display_end()
{
	printf("=================================================================\n");
	printf("=	***** *					  *			*	  *	  ***	*	 *	=\n");
	printf("=	  *	  *		****		  *	 *		 *	 *	 *	 *	*	 *	=\n");
	printf("=	  *	  *			*		  *	*		  *	*	*	  *	*	 *	=\n");
	printf("=	  *	  ****	*****	 ***  **		   *	*  *  *	*	 *	=\n");
	printf("=	  *	  *	  *	*   *	*	* *	*		   *	*	  *	*	 *	=\n");
	printf("=	  *	  *	  *	*	*	*	* *	 *		   *	 *	 *	*	 *	=\n");
	printf("=	  *	  *	  *	******	*	* *	  *		   *	  ***	 ****	=\n");
	printf("=================================================================\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc > 1)
	{
		g_useRelativePath = true;
	}

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



std::wstring Utf8ToUtf16( const Atlas::String& sSrc )
{
	return Utf8ToUtf16( sSrc.c_str(), (_U32)( sSrc.size() ) );
}

std::wstring Utf8ToUtf16( const char* const szSource, const _U32 uiSize )
{
	static std::wstring sWrongString = L"\"Utf8ToUtf16 Conversion Failed\"";
	if ( uiSize == 0 )
	{
		return std::wstring();
	}

	const int iRequiredLength = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, szSource, uiSize, NULL, 0 );
	if ( iRequiredLength == 0 )
	{
		//ALOGE( gLogRoot, "Failed to determine length of the converted UTF-16 string: %d", GetLastError() );
		return sWrongString;
	}

	std::wstring sResult( iRequiredLength, L'\0' );
	const int iConvertResult = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, szSource, uiSize, const_cast<wchar_t*>( sResult.data() ), iRequiredLength );
	if ( iConvertResult != iRequiredLength )
	{
		//ALOGE( gLogRoot, "Failed to convert UTF-8 string to UTF-16 string : %d", GetLastError() );
		return sWrongString;
	}

	return sResult;
}

Atlas::String Utf16ToUtf8( const std::wstring& sSrc )
{
	return Utf16ToUtf8( sSrc.c_str(), (_U32)( sSrc.size() ) );
}

Atlas::String Utf16ToUtf8( const wchar_t* const szSource, const _U32 uiSize )
{
	static Atlas::String sWrongString = "\"Utf16ToUtf8 Conversion Failed\"";
	if ( uiSize == 0 )
	{
		return Atlas::String();
	}

	const int iRequiredLength = WideCharToMultiByte( CP_UTF8, 0, szSource, uiSize, NULL, 0, NULL, NULL );
	if ( iRequiredLength == 0 )
	{
		//ALOGE( gLogRoot, "Failed to determine length of the converted UTF-8 string: %d", GetLastError() );
		return sWrongString;
	}

	Atlas::String sResult( iRequiredLength, '\0' );
	const int iConvertResult = WideCharToMultiByte( CP_UTF8, 0, szSource, uiSize, const_cast<char*>( sResult.data() ), iRequiredLength, NULL, NULL );
	if ( iConvertResult != iRequiredLength )
	{
		//ALOGE( gLogRoot, "Failed to convert UTF-16 string to UTF-8 string: %d", GetLastError() );
		return sWrongString;
	}

	return sResult;
}


Atlas::String MultiByteToUtf8( const Atlas::String& sSrc )
{
	return MultiByteToUtf8( sSrc.c_str(), (_U32)( sSrc.size() ) );
}

Atlas::String MultiByteToUtf8( const char* const szSource, const _U32 uiSize )
{
	static Atlas::String sWrongString = "\"MultiByteToUtf8 Conversion Failed\"";
	if ( uiSize == 0 )
	{
		return Atlas::String();
	}

	const int iRequiredLength = MultiByteToWideChar( CP_ACP, MB_ERR_INVALID_CHARS, szSource, uiSize, NULL, 0 );
	if ( iRequiredLength == 0 )
	{
		//ALOGE( gLogRoot, "Failed to determine length of converted UTF-8 string: %d", GetLastError() );
		return sWrongString;
	}

	std::wstring sResult( iRequiredLength, L'\0' );
	const int iConvertResult = MultiByteToWideChar( CP_ACP, MB_ERR_INVALID_CHARS, szSource, uiSize, const_cast<wchar_t*>( sResult.data() ), iRequiredLength );
	if ( iConvertResult != iRequiredLength )
	{
		//ALOGE( gLogRoot, "Failed to convert MultiByte string to UTF-8 string: %d", GetLastError() );
		return sWrongString;
	}

	return Utf16ToUtf8( sResult );
}


Atlas::String Utf8ToMultiByte( const Atlas::String& sSrc )
{
	return Utf8ToMultiByte( sSrc.c_str(), (_U32)( sSrc.size() ) );
}

Atlas::String Utf8ToMultiByte( const char* const szSource, const _U32 uiSize )
{
	static Atlas::String sWrongString = "\"MultiByteToUtf8 Conversion Failed\"";
	if ( uiSize == 0 )
	{
		return Atlas::String();
	}

	std::wstring wsTemp = Utf8ToUtf16( szSource, uiSize );

	const int iRequiredLength = WideCharToMultiByte( CP_ACP, 0, wsTemp.c_str(), (_U32)( wsTemp.length() ), NULL, 0, NULL, NULL );
	if ( iRequiredLength == 0 )
	{
		//ALOGE( gLogRoot, "Failed to determine length of the converted MultiByte string: %d", GetLastError() );
		return sWrongString;
	}

	Atlas::String sResult( iRequiredLength, '\0' );
	const int iConvertResult = WideCharToMultiByte( CP_ACP, 0, wsTemp.c_str(), (_U32)( wsTemp.length() ), const_cast<char*>( sResult.data() ), iRequiredLength, NULL, NULL );
	if ( iConvertResult != iRequiredLength )
	{
		//ALOGE( gLogRoot, "Failed to convert UTF-8 string to MultiByte string: %d", GetLastError() );
		return sWrongString;
	}

	return sResult;
}

std::wstring ANSIToUnicode(const Atlas::String& str)
{
	int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pUnicode;
	pUnicode = new wchar_t[unicodeLen+1];
	memset(pUnicode, 0, (unicodeLen+1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring rt;
	rt = (wchar_t*)pUnicode;
	delete pUnicode;
	return rt;
}

Atlas::String UnicodeToANSI(const std::wstring& str)
{
	char* pElementText;
	int nTextLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	pElementText = new char[nTextLen+1];
	memset((void*)pElementText, 0, sizeof(char)*(nTextLen+1));
	WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, pElementText, nTextLen, NULL, NULL);
	Atlas::String strText = pElementText;
	delete[] pElementText;
	return strText;
}

char* replace(char* szSrc, int len, char szTarget, char szValue)
{
	for(int i = 0; szSrc+i != NULL && *(szSrc+i) != '\0' && i < len; ++i)
	{
		if(*(szSrc+i) == szTarget)
		{
			*(szSrc+i) = szValue;
		}
	}

	return szSrc;
}
