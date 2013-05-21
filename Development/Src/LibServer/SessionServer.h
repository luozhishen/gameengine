#ifndef __ATLAS_SESSION_SERVER__
#define __ATLAS_SESSION_SERVER__

#define SESSION_IORECVBUF_SIZE		(1024)
#define SESSION_IOSENDBUF_SIZE		(1024)

namespace Zion
{
	class CSessionClient;
	class CSessionServer;

	template<>
	CSessionClient* CObjectLocker<CSessionClient>::Lock(_U64 ndx);
	template<>
	void CObjectLocker<CSessionClient>::Unlock(_U64 ndx);

	class CSessionServer : public CServerBase
	{
	public:
		CSessionServer(CServerApp* pServerApp);
		virtual ~CSessionServer();

		virtual CSessionClient* NewSessionClient(HCONNECT hConn, _U64 nSNDX);
		HSERVER GetDefaultCluster();

		virtual bool Start();
		virtual void Stop();

		bool OnConnected(HCONNECT hConn);
		void OnData(HCONNECT hConn, _U32 len, const _U8* data);
		void OnDisconnected(HCONNECT hConn);

		bool BindUID(_U32 uid, _U64 sndx);
		bool Unbind(_U32 uid, _U64 sndx);		
		void Tick();

	private:
		HSERVER m_hDefaultCluster;
		SOCK_ADDR m_saEndPoint;
		HIOPOOL m_hPool;
		HTCPEP m_hEp;
		_U32 m_nConnCount;
		CServerIndexMap m_mapUID;
		HANDLE m_hTimerQueue;
		HANDLE m_hTimer;
	};

	class CSessionClient : public CNoCopy
	{
		friend class CSessionServer;
	public:
		CSessionClient(CSessionServer* pServer, HCONNECT hConn, _U64 nSNDX, _U32 recvsize=SESSION_IORECVBUF_SIZE);
		virtual ~CSessionClient();

		_U32 GetUID() { return m_nUID; }
		CSessionServer* GetServer() { return m_pServer; }
		_U64 GetSNDX() { return m_nSNDX; }
		_U64 GetCNDX() { return m_nCNDX; }

		virtual void OnConnected() { }
		virtual void OnAuthPassed(_U32 nUID, const char* pToken);
		virtual void OnData(_U16 id, _U32 len, const _U8* data);
		virtual void OnDisconnected();
		virtual void SendRawData(_U16 id, _U32 len, const _U8* data);

		bool Send(_U16 iid, _U16 fid, DDL::MemoryWriter& Buf)
		{
			ATLAS_ASSERT(iid<256 && fid<256);
			SendRawData(iid|(fid<<8), Buf.GetSize(), Buf.GetBuf());
			return true;
		}

		bool NeedDelete();
		void UserLoginDone(_U64 nCNDX);
		void KickUser();
		void NodeConnect(_U32 nodeid, HSERVER hServer, _U64 nndx);
		void NodeDisconnect(_U32 nodeid);
		bool GetNode(_U32 nodeid, HSERVER& hServer, _U64 nndx);

	protected:
		virtual void OnRawConnect();
		virtual void OnRawData(_U32 len, const _U8* data);
		virtual void OnRawDisconnect();
		void SetUID(_U32 uid, const char* token);
		void ClusterLogin(HSERVER hServer);
		void ClusterLogout();

	private:
		HCONNECT m_hConnect;
		_U32 m_nUID;
		char m_sToken[1024];
		CSessionServer* m_pServer;
		_U64 m_nSNDX; // session server index
		_U64 m_nCNDX; // cluster server index
		HSERVER m_hCluster;
		_U8* m_pRecvBuff;
		_U32 m_nRecvBuffLen;
		_U32 m_nRecvBuffSize;
		struct
		{
			HSERVER hServer;
			_U64 nNNDX;
		} m_NodeStubs[256];
	};

}

#endif
