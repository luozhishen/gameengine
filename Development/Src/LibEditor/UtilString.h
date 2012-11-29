//------------------------------------------------------------------------------
// Atlas - Copyright (c) 2006-2012 by Epic Games China / Yingpei Games
//------------------------------------------------------------------------------

#ifndef _UTIL_STRING_H_
#define _UTIL_STRING_H_


#include <AtlasBase.h>
#include <AtlasCommon.h>

#include <cstdarg>
#include <iterator>

namespace Atlas
{
	namespace String
	{
		// - UTF8 to UTF16 conversion
		std::wstring Utf8ToUtf16( const char* szSource, _U32 uiSize );
		std::wstring Utf8ToUtf16( const std::string& sSrc );
		std::string Utf16ToUtf8( const wchar_t* szSource, _U32 uiSize );
		std::string Utf8ToMultiByte( const char* szSource, _U32 uiSize );
		std::string Utf8ToMultiByte( const std::string& sSrc );
		std::string MultiByteToUtf8( const char* szSource, _U32 uiSize );
		std::wstring ANSIToUnicode(const std::string& str);
		std::string UnicodeToANSI(const std::wstring& str);

		char* replace(char* szSrc, int len, char szTarget, char szValue);
	} // String

} // Atlas

#endif // _UTIL_STRING_H_

