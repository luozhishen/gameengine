//------------------------------------------------------------------------------
// Atlas - Copyright (c) 2006-2012 by Epic Games China / Yingpei Games
//------------------------------------------------------------------------------

#ifndef _OLEAUTOEXCELWRAPPER_H_
#define _OLEAUTOEXCELWRAPPER_H_

struct IDispatch;

class COLEAutoExcelWrapper
{
public:
	COLEAutoExcelWrapper();
	~COLEAutoExcelWrapper();
	bool Open(const Atlas::String& sFileName, bool bVisible);
	bool Save();
	bool Quit();
	bool GetExcelSheets(Atlas::Vector<Atlas::String>& vSheets);
	bool SetActiveSheet(const Atlas::String& sSheetName);
	bool SetCellValue(const Atlas::String& sRange, const Atlas::String& sValue);
	bool GetCellValue(const Atlas::String& sRange, Atlas::String& sValue);

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
