#ifndef __ATLAS_STRESS_CLIENT__
#define __ATLAS_STRESS_CLIENT__

#include <string>
#include <map>
#include <set>

namespace Atlas
{

	class CClient;
	class CStressManager;
	class CStressClient;
	class CStressCase;

	class CStressClient : public sigslot::has_slots<>
	{
		friend class CStressCase;
	public:
		CStressClient(_U32 nIndex, Atlas::CClient* pClient);
		~CStressClient();

		_U32 GetIndex() { return m_nIndex; }
		CClient* GetClient() { return m_pClient; }
		const std::string GetTitle();
		const std::string GetInfo();

		void SetTitle(const char* title);
		void SetInfo(const char* info);

		void Tick();

		void Login();
		void Logout();

		void OnConnectFailed();
		void OnConnected();
		void OnLoginDone();
		void OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data);
		void OnDisconnect();

		bool IsExistCase(const char* name);
		CStressCase* GetStressCase(const char* name);
		void GetStressCases(std::set<CStressCase*>& cases);
		CStressCase* NewStressCase(const char* name);

		sigslot::signal1<_U32>									_OnConnectFailed;
		sigslot::signal1<_U32>									_OnConnected;
		sigslot::signal1<_U32>									_OnLoginDone;
		sigslot::signal5<_U32, _U16, _U16, _U32, const _U8*>	_OnData;
		sigslot::signal1<_U32>									_OnDisconnect;

	protected:
		void CaseAttach(CStressCase* pCase);
		void CaseDetach(CStressCase* pCase);

	private:
		_U32 m_nIndex;
		CClient* m_pClient;
		std::string m_Title;
		std::string m_Info;
		std::map<std::string, CStressCase*> m_Cases;
		HANDLE m_hTimer;
	};

}

#endif
