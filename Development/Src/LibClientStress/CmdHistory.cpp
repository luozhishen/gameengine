#include "CmdHistory.h"

#include <fstream>
#include <assert.h>
#include <algorithm>

namespace Atlas
{
#define CMD_HISTORY_FILE "ClientConfig\\cmd_history.txt"

	CmdHistory::CmdHistory(const char* path)
	{
		m_path = path;
		m_path += "/";
		m_path += CMD_HISTORY_FILE;

		std::ifstream ifs;
		ifs.open(m_path, std::ios_base::app|std::ios_base::in);
		if(ifs.is_open())
		{
			std::string line;
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
		ofs.open(m_path, std::ios_base::out|std::ios_base::trunc);
		assert(ofs.is_open());

		for(CMD_SET::iterator it = m_cmds.begin();
			it != m_cmds.end(); ++it)
		{
			ofs<<*it;
			ofs<<"\n";
		}

		ofs.close();
	}

	void CmdHistory::FindSimilarCmd( std::string& cmd, CMD_SET& ret)
	{
		for(CMD_SET::iterator it = m_cmds.begin();
			it != m_cmds.end(); ++it)
		{
			if((*it).find(cmd) != std::string::npos)
			{
				ret.push_back((*it));
			}
		}
	}

	CmdHistory::CMD_SET& CmdHistory::GetHistorySet()
	{
		return m_cmds;
	}

	std::string CmdHistory::GetHistory( int index )
	{
		return m_cmds[index];
	}

	void CmdHistory::AddCmd( std::string& cmd )
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