#include <ZionBase.h>
#include <ZionCommon.h>
#include "ServerApp.h"
#include "ServerBase.h"
#include "ServerUtils.h"
#include "SessionServer.h"

#include "ServerRpc.s.h"
#include "ServerRpc.c.h"

#define MAX_CONNS			30000
#define TIEMR_PERIOD        2000
namespace Zion
{

	static bool SS_ON_CONNECT(HCONNECT hConn);
	static void SS_ON_DISCONNECT(HCONNECT hConn);
	static void SS_ON_DATA(HCONNECT hConn, _U32 len, const _U8* data);
	static CObjectManager<CSessionClient, MAX_CONNS> _global_session_object_manager(&CSessionClient::GetSNDX);
	template<>
	CSessionClient* CObjectLocker<CSessionClient>::Lock(_U64 ndx)
	{
		return _global_session_object_manager.Lock(ndx);
	}

	template<>
	void CObjectLocker<CSessionClient>::Unlock(_U64 ndx)
	{
		CSessionClient* pClient = _global_session_object_manager.GetObject(ndx);
		if(pClient && pClient->NeedDelete()) delete pClient;
		_global_session_object_manager.Unlock(ndx);
	}

	void CALLBACK SessionTimerProc(PVOID lpParam, BOOLEAN TimerOrWaitFired)
	{
		((CSessionServer*)lpParam)->Tick();
	}

	CSessionServer::CSessionServer(CServerApp* pServerApp) : CServerBase(pServerApp)
	{
		m_saEndPoint = pServerApp->GetListenAddrPort();
	
		m_hPool = AllocIoBufferPool(SESSION_IORECVBUF_SIZE, SESSION_IOSENDBUF_SIZE, 0, 0);
		m_hEp = NULL;
		m_nConnCount = 0;
		m_hDefaultCluster = NULL;
		
		m_hTimerQueue = NULL;
		m_hTimer = NULL;

		SessionRpc_Server_Register();
	}

	CSessionServer::~CSessionServer()
	{
		FreeIoBufferPool(m_hPool);
	}

	CSessionClient* CSessionServer::NewSessionClient(HCONNECT hConn, _U64 nSNDX)
	{
		return ATLAS_NEW CSessionClient(this, hConn, nSNDX);
	}

	HSERVER CSessionServer::GetDefaultCluster()
	{
		ATLAS_ASSERT(m_hDefaultCluster);
		return m_hDefaultCluster;
		//return GetLocalRPCServer() ? GetLocalRPCServer() : m_hDefaultCluster;
	}

	bool CSessionServer::Start()
	{
		if(!CServerBase::Start()) return false;
		
		//m_hDefaultCluster = GetRPCServer("192.168.0.15:1982");
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
		
		BOOL ret = CreateTimerQueueTimer(&m_hTimer, m_hTimerQueue, SessionTimerProc, this, 100, TIEMR_PERIOD, 0);
		ATLAS_ASSERT(ret);

		return true;
	}

	void CSessionServer::Stop()
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

		CSessionClient* pConn;
		for(_U32 i=0; i<MAX_CONNS; i++)
		{
			pConn = _global_session_object_manager.Lock((_U64)i, false);
			if(!pConn) continue;
			pConn->KickUser();
			_global_session_object_manager.Unlock((_U64)i);
		}

		while(m_nConnCount) SwitchToThread();

		DelEP(m_hEp);
		m_hEp = NULL;
		
