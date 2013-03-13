#include <wx/wx.h>
#include <wx/string.h>

#include <AtlasBase.h>
#include <AtlasCommon.h>

#include "ImportConfig.h"
#include "OLEAutoExcelWrapper.h"

namespace Atlas
{
	static Atlas::Map<int, char> g_excelmap;

	struct EXCEL_INDEX 
	{
		int i;
		char* szHead;
	};

	static EXCEL_INDEX g_excelMap[] = {
		{ 1, "A"},
		{ 2, "B"},
		{ 3, "C"},
		{ 4, "D"},
		{ 5, "E"},
		{ 6, "F"},
		{ 7, "G"},
		{ 8, "H"},
		{ 9, "I"},
		{ 10, "J"},
		{ 11, "K"},
		{ 12, "L"},
		{ 13, "M"},
		{ 14, "N"},
		{ 15, "O"},
		{ 16, "P"},
		{ 17, "Q"},
		{ 18, "R"},
		{ 19, "S"},
		{ 20, "T"},
		{ 21, "U"},
		{ 22, "V"},
		{ 23, "W"},
		{ 24, "X"},
		{ 25, "Y"},
		{ 26, "Z"},

		{ 27, "AA"},
		{ 28, "AB"},
		{ 29, "AC"},
		{ 30, "AD"},
		{ 31, "AE"},
		{ 32, "AF"},
		{ 33, "AG"},
		{ 34, "AH"},
		{ 35, "AI"},
		{ 36, "AJ"},
		{ 37, "AK"},
		{ 38, "AL"},
		{ 39, "AM"},
		{ 40, "AN"},
		{ 41, "AO"},
		{ 42, "AP"},
		{ 43, "AQ"},
		{ 44, "AR"},
		{ 45, "AS"},
		{ 46, "AT"},
		{ 47, "AU"},
		{ 48, "AV"},
		{ 49, "AW"},
		{ 50, "AX"},
		{ 51, "AY"},
		{ 52, "AZ"},
		
		{ 53, "BA"},
		{ 54, "BB"},
		{ 55, "BC"},
		{ 56, "BD"},
		{ 57, "BE"},
		{ 58, "BF"},
		{ 59, "BG"},
		{ 60, "BH"},
		{ 61, "BI"},
		{ 62, "BJ"},
		{ 63, "BK"},
		{ 64, "BL"},
		{ 65, "BM"},
		{ 66, "BN"},
		{ 67, "BO"},
		{ 68, "BP"},
		{ 69, "BQ"},
		{ 70, "BR"},
		{ 71, "BS"},
		{ 72, "BT"},
		{ 73, "BU"},
		{ 74, "BV"},
		{ 75, "BW"},
		{ 76, "BX"},
		{ 77, "BY"},
		{ 78, "BZ"},
	};

	static bool GenerateHeader(int nCol, char szIndex[], int len)
	{
		memset((void*)szIndex, 0, len);
		if(nCol >= len)
		{
			return false;
		}
		
		char c[4]; 
		memset((void*)c, 0, 4);

		for(int i=0; i<sizeof(g_excelMap)/sizeof(g_excelMap[0]); ++i)
		{
			if(nCol==g_excelMap[i].i)
			{
				memcpy(c, g_excelMap[i].szHead, strlen(g_excelMap[i].szHead));
				break;
			}
		}

		strcat_s(szIndex, len, c);
		
		return true;
	}

	static bool GenerateIndex(int nRow, int nCol, char szIndex[], int len)
	{
		memset((void*)szIndex, 0, len);
		if(nCol >= len)
		{
			return false;
		}

		if(!GenerateHeader(nCol, szIndex, len))
		{
			return false;
		}
		sprintf(szIndex, "%s%d", szIndex, nRow);

		return true;
	}

	CContentExcelImportManager::CContentExcelImportManager()
	{
		Clear();
		m_pExcelWrapper = ATLAS_NEW COLEAutoExcelWrapper();
	}

	CContentExcelImportManager::~CContentExcelImportManager()
	{
		m_pExcelWrapper->Quit();
		delete m_pExcelWrapper;
	}

