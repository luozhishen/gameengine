//------------------------------------------------------------------------------
// Atlas - Copyright (c) 2006-2012 by Epic Games China / Yingpei Games
//------------------------------------------------------------------------------

#ifndef _OLEAUTOEXCELWRAPPER_H_
#define _OLEAUTOEXCELWRAPPER_H_

#include <ole2.h>

class COLEAutoExcelWrapper2
{
public:
	COLEAutoExcelWrapper2();
	~COLEAutoExcelWrapper2();
	HRESULT SetVisible( bool bVisible = true );
	HRESULT OpenExcelBook( bool bVisible = true );
	HRESULT Save();
	HRESULT Quit();
	void SetFilePath( const wxString& sFilePath );
	HRESULT GetExcelSheets( Atlas::Vector<wxString>& vSheets );
	HRESULT SetActiveSheet( const wxString& sSheetName );
	HRESULT SetCellValue( const wxString& sRange, const wxString& sValue );
	HRESULT GetCellValue( const wxString& sRange, wxString& sValue );

private:
	HRESULT Initialize();

private:
	IDispatch* m_pExcelApp;
	IDispatch* m_pExcelBooks;
	IDispatch* m_pActiveBook;
	IDispatch* m_pExcelSheets;
	IDispatch* m_pActiveSheet;
	HRESULT m_hRes;
	wxString m_sFilePath;
};

#endif // _OLEAUTOEXCELWRAPPER_H_