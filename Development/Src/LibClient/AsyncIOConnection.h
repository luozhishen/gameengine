#ifndef WITHOUT_ZION_ASYNCIO

#ifndef __ASYNCIO_CONNECTION__
#define __ASYNCIO_CONNECTION__

#include <AsyncSockIO.h>

namespace Zion
{

	class CAsyncIOConnection : public CClientConnection
	{
	public:
		CAsyncIOConnection(CClient* pClient, _U32 recvsize);
		virtual ~CAsyncIOConnection();

		virtual bool Connect(const ASOCK_ADDR& sa);
		virtual void Disconnect();
		virtual void SendData(_U32 len, const _U8* data, bool bPending);

		void OnRawConnected(HCONNECT hConn);
		void OnRawDisconnected();
		void OnRawData(_U32 len, const _U8* data);
		void OnRawConnectFailed();

	private:
		bool m_bConnecting;
		HCONNECT m_hConn;
		_U8* m_pSendBuf;
		_U32 m_nSendBufLen;
		A_MUTEX m_mtxLock;
	};

}

#endif

#endif
