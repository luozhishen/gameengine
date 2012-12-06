#ifndef __ATLAS_NODE_SERVER__
#define __ATLAS_NODE_SERVER__

namespace Atlas
{

	class CNodeClient;
	class CNodeServer;

	template<typename TStubClass, typename TClient, typename TThis>
	class CNodeStub
	{
	public:
		CNodeStub(TThis* pThis) : m_Dispatch(pThis)
		{
		}

		void OnData(CNodeClient* pClient, _U16 fid, _U32 len, const _U8* data)
		{
			DDL::MemoryReader buf(data, len);
			m_Dispatch.Dispatcher((TClient*)pClient, fid, buf);
		}

	private:
		TStubClass m_Dispatch;
	};

	template<>
	CNodeClient* CObjectLocker<CNodeClient>::Lock(_U64 ndx);
	template<>
	void CObjectLocker<CNodeClient>::Unlock(_U64 ndx);

	// 1. 处理用户逻辑的地方
	class CNodeServer : public CServerBase
	{
	public:
		typedef std::tr1::function <void(CNodeClient*, _U16, _U32, const _U8*)> StubProc;

		CNodeServer(CServerApp* pServerApp, _U32 nodeid);
		virtual ~CNodeServer();

		_U32 GetNodeID() { return m_nNodeID; }

		virtual CNodeClient* NewNodeClient(_U64 nndx, _U64 cndx, HSERVER hCluster) = 0;

		void OnClusterConnect(HSERVER hCluster, _U64 cndx, _U32 nodeseq, _U32 len, const _U8* data);
		void OnSessionAck(CNodeClient* pClient, HSERVER hSession, _U64 sndx);
		void OnClusterData(CNodeClient* pClient, _U16 code, _U32 len, const _U8* data);
		void OnClusterDisconnect(CNodeClient* pClient, _U32 nodeseq);

		void RegisterStub(_U16 stubid, StubProc proc);
		void Tick();

	private:
		StubProc m_Stubs[256];
		_U32 m_nNodeID;
		HANDLE m_hTimerQueue;
		HANDLE m_hTimer;
		Atlas::HSERVER m_hClusterServer;
		_U32 m_nConnCount;
	};

	// 1. Node Server上表示客户端连接的对象
	class CNodeClient : public CNoCopy
	{
		friend class CNodeServer;
	public:
		enum
		{
			STATE_CONNECT,
			STATE_SESSION_ACK,
			STATE_COMPLETE,
			STATE_DISCONNECTING,
			STATE_DISCONNECTED,
		};

		CNodeClient(CNodeServer* pServer, _U64 nndx, _U64 cndx, HSERVER hCluster);
		~CNodeClient();

		CNodeServer* GetServer() { return m_pServer; }
		_U32 GetState() { return m_nState; }
		HSERVER GetCluster() { return m_hCluster; }
		HSERVER GetSession() { return m_hSession; }
		_U64 GetNNDX() { return m_nNNDX; }
		_U64 GetCNDX() { return m_nCNDX; }
		_U64 GetSNDX() { return m_nSNDX; }

		void Disconnect();

		virtual void OnConnected(_U32 len, const _U8* data);
		virtual void OnDisconnect();

		void SendData(_U16 code, _U32 len, const _U8* data);
		bool Send(_U16 iid, _U16 fid, DDL::MemoryWriter& Buf)
		{
			ATLAS_ASSERT(iid<256 && fid<256);
			SendData(iid|(fid<<8), Buf.GetSize(), Buf.GetBuf());
			return true;
		}

	protected:
		void ClusterConnect(_U32 nodeseq, _U32 len, const _U8* data);
		void SessionAck(HSERVER hSession, _U64 sndx);
		void ClusterDisconnect(_U32 nodeseq);

	private:
		CNodeServer* m_pServer;
		_U32 m_nState;
		HSERVER m_hCluster, m_hSession;
		_U64 m_nNNDX, m_nCNDX, m_nSNDX;

		struct
		{
			_U32 len;
			_U8* data;
		} m_ConnectData;
		struct
		{
			_U32 nodeseq;
		} m_DisconnectData;

	};

}

#endif
