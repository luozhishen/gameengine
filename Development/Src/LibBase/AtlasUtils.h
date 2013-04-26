#ifndef	__ATLAS_UTILS__
#define	__ATLAS_UTILS__

#include <string>
#include <vector>
#include <map>
#include <functional>

#ifdef WIN32
	#define ATLAS_PATH_SLASH		'\\'
#else
	#define ATLAS_PATH_SLASH		'/'
#endif

namespace Atlas
{

	const char* GetExeDir();

	void StringSplit(const String& value, char splitchar, Vector<String>& result);
	Atlas::String StringFormat(const char* fmt, ...);

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
