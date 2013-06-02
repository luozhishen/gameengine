#ifndef	__ZION_UTILS__
#define	__ZION_UTILS__

#include <string>
#include <vector>
#include <map>
#include <functional>

#ifdef WIN32
	#define ZION_PATH_SLASH		'\\'
#else
	#define ZION_PATH_SLASH		'/'
#endif

namespace Zion
{

	const char* GetExeDir();

	void StringSplit(const String& value, char splitchar, Array<String>& result);
	String StringFormat(const char* fmt, ...);
	String StringTrim(const char* str);
	String StringTrim(const Zion::String& str);

	bool MakeDirectory(const char* dir);
	bool RemoveDirectory(const char* dir);
	bool RenameFile(const char* n1, const char* n2);
	bool RemoveFile(const char* n);

	void ToHexString(const void* data, _U32 size, String& out);

	class CNoCopy
	{
	private:
		CNoCopy(const CNoCopy&) { }
		CNoCopy& operator=(const CNoCopy&) { return *this; }
	public:
		CNoCopy() { }
	};

}

#endif
