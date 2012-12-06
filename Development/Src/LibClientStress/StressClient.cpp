#include <AtlasBase.h>
#include <AtlasCommon.h>
#include <AtlasClient.h>
#include <AtlasClientApp.h>

#include "StressCase.h"
#include "StressClient.h"
#include "StressManager.h"

#define TIMER_PERIOD 100

namespace Atlas
{

	void CALLBACK StressTimerProc(PVOID lpParam, BOOLEAN TimerOrWaitFired)
	{
		((CStressClient*)lpParam)->Tick();
	}
	
	CStressClient::CStressClient(_U32 nIndex, Atlas::CClient* pClient)
	{
		m_nIndex = nIndex;
		m_pClient = pClient;
		pClient->_OnConnectFailed.connect(this, &CStressClient::OnConnectFailed);
		pClient->_OnConnected.connect(this, &CStressClient::OnConnected);
		pClient->_OnLoginDone.connect(this, &CStressClient::OnLoginDone);
		pClient->_OnData.connect(this, &CStressClient::OnData);
		pClient->_OnDisconnect.connect(this, &CStressClient::OnDisconnect);
		SetTitle("NA");

		m_hTimer = NULL;
		if(pClient->GetClientApp()->IsThread())
		{
			CreateTimerQueueTimer(&m_hTimer, CStressManager::Get().GetTimerQueue(), StressTimerProc, this, 1, TIMER_PERIOD, 0);
		}
	}

	CStressClient::~CStressClient()
	{
		if(m_hTimer)
		{
			HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			DeleteTimerQueueTimer(CStressManager::Get().GetTimerQueue(), m_hTimer, hEvent);
			while(WaitForSingleObject(hEvent, INFINITE)!=WAIT_OBJECT_0)
			{
				SwitchToThread();
			}
			CloseHandle(hEvent);
			m_hTimer = NULL;
		}

		while(!m_Cases.empty())
		{
			CStressCase* pCase = m_Cases.begin()->second;
			CaseDetach(pCase);
			delete pCase;
		}
		delete m_pClient;
		m_pClient = NULL;
	}

	const std::string CStressClient::GetTitle()
	{
		std::string ret;
		A_MUTEX_LOCK(&m_pClient->m_mtxClient);
		ret = m_Title;
		A_MUTEX_UNLOCK(&m_pClient->m_mtxClient);
		return ret;
	}
	
	const std::string CStressClient::GetInfo()
	{
		std::string ret;
		A_MUTEX_LOCK(&m_pClient->m_mtxClient);
		ret = m_Info;
		A_MUTEX_UNLOCK(&m_pClient->m_mtxClient);
		return ret;
	}

	void CStressClient::SetTitle(const char* title)
	{
		char t[1000];
		sprintf(t, "%03d : %s", GetIndex(), title);
		m_Title = t;
	}

	void CStressClient::SetInfo(const char* info)
	{
		m_Info = info;
	}

	void CStressClient::Tick()
	{
		A_MUTEX_LOCK(&m_pClient->m_mtxClient);

		std::map<std::string, CStressCase*>::iterator i;
		for(i=m_Cases.begin(); i!=m_Cases.end(); i++)
		{
			i->second->OnTick();
		}

		A_MUTEX_UNLOCK(&m_pClient->m_mtxClient);
	}

	void CStressClient::Login()
	{
		if(m_pClient->GetClientState()==Atlas::CClient::STATE_NA)
		{
			m_pClient->LoginForStress(NULL, m_nIndex);
		}
	}

	void CStressClient::Logout()
	{
		if(m_pClient->GetClientState() != Atlas::CClient::STATE_NA)
		{
			//if(m_hTimer)
			//{
			//	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			//	while(!DeleteTimerQueueTimer(CStressManager::Get().GetTimerQueue(), m_hTimer, hEvent))
			//	{
			//		if(GetLastError() == ERROR_IO_PENDING)
			//		{
			//			Sleep(0);
			//			break;
			//		}
			//		OutputDebugString("in loop\n");
			//	}

			//	while(WaitForSingleObject(hEvent, INFINITE) != WAIT_OBJECT_0)
			//	{
			//		SwitchToThread();
			//	}
			//	CloseHandle(hEvent);
			//	m_hTimer = NULL;
			//}
		
			m_pClient->Logout();
			//m_pClient->OnDisconnect();
		}
	}

	void CStressClient::OnConnectFailed()
	{
		SetTitle("Connect Failed");
		if(!CClientApp::GetDefault()->IsThread())
			_OnConnectFailed(m_nIndex);
	}

	void CStressClient::OnConnected()
	{
		SetTitle("Connected");
		if(!CClientApp::GetDefault()->IsThread())
			_OnConnected(m_nIndex);
	}

	void CStressClient::OnLoginDone()
	{
		SetTitle("Login Done");
		if(!CClientApp::GetDefault()->IsThread())
			_OnLoginDone(m_nIndex);
	}

	void CStressClient::OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		if(!CClientApp::GetDefault()->IsThread())
			_OnData(m_nIndex, iid, fid, len, data);
	}

	void CStressClient::OnDisconnect()
	{
		SetTitle("Disconnect");
		if(!CClientApp::GetDefault()->IsThread())
			_OnDisconnect(m_nIndex);
	}

	bool CStressClient::IsExistCase(const char* name)
	{
		std::map<std::string, CStressCase*>::iterator i;
		i = m_Cases.find(name);
		return i!=m_Cases.end();
	}

	CStressCase* CStressClient::GetStressCase(const char* name)
	{
		std::map<std::string, CStressCase*>::iterator i;
		i = m_Cases.find(name);
		if(i==m_Cases.end()) return NULL;
		return i->second;
	}

	void CStressClient::GetStressCases( std::set<CStressCase*>& cases )
	{
		cases.clear();
		std::map<std::string, CStressCase*>::iterator i;
		for(i=m_Cases.begin(); i!=m_Cases.end(); i++)
		{
			cases.insert(i->second);
		}
	}

	CStressCase* CStressClient::NewStressCase(const char* name)
	{
		CStressCase* pCase = NULL;

		A_MUTEX_LOCK(&m_pClient->m_mtxClient);

		std::map<std::string, CStressCase*>::iterator i;
		i = m_Cases.find(name);
		if(i==m_Cases.end())
		{
			pCase = CStressManager::Get().CreateCase(name);
			if(pCase)
			{
				CaseAttach(pCase);
			}
		}

		A_MUTEX_UNLOCK(&m_pClient->m_mtxClient);

		return pCase;
	}

	void CStressClient::CaseAttach(CStressCase* pCase)
	{
		std::map<std::string, CStressCase*>::iterator i;
		i = m_Cases.find(pCase->GetName());
		ATLAS_ASSERT(i==m_Cases.end());
		if(i!=m_Cases.end()) return;
		m_Cases[pCase->GetName()] = pCase;
		pCase->Attach(this);
	}

	void CStressClient::CaseDetach(CStressCase* pCase)
	{
		std::map<std::string, CStressCase*>::iterator i;
		i = m_Cases.find(pCase->GetName());
		ATLAS_ASSERT(i!=m_Cases.end());
		if(i==m_Cases.end()) return;
		pCase->Detach(this);
		m_Cases.erase(i);		
	}

}