	bool CContentExcelImportManager::Load(const wxString& szExcelFilePath)
	{
		if(m_strPath != szExcelFilePath)
		{
			Clear();
		}

		m_strPath = szExcelFilePath;

		return true;
	}

	void CContentExcelImportManager::Clear(bool bAll)
	{
		if(bAll)
		{
			m_Keys.clear();
			m_ObjectMap.clear();
		}

		m_strPath.clear();
	}

	bool CContentExcelImportManager::IsExistSheet(const char* szSheetName)
	{
		Atlas::Vector<wxString> vSheets;
		if(!GetSheets(vSheets)) return false;

		for(Atlas::Vector<wxString>::iterator it = vSheets.begin(); it != vSheets.end(); ++it)
		{
			wxString strSheetName = wxString::FromUTF8(szSheetName);
			if(*it == strSheetName)
			{
				return true;
			}
		}

		return false;
	}

	bool CContentExcelImportManager::GetSheets(Atlas::Vector<wxString>& vSheets)
	{
		m_pExcelWrapper->SetFilePath(m_strPath.c_str());
		m_pExcelWrapper->OpenExcelBook(false);
		m_pExcelWrapper->GetExcelSheets(vSheets);  //all sheets
		m_pExcelWrapper->Quit();
		return true;
	}

	void CContentExcelImportManager::ClearFieldMaps()
	{
		m_FieldMaps.clear();
	}
		
	void CContentExcelImportManager::SetFieldMap(const char* field, const Atlas::Map<Atlas::String, Atlas::String>& fmap)
	{
		m_FieldMaps[field] = fmap;
	}

	bool CContentExcelImportManager::PrepareProcess(const DDLReflect::STRUCT_INFO* pInfo, const Atlas::Vector<Atlas::String>& keys)
	{
		//check keys whether all in struct_info
		for(size_t j = 0; j < keys.size(); ++j)
		{
			DDLReflect::FIELD_INFO finfo;
			const void* fdata;
			if(!DDLReflect::GetStructFieldInfo(pInfo, keys[j].c_str(), (const void*)NULL, finfo, fdata))
			{
				m_Err = StringFormat("input key not found in struct:%s\n column:%s", pInfo->name, keys[j].c_str());
				return false;
			}
		}
		
		m_pStructInfo = pInfo;
		m_Keys = keys;

		//generate key map
		const A_CONTENT_OBJECT* pObject = Atlas::ContentObject::FindFirst(pInfo, true);
		Atlas::String strKey;
		while(pObject)
		{
			strKey.clear();
			if(!GetObjectUnqiueID(pObject, strKey))
			{
				m_Err = StringFormat("invalidate or no value\nobject %s", pObject->name._Value); 
				return false;
			}

			m_ObjectMap[strKey] = pObject->uuid;

			pObject = Atlas::ContentObject::FindNext(pInfo, true, pObject);
		}

		return true;
	}
	

	bool CContentExcelImportManager::ProcessSheet(const Atlas::String& name, int nStartLine)
	{
		Atlas::Map<Atlas::String, Atlas::String> fmap;
		return ProcessSheet(name, fmap);
	}
	