		CServerBase::Stop();
	}

	bool CSessionServer::OnConnected(HCONNECT hConn)
	{
		_U64 nSNDX = _global_session_object_manager.Alloc();
		if(nSNDX==-1) return false;
		CSessionClient* pConn = NewSessionClient(hConn, nSNDX);
		SetKey(hConn, pConn);
		pConn->OnRawConnect();
		_global_session_object_manager.Unlock(nSNDX);
		A_ATOM_INC(&m_nConnCount);
		return true;
	}

	void CSessionServer::OnData(HCONNECT hConn, _U32 len, const _U8* data)
	{
		CSessionClient* pConn = (CSessionClient*)KeyOf(hConn);
		ATLAS_ASSERT(pConn);
		if(!pConn) return;
		_global_session_object_manager.Lock(pConn->GetSNDX());
		pConn->OnRawData(len, data);
		_global_session_object_manager.Unlock(pConn->GetSNDX());
	}

	void CSessionServer::OnDisconnected(HCONNECT hConn)
	{
		CSessionClient* pConn = (CSessionClient*)KeyOf(hConn);
		ATLAS_ASSERT(pConn);
		if(!pConn) return;
		_U64 sndx = pConn->GetSNDX();
		_global_session_object_manager.Lock(sndx);
		pConn->OnRawDisconnect();
		if(pConn->NeedDelete()) delete pConn;
		_global_session_object_manager.Unlock(sndx);

		A_ATOM_DEC(&m_nConnCount);
	}

	bool CSessionServer::BindUID(_U32 uid, _U64 sndx)
	{
		return m_mapUID.Bind(uid, sndx)==-1;
	}

	bool CSessionServer::Unbind(_U32 uid, _U64 sndx)
	{
		return m_mapUID.Unbind(uid, sndx);
	}

	void CSessionServer::Tick()
	{
		WORKLOAD_INFO info;
		info.ip = m_saEndPoint.ip;
		info.port = m_saEndPoint.port;
		info.capacity = MAX_CONNS;
		info.workload = m_nConnCount;

		if(m_hDefaultCluster)
			CRPC_SetSessionWorkload(m_hDefaultCluster, info);
	}

	CSessionClient::CSessionClient(CSessionServer* pServer, HCONNECT hConn, _U64 nSNDX, _U32 recvsize) : m_pServer(pServer)
	{
		_global_session_object_manager.BindObject(nSNDX, this);
		memset(m_NodeStubs, 0, sizeof(m_NodeStubs));

		m_hConnect = hConn;
		m_nUID = (_U32)-1;
		m_hCluster = NULL;
		m_pRecvBuff = 0;
		m_nSNDX = nSNDX;
		m_nCNDX = (_U64)-1;
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

	CSessionClient::~CSessionClient()
	{
		if(m_nUID!=-1)
		{
			m_pServer->Unbind(m_nUID, m_nSNDX);
		}

		if(m_pRecvBuff) ATLAS_FREE(m_pRecvBuff);
		_global_session_object_manager.UnbindObject(m_nSNDX, this);
	}

	void CSessionClient::OnAuthPassed(_U32 nUID, const char* pToken)
	{
		if(!m_pServer->BindUID(nUID, m_nSNDX))
		{
			KickUser();
			return;
		}
		SendRawData(0, 0, NULL);
		SetUID(nUID, pToken);
		ClusterLogin(GetServer()->GetDefaultCluster());
	}

	void CSessionClient::OnData(_U16 id, _U32 len, const _U8* data)
	{
		_U16 iid = id & 0xff;
		_U16 fid = id >> 8;
		const DDLReflect::CLASS_INFO* pClassInfo;
		_U32 nodeid;
		if(!GetServerStub(iid, pClassInfo, &nodeid) || fid>=pClassInfo->fcount)
		{
			KickUser();
			return;
		}
		if(m_NodeStubs[nodeid].hServer)
		{
			NRPC_OnUserData(m_NodeStubs[nodeid].hServer, m_NodeStubs[nodeid].nNNDX, id, len, data);
		}
	}

	void CSessionClient::OnDisconnected()
	{
		ClusterLogout();
	}

	void CSessionClient::SendRawData(_U16 id, _U32 len, const _U8* data)
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
			Zion::Send(m_hConnect, len+sizeof(_U16)*2, buf);
			return;
		}
		else
		{
			Zion::Send(m_hConnect, blen, buf);
			len -= blen - sizeof(_U16)*2;
			data += blen - sizeof(_U16)*2;
		}

		for(;;)
		{
			buf = LockIoBuffer(pool);
			if(len<=blen)
			{
				memcpy(buf, data, len);
				Zion::Send(m_hConnect, len, buf);
				return;
			}
			else
			{
				memcpy(buf, data, blen);
				Zion::Send(m_hConnect, blen, buf);
				len -= blen;
				buf += blen;
			}
		}
	}

	bool CSessionClient::NeedDelete()
	{
		if(m_hConnect) return false;
		return true;
/*
		for(_U32 i=0; i<sizeof(m_NodeStubs)/sizeof(m_NodeStubs[0]); i++)
		{
			if(m_NodeStubs[i].hServer) return false;
		}
		return true;
*/
	}

	void CSessionClient::UserLoginDone(_U64 nCNDX)
	{
		m_nCNDX = nCNDX;
	}

	void CSessionClient::KickUser()
	{
		if(!m_hConnect) return;
		Disconnect(m_hConnect);
	}

	void CSessionClient::NodeConnect(_U32 nodeid, HSERVER hServer, _U64 nndx)
	{
		ATLAS_ASSERT(nodeid<sizeof(m_NodeStubs)/sizeof(m_NodeStubs[0]));
		if(nodeid>=sizeof(m_NodeStubs)/sizeof(m_NodeStubs[0])) return;
		ATLAS_ASSERT(!m_NodeStubs[nodeid].hServer);
		m_NodeStubs[nodeid].hServer = hServer;
		m_NodeStubs[nodeid].nNNDX = nndx;
		NRPC_SessionAck(hServer, nndx, m_nSNDX);
	}

	void CSessionClient::NodeDisconnect(_U32 nodeid)
	{
		ATLAS_ASSERT(nodeid<sizeof(m_NodeStubs)/sizeof(m_NodeStubs[0]));
		if(nodeid>=sizeof(m_NodeStubs)/sizeof(m_NodeStubs[0])) return;
		ATLAS_ASSERT(m_NodeStubs[nodeid].hServer);
		m_NodeStubs[nodeid].hServer = NULL;
		m_NodeStubs[nodeid].nNNDX = (_U64)-1;
	}

	bool CSessionClient::GetNode(_U32 nodeid, HSERVER& hServer, _U64 nndx)
	{
		if(nodeid>=sizeof(m_NodeStubs)/sizeof(m_NodeStubs[0])) return false;
		if(!m_NodeStubs[nodeid].hServer) return false;
		hServer = m_NodeStubs[nodeid].hServer;
		nndx = m_NodeStubs[nodeid].nNNDX;
		return true;
	}

	void CSessionClient::OnRawConnect()
	{
		ATLAS_ASSERT(m_pRecvBuff);
		m_nRecvBuffLen = 0;
		OnConnected();
	}

	void CSessionClient::OnRawData(_U32 len, const _U8* data)
	{
		_U16 pkglen;
		_U16 id;
		while(len>0)
		{
			_U32 copylen = len;
			if(m_nRecvBuffLen+copylen>m_nRecvBuffSize) copylen = m_nRecvBuffSize - m_nRecvBuffLen;
			memcpy(m_pRecvBuff+m_nRecvBuffLen, data, copylen);
			m_nRecvBuffLen += copylen;
			len -= copylen;
			data = data + copylen;
			while(m_nRecvBuffLen>=sizeof(pkglen)+sizeof(id))
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
					if(pkglen<sizeof(id))
					{
						KickUser();
						return;
					}
					memcpy(&id, m_pRecvBuff+sizeof(pkglen), sizeof(id));
					OnData(id, pkglen-sizeof(id), m_pRecvBuff+sizeof(pkglen)+sizeof(id));
				}
				else
				{
					if(pkglen<sizeof(m_nUID))
					{
						KickUser();
						return;
					}
					_U32 nUID;
					memcpy(&nUID, m_pRecvBuff+sizeof(pkglen), sizeof(nUID));
					if(nUID==-1)
					{
						KickUser();
						return;
					}
					m_pRecvBuff[sizeof(pkglen)+pkglen-1] = '\0';
					OnAuthPassed(nUID, (const char*)m_pRecvBuff+sizeof(pkglen)+sizeof(nUID));
				}

				_U32 ulen = pkglen + sizeof(pkglen);
				m_nRecvBuffLen -= ulen;
				memmove(m_pRecvBuff, m_pRecvBuff+ulen, m_nRecvBuffLen);
			}
		}
	}

	void CSessionClient::OnRawDisconnect()
	{
		m_hConnect = NULL;
		OnDisconnected();
	}

	void CSessionClient::SetUID(_U32 uid, const char* token)
	{
		m_nUID = uid;
		strcpy(m_sToken, token);
	}

	void CSessionClient::ClusterLogin(HSERVER hServer)
	{
		ATLAS_ASSERT(!m_hCluster);
		m_hCluster = hServer;
		CRPC_UserLogin(m_hCluster, m_nUID, m_sToken, m_nSNDX);
	}

	void CSessionClient::ClusterLogout()
	{
		if(m_hCluster)
		{
			CRPC_UserLogout(m_hCluster, m_nUID, m_nSNDX);
			m_hCluster = NULL;
		}
	}

	bool SS_ON_CONNECT(HCONNECT hConn)
	{
		HTCPEP hep = HepOf(hConn);
		ATLAS_ASSERT(hep);
		CSessionServer* pServer = (CSessionServer*)KeyOf(hep);
		ATLAS_ASSERT(pServer);
		pServer->OnConnected(hConn);
		return true;
	}

	void SS_ON_DISCONNECT(HCONNECT hConn)
	{
		CSessionClient* pConn = (CSessionClient*)KeyOf(hConn);
		ATLAS_ASSERT(pConn);
		pConn->GetServer()->OnDisconnected(hConn);
		CloseConn(hConn);
	}

	void SS_ON_DATA(HCONNECT hConn, _U32 len, const _U8* data)
	{
		CSessionClient* pConn = (CSessionClient*)KeyOf(hConn);
		ATLAS_ASSERT(pConn);
		pConn->GetServer()->OnData(hConn, len, data);
	}

}

