#include <AtlasSTL.h>
#include "CmdHistory.h"

#include <fstream>
#include <assert.h>
#include <algorithm>

namespace Atlas
{

	CmdHistory::CmdHistory(const char* path)
	{
		m_path = path;
		m_path += "/";
		m_path += "Config\\StressHistory.txt";

		std::ifstream ifs;
		ifs.open(m_path.c_str(), std::ios_base::app|std::ios_base::in);
		if(ifs.is_open())
		{
			Atlas::String line;
			while(getline(ifs, line))
			{
				m_cmds.push_back(line);
			}
			ifs.close();
		}
	}

	CmdHistory::~CmdHistory()
	{
		std::ofstream ofs;
		ofs.open(m_path.c_str(), std::ios_base::out|std::ios_base::trunc);
		if(ofs.is_open())
		{
			for(CMD_SET::iterator it = m_cmds.begin(); it != m_cmds.end(); ++it)
			{
				ofs<<*it<<"\n";
			}

			ofs.close();
		}
	}

	void CmdHistory::FindSimilarCmd( Atlas::String& cmd, CMD_SET& ret)
	{
		for(CMD_SET::iterator it = m_cmds.begin();
			it != m_cmds.end(); ++it)
		{
			if((*it).find(cmd) != Atlas::String::npos)
			{
				ret.push_back((*it));
			}
		}
	}

	CmdHistory::CMD_SET& CmdHistory::GetHistorySet()
	{
		return m_cmds;
	}

	Atlas::String CmdHistory::GetHistory( int index )
	{
		return m_cmds[index];
	}

	void CmdHistory::AddCmd( Atlas::String& cmd )
	{
		//std::unique(m_cmds.begin(), m_cmds.end());
		CMD_SET::iterator it = find(m_cmds.begin(), m_cmds.end(), cmd);
		if(it == m_cmds.end())
		{
			m_cmds.push_back(cmd);
		}
	}

	int CmdHistory::GetHistoryNum()
	{
		return (int)m_cmds.size();
	}
}