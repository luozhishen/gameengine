#include <ZionBase.h>
#include <AsyncSockIO.h>
#include <AsyncRPC.h>

#include "ServerApp.h"
#include "ServerBase.h"
#include "ServerUtils.h"
#include "NodeServer.h"

#include "ServerRpc.s.h"
#include "ServerRpc.c.h"

#define MAX_NODE_OBJECT 30000
#define TIEMR_PERIOD 2000
#define MAX_CONNS 10000

namespace Zion
{

	static CObjectManager<CNodeClient, MAX_NODE_OBJECT> _global_node_object_manager(&CNodeClient::GetNNDX);
	template<>
	CNodeClient* CObjectLocker<CNodeClient>::Lock(_U64 ndx)
	{
		return _global_node_object_manager.Lock(ndx);
	}

	template<>
	void CObjectLocker<CNodeClient>::Unlock(_U64 ndx)
	{
		CNodeClient* pClient = _global_node_object_manager.GetObject(ndx);
		if(pClient && pClient->GetState()==CNodeClient::STATE_DISCONNECTED) delete pClient;
		_global_node_object_manager.Unlock(ndx);
	}

	void CALLBACK NodeTimerProc(PVOID lpParam, BOOLEAN TimerOrWaitFired)
	{
		((CNodeServer*)lpParam)->Tick();
	}
	
	static CNodeServer* _global_node_map[256] = { NULL };
	CNodeServer::CNodeServer(CServerApp* pServerApp, _U32 nodeid) : CServerBase(pServerApp), m_nNodeID(nodeid)
	{
		ZION_ASSERT(nodeid<sizeof(_global_node_map)/sizeof(_global_node_map[0]));
		ZION_ASSERT(!_global_node_map[nodeid]);
		_global_node_map[nodeid] = this;
		NodeRpc_Server_Register();
		
		m_hTimerQueue = CreateTimerQueue();
		ZION_ASSERT(m_hTimerQueue != NULL );

		BOOL ret = CreateTimerQueueTimer(&m_hTimer, m_hTimerQueue, NodeTimerProc, this, 100, TIEMR_PERIOD, 0);
		ZION_ASSERT(ret);
		
		m_hClusterServer = GetRPCServer("127.0.0.1:1982");
		m_nConnCount = 0;
	}

	CNodeServer::~CNodeServer()
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
	}

	void CNodeServer::OnClusterConnect(HSERVER hCluster, _U64 cndx, _U32 nodeseq, _U32 len, const _U8* data)
	{
		_U64 nNNDX = _global_node_object_manager.Alloc();
		if(nNNDX==-1) return;
		CNodeClient* pClient = NewNodeClient(nNNDX, cndx, hCluster);
		pClient->ClusterConnect(nodeseq, len, data);
		_global_node_object_manager.Unlock(nNNDX);
	}

	void CNodeServer::OnSessionAck(CNodeClient* pClient, HSERVER hSession, _U64 sndx)
	{
		pClient->SessionAck(hSession, sndx);
		A_ATOM_INC(&m_nConnCount);
	}

	void CNodeServer::OnClusterData(CNodeClient* pClient, _U16 code, _U32 len, const _U8* data)
	{
		_U16 iid = code & 0xff;
 		_U16 fid = code >> 8;
		m_Stubs[iid](pClient, fid, len, data);
	}

	void CNodeServer::OnClusterDisconnect(CNodeClient* pClient, _U32 nodeseq)
	{
		pClient->ClusterDisconnect(nodeseq);
		A_ATOM_DEC(&m_nConnCount);
	}

	void CNodeServer::RegisterStub(_U16 stubid, StubProc proc)
	{
		if(stubid>=sizeof(m_Stubs)/sizeof(m_Stubs[0])) return;
		m_Stubs[stubid] = proc;
	}

	void CNodeServer::Tick()
	{
		SOCK_ADDR& sockInfo = GetServerApp()->GetRPCAddrPort();
		WORKLOAD_INFO info;
		info.ip = sockInfo.ip;
		info.port = sockInfo.port;
		info.capacity = MAX_CONNS;
		info.workload = m_nConnCount;
		info.workload = 0;

		if(m_hClusterServer)
			CRPC_SetNodeWorkload(m_hClusterServer, info, m_nNodeID);
	}

	CNodeClient::CNodeClient(CNodeServer* pServer, _U64 nndx, _U64 cndx, HSERVER hCluster) : m_pServer(pServer)
	{
		_global_node_object_manager.BindObject(nndx, this);

		m_nState = STATE_CONNECT;
		m_hCluster = hCluster;
		m_hSession = NULL;
		m_nNNDX = nndx;
		m_nCNDX = cndx;
		m_nSNDX = (_U64)-1;
		m_ConnectData.data = NULL;
	}

	CNodeClient::~CNodeClient()
	{
		if(m_ConnectData.data) delete [] m_ConnectData.data;
		_global_node_object_manager.UnbindObject(m_nNNDX, this);
	}

	void CNodeClient::Disconnect()
	{
		ZION_ASSERT(m_nState==STATE_DISCONNECTING);
		if(m_nState==STATE_DISCONNECTING)
		{
			m_nState = STATE_DISCONNECTED;
			CRPC_NodeDisconnect(m_hCluster, m_nCNDX, m_pServer->GetNodeID(), m_DisconnectData.nodeseq);
		}
	}

	void CNodeClient::OnConnected(_U32 len, const _U8* data)
	{
	}

	void CNodeClient::OnDisconnected()
	{
		Disconnect();
	}

	void CNodeClient::SendData(_U16 code, _U32 len, const _U8* data)
	{
		ZION_ASSERT(m_nSNDX!=-1 && m_hSession!=NULL);
		if(m_nSNDX==-1 || m_hSession==NULL) return;
		SRPC_ForwardUserData(m_hSession, m_nSNDX, code, len, data);
	}
	void CNodeClient::ClusterConnect(_U32 nodeseq, _U32 len, const _U8* data)
	{
		m_nState = STATE_SESSION_ACK;
		CRPC_NodeConnect(m_hCluster, m_nCNDX, m_pServer->GetNodeID(), nodeseq, m_nNNDX);
		if(m_ConnectData.data) delete [] m_ConnectData.data;
		if(len>0)
		{
			m_ConnectData.data = ZION_NEW _U8[len];
			memcpy(m_ConnectData.data, data, len);
		}
		m_ConnectData.len = len;
	}

	void CNodeClient::SessionAck(HSERVER hSession, _U64 sndx)
	{
		m_nState = STATE_COMPLETE;
		m_hSession = hSession;
		m_nSNDX = sndx;
		OnConnected(m_ConnectData.len, m_ConnectData.data);
		if(m_ConnectData.data) delete [] m_ConnectData.data;
		m_ConnectData.data = NULL;
	}

	void CNodeClient::ClusterDisconnect(_U32 nodeseq)
	{
		m_nState = STATE_DISCONNECTING;
		m_DisconnectData.nodeseq = nodeseq;
		OnDisconnected();
	}

}

