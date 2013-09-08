#ifndef WITHOUT_ZION_ASYNCIO

#include "ZionBase.h"
#include "ZionClient.h"
#include "ZionClientApp.h"
#include "ClientConnection.h"
#include "AsyncSockIO.h"
#include "AsyncIOConnection.h"

namespace Zion
{

	static bool CLT_ON_CONNECT(HCONNECT);
	static void CLT_ON_DISCONNECT(HCONNECT);
	static void CLT_ON_DATA(HCONNECT, _U32, const _U8*);
	static void CLT_ON_CONNECTFAILED(void* key);
	static ASOCKIO_HANDLER g_client_handler = { CLT_ON_CONNECT, CLT_ON_DISCONNECT, CLT_ON_DATA, CLT_ON_CONNECTFAILED };

	class CAsyncIOInit
	{
	public:
		static CAsyncIOInit& Get()
		{
			static CAsyncIOInit _init;
			return _init;
		}

		CAsyncIOInit()
		{
			ASockIOInit();
			ZION_ASSERT(!m_workers);
			ZION_ASSERT(!m_iopool);
			m_workers = CreateWorkers(1);
			m_iopool = AllocIoBufferPool(1024, 1024, 0, 0);
		}

		~CAsyncIOInit()
		{
			KillWorkers(m_workers);
			FreeIoBufferPool(m_iopool);
			ASockIOFini();
		}

		HWORKERS m_workers;
		HIOPOOL m_iopool;
	};
	
	CAsyncIOConnection::CAsyncIOConnection(CClient* pClient, _U32 recvsize) : CClientConnection(pClient, recvsize)
	{
		m_bConnecting = false;
		m_hConn = NULL;
		m_pSendBuf = NULL;
		m_nSendBufLen = 0;
		A_MUTEX_INIT(&m_mtxLock);
	}

	CAsyncIOConnection::~CAsyncIOConnection()
	{
		A_MUTEX_DESTROY(&m_mtxLock);
	}

	bool CAsyncIOConnection::Connect(const SOCK_ADDR& sa)
	{
		bool bRet;

		A_MUTEX_LOCK(&m_mtxLock);

		ZION_ASSERT(!m_pSendBuf);
		ZION_ASSERT(!m_hConn);
		ZION_ASSERT(!m_bConnecting);
		if(m_hConn || m_bConnecting)
		{
			bRet = false;
		}
		else
		{
			m_pSendBuf = NULL;
			m_bConnecting = true;
			bRet = Zion::Connect(sa, g_client_handler, CAsyncIOInit::Get().m_iopool, CAsyncIOInit::Get().m_workers, this);
			if(!bRet) m_bConnecting = false;
		}

		A_MUTEX_UNLOCK(&m_mtxLock);

		return bRet;
	}

	void CAsyncIOConnection::Disconnect()
	{
		A_MUTEX_LOCK(&m_mtxLock);
		if(m_hConn) Zion::Disconnect(m_hConn);
		A_MUTEX_UNLOCK(&m_mtxLock);
	}

	void CAsyncIOConnection::SendData(_U32 len, const _U8* data, bool bPending)
	{
		A_MUTEX_LOCK(&m_mtxLock);

		HIOPOOL pool = HIOPoolOf(m_hConn);
		_U32 blen = GetIoBufferSize(pool);
		_U32 dolen;
		while(len>0)
		{
			if(!m_pSendBuf)
			{
				m_pSendBuf = LockIoBuffer(pool);
				m_nSendBufLen = 0;
			}

			dolen = (len>blen-m_nSendBufLen)?blen-m_nSendBufLen:len;

			memcpy(m_pSendBuf+m_nSendBufLen, data, dolen);
			len -= dolen;
			data += dolen;
			m_nSendBufLen += dolen;
			if(m_nSendBufLen==blen)
			{
				Send(m_hConn, m_nSendBufLen, m_pSendBuf);
				m_pSendBuf = NULL;
			}
		}

		if(!bPending && m_pSendBuf!=NULL && m_nSendBufLen>0)
		{
			Send(m_hConn, m_nSendBufLen, m_pSendBuf);
			m_pSendBuf = NULL;
		}

		A_MUTEX_UNLOCK(&m_mtxLock);
	}

	void CAsyncIOConnection::OnRawConnected(HCONNECT hConn)
	{
		A_MUTEX_LOCK(&m_mtxLock);
		ZION_ASSERT(!m_hConn);
		m_hConn = hConn;
		m_bConnecting = false;
		A_MUTEX_UNLOCK(&m_mtxLock);

		CClientConnection::OnRawConnected();
	}

	void CAsyncIOConnection::OnRawDisconnected()
	{
		A_MUTEX_LOCK(&m_mtxLock);
		if(m_pSendBuf)
		{
			UnlockIoBuffer(m_pSendBuf);
			m_pSendBuf = NULL;
		}
		m_hConn = NULL;
		A_MUTEX_UNLOCK(&m_mtxLock);

		CClientConnection::OnRawDisconnected();
	}

	void CAsyncIOConnection::OnRawData(_U32 len, const _U8* data)
	{
		CClientConnection::OnRawData(len, data);
	}

	void CAsyncIOConnection::OnRawConnectFailed()
	{
		m_bConnecting = false;
		CClientConnection::OnRawConnectFailed();
	}

	bool CLT_ON_CONNECT(HCONNECT hConn)
	{
		CAsyncIOConnection* pConnection = (CAsyncIOConnection*)KeyOf(hConn);
		if(!pConnection) return false;
		pConnection->OnRawConnected(hConn);
		return true;
	}

	void CLT_ON_DISCONNECT(HCONNECT hConn)
	{
		CAsyncIOConnection* pConnection = (CAsyncIOConnection*)KeyOf(hConn);
		if(!pConnection) return;
		pConnection->OnRawDisconnected();
		CloseConn(hConn);
	}

	void CLT_ON_DATA(HCONNECT hConn, _U32 len, const _U8* data)
	{
		CAsyncIOConnection* pConnection = (CAsyncIOConnection*)KeyOf(hConn);
		if(!pConnection) return;
		pConnection->OnRawData(len, data);
	}

	void CLT_ON_CONNECTFAILED(void* key)
	{
		CAsyncIOConnection* pConnection = (CAsyncIOConnection*)key;
		if(!pConnection) return;
		pConnection->OnRawConnectFailed();
	}

}

#endif
