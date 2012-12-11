#include <AtlasBase.h>
#include <AtlasCommon.h>
#include "ServerApp.h"
#include "ServerBase.h"
#include "ServerUtils.h"
#include "SessionServer.h"

#include "ServerRpc.s.h"
#include "ServerRpc.c.h"

#include "LoginServer.h"

#define MAX_CONNS			30000
#define TOKE_LEN            256
#define TIMER_PERIOD        2000

namespace Atlas
{

	static bool SS_ON_CONNECT(HCONNECT hConn);
	static void SS_ON_DISCONNECT(HCONNECT hConn);
	static void SS_ON_DATA(HCONNECT hConn, _U32 len, const _U8* data);

	static CObjectManager<CLoginClient, MAX_CONNS> _global_login_object_manager(&CLoginClient::GetLNDX);
	static CLoginServer* _global_login_server_inst = NULL;

	template<>
	CLoginClient* CObjectLocker<CLoginClient>::Lock(_U64 ndx)
	{
		return _global_login_object_manager.Lock(ndx);
	}

	template<>
	void CObjectLocker<CLoginClient>::Unlock(_U64 ndx)
	{
		CLoginClient* pClient = _global_login_object_manager.GetObject(ndx);
		if(pClient && pClient->NeedDelete()) delete pClient;
		_global_login_object_manager.Unlock(ndx);
	}

	void CALLBACK LoginTimerProc(PVOID lpParam, BOOLEAN TimerOrWaitFired)
	{
		((CLoginServer*)lpParam)->Tick();
	}

	CLoginServer::CLoginServer(CServerApp* pServerApp) : CServerBase(pServerApp)
	{
		//STR2ADDR("127.0.0.1:1978", m_saEndPoint);
		m_saEndPoint = pServerApp->GetListenAddrPort();

		m_hPool = AllocIoBufferPool(LOGIN_IORECVBUF_SIZE, LOGIN_IOSENDBUF_SIZE, 0, 0);
		m_hEp = NULL;
		m_nConnCount = 0;
		m_hDefaultCluster = NULL;
		if(_global_login_server_inst == NULL)
		{
			_global_login_server_inst = this;
		}

		A_MUTEX_INIT(&m_mtxWorkLoad);
	}
	
	CLoginServer::~CLoginServer()
	{
		A_MUTEX_DESTROY(&m_mtxWorkLoad);
		FreeIoBufferPool(m_hPool);
	}

	CLoginClient* CLoginServer::NewLoginClient(HCONNECT hConn, _U64 nLNDX)
	{
		return ATLAS_NEW CLoginClient(this, hConn, nLNDX);
	}
		
	HSERVER CLoginServer::GetDefaultCluster()
	{
		ATLAS_ASSERT(m_hDefaultCluster);
		return m_hDefaultCluster;
	}

	bool CLoginServer::Start()
	{
		if(!CServerBase::Start()) return false;

		SOCK_ADDR& cluster = GetServerApp()->GetClusterRpcEP();
		m_hDefaultCluster = GetRPCServer(cluster.ip, cluster.port);

		ASOCKIO_HANDLER handler =
		{
			SS_ON_CONNECT,
			SS_ON_DISCONNECT,
			SS_ON_DATA,
			NULL
		};

		m_hEp = NewEP(m_saEndPoint, handler, m_hPool, GetServerApp()->GetIOWorkers(), this);
		ATLAS_ASSERT(m_hEp);
		if(!m_hEp) return false;
		StartEP(m_hEp);

		m_hTimerQueue = CreateTimerQueue();
		ATLAS_ASSERT(m_hTimerQueue != NULL );
		
		BOOL ret = CreateTimerQueueTimer(&m_hTimer, m_hTimerQueue, LoginTimerProc, this, 100, TIMER_PERIOD, 0);
		ATLAS_ASSERT(ret);

		return true;
	}

	void CLoginServer::Stop()
	{
		if(m_hTimer)
		{
			HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			DeleteTimerQueueTimer(m_hTimerQueue, m_hTimer, hEvent);
			while(WaitForSingleObject(hEvent, INFINITE)!=WAIT_OBJECT_0)
			{
				SwitchToThread();
			}
			CloseHandle(hEvent);
		}

		DeleteTimerQueue(m_hTimerQueue);
		m_hTimerQueue = NULL;

		ATLAS_ASSERT(m_hEp);
		StopEP(m_hEp);
		while(IsRunning(m_hEp))
		{
			SwitchToThread();
		}

		ATLAS_ASSERT(m_hEp);
		StopEP(m_hEp);
		while(IsRunning(m_hEp))
		{
			SwitchToThread();
		}

		CLoginClient* pConn;
		for(_U32 i=0; i<MAX_CONNS; i++)
		{
			pConn = _global_login_object_manager.Lock((_U64)i, false);
			if(!pConn) continue;
			pConn->KickUser();
			_global_login_object_manager.Unlock((_U64)i);
		}

		while(m_nConnCount) SwitchToThread();

		DelEP(m_hEp);
		m_hEp = NULL;

		CServerBase::Stop();
	}

