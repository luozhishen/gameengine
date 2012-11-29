//------------------------------------------------------------------------------
// Atlas - Copyright (c) 2006-2012 by Epic Games China / Yingpei Games
//------------------------------------------------------------------------------

#ifndef _OLEAUTOEXCELWRAPPER_H_
#define _OLEAUTOEXCELWRAPPER_H_

#include <vector>
#include <ole2.h>

namespace Atlas
{
	class COLEAutoExcelWrapper
	{
	public:
		COLEAutoExcelWrapper();
		~COLEAutoExcelWrapper();
		HRESULT SetVisible( bool bVisible = true );
		HRESULT OpenExcelBook( bool bVisible = true );
		HRESULT Save();
		HRESULT Quit();
		void SetFilePath( const wxString& sFilePath );
		HRESULT GetExcelSheets( std::vector<wxString>& vSheets );
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

} // Atlas

#endif // _OLEAUTOEXCELWRAPPER_H_
