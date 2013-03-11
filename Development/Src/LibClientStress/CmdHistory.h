#ifndef _CMD_HISTORY_H__
#define _CMD_HISTORY_H__

namespace Atlas
{
	class CmdHistory
	{
	public:
		typedef Atlas::Vector<Atlas::String> CMD_SET;
	public:
		CmdHistory(const char* path = "./");
		~CmdHistory();

		void FindSimilarCmd(Atlas::String& cmd, CMD_SET& ret);
		int GetHistoryNum();
		CMD_SET& GetHistorySet();
		Atlas::String GetHistory(int index);
		void AddCmd(Atlas::String& cmd);
	private:
		CMD_SET m_cmds;
		Atlas::String m_path;
	};
}

#endif //_CMD_HISTORY_H__