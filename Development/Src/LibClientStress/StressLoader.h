#ifndef __STRESS_LOADER_H__
#define __STRESS_LOADER_H__

namespace Atlas
{

	class CStressCase;

	class CStressLoader
	{
		struct Script_Case
		{
			Atlas::String case_name;
			Atlas::String param1;
			Atlas::String param2;
			Atlas::String param3;
		};

		struct Stress_Case
		{
			_U32 count;
			Atlas::Vector<Script_Case> scripts;
		};

		struct Server_Param
		{
			Atlas::String key;
			Atlas::String value;
		};

		typedef Atlas::Vector<Server_Param> SERVER_PARAMS;
		typedef Atlas::Vector<Stress_Case> SERVER_CASES;
	public:
		CStressLoader();
		~CStressLoader();

		bool RunScript(const char* script);
		bool LoadScript(const char* szScript);
		void SaveSvrParam();

		sigslot::signal1<_U32>					_OnNewClient;
		sigslot::signal2<_U32, CStressCase*>	_OnNewCase;

	private:
		bool ScriptImpl();
		void SetSvrParam();

	private:
		SERVER_CASES m_ServerCases;
		SERVER_PARAMS m_ServerParams;
		Atlas::String m_strPath;
	};

}

#endif