	bool CContentExcelImportManager::ProcessSheet(const Atlas::String& name, const Atlas::Map<Atlas::String, Atlas::String>& fmap, int nStartLine)
	{
		m_pExcelWrapper->SetFilePath(m_strPath);
		m_pExcelWrapper->OpenExcelBook(false);
		
		wxString strSheet = wxString::FromUTF8(name.c_str());
		if(m_pExcelWrapper->SetActiveSheet(strSheet) != S_OK)
		{
			m_Err = StringFormat("no this sheet[%s]", name.c_str());
			m_pExcelWrapper->Quit();
			return false;
		}

		if(nStartLine<1)
		{
			m_Err = StringFormat("start with invalidate num of line[%d]", nStartLine);
			m_pExcelWrapper->Quit();
			return false;
		}

		Atlas::Map<Atlas::String, Atlas::String> columnMap; // <header, column_name>
		int nRow = nStartLine;//row line  
		int nCol = 1;
		wxString strRange;
		wxString strValue;
		char szTmp[512];
		bool bExist = false;// whether col:name exist

		do // start header line
		{
			if(!GenerateIndex(nRow, nCol, szTmp, 512))
			{
				m_Err = StringFormat("Get Value from excel failed\n %s", szTmp);
				m_pExcelWrapper->Quit();
				return false;
			}

			strRange = wxString::FromUTF8(szTmp);
			strValue.clear();
			m_pExcelWrapper->GetCellValue(strRange, strValue);

			if(strValue.empty()) // the line end
			{
				break;
			}

			GenerateHeader(nCol, szTmp, 512);

			//remove no need column
			bool bNeed = false;
			for(Atlas::Map<Atlas::String, Atlas::String>::const_iterator it_find = fmap.begin(); it_find != fmap.end(); ++it_find)
			{
				if(it_find->second == strValue.ToUTF8().data())
				{
					bNeed = true;
					break;
				}
			}

			if(bNeed)
				columnMap[szTmp] = (char*)strValue.ToUTF8().data();

			++nCol;
		} while (true);
	
		//lack of some column
		if(fmap.size() != columnMap.size())
		{
			Atlas::String strLoseInfo;
			for(Atlas::Map<Atlas::String, Atlas::String>::const_iterator it = fmap.begin(); it != fmap.end(); ++it)
			{
				Atlas::Map<Atlas::String, Atlas::String>::iterator it_colmap;
				for(it_colmap = columnMap.begin(); it_colmap != columnMap.end(); ++it_colmap)
				{
					if(it->second == it_colmap->second)
					{
						break;
					}
				}

				if(it_colmap == columnMap.end())
				{
					strLoseInfo += it->first;
					strLoseInfo += ":";
					strLoseInfo += it->second;
					strLoseInfo += " ";
				}
			}

			m_Err = StringFormat("excel lose column \n%s", strLoseInfo.c_str());
			m_pExcelWrapper->Quit();
			return false;
		}

		Atlas::Map<Atlas::String, Atlas::String>::iterator it_col;
		//whether all keys exist in excel
		for(size_t i = 0; i < m_Keys.size(); ++i)
		{
			for(it_col = columnMap.begin(); it_col != columnMap.end(); ++it_col)
			{
				bool bFind = false;
				for(Atlas::Map<Atlas::String, Atlas::String>::const_iterator it_find = fmap.begin(); it_find != fmap.end(); ++it_find)
				{
					if(it_col->second == it_find->second)
					{
						bFind = true;
						break;
					}
				}

				if(bFind)
				{
					break;
				}
			}

			if(it_col == columnMap.end())
			{
				m_pExcelWrapper->Quit();
				m_Err = StringFormat("key %s not exist in excel\n", m_Keys[i].data());
				return false;
			}
		}

		//whether all column in excel match struct_info 
		for(it_col = columnMap.begin(); it_col != columnMap.end(); ++it_col)
		{
			DDLReflect::FIELD_INFO finfo;
			const void* fdata;
			Atlas::Map<Atlas::String, Atlas::String>::const_iterator it_find;
			for(it_find = fmap.begin(); it_find != fmap.end(); ++it_find)
			{
				if(it_col->second == it_find->second)
				{
					break;
				}
			}

			if(!DDLReflect::GetStructFieldInfo(m_pStructInfo, it_find->first.c_str(), (const void*)NULL, finfo, fdata))
			{
				m_Err = StringFormat("column not match in struct:%s\n column:%s\n", m_pStructInfo->name, it_col->second.c_str()); 
				m_pExcelWrapper->Quit();
				return false;
			}
		}

		A_CONTENT_OBJECT* pObject = (A_CONTENT_OBJECT*)ATLAS_ALIGN_ALLOC(m_pStructInfo->size);
		nRow = nStartLine + 1;
		size_t count;
		m_nUpdateRowNum = 0;
		m_nInsertRowNum = 0;
		do{
			memset(pObject, 0, (size_t)m_pStructInfo->size);
			count = 0;

			for(it_col = columnMap.begin(); it_col != columnMap.end(); ++it_col)
			{
				sprintf(szTmp, "%s%d", it_col->first.c_str(), nRow);
				strRange = wxString::FromUTF8(szTmp);
				m_pExcelWrapper->GetCellValue(strRange, strValue);

				//line data end
				if(strValue.empty())
				{
					count++;
					continue;
				}
	
				Atlas::String fieldvalue = (const char*)strValue.ToUTF8();	
				//for float append zero at head
				if(fieldvalue.find(".") == 0)
				{
					fieldvalue = "0" + fieldvalue;
				}

				Atlas::Map<Atlas::String, Atlas::String>::const_iterator it_find;
				for(it_find = fmap.begin(); it_find != fmap.end(); ++it_find)
				{
					if(it_find->second == it_col->second)
					{
						break;
					}
				}
				
				if(it_find != fmap.end())
				{
					Atlas::String fieldname = Atlas::StringFormat("%s.%s", m_pStructInfo->name, it_find->second.c_str());
					if(m_FieldMaps.find(fieldname)!=m_FieldMaps.end())
					{
						Atlas::Map<Atlas::String, Atlas::String>& ffmap = m_FieldMaps[fieldname];
						if(ffmap.find(fieldvalue)==ffmap.end())
						{
							m_Err = StringFormat("Invalidate enum field \n struct %s Column %s ", m_pStructInfo->name, it_col->second.c_str());
							m_pExcelWrapper->Quit();
							return false;
						}
						fieldvalue = ffmap[fieldvalue];
					}
				}

				//fill content 
				if(!StructParamFromString(m_pStructInfo, it_find->first.c_str(), pObject, fieldvalue.c_str()))
				{
					m_Err = StringFormat("Get Value %s failed\n %s", it_col->second.c_str(), it_col->first.c_str());
					m_pExcelWrapper->Quit();
					return false;	
				}
			}

			if(count==columnMap.size())
			{
				break;
			}

			if(!UpdateCacheData(pObject))
			{
				ATLAS_ALIGN_FREE(pObject);
				m_pExcelWrapper->Quit();
				return false;
			}
		}while(++nRow);

		ATLAS_ALIGN_FREE(pObject);
		m_pExcelWrapper->Quit();

		return true;
	}

