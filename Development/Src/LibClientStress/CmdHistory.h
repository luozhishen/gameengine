#ifndef _CMD_HISTORY_H__
#define _CMD_HISTORY_H__

namespace Zion
{
	class CmdHistory
	{
	public:
		typedef Zion::Array<Zion::String> CMD_SET;
	public:
		CmdHistory(const char* path = "./");
		~CmdHistory();

		void FindSimilarCmd(Zion::String& cmd, CMD_SET& ret);
		int GetHistoryNum();
		CMD_SET& GetHistorySet();
		Zion::String GetHistory(int index);
		void AddCmd(Zion::String& cmd);
	private:
		CMD_SET m_cmds;
		Zion::String m_path;
	};
}

#endif //_CMD_HISTORY_H__