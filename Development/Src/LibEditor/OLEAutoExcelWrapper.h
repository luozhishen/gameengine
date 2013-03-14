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
	bool SetVisible(bool bVisible = true);
	bool OpenExcelBook(bool bVisible = true);
	bool Save();
	bool Quit();
	void SetFilePath(const Atlas::String& sFilePath);
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
	Atlas::String m_sFilePath;
};

#endif // _OLEAUTOEXCELWRAPPER_H_
