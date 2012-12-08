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
	}

	void CAsyncIOConnection::Fini()
	{
	}
	
	CAsyncIOConnection::CAsyncIOConnection(CClient* pClient) : CClientConnection(pClient)
	{
		m_hConn = NULL;

		m_nRecvBuffLen = 0;
		m_nRecvBuffSize = GetClient()->GetClientApp()->GetDownPacketSize();
		m_pRecvBuff = (_U8*)ATLAS_ALLOC(m_nRecvBuffSize);
	}

	CAsyncIOConnection::~CAsyncIOConnection()
	{
		if(m_pRecvBuff)
		{
			ATLAS_FREE(m_pRecvBuff);
			m_pRecvBuff = NULL;
		}
	}

	bool CAsyncIOConnection::Connect(const SOCKADDR& sa, const char* token)
	{
		ATLAS_ASSERT(GetClient()->GetState()==CClient::STATE_LOGINING);
		ATLAS_ASSERT(!m_hConn);

		m_sRedirectToken = token;

		if(!Atlas::Connect(sa, g_client_handler, g_client_iopool, g_client_workers, this))
		{
			GetClient()->SetErrCode(CClient::ERRCODE_NETWORK);
			GetClient()->GetClientApp()->QueueLoginFailed(GetClient());
			return false;
		}

		return true;
	}

	bool CAsyncIOConnection::Disconnect()
	{
		if(m_hConn) Atlas::Disconnect(m_hConn);
		return true;
	}

	void CAsyncIOConnection::SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
	}

	void CAsyncIOConnection::OnRawConnected(HCONNECT hConn)
	{
		ATLAS_ASSERT(GetClient()->GetState()==CClient::STATE_LOGINING);
		ATLAS_ASSERT(!m_hConn);
		m_hConn = hConn;

		IOBUFFER_STREAM<10> stream(HIOPoolOf(hConn), 0);
		_U32 token_len = (_U32)m_sRedirectToken.size();
		stream.Write(token_len);
		stream.WriteData(m_sRedirectToken.c_str(), token_len);
		stream.Send(hConn);
	}

	void CAsyncIOConnection::OnRawDisconnected()
	{
		if(!m_bRedirect)
		{
			if(GetClient()->GetState()==CClient::STATE_LOGINED)
			{
				GetClient()->GetClientApp()->QueueDisconnected(GetClient());
			}
			else
			{
				GetClient()->SetErrCode(CClient::ERRCODE_NETWORK);
				GetClient()->GetClientApp()->QueueLoginFailed(GetClient());
			}
		}
		else
		{
			ATLAS_ASSERT(GetClient()->GetState()==CClient::STATE_LOGINING);
			m_bRedirect = false;

			if(!Atlas::Connect(m_saRedirectAddr, g_client_handler, g_client_iopool, g_client_workers, this))
			{
				GetClient()->SetErrCode(CClient::ERRCODE_NETWORK);
				GetClient()->GetClientApp()->QueueLoginFailed(GetClient());
			}
		}
	}

	void CAsyncIOConnection::OnRawData(_U32 len, const _U8* data)
	{
		_U16 pkglen;
		_U32 copylen;
		while(len>0)
		{
			copylen = len;
			if(m_nRecvBuffLen+copylen>m_nRecvBuffSize) copylen = m_nRecvBuffSize - m_nRecvBuffLen;
			memcpy(m_pRecvBuff+m_nRecvBuffLen, data, copylen);
			m_nRecvBuffLen += copylen;
			len -= copylen;
			data = data + copylen;
			while(m_nRecvBuffLen>=sizeof(pkglen))
			{
				pkglen = *((const _U16*)m_pRecvBuff);

				if(pkglen>m_nRecvBuffSize-sizeof(pkglen))
				{
					Disconnect();
					return;
				}

				if(pkglen+sizeof(pkglen)>m_nRecvBuffLen)
				{
					break;
				}

				GetClient()->ProcessPacket((_U32)pkglen, m_pRecvBuff+sizeof(pkglen));

				_U32 ulen = pkglen + sizeof(pkglen);
				m_nRecvBuffLen -= ulen;
				memmove(m_pRecvBuff, m_pRecvBuff+ulen, m_nRecvBuffLen);
			}
		}
	}

	void CAsyncIOConnection::OnRawConnectFailed()
	{
		ATLAS_ASSERT(GetClient()->GetState()==CClient::STATE_LOGINING);
		GetClient()->SetErrCode(CClient::ERRCODE_NETWORK);
		GetClient()->GetClientApp()->QueueLoginFailed(GetClient());
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
