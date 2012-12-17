#ifndef __ATLAS_STRESS_MANAGER__
#define __ATLAS_STRESS_MANAGER__

#include <string>
#include <map>
#include <set>

namespace Atlas
{

	class CStressManager;
	class CStressClient;
	class CStressCase;

	class CStressManager
	{
		friend class CStressClient;
	private:
		CStressManager();
		~CStressManager();

	public:
		typedef CStressCase* (*CREATE_PROC)();
		static CStressManager& Get();

		void DisconnectAll();

		_U32 NewClient();
		_U32 GetClientCount();
		CStressClient* GetClient(_U32 id);
		void GetClients(std::vector<_U32>& clients);
		void UpdateAll();

		void RegisterCase(CStressCase* pCase);
		void GetCases(std::vector<std::string>& cases);
		CStressCase* CreateCase(const char* name);
		const DDLReflect::STRUCT_INFO* GetCaseConfigType(const char* name);
		bool GetCaseConfigDefault(const char* name, void* data, _U32 size);

	protected:
		HANDLE GetTimerQueue() { return m_hTimerQueue; }

	private:
		HANDLE m_hTimerQueue;
		std::map<_U32, CStressClient*> m_mapClients;
		std::map<std::string, CStressCase*> m_mapCases;
		A_MUTEX m_mtxLocker;
		_U32 m_nIDSeq;
	};

}

#endif