	const char* CContentExcelImportManager::GetErrorMsg()
	{
		return m_Err.c_str();
	}

	const char* CContentExcelImportManager::GetImportInfoMsg()
	{
		m_Err = StringFormat("Update %d lines\n Insert %d lines", m_nUpdateRowNum, m_nInsertRowNum);
		return m_Err.c_str();
	}

	bool CContentExcelImportManager::UpdateCacheData(const A_CONTENT_OBJECT* pTmpObject)
	{
		bool bRet = false;
		Atlas::String strKey;
		if(!GetObjectUnqiueID(pTmpObject, strKey))
		{
			return false;
		}

		A_UUID uuid;
		A_CONTENT_OBJECT* pObject = NULL;
		Atlas::Map<Atlas::String, A_UUID>::iterator it_obj = m_ObjectMap.find(strKey);
		if(it_obj == m_ObjectMap.end()) //insert
		{
			pObject = ContentObject::Create(m_pStructInfo, uuid);
			pObject->uuid = uuid;
			m_ObjectMap[strKey] = uuid;
			m_nInsertRowNum++;
		}
		else
		{
			uuid = it_obj->second;
			pObject = (A_CONTENT_OBJECT*)ContentObject::QueryByUUID(uuid);
			m_nUpdateRowNum++;
		}

		memcpy(pObject, pTmpObject, m_pStructInfo->size);
		pObject->uuid = uuid;

		return true;
	}

	bool CContentExcelImportManager::GetObjectUnqiueID(const A_CONTENT_OBJECT* pObejct, Atlas::String& id)
	{
		Atlas::String strTmp;
		for(size_t i = 0; i < m_Keys.size(); ++i)
		{
			if(!DDLReflect::StructParamToString(m_pStructInfo, m_Keys[i].c_str(), pObejct, strTmp)) 
			{
				id.clear();
				return false;	
			}

			id += strTmp;
		}

		return true;
	}
}