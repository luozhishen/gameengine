//------------------------------------------------------------------------------
// Zion - Copyright (c) 2006-2012 by Epic Games China / Yingpei Games
//------------------------------------------------------------------------------

#ifndef _OLEAUTOEXCELWRAPPER_H_
#define _OLEAUTOEXCELWRAPPER_H_

struct IDispatch;

class COLEAutoExcelWrapper
{
public:
	COLEAutoExcelWrapper();
	~COLEAutoExcelWrapper();
	bool Open(const Zion::String& sFileName, bool bVisible);
	bool Save();
	bool Quit();
	bool GetExcelSheets(Zion::Array<Zion::String>& vSheets);
	bool SetActiveSheet(const Zion::String& sSheetName);
	bool SetCellValue(const Zion::String& sRange, const Zion::String& sValue);
	bool GetCellValue(const Zion::String& sRange, Zion::String& sValue);

private:
	bool Initialize();

private:
	IDispatch* m_pExcelApp;
	IDispatch* m_pExcelBooks;
	IDispatch* m_pActiveBook;
	IDispatch* m_pExcelSheets;
	IDispatch* m_pActiveSheet;
};

#endif // _OLEAUTOEXCELWRAPPER_H_
