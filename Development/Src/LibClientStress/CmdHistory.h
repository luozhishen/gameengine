#ifndef _CMD_HISTORY_H__
#define _CMD_HISTORY_H__

#include <string>
#include <vector>

namespace Atlas
{
	class CmdHistory
	{
	public:
		typedef std::vector<std::string> CMD_SET;
	public:
		CmdHistory(const char* path = "./");
		~CmdHistory();

		void FindSimilarCmd(std::string& cmd, CMD_SET& ret);
		int GetHistoryNum();
		CMD_SET& GetHistorySet();
		std::string GetHistory(int index);
		void AddCmd(std::string& cmd);
	private:
		CMD_SET m_cmds;
		std::string m_path;
	};
}

#endif //_CMD_HISTORY_H__