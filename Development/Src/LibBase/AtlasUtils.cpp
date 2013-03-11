#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "AtlasBase.h"

#include <sstream>

namespace Atlas
{

	void StringSplit(const Atlas::String& value, char splitchar, Atlas::Vector<Atlas::String>& result)
	{
		result.clear();
		Atlas::StringStream sstr(value);
		Atlas::String tmp;
	    while(std::getline(sstr, tmp, splitchar))
		{
			result.push_back(tmp);
		}
	}

	Atlas::String StringFormat(const char* fmt, ...)
	{
		size_t size=10;
		Atlas::String str;
		va_list ap;
		for(;;)
		{
			str.resize(size);
			va_start(ap, fmt);
			int n = vsnprintf((char *)str.c_str(), size, fmt, ap);
			va_end(ap);
			if(n>-1 && n<(int)size)
			{
				str.resize((size_t)n);
				return str;
			}
			if(n>-1)
				size=n+1;
			else
				size*=2;
		}
	}

#ifndef IPHONE
	class CAtlasExeDirInit
	{
	public:
		CAtlasExeDirInit()
		{
#ifdef WIN32
			if(GetModuleFileNameA(NULL, path, sizeof(path)))
			{
				char curpath[400];
				*(strrchr(path, '\\')+1) = '\0';
				ATLAS_VERIFY(getcwd(curpath, sizeof(curpath)));
				ATLAS_VERIFY(chdir(path)==0);
				ATLAS_VERIFY(getcwd(path, sizeof(path)));
				ATLAS_VERIFY(chdir(curpath)==0);
			}
			else
#endif
			{
#ifndef WIN32
				int	res = readlink("/proc/self/exe", path, sizeof(path));  
				if(res>=0 && res<size)
				{
					path[res] = '\0';
					*(strrchr(path, '\\')+1) = '\0';
				}
				else
#endif
				{
					char curpath[400];
					ATLAS_VERIFY(getcwd(curpath, sizeof(curpath)));
					char exepath[300];
					strcpy(exepath, __argv[0]);
					char* pos1 = strrchr(exepath, '\\');
					char* pos2 = strrchr(exepath, '/');
					if(pos1<pos2) pos1 = pos2;
					if(!pos1)
					{
						strcpy(path, curpath);
					}
					else
					{
						*pos1 = '\0';
						chdir(exepath);
						ATLAS_VERIFY(getcwd(path, sizeof(path)));
						chdir(curpath);
					}
				}
			}

			size_t slen = strlen(path);
			ATLAS_ASSERT(slen>0);
			if(path[slen-1]!='\\' && path[slen-1]!='/')
			{
				path[slen] = ATLAS_PATH_SLASH;
				path[slen+1] = '\0';
			}
		}
		char path[400];
	};
	static CAtlasExeDirInit __global_exe_dir_init;
	const char* GetExeDir()
	{
		return __global_exe_dir_init.path;
	}
#else
	const char* GetExeDir()
	{
		return NULL;
	}
#endif
}
