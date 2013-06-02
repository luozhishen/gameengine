#include "ZionBase.h"
#include "ServerApp.h"
#include "ServerBase.h"
#include "ServerUtils.h"
#include "ClusterServer.h"

#include "ServerRpc.s.h"
#include "ServerRpc.c.h"

#define MAX_CLIENTS		100000
#define SESSION_WORKLOAD_LIST_LEN 100
#define NODE_WORKLOAD_LIST_LEN 255

namespace Zion
{

	static CObjectManager<CClusterClient, MAX_CLIENTS> _global_cluster_object_manager(&CClusterClient::GetCNDX);
	static CClusterServer* _global_cluster_singleton = NULL;
	template<>
	CClusterClient* CObjectLocker<CClusterClient>::Lock(_U64 ndx)
	{
		return _global_cluster_object_manager.Lock(ndx);
	}

	template<>
	void CObjectLocker<CClusterClient>::Unlock(_U64 ndx)
	{
		CClusterClient* pClient = _global_cluster_object_manager.GetObject(ndx);
		if(pClient && pClient->GetState()==CClusterClient::STATE_DELETE) delete pClient;
		_global_cluster_object_manager.Unlock(ndx);
	}

	CClusterServer::CClusterServer(CServerApp* pServerApp) : CServerBase(pServerApp)
	{
		ZION_ASSERT(!_global_cluster_singleton);
		_global_cluster_singleton = this;
		ClusterRpc_Server_Register();
		A_MUTEX_INIT(&m_mtxWorkLoad);
	}

	CClusterServer::~CClusterServer()
	{
		A_MUTEX_DESTROY(&m_mtxWorkLoad);
	}

	CClusterClient* CClusterServer::NewClusterClient(_U64 nCNDX)
	{
		return ZION_NEW CClusterClient(this, nCNDX);
	}

	void CClusterServer::ClientLogin(HSERVER hServer, _U32 nUID, const char* pToken, _U64 nSNDX)
	{
		_U64 nCNDX = _global_cluster_object_manager.Alloc();
		if(nCNDX==-1)
		{
			SRPC_KickUser(hServer, nSNDX);
			 return;
		}
		CClusterClient* pClient = NewClusterClient(nCNDX);
		if(pClient->BindUser(nUID, pToken, hServer, nSNDX))
		{
			SRPC_UserLoginDone(hServer, nSNDX, pClient->GetCNDX());
			pClient->UserLogin();
		}
		else
		{
			delete pClient;
			SRPC_KickUser(hServer, nSNDX);
		}
		_global_cluster_object_manager.Unlock(nCNDX);
	}

	void CClusterServer::ClientLogout(_U32 nUID, _U64 nSNDX)
	{
		_U64 cndx;
		if(GetCNDXByUID(nUID, cndx))
		{
			Zion::CObjectLocker<Zion::CClusterClient> Locker(cndx);
			if(Locker.GetObject())
			{
				if(Locker.GetObject()->GetSNDX()==nSNDX)
				{
					Locker.GetObject()->UserLogout();
				}
				return;
			}
		}
	}

	bool CClusterServer::GetCNDXByUID(_U32 nUID, _U64& nCNDX)
	{
		return m_mapUID.GetValue(nUID, nCNDX);
	}

	bool CClusterServer::GetCNDXByAID(_U32 nAID, _U64& nCNDX)
	{
		return m_mapUID.GetValue(nAID, nCNDX);
	}

	bool CClusterServer::UIDBindCNDX(_U32 nUID, CClusterClient* pClient)
	{
		return m_mapUID.Bind(nUID, pClient->GetCNDX())==-1;
	}

	bool CClusterServer::AIDBindCNDX(_U32 nAID, CClusterClient* pClient)
	{
		return m_mapAID.Bind(nAID, pClient->GetCNDX())==-1;
	}

	bool CClusterServer::UIDUnbindCNDX(_U32 nUID, CClusterClient* pClient)
	{
		return m_mapUID.Unbind(nUID, pClient->GetCNDX());
	}

	bool CClusterServer::AIDUnbindCNDX(_U32 nAID, CClusterClient* pClient)
	{
		return m_mapAID.Unbind(nAID, pClient->GetCNDX());
	}

