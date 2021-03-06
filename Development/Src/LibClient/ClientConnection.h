#ifndef __ZION_CLIENT_CONNECTION__
#define __ZION_CLIENT_CONNECTION__

namespace Zion
{

	class CClientConnection : public CClientConnectionBase
	{
	public:
		CClientConnection(CClient* pClient, _U32 recvsize);
		virtual ~CClientConnection();

		CClient::CLIENT_STATE GetState();
		_U32 GetErrorCode();

		virtual bool Login(const char* pUrl, const CClientLoginMethod* pMethod);
		virtual void Logout();
		virtual void SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data);

	protected:
		void OnRawConnected();
		void OnRawData(_U32 len, const _U8* data);
		void OnRawDisconnected();
		void OnRawConnectFailed();

		void ProcessPacket(_U32 len, const _U8* data);

		virtual bool Connect(const ASOCK_ADDR& sa) = 0;
		virtual void Disconnect() = 0;
		virtual void SendData(_U32 len, const _U8* data, bool bPending=false) = 0;

	private:
		_U8* m_pRecvBuff;
		_U32 m_nRecvBuffLen;
		_U32 m_nRecvBuffSize;
		bool m_bNeedRedirect;
		ASOCK_ADDR m_saRedirectAddr;
		_U8 m_LoginData[1024];
		_U16 m_nLoginDataSize;
	};

}

#endif