	bool CLoginServer::OnConnected(HCONNECT hConn)
	{
		_U64 nLNDX = _global_login_object_manager.Alloc();
		if(nLNDX==-1) return false;
		CLoginClient* pConn = NewLoginClient(hConn, nLNDX);
		SetKey(hConn, pConn);
		pConn->OnRawConnect();
		_global_login_object_manager.Unlock(nLNDX);
		A_ATOM_INC(&m_nConnCount);
		return true;
	}
	void CLoginServer::OnData(HCONNECT hConn, _U32 len, const _U8* data)
	{
		CLoginClient* pConn = (CLoginClient*)KeyOf(hConn);
		ATLAS_ASSERT(pConn);
		if(!pConn) return;
		_global_login_object_manager.Lock(pConn->GetLNDX());
		pConn->OnRawData(len, data);
		_global_login_object_manager.Unlock(pConn->GetLNDX());
	}

	void CLoginServer::OnDisconnected(HCONNECT hConn)
	{
		CLoginClient* pConn = (CLoginClient*)KeyOf(hConn);
		ATLAS_ASSERT(pConn);
		if(!pConn) return;
		_U64 lndx = pConn->GetLNDX();
		_global_login_object_manager.Lock(lndx);
		pConn->OnRawDisconnect();
		if(pConn->NeedDelete()) delete pConn;
		_global_login_object_manager.Unlock(lndx);

		A_ATOM_DEC(&m_nConnCount);
	}

	bool CLoginServer::BindUID(_U32 uid, _U64 lndx)
	{
		return m_mapUID.Bind(uid, lndx)==-1;
	}

	bool CLoginServer::Unbind(_U32 uid, _U64 lndx)
	{
		return m_mapUID.Unbind(uid, lndx);
	}
		
	bool CLoginServer::GetSession(_U32 uid, SOCK_ADDR& sa)
	{
		if(m_WorkLoads.empty()) 
			return false;
		A_MUTEX_LOCK(&m_mtxWorkLoad);
		_U32 ndx = 0;
		for(_U32 i = 0; i < m_WorkLoads.size(); ++i)
		{
			if(m_WorkLoads[i].workload < m_WorkLoads[ndx].workload)
			{
				ndx = i;
			}
		}

		++m_WorkLoads[ndx].workload;

		sock_addr(&sa, m_WorkLoads[ndx].ip, m_WorkLoads[ndx].port);
		A_MUTEX_UNLOCK(&m_mtxWorkLoad);

		return true;
	}

	void CLoginServer::UpdateWorkLoadInfo(const WORKLOAD_INFO* info_list, _U32 count)
	{
		A_MUTEX_LOCK(&m_mtxWorkLoad);
		if(m_WorkLoads.capacity() < count)
		{
			m_WorkLoads.resize(count);
		}

		for(_U32 i = 0; i < count; ++i)
		{
			m_WorkLoads[i] = info_list[i];
		}
		A_MUTEX_UNLOCK(&m_mtxWorkLoad);
	}

	void CLoginServer::Tick()
	{
		if(m_hDefaultCluster)
			CRPC_GetSessionWorkload(m_hDefaultCluster);
	}

	bool SS_ON_CONNECT(HCONNECT hConn)
	{
		Atlas::SLog("%s", __FUNCTION__);
		HTCPEP hep = HepOf(hConn);
		ATLAS_ASSERT(hep);
		CLoginServer* pServer = (CLoginServer*)KeyOf(hep);
		ATLAS_ASSERT(pServer);
		pServer->OnConnected(hConn);
		return true;
	}

	void SS_ON_DISCONNECT(HCONNECT hConn)
	{
		Atlas::SLog("%s", __FUNCTION__);
		CLoginClient* pConn = (CLoginClient*)KeyOf(hConn);
		ATLAS_ASSERT(pConn);
		pConn->GetServer()->OnDisconnected(hConn);
		CloseConn(hConn);
	}

	void SS_ON_DATA(HCONNECT hConn, _U32 len, const _U8* data)
	{
		Atlas::SLog("%s", __FUNCTION__);
		CLoginClient* pConn = (CLoginClient*)KeyOf(hConn);
		ATLAS_ASSERT(pConn);
		pConn->GetServer()->OnData(hConn, len, data);
	}

	CLoginClient::CLoginClient(CLoginServer* pServer, HCONNECT hConn, _U64 nLNDX, _U32 recvsize)
	{
		_global_login_object_manager.BindObject(nLNDX, this);
		m_pServer = pServer;
		m_hConnect = hConn;
		m_nLNDX = nLNDX;

		m_nSNDX = (_U64)-1;

		if(recvsize)
		{
			m_pRecvBuff = (_U8*)ATLAS_ALLOC(recvsize);
			m_nRecvBuffLen = 0;
			m_nRecvBuffSize = recvsize;
		}
		else
		{
			m_pRecvBuff = NULL;
		}
	}
	
	CLoginClient::~CLoginClient()
	{
		if(m_nUID!=-1)
		{
			//m_pServer->Unbind(m_nUID, m_nLNDX);
		}

		if(m_pRecvBuff) ATLAS_FREE(m_pRecvBuff);
		_global_login_object_manager.UnbindObject(m_nLNDX, this);
	}
	
