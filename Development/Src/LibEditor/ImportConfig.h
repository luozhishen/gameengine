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

		bool GetSheets(std::vector<wxString>& vSheets);

		void ClearFieldMaps();
		void SetFieldMap(const char* field, const std::map<std::string, std::string>& fmap);

		bool PrepareProcess(const DDLReflect::STRUCT_INFO* pInfo, const std::vector<std::string>& keys);
		bool ProcessSheet(const std::string& name);
		const char* GetErrorMsg();
		const char* GetImportInfoMsg();

	protected:
		bool GetObjectUnqiueID(const A_CONTENT_OBJECT* pObejct, std::string& id);
		bool UpdateCacheData(const A_CONTENT_OBJECT* pObject);//map<column_name, strValue>

	private:

		std::map<std::string, std::map<std::string, std::string>> m_FieldMaps;

		const DDLReflect::STRUCT_INFO* m_pStructInfo;
		std::vector<std::string> m_Keys;
		std::map<std::string, A_UUID> m_ObjectMap;// map<keys, A_UUID> this map deal with loading data exist and data from excel
		COLEAutoExcelWrapper* m_pExcelWrapper;
		wxString m_strPath;
		std::string m_Err;
		int m_nUpdateRowNum;
		int m_nInsertRowNum;
	};
}

#endif	//_IMPORT_MANAGER_H__