	void CClusterServer::UpdateSessionWorkLoad(HSERVER hServer, const WORKLOAD_INFO& WORKLOAD_INFO)
	{
		A_MUTEX_LOCK(&m_mtxWorkLoad);
		Zion::SLog("%s", __FUNCTION__);
		SESSION_WORKLOADS::iterator it = m_WorkLoads.find(hServer);
		if(it != m_WorkLoads.end())
		{
			it->second = WORKLOAD_INFO;
		} 
		else
		{
			m_WorkLoads[hServer] = WORKLOAD_INFO;
		}

		A_MUTEX_UNLOCK(&m_mtxWorkLoad);
	}
	
	_U32 CClusterServer::GetSessionWorkLoads(WORKLOAD_INFO workLoads[])
	{
		A_MUTEX_LOCK(&m_mtxWorkLoad);

		_U32 i = 0;
		SESSION_WORKLOADS::iterator it = m_WorkLoads.begin();
		for(; i < m_WorkLoads.size() && it != m_WorkLoads.end() && i< SESSION_WORKLOAD_LIST_LEN; ++it, ++i)
		{
			workLoads[i] = it->second;
		}

		i = i > SESSION_WORKLOAD_LIST_LEN ? SESSION_WORKLOAD_LIST_LEN : i;
		A_MUTEX_UNLOCK(&m_mtxWorkLoad);

		return i;
	}

	void CClusterServer::NotifyNodeWorkLoad(HSERVER hServer, const WORKLOAD_INFO& info, _U32 type)
	{
		if(type > NODETYPE_MAX)
		{
			return;
		}

		NODE_LOADLIST& loadlist = m_NodeLoads[type];
		NODE_LOADLIST::iterator it;

		for(it = loadlist.begin(); it != loadlist.end(); ++it)
		{
			if(it->first == hServer)
			{
				break;
			}
		}

		if(it == loadlist.end())
		{
			loadlist.push_back(std::make_pair(hServer, info));
		}
		else
		{
			it->second = info;
		}
	}

	_U32 CClusterServer::GetNodeWorkLoad(WORKLOAD_INFO infoes[], _U32 type)
	{
		_U32 i = 0;
		if(type > NODETYPE_MAX)
		{
			return i;
		}

		NODE_LOADLIST& loadlist = m_NodeLoads[type];
		NODE_LOADLIST::iterator it;

		for(it = loadlist.begin(); it != loadlist.end() && i < loadlist.size() && i < NODE_WORKLOAD_LIST_LEN; ++it, ++i)
		{
			infoes[i] = it->second;
		}

		i = i > NODE_WORKLOAD_LIST_LEN ? NODE_WORKLOAD_LIST_LEN : i;

		return i;
	}

	CClusterClient::CClusterClient(CClusterServer* pServer, _U64 nCNDX) : m_pServer(pServer)
	{
		_global_cluster_object_manager.BindObject(nCNDX, this);
		m_nCNDX = nCNDX;
		memset(m_NodeStubs, 0, sizeof(m_NodeStubs));
		m_nUID = (_U32)-1;
		m_nAID = (_U32)-1;
	}

	CClusterClient::~CClusterClient()
	{
		if(m_nAID!=-1) m_pServer->AIDUnbindCNDX(m_nAID, this);
		if(m_nUID!=-1) m_pServer->UIDUnbindCNDX(m_nUID, this);

		_global_cluster_object_manager.UnbindObject(m_nCNDX, this);
	}

	void CClusterClient::UserLogin()
	{
		m_nState = STATE_CONNECT;
		OnUserLogin();
	}

	void CClusterClient::UserLogout()
	{
		m_nState = STATE_DISCONNECTING;
		bool bDelete = true;
		for(_U32 i=0; i<NODETYPE_MAX+1; i++)
		{
			if(m_NodeStubs[i].Current.hServer)
			{
				NodeDisconnect(i);
				bDelete = false;
			}
		}
		if(bDelete)
		{
			m_nState = STATE_DISCONNECTED;
			OnUserLogout();
		}
	}

	void CClusterClient::OnUserLogin()
	{
	}

	void CClusterClient::OnUserLogout()
	{
		ZION_ASSERT(m_nState==STATE_DISCONNECTED);
		if(m_nState!=STATE_DISCONNECTED) return;
		m_nState = STATE_DELETE;
	}

	void CClusterClient::OnUserData(_U16 code, _U32 len, const _U8* data)
	{
	}

