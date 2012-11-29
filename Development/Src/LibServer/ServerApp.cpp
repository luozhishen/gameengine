#include <AtlasBase.h>
#include <AtlasCommon.h>

#include "ServerApp.h"
#include "ServerBase.h"
#include "ServerTask.h"

namespace Atlas
{
	class CServerTaskManagerSingleton : public CServerTaskManager { };
	static CServerTaskManagerSingleton _GlobalServerTaskManager;

	CServerTaskManager::CServerTaskManager()
	{
		m_hWorkers = CreateWorkers(0);
	}

	CServerTaskManager::~CServerTaskManager()
	{
		KillWorkers(m_hWorkers);
	}

	CServerTaskManager& CServerTaskManager::GetSingileton()
	{
		return _GlobalServerTaskManager;
	}

	void CServerTaskManager::SetWorkerCount(_U32 nCount)
	{
		_U32 nCurCount = CountOf(m_hWorkers);
		while(nCurCount<nCount)
		{
			IncWorker(m_hWorkers);
			nCurCount++;
		}
	}

	CServerApp::CServerApp(_U32 nIOThreadCount)
	{
		m_hIOWorkers = CreateWorkers(nIOThreadCount);
		ADDR(m_saRPC, 0, 0);
		ADDR(m_saListen, 0, 0);
	}

	CServerApp::~CServerApp()
	{
		KillWorkers(m_hIOWorkers);
	}

	_U64 CServerApp::Queue(CServerTask* pTask)
	{
		return 0;
	}

	_U64 CServerApp::Queue(HSERVER hServer, CServerTask* pTask)
	{
		return 0;
	}

	bool CServerApp::Cancel(_U64 nTaskID)
	{
		return true;
	}

	void CServerApp::Run()
	{
		StartRPCService(m_saRPC.ip, m_saRPC.port, 20);
		InitApp();
		getchar();
		StopRPCService();
		FiniApp();
	}

	void CServerApp::SetListenAddrPort(const _STR str)
	{
		Atlas::STR2ADDR(str, m_saListen);
	}

	SOCKADDR& CServerApp::GetListenAddrPort()
	{
		ATLAS_ASSERT(m_saListen.ip&&m_saListen.port);
		return m_saListen;
	}

	SOCKADDR& CServerApp::GetRPCAddrPort()
	{
		return m_saRPC;
	}

	void CServerApp::SetRPCAddrPort(_U32 ip, _U16 port)
	{
		ADDR(m_saRPC, ip, port);
	}

	void CServerApp::SetRPCAddrPort(const _STR str)
	{
		Atlas::STR2ADDR(str, m_saRPC);
	}

	void CServerApp::SetClusterAddrPort(const _STR str)
	{
		Atlas::STR2ADDR(str, m_saCluster);
	}

	SOCKADDR& CServerApp::GetClusterAddrPort()
	{
		return m_saCluster;
	}

	void CServerApp::InitApp()
	{
		Atlas::InitDDLStub();
		Atlas::InitContentObjects();
		Atlas::InitLiveObjects();
		Atlas::InitSyncObjects();
	}

	void CServerApp::FiniApp()
	{
	}

}
