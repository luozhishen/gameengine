#include <ZionBase.h>

#include "bundle.h"

#include <fstream>
#include <algorithm>
#include <functional>

namespace JPatch
{

	bool SectionGreater(const CSection& s1, const CSection& s2)
	{
		return s1._Offset < s2._Offset;
	}

	CSection* CFile::GetSectionByMD5(Zion::String& md5)
	{
		for(_U32 i=0; i<_Sections.size(); i++)
		{
			if(_Sections[i]._MD5==md5) return &_Sections[i];
		}
		return NULL;
	}

	CSection* CFile::GetSectionByName(Zion::String& name)
	{
		for(_U32 i=0; i<_Sections.size(); i++)
		{
			if(_Sections[i]._Name==name) return &_Sections[i];
		}
		return NULL;
	}

	bool CBundle::InitFromBLS(const char* bls, const char* basedir)
	{
		_BaseDir = basedir;
		_Files.clear();

		std::ifstream f(bls);
		if(!f.is_open())
		{
			printf("failed to open %s\n", bls);
			return false;
		}

		CFile* file = NULL;
		Zion::String line;
		while(!f.eof())
		{
			std::getline(f, line);
			line = Zion::StringTrim(line);
			if(line.size()==0) continue;
			if(line[0]=='*')
			{
				Zion::Vector<Zion::String> rs;
				Zion::StringSplit(line, ' ', rs);

				rs[0] = rs[0].substr(1);
				if(_Files.find(rs[0])!=_Files.end())
				{
					printf("%s : reduplicate file %s\n", bls, rs[0].c_str());
					return false;
				}
				file = &_Files[rs[0]];
				file->_Name = rs[0];
				file->_MD5 = rs[1];
				file->_Bundle = this;
			}
			else
			{
				if(file==NULL)
				{
					printf("%s : no file\n", bls);
					return false;
				}
				Zion::Vector<Zion::String> rs;
				Zion::StringSplit(line, ' ', rs);
				if(rs.size()!=3 && rs.size()!=4)
				{
					printf("%s : error data\n", bls);
					return false;
				}

				CSection section;
				section._Name	= rs[0];
				section._Offset = std::stoi(rs[1].c_str());
				section._Size	= std::stoi(rs[2].c_str());
				section._MD5	= rs.size()==3?"":rs[3].c_str();
				file->_Sections.push_back(section);
			}
		}

		Zion::Map<Zion::String, CFile>::iterator i;
		for(i=_Files.begin(); i!=_Files.end(); i++)
		{
			file = &i->second;
			std::sort(file->_Sections.begin(), file->_Sections.end(), SectionGreater);
			file->_SectionMap.clear();
			for(_U32 c=0; c<file->_Sections.size(); c++)
			{
				if(file->_SectionMap.find(file->_Sections[c]._Name)!=file->_SectionMap.end())
				{
					printf("%s : %s : reduplicate section %s\n", bls, file->_Name.c_str(), file->_Sections[c]._Name.c_str());
					return false;
				}
				if(c==0)
				{
					if(file->_Sections[0]._Offset!=0)
					{
						printf("%s : %s : section [%s] offset != 0\n", bls, file->_Name.c_str(), file->_Sections[0]._Name.c_str());
						return false;
					}
				}
				else
				{
					if(file->_Sections[c]._Offset!=file->_Sections[c-1]._Offset+file->_Sections[c-1]._Size)
					{
						printf("%s : %s : section [%s] offset not valid\n", bls, file->_Name.c_str(), file->_Sections[0]._Name.c_str());
						return false;
					}
				}
				file->_SectionMap[file->_Sections[c]._Name] = &file->_Sections[c];
			}
		}

		return true;
	}

}