	static _U32 _global_node_seq = 1980;
	bool CClusterClient::OnNodeConnected(_U32 nodeid, _U64 nndx, _U32 nodeseq)
	{
		ZION_ASSERT(nodeid<=NODETYPE_MAX);
		if(nodeid>NODETYPE_MAX) return false;
		ZION_ASSERT(m_NodeStubs[nodeid].Current.hServer);
		if(!m_NodeStubs[nodeid].Current.hServer) return false;
		ZION_ASSERT(m_NodeStubs[nodeid].Current.nNNDX==-1);
		if(m_NodeStubs[nodeid].Current.nNNDX!=-1) return false;
		ZION_ASSERT(m_NodeStubs[nodeid].Current.connect_seq==nodeseq);
		if(m_NodeStubs[nodeid].Current.connect_seq!=nodeseq) return false;
		m_NodeStubs[nodeid].Current.nNNDX = nndx;
		if(m_NodeStubs[nodeid].Current.disconnect_seq!=-1)
		{
			NRPC_Disconnect(m_NodeStubs[nodeid].Current.hServer, nndx, m_NodeStubs[nodeid].Current.disconnect_seq);
			if(m_nState==STATE_CONNECT) SRPC_SetNode(m_hSession, m_nSNDX, nodeid, (_U64)-1, 0, 0);
		}
		else
		{
			SRPC_SetNode(m_hSession, m_nSNDX, nodeid, nndx, GetRPCServerAddr(m_NodeStubs[nodeid].Current.hServer), GetRPCServerPort(m_NodeStubs[nodeid].Current.hServer));
		}
		return true;
	}

	bool CClusterClient::OnNodeConnectFailed(_U32 nodeid, _U32 nodeseq)
	{
		ZION_ASSERT(nodeid<=NODETYPE_MAX);
		if(nodeid>NODETYPE_MAX) return false;
		ZION_ASSERT(m_NodeStubs[nodeid].Current.hServer);
		if(!m_NodeStubs[nodeid].Current.hServer) return false;
		ZION_ASSERT(m_NodeStubs[nodeid].Current.nNNDX==-1);
		if(m_NodeStubs[nodeid].Current.nNNDX!=-1) return false;
		ZION_ASSERT(m_NodeStubs[nodeid].Current.connect_seq==nodeseq);
		if(m_NodeStubs[nodeid].Current.connect_seq!=nodeseq) return false;

		if(m_NodeStubs[nodeid].Next.hServer)
		{
			m_NodeStubs[nodeid].Current.hServer = m_NodeStubs[nodeid].Next.hServer;
			m_NodeStubs[nodeid].Current.nNNDX = (_U64)-1;
			m_NodeStubs[nodeid].Current.connect_seq = A_ATOM_INC(&_global_node_seq) & 0x7fffffff;
			m_NodeStubs[nodeid].Current.disconnect_seq = (_U32)-1;
			NRPC_Connect(m_NodeStubs[nodeid].Current.hServer, GetCNDX(), nodeid, m_NodeStubs[nodeid].Current.connect_seq, m_NodeStubs[nodeid].Next.len, m_NodeStubs[nodeid].Next.data);
			m_NodeStubs[nodeid].Next.hServer = NULL;
			if(m_NodeStubs[nodeid].Next.data) delete [] m_NodeStubs[nodeid].Next.data;
			m_NodeStubs[nodeid].Next.data = NULL;
		}
		else
		{
			m_NodeStubs[nodeid].Current.hServer = NULL;
			if(m_nState==STATE_DISCONNECTING) UserLogout();
		}

		return true;
	}

