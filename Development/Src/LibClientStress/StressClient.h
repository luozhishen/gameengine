#ifndef __ZION_STRESS_CLIENT__
#define __ZION_STRESS_CLIENT__

#include <string>
#include <map>
#include <set>

namespace Zion
{

	class CClient;
	class CStressManager;
	class CStressClient;
	class CStressCase;

	class CStressClient : public sigslot::has_slots<>
	{
		friend class CStressCase;
	public:
		CStressClient(_U32 nIndex, Zion::CClient* pClient);
		~CStressClient();

		_U32 GetIndex() { return m_nIndex; }
		CClient* GetClient() { return m_pClient; }
		const Zion::String GetTitle();
		const Zion::String GetInfo();

		void SetTitle(const char* title);
		void SetInfo(const char* info);

		void Tick();

		void Login();
		void Logout();

		void OnLoginDone();
		void OnLoginFailed();
		void OnDisconnected();
		void OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data);
		void OnLogMessage(const char* msg);

		bool IsExistCase(const char* name);
		CStressCase* GetStressCase(const char* name);
		void GetStressCases(Zion::Set<CStressCase*>& cases);
		CStressCase* NewStressCase(const char* name);

		sigslot::signal1<_U32>									_OnLoginDone;
		sigslot::signal1<_U32>									_OnLoginFailed;
		sigslot::signal5<_U32, _U16, _U16, _U32, const _U8*>	_OnData;
		sigslot::signal1<_U32>									_OnDisconnected;
		sigslot::signal2<_U32, const char*>						_OnLogMessage;

	protected:
		void CaseAttach(CStressCase* pCase);
		void CaseDetach(CStressCase* pCase);

	private:
		_U32 m_nIndex;
		CClient* m_pClient;
		Zion::String m_Title;
		Zion::String m_Info;
		Zion::Map<Zion::String, CStressCase*> m_Cases;
		HANDLE m_hTimer;
	};

}

#endif
