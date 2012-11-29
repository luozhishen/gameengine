#ifndef _SIMPLE_DBFILE_H__
#define _SIMPLE_DBFILE_H__

#include <string>
#include <fstream>

#include <AtlasBase.h>
#include <AtlasDefines.h>

#include <json/jsoncpp.h>

namespace Atlas
{
	struct Simple_InfoArray
	{
		//Simple_Info[];
	};

	struct Simple_Info
	{
		friend std::ifstream;
		static char* szCol[];

		_U32 nUID;
		//std::string strPassword;
		//std::string strAvatar;
		//_U16 nLevel;
		//_U32 nMoney;

		Simple_Info()
		{
			//nUID = 0;
			//strAvatar = "";
			//nLevel = 0;
			//nMoney = 0;
		}

		/*std::string GetData(std::string& strKey)
		{
			char szFind[256];
			std::string strFind;
			for(_U32 i = 0; szCol[i] != NULL; ++i)
			{
				if(strKey == szCol[i])
				{
					switch(i)
					{
						case 0:
							{
								sprintf(szFind, "%d", nUID);
								strFind = szFind;
							}
							break;
						case 1:
							{
								strFind = strPassword;
							}
							break;

						case 2:
							{
								strFind = strAvatar;
							}
							break;
						case 3:
							{
								sprintf(szFind, "%d", nLevel);
								strFind = szFind;
							}
							break;
						case 4:
							{
								sprintf(szFind, "%d", nMoney);
								strFind = szFind;
							}
							break;
					}

					break;
				}
			}
			
			return strFind;
		}
		*/

		std::ifstream& operator >> (std::ifstream& ifs)
		{
			Json::Reader reader;
			Json::Value root;
			if(!reader.parse(ifs, root, false))
			{
				return ifs;
			}
			
			for(unsigned int i = 0; i < root.size(); ++i)
			{
			/*	nUID = root[i]["UID"].asUInt();
				strAvatar = root[i]["AVATAR"].asString();
				nLevel = root[i]["LEVEL"].asUInt();
				nMoney = root[i]["MONEY"].asUInt();*/
			}

			return ifs;
		}

		std::ofstream& operator << (std::ofstream& ofs)
		{
			Json::FastWriter writer;
			Json::Value root;
			Json::Value person;
			
			/*person["UID"] = (unsigned int)nUID;
			person["MONEY"] = (unsigned int)nMoney;
			person["LEVEL"] = (unsigned int)nLevel;
			person["AVATAR"] = strAvatar;
			
			root.append(person);*/

			ofs << writer.write(root);

			return ofs;
		}

	};

	template<class T = Simple_Info>
	class CSimpleDBFile : public Atlas::CNoCopy
	{
	public:
		CSimpleDBFile(_U32 nUID,  std::string& strFile)
		{
			//m_content.nUID = nUID;
			m_nUID = nUID;
			m_strFile = strFile;

			assert(nUID && !strFile.empty());
			std::ifstream ifs;

			std::string strPath;
			strPath += AtlasBaseDir();

			char szFile[256];
			sprintf_s(szFile, 256, "\\%s\\%s_%d.json", AtlasGameName(), m_strFile.c_str(), nUID);
			strPath += szFile;

			ifs.open(strPath, std::ios_base::in);
			m_bReady = ifs.is_open();
			if(m_bReady)
			{
				//ifs >> m_content;
				m_content.operator>>(ifs);
			}

			ifs.close();
		}

		virtual ~CSimpleDBFile()
		{

		}

		bool IsExist()
		{
			return m_bReady;
		}

		bool Generate()
		{
			return Save();
		}

		bool Save()
		{
			std::ofstream ofs;
			std::string strPath;
			strPath += AtlasBaseDir();

			char szFile[256];
			sprintf_s(szFile, 256, "\\%s\\%s_%d.json", AtlasGameName(), m_strFile.c_str(), m_nUID);
			strPath += szFile;

			//ofs.open(strPath, std::ios_base::out|std::ios_base::app);
			ofs.open(strPath, std::ios_base::out|std::ios_base::trunc);
			//ofs << m_content;
			m_content.operator<<(ofs);
			ofs.close();

			return true;
		}

		T& GetContent()
		{
			return m_content;
		}

		std::string GetValueByKey(std::string& strKey, std::string strDefault = "")
		{
			std::string strData = m_content.GetData(strKey);
			if(!strData.empty())
			{
				return strData;
			}

			return strDefault;
		}

		bool UpdateValue(std::string& strKey, std::string strDefault = "")
		{
			//m_content.Update(strKey, strDefault);
			return true;
		}

	private:
		T m_content;
		_U32 m_nUID;
		bool m_bReady;
		std::string m_strFile;
	};

}
#endif //_SIMPLE_DBFILE_H__