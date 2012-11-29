#ifndef __STRESS_LOADER_H__
#define __STRESS_LOADER_H__

namespace Atlas
{

	class CStressCase;

	class CStressLoader
	{
		struct Script_Case
		{
			std::string case_name;
			std::string param1;
			std::string param2;
			std::string param3;
		};

		struct Stress_Case
		{
			_U32 count;
			std::vector<Script_Case> scripts;
		};

		struct Server_Param
		{
			std::string key;
			std::string value;
		};

		typedef std::vector<Server_Param> SERVER_PARAMS;
		typedef std::vector<Stress_Case> SERVER_CASES;
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
		std::string m_strPath;
	};

}

#endif
