#ifndef __CONNECTION_SERVER__
#define __CONNECTION_SERVER__

namespace Atlas
{

	class CConnectionClient;

	class CConnectionServer : public CServerBase
	{
	public:
		CConnectionServer(CServerApp* pServerApp);
		virtual ~CConnectionServer();

		void SetEP(const SOCK_ADDR& sa);

		virtual bool Start();
		virtual void Stop();

		virtual CConnectionClient* NewConnection(HCONNECT hConn) = 0;
		virtual bool OnConnected(HCONNECT hConn);

	private:
		SOCK_ADDR m_saAddr;
		HIOPOOL m_hPool;
		HTCPEP m_hEp;
	};

	class CConnectionClient
	{
	public:
		CConnectionClient(HCONNECT hConn, _U32 ndx, _U32 recvsize);
		virtual ~CConnectionClient();

		_U32 GetIndex() { return m_nNDX; }

		virtual void OnConnected();
		virtual void OnDisconnected();
		virtual void OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data);

		virtual void OnRawConnected();
		virtual void OnRawDisconnected();
		virtual void OnRawData(_U32 len, const _U8* data);

		virtual void ProcessPacket(_U32 len, const _U8* data);
		virtual void SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data);
		void Disconnect();

	private:
		HCONNECT m_hConn;
		_U32 m_nNDX;
		_U8* m_pRecvBuff;
		_U32 m_nRecvBuffLen;
		_U32 m_nRecvBuffSize;
	};

}

#endif
