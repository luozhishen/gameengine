#include <ZionBase.h>
#include <AsyncSockIO.h>
#include <ZionCommon.h>
#include <ZionClient.h>
#include <ZionClientApp.h>

#include "StressCase.h"
#include "StressClient.h"
#include "StressManager.h"

namespace Zion
{

	CStressManager::CStressManager()
	{
		ASockIOInit();
		A_MUTEX_INIT(&m_mtxLocker);
		m_nIDSeq = 0;
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
		m_hTimerQueue = CreateTimerQueue();
	}

	CStressManager::~CStressManager()
	{
		m_mapCases.clear();

		if(m_hTimerQueue)
		{
			DeleteTimerQueue(m_hTimerQueue);
			m_hTimerQueue = NULL;
		}
		
		ASockIOFini();
		A_MUTEX_DESTROY(&m_mtxLocker);
	}

	CStressManager& CStressManager::Get()
	{
		static CStressManager _singleton;
		return _singleton;
	}

	void CStressManager::DisconnectAll()
	{	
		Zion::Map<_U32, CStressClient*>::iterator i;
		_U32 count = 1;
		while(count>0)
		{
			count = 0;
			A_MUTEX_LOCK(&m_mtxLocker);
			for(i=m_mapClients.begin(); i!=m_mapClients.end(); i++)
			{
				i->second->Logout();
				if(i->second->GetClient()->GetState()!=CClient::STATE_NA && i->second->GetClient()->GetState()!=CClient::STATE_FAILED) count++;
			}
			A_MUTEX_UNLOCK(&m_mtxLocker);
			SwitchToThread();

			if(!CClientApp::GetDefault()->IsThread())
			{
				CClientApp::GetDefault()->Tick();
			}
		}

		for(i=m_mapClients.begin(); i!=m_mapClients.end(); i++)
		{
			delete i->second;
		}
		m_mapClients.clear();
	}

	_U32 CStressManager::NewClient()
	{
		_U32 index = A_ATOM_INC(&m_nIDSeq);
		CStressClient* pClient = ZION_NEW CStressClient(index, CClientApp::GetDefault()->NewClient());
		A_MUTEX_LOCK(&m_mtxLocker);
		m_mapClients[index] = pClient;
		A_MUTEX_UNLOCK(&m_mtxLocker);
		return index;
	}

	_U32 CStressManager::GetClientCount()
	{
		return (_U32)m_mapClients.size();
	}

	CStressClient* CStressManager::GetClient(_U32 id)
	{
		CStressClient* pClient;
		Zion::Map<_U32, CStressClient*>::iterator i;
		A_MUTEX_LOCK(&m_mtxLocker);
		i = m_mapClients.find(id);
		pClient = (i==m_mapClients.end()?NULL:i->second);
		A_MUTEX_UNLOCK(&m_mtxLocker);
		return pClient;
	}

	void CStressManager::GetClients(Zion::Array<_U32>& clients)
	{
		clients.clear();
		Zion::Map<_U32, CStressClient*>::iterator i;
		A_MUTEX_LOCK(&m_mtxLocker);
		for(i=m_mapClients.begin(); i!=m_mapClients.end(); i++)
		{
			clients.push_back(i->first);
		}
		A_MUTEX_UNLOCK(&m_mtxLocker);
	}

	void CStressManager::UpdateAll()
	{
		A_MUTEX_LOCK(&m_mtxLocker);
		
		Zion::Map<_U32, CStressClient*>::iterator it = m_mapClients.begin();
		while(it != m_mapClients.end())
		{
			it->second->Tick();
			++it;
		}

		A_MUTEX_UNLOCK(&m_mtxLocker);
	}

	void CStressManager::RegisterCase(CStressCase* pCase)
	{
		m_mapCases[pCase->GetName()] = pCase;
	}

	void CStressManager::GetCases(Zion::Array<Zion::String>& cases)
	{
		Zion::Map<Zion::String, CStressCase*>::iterator i;
		cases.clear();
		for(i=m_mapCases.begin(); i!=m_mapCases.end(); i++)
		{
			cases.push_back(i->first);
		}
	}

	CStressCase* CStressManager::CreateCase(const char* name)
	{
		Zion::Map<Zion::String, CStressCase*>::iterator i;
		i = m_mapCases.find(name);
		if(i==m_mapCases.end()) return NULL;
		return i->second->Create();
	}

	bool CStressManager::HasCase(const char* name)
	{
		return m_mapCases.find(name)!=m_mapCases.end();
	}

	const DDLReflect::STRUCT_INFO* CStressManager::GetCaseConfigType(const char* name)
	{
		Zion::Map<Zion::String, CStressCase*>::iterator i;
		i = m_mapCases.find(name);
		if(i==m_mapCases.end()) return NULL;
		return i->second->GetConfigType();
	}

	const DDLReflect::STRUCT_INFO* CStressManager::GetCaseStatusType(const char* name)
	{
		Zion::Map<Zion::String, CStressCase*>::iterator i;
		i = m_mapCases.find(name);
		if(i==m_mapCases.end()) return NULL;
		return i->second->GetStatusType();
	}

	bool CStressManager::GetCaseConfigDefault(const char* name, void* data, _U32 size)
	{
		Zion::Map<Zion::String, CStressCase*>::iterator i;
		i = m_mapCases.find(name);
		if(i==m_mapCases.end()) return false;
		if(i->second->GetConfigType()==NULL) return false;
		memcpy(data, i->second->GetConfigData(), (size_t)size);
		return true;
	}
	
}