	bool CClusterClient::OnNodeDisconnected(_U32 nodeid, _U32 nodeseq)
	{
		ZION_ASSERT(nodeid<=NODETYPE_MAX);
		if(nodeid>NODETYPE_MAX) return false;
		ZION_ASSERT(m_NodeStubs[nodeid].Current.hServer);
		if(!m_NodeStubs[nodeid].Current.hServer) return false;
		ZION_ASSERT(m_NodeStubs[nodeid].Current.nNNDX!=-1);
		if(m_NodeStubs[nodeid].Current.nNNDX==-1) return false;
		ZION_ASSERT(m_NodeStubs[nodeid].Current.disconnect_seq!=-1);
		if(m_NodeStubs[nodeid].Current.disconnect_seq==-1) return false;
		ZION_ASSERT(m_NodeStubs[nodeid].Current.disconnect_seq==nodeseq);
		if(m_NodeStubs[nodeid].Current.disconnect_seq!=nodeseq) return false;

		if(m_nState==STATE_CONNECT) SRPC_SetNode(m_hSession, m_nSNDX, nodeid, (_U64)-1, 0, 0);

		if(m_NodeStubs[nodeid].Next.hServer)
		{
			m_NodeStubs[nodeid].Current.hServer = m_NodeStubs[nodeid].Next.hServer;
			m_NodeStubs[nodeid].Current.nNNDX = (_U64)-1;
			m_NodeStubs[nodeid].Current.connect_seq = A_ATOM_INC(&_global_node_seq) & 0x7fffffff;
			m_NodeStubs[nodeid].Current.disconnect_seq = (_U32)-1;
			NRPC_Connect(m_NodeStubs[nodeid].Current.hServer, GetCNDX(), nodeid, m_NodeStubs[nodeid].Current.connect_seq, m_NodeStubs[nodeid].Next.len, m_NodeStubs[nodeid].Next.data);
			m_NodeStubs[nodeid].Next.hServer = NULL;
			if(m_NodeStubs[nodeid].Next.data) delete [] m_NodeStubs[nodeid].Next.data;
			m_NodeStubs[nodeid].Next.data = NULL;
		}
		else
		{
			m_NodeStubs[nodeid].Current.hServer = NULL;
			if(m_nState==STATE_DISCONNECTING) UserLogout();
		}

		return true;
	}

	void CClusterClient::OnNodeEvent(_U16 code, _U32 len, const _U8* data)
	{
	}

	void CClusterClient::NodeSwitch(_U32 nodeid, HSERVER hServer, _U16 code, _U32 len, const _U8* data)
	{
		if(m_nState!=STATE_CONNECT) return;

		ZION_ASSERT(nodeid<NODETYPE_MAX);
		if(nodeid>NODETYPE_MAX) return;
		if(m_NodeStubs[nodeid].Current.hServer)
		{
			NodeDisconnect(nodeid);
			m_NodeStubs[nodeid].Next.hServer = hServer;
			m_NodeStubs[nodeid].Next.len = len;
			if(len>0)
			{
				m_NodeStubs[nodeid].Next.data = ZION_NEW _U8[len];
				memcpy(m_NodeStubs[nodeid].Next.data, data, len);
			}
		}
		else
		{
			m_NodeStubs[nodeid].Current.hServer = hServer;
			m_NodeStubs[nodeid].Current.nNNDX = (_U64)-1;
			m_NodeStubs[nodeid].Current.connect_seq = A_ATOM_INC(&_global_node_seq) & 0x7fffffff;
			m_NodeStubs[nodeid].Current.disconnect_seq = (_U32)-1;
			NRPC_Connect(hServer, GetCNDX(), nodeid, m_NodeStubs[nodeid].Current.connect_seq, len, data);
		}
	}

	void CClusterClient::NodeDisconnect(_U32 nodeid)
	{
		ZION_ASSERT(nodeid<NODETYPE_MAX);
		if(nodeid>NODETYPE_MAX) return;
		if(m_NodeStubs[nodeid].Current.nNNDX==-1)
		{
			m_NodeStubs[nodeid].Current.disconnect_seq = A_ATOM_INC(&_global_node_seq) & 0x7fffffff;
		}
		else if(m_NodeStubs[nodeid].Current.disconnect_seq==-1)
		{
			m_NodeStubs[nodeid].Current.disconnect_seq = A_ATOM_INC(&_global_node_seq) & 0x7fffffff;
			NRPC_Disconnect(m_NodeStubs[nodeid].Current.hServer, m_NodeStubs[nodeid].Current.nNNDX, m_NodeStubs[nodeid].Current.disconnect_seq);
			if(m_nState==STATE_CONNECT) SRPC_SetNode(m_hSession, m_nSNDX, nodeid, (_U64)-1, 0, 0);
		}
		if(m_NodeStubs[nodeid].Next.hServer)
		{
			m_NodeStubs[nodeid].Next.hServer = NULL;
			if(m_NodeStubs[nodeid].Next.data)
			{
				delete [] m_NodeStubs[nodeid].Next.data;
				m_NodeStubs[nodeid].Next.data = NULL;
			}
		}
	}