void NRPC_Connect(Zion::HCLIENT hClient, _U64 cndx, _U32 nodeid, _U32 nodeseq, _U32 len, const _U8* data)
{
	Zion::SLog("%s", __FUNCTION__);

	Zion::HSERVER hServer = Zion::GetRPCServer(hClient);
	
	ZION_ASSERT(hServer);
	if(!hServer) return;

	ZION_ASSERT(nodeid<sizeof(Zion::_global_node_map)/sizeof(Zion::_global_node_map[0]));
	if(nodeid>=sizeof(Zion::_global_node_map)/sizeof(Zion::_global_node_map[0])) return;
	ZION_ASSERT(Zion::_global_node_map[nodeid]);
	if(!Zion::_global_node_map[nodeid]) return;
	Zion::_global_node_map[nodeid]->OnClusterConnect(hServer, cndx, nodeseq, len, data);
}

void NRPC_SessionAck(Zion::HCLIENT hClient, _U64 nndx, _U64 sndx)
{
	Zion::SLog("%s", __FUNCTION__);

	Zion::CObjectLocker<Zion::CNodeClient> Locker(nndx);
	if(!Locker.GetObject()) return;
	Locker.GetObject()->GetServer()->OnSessionAck(Locker.GetObject(), Zion::GetRPCServer(hClient), sndx);
}

void NRPC_Disconnect(Zion::HCLIENT hClient, _U64 nndx, _U32 nodeseq)
{
	Zion::SLog("%s", __FUNCTION__);

	Zion::CObjectLocker<Zion::CNodeClient> Locker(nndx);
	if(!Locker.GetObject()) return;
	Locker.GetObject()->GetServer()->OnClusterDisconnect(Locker.GetObject(), nodeseq);
}

void NRPC_OnUserData(Zion::HCLIENT hClient, _U64 nndx, _U16 code, _U32 len, const _U8* data)
{
	Zion::SLog("%s", __FUNCTION__);

	Zion::CObjectLocker<Zion::CNodeClient> Locker(nndx);
	if(!Locker.GetObject()) return;
	Locker.GetObject()->GetServer()->OnClusterData(Locker.GetObject(), code, len, data);
}

void NRPC_OnForwardEvent(Zion::HCLIENT hClient, _U64 nndx, _U16 code, _U32 len, const _U8* data)
{
	Zion::SLog("%s", __FUNCTION__);

	Zion::CObjectLocker<Zion::CNodeClient> Locker(nndx);
	if(!Locker.GetObject()) return;
//	Locker.GetObject()->OnEvent(code, len, data);
}
