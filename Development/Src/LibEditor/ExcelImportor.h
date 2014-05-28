#ifndef _EXCEL_IMPORTOR_H__
#define _EXCEL_IMPORTOR_H__

#include <string>
#include <vector>
#include <utility>

class COLEAutoExcelWrapper;
struct EXCEL_TEMPLATE;
typedef Zion::Map<Zion::String, Zion::String> EXCEL_ENUM;

class CContentExcelImportor
{
public:
	CContentExcelImportor();
	~CContentExcelImportor();

	const char* GetErrorInfo() { return m_errmsg.c_str(); }

	void GetTemplateList(Zion::Array<Zion::String>& list);
	bool GetSheetNames(const char* filename, Zion::Array<Zion::String>& sheets);

	void ClearTemplateDefine();
	bool LoadTemplateDefine(const char* filename);

	void AddTask(const char* tmpl, const char* filename, const char* sheetname, bool clear);
	void ClearTasks();
	bool RunTasks();

protected:
	bool ImportSheet(const char* tmpl, const char* filename, const char* sheetname, bool clear);

private:
	Zion::Map<Zion::String, EXCEL_ENUM*>		m_enum_map;
	Zion::Map<Zion::String, EXCEL_TEMPLATE*>	m_tmpl_map;
	Zion::String m_errmsg;

	struct TASK_INFO
	{
		Zion::String tmpl;
		Zion::String filename;
		Zion::String sheetname;
		bool clear;
	};

	Zion::Array<TASK_INFO>						m_Tasks;
	Zion::Set<A_UUID>							m_DeleteList;
	Zion::String								m_XlsFileName;
	COLEAutoExcelWrapper*						m_pXlsFile;
};

#endif	// _EXCEL_IMPORTOR_H__