	void CClusterClient::KickUser()
	{
		SRPC_KickUser(m_hSession, m_nSNDX);
	}

	void CClusterClient::ForwardUserData(_U16 code, _U32 len, const _U8* data)
	{
		SRPC_ForwardUserData(m_hSession, m_nSNDX, code, len, data);
	}

	void CClusterClient::ForwardEvent(_U32 nodeid, _U16 code, _U32 len, const _U8* data)
	{
		if(nodeid==-1)
		{
			OnNodeEvent(code, len, data);
		}
		else
		{
			ZION_ASSERT(nodeid<=NODETYPE_MAX);
			if(nodeid>NODETYPE_MAX) return;
			if(m_NodeStubs[nodeid].Current.hServer)
			{
				if(m_NodeStubs[nodeid].Current.nNNDX!=-1 && m_NodeStubs[nodeid].Current.disconnect_seq==-1)
				{
					NRPC_OnForwardEvent(m_NodeStubs[nodeid].Current.hServer, m_NodeStubs[nodeid].Current.nNNDX, code, len, data);
				}
			}
		}
	}

	bool CClusterClient::BindUser(_U32 nUID, const char* pToken, HSERVER hSession, _U64 nSNDX)
	{
		ZION_ASSERT(m_nUID==-1);
		if(m_nUID!=-1) return false;

		if(!m_pServer->UIDBindCNDX(nUID, this)) return false;
		m_hSession = hSession;
		m_nSNDX = nSNDX;
		m_nUID = nUID;
		return true;
	}

	void CClusterClient::UnbindUser()
	{
		if(m_nUID!=-1) m_pServer->UIDUnbindCNDX(m_nUID, this);
	}

	bool CClusterClient::BindAvatar(_U32 nAID)
	{
		ZION_ASSERT(m_nAID==-1);
		if(m_nAID!=-1) return false;

		if(!m_pServer->AIDBindCNDX(nAID, this)) return false;
		m_nAID = nAID;
		return true;
	}

	void CClusterClient::UnbindAvatar()
	{
		if(m_nAID!=-1) m_pServer->AIDUnbindCNDX(m_nAID, this);
	}

}

void CRPC_UserLogin(Zion::HCLIENT hClient, _U32 uid, const char* token, _U64 sndx)
{
	Zion::SLog("%s", __FUNCTION__);
	Zion::_global_cluster_singleton->ClientLogin(GetRPCServer(hClient), uid, token, sndx);
}

void CRPC_UserLogout(Zion::HCLIENT hClient, _U32 uid, _U64 sndx)
{
	Zion::SLog("%s", __FUNCTION__);
	Zion::_global_cluster_singleton->ClientLogout(uid, sndx);
}

void CRPC_KickUser(Zion::HCLIENT hClient, _U32 uid)
{
	Zion::SLog("%s", __FUNCTION__);
	_U64 cndx;
	if(!Zion::_global_cluster_singleton->GetCNDXByUID(uid, cndx)) return;
	Zion::CObjectLocker<Zion::CClusterClient> Locker(cndx);
	if(!Locker.GetObject()) return;
	Locker.GetObject()->KickUser();
}

void CRPC_OnUserData(Zion::HCLIENT hClient, _U64 cndx, _U16 code, _U32 len, const _U8* data)
{
	Zion::SLog("%s", __FUNCTION__);
	Zion::CObjectLocker<Zion::CClusterClient> Locker(cndx);
	if(!Locker.GetObject()) return;
	Locker.GetObject()->OnUserData(code, len, data);
}

void CRPC_ForwardUserDataByCNDX(Zion::HCLIENT hClient, _U64 cndx, _U16 code, _U32 len, const _U8* data)
{
	Zion::SLog("%s", __FUNCTION__);
	Zion::CObjectLocker<Zion::CClusterClient> Locker(cndx);
	if(!Locker.GetObject()) return;
	Locker.GetObject()->ForwardUserData(code, len, data);
}

void CRPC_ForwardUserDataByUID(Zion::HCLIENT hClient, _U32 uid, _U16 code, _U32 len, const _U8* data)
{
	Zion::SLog("%s", __FUNCTION__);
	_U64 cndx;
	if(!Zion::_global_cluster_singleton->GetCNDXByUID(uid, cndx)) return;
	CRPC_ForwardUserDataByCNDX(hClient, cndx, code, len, data);
}