	void CLoginClient::OnAuthPassed(_U32 nUID, _U32 len, const _U8* pToken)
	{
		SetUID(nUID, "");

		//return session add and new token
		SOCK_ADDR sa;
		if(!m_pServer->GetSession(nUID, sa))
		{
			KickUser();
			return;
		}

		//strNewToken = "127.0.0.1:1980";
		_U8 sendBuf[TOKE_LEN];
		DDL::MemoryWriter Buf(sendBuf, TOKE_LEN);
		Buf.Write(sa);
		Buf.Write(nUID);
		Buf.WriteData(pToken, len);
		Send(1, 0, Buf);
	}

	void CLoginClient::OnData(_U16 id, _U32 len, const _U8* data)
	{
		//_U16 iid = id & 0xff;
		//_U16 fid = id >> 8;

		SetUID((_U32)id, (char*)data);
	}

	void CLoginClient::OnDisconnected()
	{
		//m_pServer->Unbind(m_nUID, m_nLNDX);
	}

	void CLoginClient::SendRawData(_U16 id, _U32 len, const _U8* data)
	{
		if(!m_hConnect) return;

		HIOPOOL pool = HIOPoolOf(m_hConnect);
		_U32 blen = GetIoBufferSize(pool);
		_U8* buf = LockIoBuffer(pool);
		*((_U16*)(buf+0)) = (_U16)len + sizeof(id);
		*((_U16*)(buf+2)) = id;
		if(len<=blen-sizeof(_U16)*2)
		{
			memcpy(buf+4, data, len);
			Atlas::Send(m_hConnect, len+sizeof(_U16)*2, buf);
			return;
		}
		else
		{
			Atlas::Send(m_hConnect, blen, buf);
			len -= blen - sizeof(_U16)*2;
			data += blen - sizeof(_U16)*2;
		}

		for(;;)
		{
			buf = LockIoBuffer(pool);
			if(len<=blen)
			{
				memcpy(buf, data, len);
				Atlas::Send(m_hConnect, len, buf);
				return;
			}
			else
			{
				memcpy(buf, data, blen);
				Atlas::Send(m_hConnect, blen, buf);
				len -= blen;
				buf += blen;
			}
		}
	}

	bool CLoginClient::NeedDelete()
	{
		if(m_hConnect) return false;
		return true;
	}

	void CLoginClient::UserLoginDone(_U64 nCNDX)
	{
		m_nCNDX = nCNDX;
	}

	void CLoginClient::KickUser()
	{
		if(!m_hConnect) return;
		Disconnect(m_hConnect);
	}

	void CLoginClient::OnRawConnect()
	{

	}

	void CLoginClient::OnRawData(_U32 len, const _U8* data)
	{
		_U16 pkglen;
		while(len>0)
		{
			_U32 copylen = len;
			if(m_nRecvBuffLen+copylen>m_nRecvBuffSize) copylen = m_nRecvBuffSize - m_nRecvBuffLen;
			memcpy(m_pRecvBuff+m_nRecvBuffLen, data, copylen);
			m_nRecvBuffLen += copylen;
			len -= copylen;
			data = data + copylen;
			while(m_nRecvBuffLen>=sizeof(pkglen))
			{
				memcpy(&pkglen, m_pRecvBuff, sizeof(pkglen));

				if(pkglen>m_nRecvBuffSize-sizeof(pkglen))
				{
					KickUser();
					return;
				}

				if(pkglen+sizeof(pkglen)>m_nRecvBuffLen)
					break;

				if(m_nUID!=-1)
				{
					_U32 uid;
					if(pkglen<sizeof(uid))
					{
						KickUser();
						return;
					}
					memcpy(&uid, m_pRecvBuff+sizeof(pkglen), sizeof(uid));
					OnAuthPassed(uid, pkglen-sizeof(uid), m_pRecvBuff+sizeof(pkglen)+sizeof(uid));
				}
				
				_U32 ulen = pkglen + sizeof(pkglen);
				m_nRecvBuffLen -= ulen;
				memmove(m_pRecvBuff, m_pRecvBuff+ulen, m_nRecvBuffLen);
			}
		}
	}

	void CLoginClient::OnRawDisconnect()
	{
		//m_pServer->Unbind(m_nUID, m_nLNDX);
		m_hConnect = NULL;
	}

	void CLoginClient::SetUID(_U32 uid, const char* token)
	{
		m_nUID = uid;
	}

}

void CRPC_SessionWorkLoadResult(Atlas::HSERVER hServer, const WORKLOAD_INFO* info, _U32 count)
{
	Atlas::SLog("%s", __FUNCTION__);
	Atlas::_global_login_server_inst->UpdateWorkLoadInfo(info, count);
}

void CRPC_NodeWorkLoadResult(Atlas::HSERVER hServer, _U32 type, const WORKLOAD_INFO* info, _U32 count)
{
	Atlas::SLog("%s", __FUNCTION__);
}