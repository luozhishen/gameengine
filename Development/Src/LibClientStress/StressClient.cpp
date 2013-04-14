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
		pClient->_OnLoginDone.connect(this, &CStressClient::OnLoginDone);
		pClient->_OnLoginFailed.connect(this, &CStressClient::OnLoginFailed);
		pClient->_OnDisconnected.connect(this, &CStressClient::OnDisconnected);
		pClient->_OnData.connect(this, &CStressClient::OnData);
		pClient->SetLogCallback(std::tr1::bind(&CStressClient::OnLogMessage, this, std::tr1::placeholders::_1));
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

	const Atlas::String CStressClient::GetTitle()
	{
		Atlas::String ret;
		A_MUTEX_LOCK(&m_pClient->m_mtxClient);
		ret = m_Title;
		A_MUTEX_UNLOCK(&m_pClient->m_mtxClient);
		return ret;
	}
	
	const Atlas::String CStressClient::GetInfo()
	{
		Atlas::String ret;
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

		Atlas::Map<Atlas::String, CStressCase*>::iterator i;
		for(i=m_Cases.begin(); i!=m_Cases.end(); i++)
		{
			i->second->OnTick();
		}

		A_MUTEX_UNLOCK(&m_pClient->m_mtxClient);
	}

	void CStressClient::Login()
	{
		if(m_pClient->GetState()==Atlas::CClient::STATE_NA || m_pClient->GetState()==Atlas::CClient::STATE_FAILED)
		{
			if(m_pClient->LoginForStress(m_nIndex))
			{
				SetTitle("Logining...");
			}
		}
	}

	void CStressClient::Logout()
	{
		m_pClient->Logout();
	}

	void CStressClient::OnLoginDone()
	{
		SetTitle("Login Done");
		if(!CClientApp::GetDefault()->IsThread())
			_OnLoginDone(m_nIndex);
	}

	void CStressClient::OnLoginFailed()
	{
		SetTitle("Login Failed");
		if(!CClientApp::GetDefault()->IsThread())
			_OnLoginFailed(m_nIndex);
	}

	void CStressClient::OnDisconnected()
	{
		SetTitle("Disconnect");
		if(!CClientApp::GetDefault()->IsThread())
			_OnDisconnected(m_nIndex);
	}

	void CStressClient::OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		if(!CClientApp::GetDefault()->IsThread())
			_OnData(m_nIndex, iid, fid, len, data);
	}

	void CStressClient::OnLogMessage(const char* msg)
	{
		if(!CClientApp::GetDefault()->IsThread())
			_OnLogMessage(m_nIndex, msg);
	}

	bool CStressClient::IsExistCase(const char* name)
	{
		Atlas::Map<Atlas::String, CStressCase*>::iterator i;
		i = m_Cases.find(name);
		return i!=m_Cases.end();
	}

	CStressCase* CStressClient::GetStressCase(const char* name)
	{
		Atlas::Map<Atlas::String, CStressCase*>::iterator i;
		i = m_Cases.find(name);
		if(i==m_Cases.end()) return NULL;
		return i->second;
	}

	void CStressClient::GetStressCases( Atlas::Set<CStressCase*>& cases )
	{
		cases.clear();
		Atlas::Map<Atlas::String, CStressCase*>::iterator i;
		for(i=m_Cases.begin(); i!=m_Cases.end(); i++)
		{
			cases.insert(i->second);
		}
	}

	CStressCase* CStressClient::NewStressCase(const char* name)
	{
		CStressCase* pCase = NULL;

		A_MUTEX_LOCK(&m_pClient->m_mtxClient);

		Atlas::Map<Atlas::String, CStressCase*>::iterator i;
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
		Atlas::Map<Atlas::String, CStressCase*>::iterator i;
		i = m_Cases.find(pCase->GetName());
		ATLAS_ASSERT(i==m_Cases.end());
		if(i!=m_Cases.end()) return;
		m_Cases[pCase->GetName()] = pCase;
		pCase->Attach(this);
	}

	void CStressClient::CaseDetach(CStressCase* pCase)
	{
		Atlas::Map<Atlas::String, CStressCase*>::iterator i;
		i = m_Cases.find(pCase->GetName());
		ATLAS_ASSERT(i!=m_Cases.end());
		if(i==m_Cases.end()) return;
		pCase->Detach(this);
		m_Cases.erase(i);		
	}

}
