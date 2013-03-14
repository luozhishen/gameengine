//------------------------------------------------------------------------------
// Atlas - Copyright (c) 2006-2012 by Epic Games China / Yingpei Games
//------------------------------------------------------------------------------

#include <wx/wx.h>
#include <wx/string.h>
#include <AtlasSTL.h>
#include "OLEAutoExcelWrapper.h"

HRESULT OLEWrap(WORD autoType, VARIANT *pvResult, IDispatch *pDispatch, LPOLESTR ptName, DWORD cArgs...)
{
	if ( !pDispatch )
	{
		assert(!"NULL IDispatch passed to OLEWrap");
		return E_FAIL;
	}

	va_list marker;
	va_start(marker, cArgs);

	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	DISPID dispidNamed = DISPID_PROPERTYPUT;
	DISPID dispID;
	HRESULT hRes;

	hRes = pDispatch->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispID);
	if ( FAILED(hRes) )
	{
		wxString sMsg = wxString::Format( wxT("IDispatch::GetIDsOfNames: %s failed, error 0x%08x"), ptName, hRes );
		wxMessageBox( sMsg, wxT("Warning") );
		return hRes;
	}

	VARIANT *pArgs = new VARIANT[cArgs+1];

	for ( DWORD ii = 0; ii < cArgs; ++ii )
	{
		pArgs[ii] = va_arg(marker, VARIANT);
	}

	dp.cArgs = cArgs;
	dp.rgvarg = pArgs;

	if ( autoType & DISPATCH_PROPERTYPUT )
	{
		dp.cNamedArgs = 1;
		dp.rgdispidNamedArgs = &dispidNamed;
	}

	hRes = pDispatch->Invoke( dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, autoType, &dp, pvResult, NULL, NULL );
	if ( FAILED(hRes) )
	{
		wxString sMsg = wxString::Format(wxT("IDispatch::Invoke: %s=%08x failed, error: 0x%08x"), ptName, dispID, hRes );
		wxMessageBox( sMsg, wxT("Warning") );
	}
	else
	{
		va_end(marker);
	}

	delete[] pArgs;
	return hRes;
}

COLEAutoExcelWrapper::COLEAutoExcelWrapper()
{
	m_hRes = S_OK;
	m_pExcelApp = NULL;
	m_pExcelBooks = NULL;
	m_pActiveBook = NULL;
	m_pExcelSheets = NULL;
	m_pActiveSheet = NULL;
}

COLEAutoExcelWrapper::~COLEAutoExcelWrapper()
{
	Quit();
	CoUninitialize();
}

HRESULT COLEAutoExcelWrapper::Quit()
{
	if ( !m_pExcelApp )
	{
		return E_FAIL;
	}
	DISPID dispID;
	LPOLESTR ptName = L"Quit";

	m_hRes = m_pExcelApp->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispID);

	if ( SUCCEEDED(m_hRes) )
	{
		DISPPARAMS dp = { NULL, NULL, 0, 0 };
		m_hRes = m_pExcelApp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, NULL, NULL, NULL);
	}

	if ( m_pActiveSheet )
	{
		m_pActiveSheet->Release();
		m_pActiveSheet = NULL;
	}
	if ( m_pExcelSheets )
	{
		m_pExcelSheets->Release();
		m_pExcelSheets = NULL;
	}
	if ( m_pActiveBook )
	{
		m_pActiveBook->Release();
		m_pActiveBook = NULL;
	}
	if ( m_pExcelBooks )
	{
		m_pExcelBooks->Release();
		m_pExcelBooks = NULL;
	}
	if ( m_pExcelApp )
	{
		m_pExcelApp->Release();
		m_pExcelApp = NULL;
	}

	return m_hRes;
}

HRESULT COLEAutoExcelWrapper::Initialize()
{
	CoInitialize(NULL);
	CLSID clsid;
	m_hRes = CLSIDFromProgID(L"Excel.Application", &clsid);
	if ( SUCCEEDED(m_hRes) )
	{
		m_hRes = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void **)&m_pExcelApp);
		if ( FAILED(m_hRes) )
		{
			m_pExcelApp=NULL;
		}
	}

	return m_hRes;
}

HRESULT COLEAutoExcelWrapper::SetVisible( bool bVisible )
{
	if ( !m_pExcelApp )
	{
		return E_FAIL;
	}

	VARIANT param1;
	param1.vt = VT_I4;
	param1.lVal = bVisible;
	m_hRes = OLEWrap( DISPATCH_PROPERTYPUT, NULL, m_pExcelApp, L"Visible", 1, param1 );

	return m_hRes;
}

HRESULT COLEAutoExcelWrapper::OpenExcelBook( bool bVisible )
{
	if ( !m_pExcelApp ) 
	{
		if ( FAILED(m_hRes = Initialize()) )
		{
			return m_hRes;
		}

		if ( FAILED(m_hRes = SetVisible(bVisible)) )
		{
			return m_hRes;
		}
	}

	{
		VARIANT result;
		VariantInit(&result);
		m_hRes = OLEWrap(DISPATCH_PROPERTYGET, &result, m_pExcelApp, L"Workbooks", 0);
		if ( FAILED(m_hRes) )
		{
			return m_hRes;
		}

		m_pExcelBooks = result.pdispVal;
	}	

	{
		VARIANT result;
		VariantInit(&result);
		VARIANT param1;
		param1.vt = VT_BSTR;
		param1.bstrVal = SysAllocString((OLECHAR*)m_sFilePath.c_str());
		m_hRes = OLEWrap(DISPATCH_PROPERTYGET, &result, m_pExcelBooks, L"Open", 1, param1);
		SysFreeString( param1.bstrVal );

		if ( FAILED(m_hRes) )
		{
			return m_hRes;
		}
		m_pActiveBook = result.pdispVal;
	}

	{
		VARIANT result;
		VariantInit(&result);

		m_hRes = OLEWrap(DISPATCH_PROPERTYGET, &result, m_pActiveBook, L"Sheets", 0);
		if ( FAILED(m_hRes) )
		{
			return m_hRes;
		}
		m_pExcelSheets = result.pdispVal;
	}

	return m_hRes;
}

