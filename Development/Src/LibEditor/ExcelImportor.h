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

	void ClearTemplateDefine();
	bool LoadTemplateDefine(const char* filename);

	void Begin();
	void ClearData(const DDLReflect::STRUCT_INFO* info, bool bExactMatch);
	bool ImportSheet(const char* tmpl, COLEAutoExcelWrapper* excel);
	void End();

private:
	Zion::Map<Zion::String, EXCEL_ENUM*>		m_enum_map;
	Zion::Map<Zion::String, EXCEL_TEMPLATE*>	m_tmpl_map;
	Zion::String m_errmsg;

	Zion::Set<A_UUID>							m_DeleteList;
};

#endif	// _EXCEL_IMPORTOR_H__