void SRPC_UserLoginDone(Zion::HCLIENT hClient, _U64 sndx, _U64 cndx)
{
	Zion::SLog("%s", __FUNCTION__);

	Zion::CObjectLocker<Zion::CSessionClient> Locker(sndx);
	if(!Locker.GetObject()) return;
	Locker.GetObject()->UserLoginDone(cndx);
}

void SRPC_KickUser(Zion::HCLIENT hClient, _U64 sndx)
{
	Zion::SLog("%s", __FUNCTION__);

	Zion::CObjectLocker<Zion::CSessionClient> Locker(sndx);
	if(!Locker.GetObject()) return;
	Locker.GetObject()->KickUser();
}

void SRPC_ForwardUserData(Zion::HCLIENT hClient, _U64 sndx, _U16 code, _U32 len, const _U8* data)
{
	Zion::SLog("%s", __FUNCTION__);

	Zion::CObjectLocker<Zion::CSessionClient> Locker(sndx);
	Zion::CSessionClient* pClient = Locker.GetObject();
	if(!pClient) return;
	pClient->SendRawData(code, len, data);
}

void SRPC_SetNode(Zion::HCLIENT hClient, _U64 sndx, _U32 nodeid, _U64 nndx, _U32 ip, _U16 port)
{
	Zion::SLog("%s", __FUNCTION__);

	Zion::CObjectLocker<Zion::CSessionClient> Locker(sndx);
	if(!Locker.GetObject()) return;
	if(nndx==-1)
	{
		Locker.GetObject()->NodeDisconnect(nodeid);
	}
	else
	{
		Locker.GetObject()->NodeConnect(nodeid, Zion::GetRPCServer(ip, port), nndx);
	}
}