void CRPC_ForwardUserDataByAID(Zion::HCLIENT hClient, _U32 aid, _U16 code, _U32 len, const _U8* data)
{
	Zion::SLog("%s", __FUNCTION__);
	_U64 cndx;
	if(!Zion::_global_cluster_singleton->GetCNDXByAID(aid, cndx)) return;
	CRPC_ForwardUserDataByCNDX(hClient, cndx, code, len, data);
}

void CRPC_ForwardEventByCNDX(Zion::HCLIENT hClient, _U64 cndx, _U32 nodeid, _U16 code, _U32 len, const _U8* data)
{
	Zion::SLog("%s", __FUNCTION__);
	Zion::CObjectLocker<Zion::CClusterClient> Locker(cndx);
	if(!Locker.GetObject()) return;
	Locker.GetObject()->ForwardEvent(nodeid, code, len, data);
}

void CRPC_ForwardEventByUID(Zion::HCLIENT hClient, _U32 uid, _U32 nodeid, _U16 code, _U32 len, const _U8* data)
{
	Zion::SLog("%s", __FUNCTION__);
	_U64 cndx;
	if(!Zion::_global_cluster_singleton->GetCNDXByUID(uid, cndx)) return;
	CRPC_ForwardEventByCNDX(hClient, cndx, nodeid, code, len, data);
}

void CRPC_ForwardEventByAID(Zion::HCLIENT hClient, _U32 aid, _U32 nodeid, _U16 code, _U32 len, const _U8* data)
{
	Zion::SLog("%s", __FUNCTION__);
	_U64 cndx;
	if(!Zion::_global_cluster_singleton->GetCNDXByAID(aid, cndx)) return;
	CRPC_ForwardEventByCNDX(hClient, cndx, nodeid, code, len, data);
}

void CRPC_NodeConnect(Zion::HCLIENT hClient, _U64 cndx, _U32 nodeid, _U32 nodeseq, _U64 nndx)
{
	Zion::SLog("%s", __FUNCTION__);
	Zion::CObjectLocker<Zion::CClusterClient> Locker(cndx);
	if(!Locker.GetObject()) return;
	if(nndx==-1)
	{
		Locker.GetObject()->OnNodeConnectFailed(nodeid, nodeseq);
	}
	else
	{
		Locker.GetObject()->OnNodeConnected(nodeid, nndx, nodeseq);
	}
}

void CRPC_NodeDisconnect(Zion::HCLIENT hClient, _U64 cndx, _U32 nodeid, _U32 nodeseq)
{
	Zion::SLog("%s", __FUNCTION__);
	Zion::CObjectLocker<Zion::CClusterClient> Locker(cndx);
	if(!Locker.GetObject()) return;
	Locker.GetObject()->OnNodeDisconnected(nodeid, nodeseq);
}

void CRPC_SetSessionWorkload(Zion::HCLIENT hClient, const WORKLOAD_INFO& info)
{
	Zion::SLog("%s", __FUNCTION__);
	Zion::HSERVER hServer = GetRPCServer(hClient);
	Zion::_global_cluster_singleton->UpdateSessionWorkLoad(hServer, info);
}

void CRPC_GetSessionWorkload(Zion::HCLIENT hClient)
{
	Zion::SLog("%s", __FUNCTION__);
	WORKLOAD_INFO infoes[SESSION_WORKLOAD_LIST_LEN];
	_U32 count = Zion::_global_cluster_singleton->GetSessionWorkLoads(infoes);
	CRPC_SessionWorkLoadResult(hClient, infoes, count);
}

void CRPC_SetNodeWorkload(Zion::HCLIENT hClient, const WORKLOAD_INFO& info, _U32 type)
{
	Zion::SLog("%s", __FUNCTION__);
	Zion::HSERVER hServer = GetRPCServer(hClient);
	Zion::_global_cluster_singleton->NotifyNodeWorkLoad(hServer, info, type);
}

void CRPC_GetNodeWorkload(Zion::HCLIENT hClient, _U32 type)
{
	Zion::SLog("%s", __FUNCTION__);
	WORKLOAD_INFO infoes[NODE_WORKLOAD_LIST_LEN];
	_U32 count = Zion::_global_cluster_singleton->GetNodeWorkLoad(infoes, type);
	CRPC_NodeWorkLoadResult(hClient, type, infoes, count);
}


