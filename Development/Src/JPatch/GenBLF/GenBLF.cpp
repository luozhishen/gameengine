#include <io.h>
#include <errno.h>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <AtlasBase.h>

static bool _dir_listfile(Atlas::String basepath, Atlas::String path, Atlas::Vector<Atlas::String>& files)
{
	_finddata_t info;
	Atlas::String strfind = basepath + path + "*";
	intptr_t h = _findfirst(strfind.c_str(), &info);
	if(h == -1L) return errno==ENOENT;

	do
	{
		if(info.attrib & _A_SUBDIR)
		{
			if(strcmp(info.name,".")!=0 && strcmp(info.name, "..")!=0)
			{
				if(!_dir_listfile(basepath, path+info.name+"/", files)) return false;
            }
		}
		else
		{
			files.push_back(path+info.name);
		}
	} while(_findnext(h, &info)==0);
  
    _findclose(h);
	return true;
}

bool dir_listfile(const char* basepath, Atlas::Vector<Atlas::String>& files)
{
	files.clear();
	return _dir_listfile(basepath, "", files);
}

struct SECTION_INFO
{
	Atlas::String name;
	size_t offset, size;
};

bool load_patchinfo(const char* filename, std::map<Atlas::String, std::list<SECTION_INFO>>& files)
{
	std::ifstream f(filename);
	if(!f.is_open()) return false;
	Atlas::String line;
	while(!f.eof())
	{
		std::getline(f, line);
		line = Atlas::StringTrim(line);
		if(line.empty()) continue;
		if(line.find("[PATCHINFO]")==Atlas::String::npos) continue;

		Atlas::Vector<Atlas::String> fields;
		Atlas::StringSplit(line, ' ', fields);
		if(fields.size()!=7) continue;

		size_t a = fields[3].find_last_of('\\');
		size_t b = fields[3].find_last_of('/');
		if(b==Atlas::String::npos) b = a;
		if(a==Atlas::String::npos || a<b) a = b;
		if(a!=Atlas::String::npos) fields[3] = fields[3].substr(a+1, fields[3].size()-a-1);

		if(fields[4]=="$$HEADER$$")
		{
			files[fields[3]].clear();
		}
		SECTION_INFO info;
		info.name = fields[4];
		info.offset = atoi(fields[5].c_str());
		info.size = atoi(fields[6].c_str());
		files[fields[3]].push_back(info);
	}

	return true;
}

int main(int argc, char* argv[])
{
	if(argc!=4 && argc!=3)
	{
		printf("invalid parameter\n");
		return 1;
	}

	std::map<Atlas::String, std::list<SECTION_INFO>> files;
	if(argc==4 && !load_patchinfo(argv[3], files))
	{
		printf("error in load_patchinfo\n");
		return 1;
	}

	Atlas::String basedir = argv[2];
	Atlas::Vector<Atlas::String> list;
	dir_listfile(basedir.c_str(), list);
	FILE* fp;
	char* mem;
	size_t size;

	FILE* fout = fopen(argv[1], "wt");
	if(fout==NULL)
	{
		printf("failed to open %s\n", argv[2]);
		return -1;
	}

	for(size_t i=0; i<list.size(); i++)
	{
		Atlas::String path = basedir + list[i];
		if((	fp=fopen(path.c_str(), "rb"))==NULL
			||	fseek(fp, 0, SEEK_END)!=0
			||	(size=ftell(fp))<0
			||	fseek(fp, 0, SEEK_SET)!=0
			||	(mem=(char*)malloc(size))==NULL
			||	fread(mem, 1, size, fp)!=size
			||	fclose(fp)!=0)
		{
			printf("failed to open %s\n", path.c_str());
			return -1;
		}

		size_t a = path.find_last_of('\\');
		size_t b = path.find_last_of('/');
		if(b==Atlas::String::npos) b = a;
		if(a==Atlas::String::npos || a<b) a = b;
		Atlas::String filename = path;
		if(a!=Atlas::String::npos) filename = path.substr(a+1, path.size()-a-1);

		Atlas::String sha1 = Atlas::CHashMD5::CalcToString(mem, (_U32)size);
		fprintf(fout, "*%s %s\n", list[i].c_str(), sha1.c_str());
		if(files.find(filename)!=files.end())
		{
			std::list<SECTION_INFO>& slist = files[filename];
			std::list<SECTION_INFO>::iterator i;
			size_t bytes = 0;
			for(i=slist.begin(); i!=slist.end(); i++)
			{
				sha1 = Atlas::CHashMD5::CalcToString(mem+(*i).offset, (_U32)(*i).size);
				if((*i).offset!=bytes)
				{
					printf("%s %s : invalid offset\n", filename.c_str(), (*i).name.c_str());
					return 1;
				}
				bytes += (*i).size;
				fprintf(fout, "%s %d %d %s\n", (*i).name.c_str(), (*i).offset, (*i).size, sha1.c_str());
			}
			if(bytes!=size)
			{
				printf("%s : invalid file size\n", filename.c_str());
				return 1;
			}
		}
		else
		{
			fprintf(fout, "$$BODY$$ 0 %d %s\n", size, sha1.c_str());
		}

		free(mem);
	}

	fclose(fout);
	printf("done\n");
	return 0;
}
