#ifndef _IMPORT_MANAGER_H__
#define _IMPORT_MANAGER_H__

#include <string>
#include <vector>
#include <utility>

namespace Atlas
{
	class CContentExcelImportManager;
	class COLEAutoExcelWrapper;

	class CContentExcelImportManager
	{
	public:
		CContentExcelImportManager();
		~CContentExcelImportManager();

		bool Load(const wxString& szExcelFilePath);
		void Clear(bool bAll = false);

		bool IsExistSheet(const char* szSheetName);
		bool GetSheets(Atlas::Vector<wxString>& vSheets);

		void ClearFieldMaps();
		void SetFieldMap(const char* field, const Atlas::Map<Atlas::String, Atlas::String>& fmap);

		bool PrepareProcess(const DDLReflect::STRUCT_INFO* pInfo, const Atlas::Vector<Atlas::String>& keys);
		bool ProcessSheet(const Atlas::String& name, int nStartLine = 1);
		bool ProcessSheet(const Atlas::String& name, const Atlas::Map<Atlas::String, Atlas::String>& fmap, int nStartLine = 1);
		const char* GetErrorMsg();
		const char* GetImportInfoMsg();

	protected:
		bool GetObjectUnqiueID(const A_CONTENT_OBJECT* pObejct, Atlas::String& id);
		bool UpdateCacheData(const A_CONTENT_OBJECT* pObject);//map<column_name, strValue>

	private:
		Atlas::Map<Atlas::String, Atlas::Map<Atlas::String, Atlas::String>> m_FieldMaps;	//enum field map     map<table_name, map<field, value>>

		const DDLReflect::STRUCT_INFO* m_pStructInfo;
		Atlas::Vector<Atlas::String> m_Keys;
		Atlas::Map<Atlas::String, A_UUID> m_ObjectMap;// map<keys, A_UUID> this map deal with loading data exist and data from excel
		COLEAutoExcelWrapper* m_pExcelWrapper;
		wxString m_strPath;
		Atlas::String m_Err;
		int m_nUpdateRowNum;
		int m_nInsertRowNum;
	};
}

#endif	//_IMPORT_MANAGER_H__
