#ifndef __ATLAS_CLIENT_CONNECTION__
#define __ATLAS_CLIENT_CONNECTION__

namespace Atlas
{

	class CClientConnection : public CClientConnectionBase
	{
	public:
		CClientConnection(CClient* pClient, _U32 recvsize);
		virtual ~CClientConnection();

		CClient::CLIENT_STATE GetState();
		_U32 GetErrorCode();

		virtual bool Login(const char* pUrl, const char* pToken);
		virtual void Logout();
		virtual void SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data);

	protected:
		void OnRawConnected();
		void OnRawData(_U32 len, const _U8* data);
		void OnRawDisconnected();
		void OnRawConnectFailed();

		void ProcessPacket(_U32 len, const _U8* data);

		virtual bool Connect(const SOCK_ADDR& sa) = 0;
		virtual void Disconnect() = 0;
		virtual void SendData(_U32 len, const _U8* data, bool bPending=false) = 0;

	private:
		_U8* m_pRecvBuff;
		_U32 m_nRecvBuffLen;
		_U32 m_nRecvBuffSize;
		bool m_bNeedRedirect;
		SOCK_ADDR m_saRedirectAddr;
		_U8 m_LoginData[1024];
		_U16 m_nLoginDataSize;
	};

}

#endif
