#include <AtlasBase.h>
#include <AtlasCommon.h>
#include <AtlasClient.h>
#include <AtlasClientApp.h>

#include "StressCase.h"
#include "StressClient.h"
#include "StressManager.h"

namespace Atlas
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
		std::map<std::string, CStressCase*>::iterator i;
		for(i=m_mapCases.begin(); i!=m_mapCases.end(); i++)
		{
			delete i->second;
		}
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
		std::map<_U32, CStressClient*>::iterator i;
		_U32 count = 1;
		while(count>0)
		{
			count = 0;
			A_MUTEX_LOCK(&m_mtxLocker);
			for(i=m_mapClients.begin(); i!=m_mapClients.end(); i++)
			{
				i->second->Logout();
				if(i->second->GetClient()->GetClientState()!=CClient::CLIENT_NA) count++;
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
	}

	_U32 CStressManager::NewClient()
	{
		_U32 index = A_ATOM_INC(&m_nIDSeq);
		CStressClient* pClient = ATLAS_NEW CStressClient(index, CClientApp::GetDefault()->NewClient());
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
		std::map<_U32, CStressClient*>::iterator i;
		A_MUTEX_LOCK(&m_mtxLocker);
		i = m_mapClients.find(id);
		pClient = (i==m_mapClients.end()?NULL:i->second);
		A_MUTEX_UNLOCK(&m_mtxLocker);
		return pClient;
	}

	void CStressManager::GetClients(std::vector<_U32>& clients)
	{
		clients.clear();
		std::map<_U32, CStressClient*>::iterator i;
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
		
		std::map<_U32, CStressClient*>::iterator it = m_mapClients.begin();
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

	void CStressManager::GetCases(std::vector<std::string>& cases)
	{
		std::map<std::string, CStressCase*>::iterator i;
		cases.clear();
		for(i=m_mapCases.begin(); i!=m_mapCases.end(); i++)
		{
			cases.push_back(i->first);
		}
	}

	CStressCase* CStressManager::CreateCase(const char* name)
	{
		std::map<std::string, CStressCase*>::iterator i;
		i = m_mapCases.find(name);
		if(i==m_mapCases.end()) return NULL;
		return i->second->Create();
	}

	const DDLReflect::STRUCT_INFO* CStressManager::GetCaseConfigType(const char* name)
	{
		std::map<std::string, CStressCase*>::iterator i;
		i = m_mapCases.find(name);
		if(i==m_mapCases.end()) return NULL;
		return i->second->GetConfigType();
	}

	bool CStressManager::GetCaseConfigDefault(const char* name, void* data, _U32 size)
	{
		std::map<std::string, CStressCase*>::iterator i;
		i = m_mapCases.find(name);
		if(i==m_mapCases.end()) return NULL;
		return i->second->GetConfig(data, size);
	}
	
	void CStressManager::SetUIDStart(_U32 nUIDtart)
	{
		m_nIDSeq = nUIDtart;
	}

}