HRESULT COLEAutoExcelWrapper::Save()
{
	if ( !m_pActiveBook )
	{
		return E_FAIL;
	}

	m_hRes = OLEWrap( DISPATCH_METHOD, NULL, m_pActiveBook, L"Save", 0 );

	return m_hRes;
}

void COLEAutoExcelWrapper::SetFilePath(const wxString& sFilePath )
{
	m_sFilePath = sFilePath;
}

HRESULT COLEAutoExcelWrapper::GetExcelSheets( Atlas::Vector<wxString>& vSheets )
{
	if ( !m_pExcelSheets )
	{
		return E_FAIL;
	}

	DWORD iSheetCount;
	{
		VARIANT result;
		VariantInit(&result);
		m_hRes = OLEWrap(DISPATCH_PROPERTYGET, &result, m_pExcelSheets, L"Count", 0);
		if ( FAILED(m_hRes) )
		{
			return m_hRes;
		}
		iSheetCount = result.iVal;
	}

	for ( DWORD ii = 0; ii < iSheetCount; ++ii )
	{
		IDispatch *pSheet;
		{
			VARIANT result;
			VariantInit(&result);
			VARIANT param1;
			param1.vt = VT_I4;
			param1.lVal = ii + 1;
			m_hRes = OLEWrap(DISPATCH_PROPERTYGET, &result, m_pExcelSheets, L"Item", 1, param1);
			if ( FAILED(m_hRes) )
			{
				return m_hRes;
			}
			pSheet = result.pdispVal;
		}

		{
			VARIANT result;
			VariantInit(&result);
			m_hRes = OLEWrap(DISPATCH_PROPERTYGET, &result, pSheet, L"Name", 0);
			if ( FAILED(m_hRes) )
			{
				return m_hRes;
			}

			//UINT uiResultLen = (UINT)SysStringLen( result.bstrVal );
			vSheets.push_back(result.bstrVal);
		}

		pSheet->Release();
	}

	return m_hRes;
}

HRESULT COLEAutoExcelWrapper::SetActiveSheet( const wxString& sSheetName )
{
	if ( !m_pExcelSheets )
	{
		return E_FAIL;
	}

	if ( m_pActiveSheet )
	{
		m_pActiveSheet->Release();
		m_pActiveSheet = NULL;
	}

	{
		VARIANT result;
		VariantInit(&result);
		VARIANT param1;
		param1.vt = VT_BSTR;
		param1.bstrVal = SysAllocString( sSheetName );
		m_hRes = OLEWrap(DISPATCH_PROPERTYGET, &result, m_pExcelSheets, L"Item", 1, param1);
		SysFreeString( param1.bstrVal );

		if ( FAILED(m_hRes) )
		{
			return m_hRes;
		}
		m_pActiveSheet = result.pdispVal;
	}

	return m_hRes;
}

HRESULT COLEAutoExcelWrapper::SetCellValue( const wxString& sRange, const wxString& sValue )
{
	if ( !m_pActiveSheet )
	{
		return E_FAIL;
	}

	IDispatch* pRange;
	{
		VARIANT param1;
		param1.vt = VT_BSTR;
		param1.bstrVal = SysAllocString( sRange.c_str() );
		VARIANT result;
		VariantInit(&result);
		m_hRes = OLEWrap(DISPATCH_PROPERTYGET, &result, m_pActiveSheet, L"Range", 1, param1);
		SysFreeString( param1.bstrVal );
		if ( FAILED(m_hRes) )
		{
			return m_hRes;
		}
		pRange = result.pdispVal;
	}

	{
		VARIANT param1;
		param1.vt = VT_BSTR;
		param1.bstrVal = SysAllocString( sValue.c_str() );
		m_hRes = OLEWrap(DISPATCH_PROPERTYPUT, NULL, pRange, L"Value", 1, param1);
		SysFreeString( param1.bstrVal );
	}

	pRange->Release();
		
	return m_hRes;
}

HRESULT COLEAutoExcelWrapper::GetCellValue( const wxString& sRange, wxString& sValue )
{
	if ( !m_pActiveSheet )
	{
		return E_FAIL;
	}

	IDispatch* pRange;
	{
		VARIANT param1;
		param1.vt = VT_BSTR;
		param1.bstrVal = SysAllocString( sRange.c_str() );
		VARIANT result;
		VariantInit(&result);
		m_hRes = OLEWrap(DISPATCH_PROPERTYGET, &result, m_pActiveSheet, L"Range", 1, param1);
		SysFreeString( param1.bstrVal );
		if ( FAILED(m_hRes) )
		{
			return m_hRes;
		}
		pRange = result.pdispVal;
	}

	{
		VARIANT result;
		VariantInit(&result);
		m_hRes = OLEWrap(DISPATCH_PROPERTYGET, &result, pRange, L"Value", 0);
		if ( FAILED(m_hRes) )
		{
			return m_hRes;
		}
		if ( result.vt != VT_EMPTY && result.vt != VT_BSTR )
		{
			m_hRes = ::VariantChangeType(&result, &result, 0, VT_BSTR);
			if ( FAILED(m_hRes) )
			{
				return m_hRes;
			}
		}

		//UINT uiResultLen = SysStringLen( result.bstrVal );
		sValue = result.bstrVal;
	}

	pRange->Release();

	return m_hRes;
}
