#include <stdarg.h>

#include "AtlasBase.h"
#include "AtlasCommon.h"
#include "AtlasClientApp.h"
#include "AtlasClient.h"

#define CLIENT_IORECVBUF_SIZE		(1024)
#define CLIENT_IOSENDBUF_SIZE		(1024)

namespace Atlas
{

	static CClientApp* __global_client_app = NULL;
	CClientApp::CClientApp(bool bThread, _U32 nThreadCount)
	{
		ATLAS_ASSERT(!__global_client_app);
		__global_client_app = this;

		m_bThread = bThread;
		if(!m_bThread) nThreadCount = 1;
		m_hWorkers = CreateWorkers(nThreadCount);
		m_hPool = AllocIoBufferPool(CLIENT_IORECVBUF_SIZE, CLIENT_IOSENDBUF_SIZE, 0, 0);
		A_MUTEX_INIT(&m_mtxQueue);
		m_bEnableTick = true;
	}

	CClientApp::~CClientApp()
	{
		if(m_hPool)
		{
			FreeIoBufferPool(m_hPool);
			m_hPool = NULL;
		}
		if(m_hWorkers)
		{
			KillWorkers(m_hWorkers);
			m_hWorkers = NULL;
		}
		A_MUTEX_DESTROY(&m_mtxQueue);
	}

	void CClientApp::SetParam(const char* name, const char* value)
	{
		m_Params[name] = value;
	}
	
	const char* CClientApp::GetParam(const char* name, const char* default_value)
	{
		std::map<std::string, std::string>::iterator i;
		i = m_Params.find(name);
		if(i==m_Params.end()) return default_value;
		return i->second.c_str();
	}
	
	std::map<std::string, std::string>& CClientApp::GetParams()
	{
		return m_Params;
	}

	bool CClientApp::LoadParams()
	{
		return true;
	}

	bool CClientApp::SaveParams()
	{
		return true;
	}

	CClientApp* CClientApp::GetDefault()
	{
		return __global_client_app;
	}

	void CClientApp::InitApp()
	{
		InitDDLStub();
		InitContentObjects();
		InitLiveObjects();
	}

	CClient* CClientApp::NewClient()
	{
		return ATLAS_NEW CClient(this);
	}

	void CClientApp::EnableTick(bool bEnable)
	{
		m_bEnableTick = bEnable;
	}

	bool CClientApp::Tick(_U32 nTime)
	{
		if(m_bThread) return false;

		bool bQuit = false, bRet = false;
		CLIENTAPP_ITEM i;
		while(!bQuit)
		{
			A_MUTEX_LOCK(&m_mtxQueue);
			if(m_Queue.size()==0)
			{
				bQuit = true;
				A_MUTEX_UNLOCK(&m_mtxQueue);
				break;
			}
			i = m_Queue.front();
			m_Queue.pop_front();
			bRet = true;
			A_MUTEX_UNLOCK(&m_mtxQueue);

			switch(i.nCode)
			{
			case 0:
				i.pClient->OnRawConnect(i.hConn);
				break;
			case 1:
				i.pClient->OnRawDisconnect();
				CloseConn(i.hConn);
				break;
			case 2:
				i.pClient->OnRawData(i.len, i.data);
				delete [] i.data;
				break;
			case 3:
				i.pClient->OnRawConnectFailed();
				break;
			case 4:
				((CClientTask*)i.hConn)->DoTask(i.pClient);
				delete ((CClientTask*)i.hConn);
			}
		}

		return bRet;
	}

	bool CClientApp::QueueTask(CClientTask* pTask, CClient* pClient)
	{
		if(m_bThread)
		{
		}
		else
		{
			CLIENTAPP_ITEM i = { 4, pClient, (HCONNECT)pTask, 0, NULL };
			A_MUTEX_LOCK(&m_mtxQueue);
			m_Queue.push_back(i);
			A_MUTEX_UNLOCK(&m_mtxQueue);
		}
		return true;
	}

	void CClientApp::OnConnect(CClient* pClient, HCONNECT hConn)
	{
		if(m_bThread)
		{
			A_MUTEX_LOCK(&pClient->m_mtxClient);
			pClient->OnRawConnect(hConn);
			A_MUTEX_UNLOCK(&pClient->m_mtxClient);
		}
		else
		{
			CLIENTAPP_ITEM i = { 0, pClient, hConn, 0, NULL };
			A_MUTEX_LOCK(&m_mtxQueue);
			m_Queue.push_back(i);
			A_MUTEX_UNLOCK(&m_mtxQueue);
		}
	}

	void CClientApp::OnDisconnect(CClient* pClient, HCONNECT hConn)
	{
		if(m_bThread)
		{
			A_MUTEX_LOCK(&pClient->m_mtxClient);
			pClient->OnRawDisconnect();
			A_MUTEX_UNLOCK(&pClient->m_mtxClient);
			CloseConn(hConn);
		}
		else
		{
			CLIENTAPP_ITEM i = { 1, pClient, hConn, 0, NULL };
			A_MUTEX_LOCK(&m_mtxQueue);
			m_Queue.push_back(i);
			A_MUTEX_UNLOCK(&m_mtxQueue);
		}
	}

	void CClientApp::OnData(CClient* pClient, _U32 len, const _U8* data)
	{
		if(m_bThread)
		{
			A_MUTEX_LOCK(&pClient->m_mtxClient);
			pClient->OnRawData(len, data);
			A_MUTEX_UNLOCK(&pClient->m_mtxClient);
		}
		else
		{
			CLIENTAPP_ITEM i = { 2, pClient, NULL, len, NULL };
			i.data = (_U8*)ATLAS_ALLOC(len);
			memcpy(i.data, data, len);
			A_MUTEX_LOCK(&m_mtxQueue);
			m_Queue.push_back(i);
			A_MUTEX_UNLOCK(&m_mtxQueue);
		}
	}

	void CClientApp::OnConnectFailed(CClient* pClient)
	{
		if(m_bThread)
		{
			A_MUTEX_LOCK(&pClient->m_mtxClient);
			pClient->OnRawConnectFailed();
			A_MUTEX_UNLOCK(&pClient->m_mtxClient);
		}
		else
		{
			CLIENTAPP_ITEM i = { 3, pClient, NULL, 0, NULL };
			A_MUTEX_LOCK(&m_mtxQueue);
			m_Queue.push_back(i);
			A_MUTEX_UNLOCK(&m_mtxQueue);
		}
	}

}
