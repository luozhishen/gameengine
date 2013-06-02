#ifndef __STRESS_LOADER_H__
#define __STRESS_LOADER_H__

namespace Zion
{

	class CStressCase;

	class CStressLoader
	{
		struct Script_Case
		{
			Zion::String case_name;
			Zion::String param1;
			Zion::String param2;
			Zion::String param3;
		};

		struct Stress_Case
		{
			_U32 count;
			Zion::Array<Script_Case> scripts;
		};

		struct Server_Param
		{
			Zion::String key;
			Zion::String value;
		};

		typedef Zion::Array<Server_Param> SERVER_PARAMS;
		typedef Zion::Array<Stress_Case> SERVER_CASES;
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
		Zion::String m_strPath;
	};

}

#endif
