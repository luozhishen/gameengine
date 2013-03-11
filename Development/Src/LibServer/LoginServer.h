#ifndef _LOGIN_SERVER_H__
#define _LOGIN_SERVER_H__

#define LOGIN_IORECVBUF_SIZE		(1024)
#define LOGIN_IOSENDBUF_SIZE		(1024)

namespace Atlas
{
	class CLoginServer;
	class CLoginClient;

	template<>
	CLoginClient* CObjectLocker<CLoginClient>::Lock(_U64 ndx);
	template<>
	void CObjectLocker<CLoginClient>::Unlock(_U64 ndx);

	class CLoginServer : public CServerBase
	{	
		struct WorkCapacity
		{
			_U64 sndx;
			_U64 cndx;
			_U64 nWorkCapacity;
		};

		typedef Atlas::Vector<WORKLOAD_INFO> SESSION_WORKLOADS;

	public:
		CLoginServer(CServerApp* pServerApp);
		virtual ~CLoginServer();

		virtual CLoginClient* NewLoginClient(HCONNECT hConn, _U64 nLNDX);
		HSERVER GetDefaultCluster();
		
		virtual bool Start();
		virtual void Stop();

		bool OnConnected(HCONNECT hConn);
		void OnData(HCONNECT hConn, _U32 len, const _U8* data);
		void OnDisconnected(HCONNECT hConn);

		bool BindUID(_U32 uid, _U64 lndx);
		bool Unbind(_U32 uid, _U64 lndx);

		bool GetSession(_U32 uid, SOCK_ADDR& sa);
		void UpdateWorkLoadInfo(const WORKLOAD_INFO* info_list, _U32 count);
		void Tick();

	private:
		HSERVER m_hDefaultCluster;
		SESSION_WORKLOADS m_WorkLoads;
		SOCK_ADDR m_saEndPoint;
		HIOPOOL m_hPool;
		HTCPEP m_hEp;
		_U32 m_nConnCount;
		CServerIndexMap m_mapUID;//UID WorkCapacityIndex
		A_MUTEX m_mtxWorkLoad;
		HANDLE m_hTimerQueue;
		HANDLE m_hTimer;
	};

	class CLoginClient : public CNoCopy
	{
		friend class CLoginServer;
	public:
		CLoginClient(CLoginServer* pServer, HCONNECT hConn, _U64 nLNDX, _U32 recvsize=LOGIN_IORECVBUF_SIZE);
		virtual ~CLoginClient();

		_U64 GetLNDX() { return m_nLNDX; }
		_U32 GetUID() { return m_nUID; }
		_U64 GetSNDX() { return m_nSNDX; }
		_U64 GetCNDX() { return m_nCNDX; }

		CLoginServer* GetServer() { return m_pServer; }

		virtual void OnConnected() { }
		virtual void OnAuthPassed(_U32 nUID, _U32 len, const _U8* pToken);
		virtual void OnData(_U16 id, _U32 len, const _U8* data);
		virtual void OnDisconnected();
		virtual void SendRawData(_U16 id, _U32 len, const _U8* data);

		bool NeedDelete();
		void UserLoginDone(_U64 nCNDX);
		void KickUser();

	protected:
		virtual void OnRawConnect();
		virtual void OnRawData(_U32 len, const _U8* data);
		virtual void OnRawDisconnect();
		void SetUID(_U32 uid, const char* token);

		bool Send(_U16 iid, _U16 fid, DDL::MemoryWriter& Buf)
		{
			ATLAS_ASSERT(iid<256 && fid<256);
			SendRawData(iid|(fid<<8), Buf.GetSize(), Buf.GetBuf());
			return true;
		}
	private:
		HCONNECT m_hConnect;
		char m_sToken[1024];
		CLoginServer* m_pServer;
		_U64 m_nSNDX; // session server index
		_U64 m_nCNDX; // cluster server index
		_U8* m_pRecvBuff;
		_U32 m_nRecvBuffLen;
		_U32 m_nRecvBuffSize;
		_U32 m_nUID;
		_U64 m_nLNDX;
	};
}

#endif
