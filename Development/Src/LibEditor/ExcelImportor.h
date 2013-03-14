#ifndef _IMPORT_MANAGER_H__
#define _IMPORT_MANAGER_H__

#include <string>
#include <vector>
#include <utility>

class COLEAutoExcelWrapper;
struct CONTENT_EXCEL_TEMPLATE;
typedef Atlas::Map<Atlas::String, Atlas::String> CONTENT_EXCEL_ENUM;

class CContentExcelImportor
{
public:
	CContentExcelImportor();
	~CContentExcelImportor();

	const char* GetErrorInfo() { return m_errmsg.c_str(); }

	void GetTemplateList(Atlas::Vector<Atlas::String>& list);

	void ClearTemplateDefine();
	bool LoadTemplateDefine(const char* filename);

	bool ImportSheet(const char* tmplate, const char* sheet);

private:
	Atlas::Map<Atlas::String, CONTENT_EXCEL_ENUM*>		m_enum_map;
	Atlas::Map<Atlas::String, CONTENT_EXCEL_TEMPLATE*>	m_tmpl_map;
	Atlas::String m_errmsg;
};

#endif	//_IMPORT_MANAGER_H__
