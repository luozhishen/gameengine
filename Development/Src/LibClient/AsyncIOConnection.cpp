#ifndef WITHOUT_ASYNCIO

#include "AtlasBase.h"
#include "AtlasClient.h"
#include "AtlasClientApp.h"
#include "AsyncIOConnection.h"

namespace Atlas
{

	static bool CLT_ON_CONNECT(HCONNECT);
	static void CLT_ON_DISCONNECT(HCONNECT);
	static void CLT_ON_DATA(HCONNECT, _U32, const _U8*);
	static void CLT_ON_CONNECTFAILED(void* key);
	static ASOCKIO_HANDLER g_client_handler = { CLT_ON_CONNECT, CLT_ON_DISCONNECT, CLT_ON_DATA, CLT_ON_CONNECTFAILED };
	static HWORKERS g_client_workers = NULL;
	static HIOPOOL g_client_iopool = NULL;

	void CAsyncIOConnection::Init(_U32 tcount)
	{
		ATLAS_ASSERT(!g_client_workers);
		ATLAS_ASSERT(!g_client_iopool);
		g_client_workers = CreateWorkers(tcount);
		g_client_iopool = AllocIoBufferPool(1024, 1024, 0, 0);
	}

	void CAsyncIOConnection::Fini()
	{
		KillWorkers(g_client_workers);
		FreeIoBufferPool(g_client_iopool);
	}
	
	CAsyncIOConnection::CAsyncIOConnection(CClient* pClient) : CClientConnection(pClient)
	{
		m_bConnecting = false;
		m_hConn = NULL;
		m_pSendBuf = NULL;
		m_nSendBufLen = 0;
	}

	CAsyncIOConnection::~CAsyncIOConnection()
	{
	}

	bool CAsyncIOConnection::Connect(const SOCKADDR& sa)
	{
		ATLAS_ASSERT(!m_pSendBuf);
		ATLAS_ASSERT(!m_hConn);
		ATLAS_ASSERT(!m_bConnecting);
		if(m_hConn || m_bConnecting) return false;

		m_pSendBuf = NULL;
		m_bConnecting = true;
		if(!Atlas::Connect(sa, g_client_handler, g_client_iopool, g_client_workers, this))
		{
			m_bConnecting = false;
			return false;
		}
		else
		{
			return true;
		}
	}

	bool CAsyncIOConnection::Disconnect()
	{
		if(m_hConn) Atlas::Disconnect(m_hConn);
		return true;
	}

	void CAsyncIOConnection::SendData(_U32 len, const _U8* data, bool bPending)
	{
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
	}

	void CAsyncIOConnection::OnRawConnected(HCONNECT hConn)
	{
		ATLAS_ASSERT(!m_hConn);
		m_hConn = hConn;
		m_bConnecting = false;
		GetClient()->OnRawConnected();
	}

	void CAsyncIOConnection::OnRawDisconnected()
	{
		if(m_pSendBuf)
		{
			UnlockIoBuffer(m_pSendBuf);
			m_pSendBuf = NULL;
		}
		m_hConn = NULL;
		GetClient()->OnRawDisconnected();
	}

	void CAsyncIOConnection::OnRawData(_U32 len, const _U8* data)
	{
		GetClient()->OnRawData(len, data);
	}

	void CAsyncIOConnection::OnRawConnectFailed()
	{
		m_bConnecting = false;
		GetClient()->OnRawConnectFailed();
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
