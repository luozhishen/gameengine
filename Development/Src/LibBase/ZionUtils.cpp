#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <errno.h>
const char* __argv[] = {NULL, NULL, NULL};
#endif

#include "ZionBase.h"

namespace Zion
{

	void StringSplit(const Zion::String& value, char splitchar, Zion::Array<Zion::String>& result)
	{
		result.clear();
		Zion::StringStream sstr(value);
		Zion::String tmp;
	    while(std::getline(sstr, tmp, splitchar))
		{
			result.push_back(tmp);
		}
	}

	Zion::String StringFormat(const char* fmt, ...)
	{
		size_t size=10;
		Zion::String str;
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

	String StringTrim(const char* str)
	{
		String v = str;
		return StringTrim(v);
	}

	String StringTrim(const Zion::String& v)
	{
		Zion::String str = v;
		Zion::String::size_type pos = str.find_last_not_of(' ');
		if(pos != Zion::String::npos) 
		{
			str.erase(pos + 1);
			pos = str.find_first_not_of(' ');
			if(pos != std::string::npos) str.erase(0, pos);
		}
		else str.erase(str.begin(), str.end());
		return v;
	}

#ifndef IPHONE
	class CZionExeDirInit
	{
	public:
		CZionExeDirInit()
		{
#ifdef WIN32
			if(GetModuleFileNameA(NULL, path, sizeof(path)))
			{
				char curpath[400];
				*(strrchr(path, '\\')+1) = '\0';
				ZION_VERIFY(getcwd(curpath, sizeof(curpath)));
				ZION_VERIFY(chdir(path)==0);
				ZION_VERIFY(getcwd(path, sizeof(path)));
				ZION_VERIFY(chdir(curpath)==0);
			}
			else
#endif
			{
#ifndef WIN32
				int	res = readlink("/proc/self/exe", path, sizeof(path));  
				if(res>=0 && res<sizeof(path))
				{
					path[res] = '\0';
					*(strrchr(path, '\\')+1) = '\0';
				}
				else
#endif
				{
					char curpath[400];
					ZION_VERIFY(getcwd(curpath, sizeof(curpath)));
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
						ZION_VERIFY(getcwd(path, sizeof(path)));
						chdir(curpath);
					}
				}
			}

			size_t slen = strlen(path);
			ZION_ASSERT(slen>0);
			if(path[slen-1]!='\\' && path[slen-1]!='/')
			{
				path[slen] = ZION_PATH_SLASH;
				path[slen+1] = '\0';
			}
		}
		char path[400];
	};
	static CZionExeDirInit __global_exe_dir_init;
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

#ifndef _WIN32
	bool MakeDirectory(const char* dir)
	{
		std::string s = dir;

		while(s.size()>0 && (*(s.c_str()+s.size()-1)=='\\' || *(s.c_str()+s.size()-1)=='/'))
		{
			s = s.substr(0, s.size()-1);
		}
		if(s.size()==0) return false;

		if(mkdir(s.c_str(), 0777)==0) return true;
		if(errno!=ENOENT) return false;

		std::string k = s;
		while(k.size()>0 && (*(k.c_str()+k.size()-1)!='\\' && *(k.c_str()+k.size()-1)!='/'))
		{
			k = k.substr(0, k.size()-1);
		}
		if(k.size()==0) return false;
		if(!MakeDirectory(k.c_str())) return false;

		if(mkdir(s.c_str(), 0777)!=0) return false;
		return true;
	}

	bool RemoveDirectory(const char* dir)
	{
		DIR *dirp;
		struct dirent *dp;
		char subdir[256];
		struct stat pStat;

		dirp = opendir(dir);
		if(dirp==NULL)
		{
			return false;
		}
		while((dp=readdir(dirp))!=NULL)
		{
			if(strcmp(dp->d_name, ".")==0 || strcmp(dp->d_name, "..")==0)
			{
				continue;
			}
		
			strcpy(subdir, dir);
			strcat(subdir, "/");
			strcat(subdir, dp->d_name);
			if(stat(subdir, &pStat)==-1)
			{
				closedir(dirp);
				return false;
			}
			if((pStat.st_mode&S_IFMT)==S_IFDIR)
			{
				if(!RemoveDirectory(subdir))
				{
					closedir(dirp);
					return false;
				}
			}
			else
			{
				if(unlink(dir)==-1)
				{
					closedir(dirp);
					return false;
				}
			}
		}
		closedir(dirp);
		return rmdir(dir)!=-1;
	}

	bool RenameFile(const char* n1, const char* n2)
	{
		return rename(n1, n2)==0;
	}

	bool RemoveFile(const char* n)
	{
		return unlink(n)==0;
	}

#else

	bool MakeDirectory(const char* dir)
	{
		std::string s = dir;

		while(s.size()>0 && (*(s.c_str()+s.size()-1)=='\\' || *(s.c_str()+s.size()-1)=='/'))
		{
			s = s.substr(0, s.size()-1);
		}
		if(s.size()==0) return false;

		if(CreateDirectoryA(s.c_str(), NULL)) return true;
		if(GetLastError()!=ERROR_PATH_NOT_FOUND) return false;

		std::string k = s;
		while(k.size()>0 && (*(k.c_str()+k.size()-1)!='\\' && *(k.c_str()+k.size()-1)!='/'))
		{
			k = k.substr(0, k.size()-1);
		}
		if(k.size()==0) return false;
		if(!MakeDirectory(k.c_str())) return false;

		if(!CreateDirectoryA(s.c_str(), NULL)) return false;
		return true;
	}

	bool RemoveDirectory(const char* dir)
	{
		WIN32_FIND_DATAA finddata;
		std::string mask = dir;
		mask += "\\*";
		HANDLE hFind = FindFirstFileA(mask.c_str(), &finddata);
		if(hFind==INVALID_HANDLE_VALUE)
		{
			return false;
		}
	
		for(;;)
		{
			if(strcmp(finddata.cFileName, ".")!=0 && strcmp(finddata.cFileName, "..")!=0)
			{
				std::string filePath = dir;
				filePath += "\\";
				filePath += finddata.cFileName;
				if(((finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) || ((finddata.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0))
				{
					if(!RemoveDirectory(filePath.c_str()))
					{
						FindClose(hFind);
						return false;
					}
				}
				else
				{
					if(!DeleteFileA(filePath.c_str()))
					{
						FindClose(hFind);
						return false;
					}
				}
			}

			if(!FindNextFileA(hFind, &finddata))
			{
				if(GetLastError()!=ERROR_NO_MORE_FILES)
				{
					FindClose(hFind);
					return false;
				}
				else
				{
					FindClose(hFind);
					break;
				}
			}
		}

		return RemoveDirectory(dir)?true:false;
	}

	bool RenameFile(const char* n1, const char* n2)
	{
		return MoveFileA(n1, n2)?true:false;
	}

	bool RemoveFile(const char* n)
	{
		return DeleteFileA(n)?true:false;
	}

#endif

	void ToHexString(const void* data, _U32 size, String& out)
	{
		const char tab[]={"0123456789abcdef"};
		char txt[1000];
		const _U8* hash = (const _U8*)data;
		for(_U32 i=0; i<size; i++)
		{
			txt[i*2+0] = tab[(hash[i]>>4)&0xF];
			txt[i*2+1] = tab[hash[i]&0xF];
		}
		txt[size*2] ='\0';
		out = txt;
	}

}
