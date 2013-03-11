#include <AtlasBase.h>
#include <AtlasCommon.h>
#include <AtlasClient.h>

#include "StressCase.h"
#include "StressClient.h"
#include "StressManager.h"

namespace Atlas
{

	CStressCase::CStressCase(const char* name)
	{
		m_pClient = NULL;
		m_strName = name;
	}

	CStressCase::~CStressCase()
	{
		assert(m_pClient==NULL);
	}

	CClient* CStressCase::GetClient()
	{
		return m_pClient->GetClient();
	}

	bool CStressCase::GetConfig(void* pConfig, _U32 size)
	{
		bool bRet;
		A_MUTEX_LOCK(&m_pClient->GetClient()->m_mtxClient);
		bRet = _GetConfig(pConfig, size);
		A_MUTEX_UNLOCK(&m_pClient->GetClient()->m_mtxClient);
		return bRet;
	}

	bool CStressCase::SetConfig(const void* pConfig, _U32 size)
	{
		bool bRet;
		A_MUTEX_LOCK(&m_pClient->GetClient()->m_mtxClient);
		bRet = _SetConfig(pConfig, size);
		A_MUTEX_UNLOCK(&m_pClient->GetClient()->m_mtxClient);
		return bRet;
	}

	const Atlas::String CStressCase::GetInfo()
	{
		Atlas::String info;
		A_MUTEX_LOCK(&m_pClient->GetClient()->m_mtxClient);
		_GetInfo(info);
		A_MUTEX_UNLOCK(&m_pClient->GetClient()->m_mtxClient);
		return info;
	}

	void CStressCase::Attach(CStressClient* pClient)
	{
		assert(m_pClient==NULL);
		m_pClient = pClient;
		OnAttach();
	}

	void CStressCase::Detach(CStressClient* pClient)
	{
		assert(m_pClient==pClient);
		OnDetach();
		m_pClient = NULL;
	}

}
