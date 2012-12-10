//------------------------------------------------------------------------------
// Atlas - Copyright (c) 2006-2012 by Epic Games China / Yingpei Games
//------------------------------------------------------------------------------

#include <Windows.h>
#include <psapi.h>
#include <io.h>
#include <sys/stat.h>
#include <bitset>

#include <AtlasBase.h>
#include <AtlasCommon.h>

#include <cstdarg>
#include <iterator>

#include "UtilString.h"

namespace Atlas
{
	namespace String
	{


		std::wstring Utf8ToUtf16( const std::string& sSrc )
		{
			return Utf8ToUtf16( sSrc.c_str(), (_U32)( sSrc.size() ) );
		}

		std::wstring Utf8ToUtf16( const char* const szSource, const _U32 uiSize )
		{
			static std::wstring sWrongString = L"\"String::Utf8ToUtf16 Conversion Failed\"";
			if ( uiSize == 0 )
			{
				return std::wstring();
			}

			const int iRequiredLength = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, szSource, uiSize, NULL, 0 );
			if ( iRequiredLength == 0 )
			{
				//ALOGE( gLogRoot, "Failed to determine length of the converted UTF-16 string: %d", GetLastError() );
				return sWrongString;
			}

			std::wstring sResult( iRequiredLength, L'\0' );
			const int iConvertResult = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, szSource, uiSize, const_cast<wchar_t*>( sResult.data() ), iRequiredLength );
			if ( iConvertResult != iRequiredLength )
			{
				//ALOGE( gLogRoot, "Failed to convert UTF-8 string to UTF-16 string : %d", GetLastError() );
				return sWrongString;
			}

			return sResult;
		}

		std::string Utf16ToUtf8( const std::wstring& sSrc )
		{
			return Utf16ToUtf8( sSrc.c_str(), (_U32)( sSrc.size() ) );
		}

		std::string Utf16ToUtf8( const wchar_t* const szSource, const _U32 uiSize )
		{
			static std::string sWrongString = "\"String::Utf16ToUtf8 Conversion Failed\"";
			if ( uiSize == 0 )
			{
				return std::string();
			}

			const int iRequiredLength = WideCharToMultiByte( CP_UTF8, 0, szSource, uiSize, NULL, 0, NULL, NULL );
			if ( iRequiredLength == 0 )
			{
				//ALOGE( gLogRoot, "Failed to determine length of the converted UTF-8 string: %d", GetLastError() );
				return sWrongString;
			}

			std::string sResult( iRequiredLength, '\0' );
			const int iConvertResult = WideCharToMultiByte( CP_UTF8, 0, szSource, uiSize, const_cast<char*>( sResult.data() ), iRequiredLength, NULL, NULL );
			if ( iConvertResult != iRequiredLength )
			{
				//ALOGE( gLogRoot, "Failed to convert UTF-16 string to UTF-8 string: %d", GetLastError() );
				return sWrongString;
			}

			return sResult;
		}


		std::string MultiByteToUtf8( const std::string& sSrc )
		{
			return MultiByteToUtf8( sSrc.c_str(), (_U32)( sSrc.size() ) );
		}

		std::string MultiByteToUtf8( const char* const szSource, const _U32 uiSize )
		{
			static std::string sWrongString = "\"String::MultiByteToUtf8 Conversion Failed\"";
			if ( uiSize == 0 )
			{
				return std::string();
			}

			const int iRequiredLength = MultiByteToWideChar( CP_ACP, MB_ERR_INVALID_CHARS, szSource, uiSize, NULL, 0 );
			if ( iRequiredLength == 0 )
			{
				//ALOGE( gLogRoot, "Failed to determine length of converted UTF-8 string: %d", GetLastError() );
				return sWrongString;
			}

			std::wstring sResult( iRequiredLength, L'\0' );
			const int iConvertResult = MultiByteToWideChar( CP_ACP, MB_ERR_INVALID_CHARS, szSource, uiSize, const_cast<wchar_t*>( sResult.data() ), iRequiredLength );
			if ( iConvertResult != iRequiredLength )
			{
				//ALOGE( gLogRoot, "Failed to convert MultiByte string to UTF-8 string: %d", GetLastError() );
				return sWrongString;
			}

			return Utf16ToUtf8( sResult );
		}


		std::string Utf8ToMultiByte( const std::string& sSrc )
		{
			return Utf8ToMultiByte( sSrc.c_str(), (_U32)( sSrc.size() ) );
		}

		std::string Utf8ToMultiByte( const char* const szSource, const _U32 uiSize )
		{
			static std::string sWrongString = "\"String::MultiByteToUtf8 Conversion Failed\"";
			if ( uiSize == 0 )
			{
				return std::string();
			}

			std::wstring wsTemp = String::Utf8ToUtf16( szSource, uiSize );

			const int iRequiredLength = WideCharToMultiByte( CP_ACP, 0, wsTemp.c_str(), (_U32)( wsTemp.length() ), NULL, 0, NULL, NULL );
			if ( iRequiredLength == 0 )
			{
				//ALOGE( gLogRoot, "Failed to determine length of the converted MultiByte string: %d", GetLastError() );
				return sWrongString;
			}

			std::string sResult( iRequiredLength, '\0' );
			const int iConvertResult = WideCharToMultiByte( CP_ACP, 0, wsTemp.c_str(), (_U32)( wsTemp.length() ), const_cast<char*>( sResult.data() ), iRequiredLength, NULL, NULL );
			if ( iConvertResult != iRequiredLength )
			{
				//ALOGE( gLogRoot, "Failed to convert UTF-8 string to MultiByte string: %d", GetLastError() );
				return sWrongString;
			}

			return sResult;
		}

		std::wstring ANSIToUnicode(const std::string& str)
		{
			int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
			wchar_t* pUnicode;
			pUnicode = new wchar_t[unicodeLen+1];
			memset(pUnicode, 0, (unicodeLen+1)*sizeof(wchar_t));
			MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
			std::wstring rt;
			rt = (wchar_t*)pUnicode;
			delete pUnicode;
			return rt;
		}

		std::string UnicodeToANSI(const std::wstring& str)
		{
			char* pElementText;
			int nTextLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
			pElementText = new char[nTextLen+1];
			memset((void*)pElementText, 0, sizeof(char)*(nTextLen+1));
			WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, pElementText, nTextLen, NULL, NULL);
			std::string strText = pElementText;
			delete[] pElementText;
			return strText;
		}

		char* replace(char* szSrc, int len, char szTarget, char szValue)
		{
			for(int i = 0; szSrc+i != NULL && *(szSrc+i) != '\0' && i < len; ++i)
			{
				if(*(szSrc+i) == szTarget)
				{
					*(szSrc+i) = szValue;
				}
			}

			return szSrc;
		}
	}
} // Atlas

