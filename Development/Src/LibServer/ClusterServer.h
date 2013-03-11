#ifndef __ATLAS_CLUSTER_SERVER__
#define __ATLAS_CLUSTER_SERVER__

#include <hash_map>
#include "ServerInc.h"

namespace Atlas
{
	class CClusterServer;
	class CClusterClient;

	const _U32 NODETYPE_MAX = 255;

	template<>
	CClusterClient* CObjectLocker<CClusterClient>::Lock(_U64 ndx);
	template<>
	void CObjectLocker<CClusterClient>::Unlock(_U64 ndx);

	// 1. 负责把用户数据派发到对应的Node Server上处理
	// 2. 负责管理旗下的Node Server
	// 3. 负责Node Server之间的通信
	class CClusterServer : public CServerBase
	{
		//<session, workload>
		typedef Atlas::Map<HSERVER, WORKLOAD_INFO> SESSION_WORKLOADS;
		//<NODE, workload>
		typedef Atlas::List< std::pair<HSERVER, WORKLOAD_INFO> > NODE_LOADLIST;

	public:
		CClusterServer(CServerApp* pServerApp);
		virtual ~CClusterServer();

		virtual CClusterClient* NewClusterClient(_U64 nCNDX);

		// from Session Server
		void ClientLogin(HSERVER hServer, _U32 nUID, const char* pToken, _U64 nSNDX);
		void ClientLogout(_U32 nUID, _U64 nSNDX);

		//
		bool GetCNDXByUID(_U32 nUID, _U64& nCNDX);
		bool GetCNDXByAID(_U32 nAID, _U64& nCNDX);
		bool UIDBindCNDX(_U32 nUID, CClusterClient* pClient);
		bool AIDBindCNDX(_U32 nAID, CClusterClient* pClient);
		bool UIDUnbindCNDX(_U32 nUID, CClusterClient* pClient);
		bool AIDUnbindCNDX(_U32 nAID, CClusterClient* pClient);

		void UpdateSessionWorkLoad(HSERVER hServer, const WORKLOAD_INFO& info);
		_U32 GetSessionWorkLoads(WORKLOAD_INFO workLoads[]);
		void NotifyNodeWorkLoad(HSERVER hServer, const WORKLOAD_INFO& WORKLOAD_INFO, _U32 type);
		_U32 GetNodeWorkLoad(WORKLOAD_INFO infoes[], _U32 type);

	private:
		CServerIndexMap m_mapUID;
		CServerIndexMap m_mapAID;
		A_MUTEX m_mtxWorkLoad;
		SESSION_WORKLOADS m_WorkLoads;
		NODE_LOADLIST m_NodeLoads[NODETYPE_MAX+1];
	};

	// 1. Cluster Server上表示客户端连接的对象
	class CClusterClient : public CNoCopy
	{
		friend class CClusterServer;
	public:
		enum
		{
			STATE_CONNECT,
			STATE_DISCONNECTING,
			STATE_DISCONNECTED,
			STATE_DELETE,
		};

		CClusterClient(CClusterServer* pServer, _U64 nCNDX);
		virtual ~CClusterClient();

		_U32 GetState() { return m_nState; }

		void UserLogin();
		void UserLogout();

		virtual void OnUserLogin();
		virtual void OnUserLogout();
		virtual void OnUserData(_U16 code, _U32 len, const _U8* data);

		virtual bool OnNodeConnected(_U32 nodeid, _U64 nndx, _U32 nodeseq);
		virtual bool OnNodeConnectFailed(_U32 nodeid, _U32 nodeseq);
		virtual bool OnNodeDisconnected(_U32 nodeid, _U32 nodeseq);
		virtual void OnNodeEvent(_U16 code, _U32 len, const _U8* data);

		void NodeSwitch(_U32 nodeid, HSERVER hServer, _U16 code, _U32 len, const _U8* data);
		void NodeDisconnect(_U32 nodeid);
		void KickUser();
		void ForwardUserData(_U16 code, _U32 len, const _U8* data);
		void ForwardEvent(_U32 nodeid, _U16 code, _U32 len, const _U8* data);

		bool Send(_U16 iid, _U16 fid, DDL::MemoryWriter& Buf)
		{
			ATLAS_ASSERT(iid<256 && fid<256);
			ForwardUserData(iid|(fid<<8), Buf.GetSize(), Buf.GetBuf());
			return true;
		}

		CClusterServer* GetClusterServer() { return m_pServer; }
		_U64 GetCNDX() { return m_nCNDX; }
		HSERVER GetSession() { return m_hSession; }
		_U64 GetSNDX() { return m_nSNDX; }

	protected:
		bool BindUser(_U32 nUID, const char* pToken, HSERVER hSession, _U64 nSNDX);
		void UnbindUser();
		bool BindAvatar(_U32 nAID);
		void UnbindAvatar();
		_U32 GetUID() { return m_nUID; }

	private:
		CClusterServer* m_pServer;
		_U32 m_nState;
		_U64 m_nCNDX;
		HSERVER m_hSession;
		_U64 m_nSNDX;
		_U32 m_nUID, m_nAID;
		std::string m_Token;
		struct
		{
			struct
			{
				HSERVER hServer;
				_U64 nNNDX;
				_U32 connect_seq;
				_U32 disconnect_seq;
			} Current;
			struct
			{
				HSERVER hServer;
				_U32 len;
				_U8* data;
			} Next;
		} m_NodeStubs[NODETYPE_MAX+1];
	};

}

#endif
