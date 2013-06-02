#include <ZionBase.h>

#include "ServerApp.h"
#include "ServerBase.h"
#include "ConnectionServer.h"

namespace Zion
{

	static bool CS_ON_CONNECT(HCONNECT hConn);
	static void CS_ON_DISCONNECT(HCONNECT hConn);
	static void CS_ON_DATA(HCONNECT hConn, _U32 len, const _U8* data);

	CConnectionServer::CConnectionServer(CServerApp* pServerApp) : CServerBase(pServerApp)
	{
		m_hPool = AllocIoBufferPool(500, 500, 0, 0);
		m_hEp = NULL;
	}

	CConnectionServer::~CConnectionServer()
	{
		if(m_hEp) DelEP(m_hEp);
		m_hEp = NULL;

		if(m_hPool) FreeIoBufferPool(m_hPool);
		m_hPool = NULL;
	}

	void CConnectionServer::SetEP(const SOCK_ADDR& sa)
	{
		m_saAddr = sa;
	}

	bool CConnectionServer::Start()
	{
		ZION_ASSERT(m_hEp);
		ASOCKIO_HANDLER handler = { CS_ON_CONNECT, CS_ON_DISCONNECT, CS_ON_DATA, NULL };
		m_hEp = NewEP(m_saAddr, handler, m_hPool, GetServerApp()->GetIOWorkers(), this);
		ZION_ASSERT(m_hEp);
		if(!m_hEp) return false;
		StartEP(m_hEp);

		return true;
	}

	void CConnectionServer::Stop()
	{
		ZION_ASSERT(m_hEp);
		StopEP(m_hEp);
		while(IsRunning(m_hEp))
		{
			SwitchToThread();
		}
	}

	bool CConnectionServer::OnConnected(HCONNECT hConn)
	{
		CConnectionClient* pClient = NewConnection(hConn);
		if(!pClient) return false;
		SetKey(hConn, pClient);
		pClient->OnRawConnected();
		return true;
	}

	CConnectionClient::CConnectionClient(HCONNECT hConn, _U32 ndx, _U32 recvsize)
	{
		m_nUID = (_U32)-1;
		m_bLogining = true;
		m_hConn = hConn;
		m_nNDX = ndx;
		m_pRecvBuff = (_U8*)ZION_ALLOC(recvsize);;
		m_nRecvBuffLen = 0;
		m_nRecvBuffSize = recvsize;
	}

	CConnectionClient::~CConnectionClient()
	{
		ZION_FREE(m_pRecvBuff);
		m_pRecvBuff = NULL;
	}

	void CConnectionClient::OnConnected()
	{
	}

	void CConnectionClient::OnDisconnected()
	{
	}

	void CConnectionClient::OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
	}

	void CConnectionClient::ProcessPacket(_U32 len, const _U8* data)
	{
	}

	void CConnectionClient::SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
	}

	void CConnectionClient::Disconnect()
	{
		if(m_hConn) Zion::Disconnect(m_hConn);
	}

	void CConnectionClient::OnRawConnected()
	{
	}

	void CConnectionClient::OnRawDisconnected()
	{
		m_hConn = NULL;
	}

	void CConnectionClient::OnRawData(_U32 len, const _U8* data)
	{
		_U16 pkglen;
		while(len>0)
		{
			_U32 copylen = len;
			if(m_nRecvBuffLen+copylen>m_nRecvBuffSize) copylen = m_nRecvBuffSize - m_nRecvBuffLen;
			memcpy(m_pRecvBuff+m_nRecvBuffLen, data, copylen);
			m_nRecvBuffLen += copylen;
			len -= copylen;
			data = data + copylen;
			while(m_nRecvBuffLen>=sizeof(pkglen))
			{
				pkglen = *((const _U16*)data);

				if(pkglen>m_nRecvBuffSize-sizeof(pkglen))
				{
					Disconnect();
					return;
				}

				if(pkglen+sizeof(pkglen)>m_nRecvBuffLen) break;

				ProcessPacket(pkglen, data+sizeof(pkglen));

				_U32 ulen = pkglen + sizeof(pkglen);
				m_nRecvBuffLen -= ulen;
				memmove(m_pRecvBuff, m_pRecvBuff+ulen, m_nRecvBuffLen);
			}
		}
	}

	bool CS_ON_CONNECT(HCONNECT hConn)
	{
		HTCPEP hep = HepOf(hConn);
		ZION_ASSERT(hep);
		CConnectionServer* pServer = (CConnectionServer*)KeyOf(hep);
		ZION_ASSERT(pServer);
		return pServer->OnConnected(hConn);
	}

	void CS_ON_DISCONNECT(HCONNECT hConn)
	{
		CConnectionClient* pConn = (CConnectionClient*)KeyOf(hConn);
		ZION_ASSERT(pConn);
		pConn->OnRawDisconnected();
		CloseConn(hConn);
	}

	void CS_ON_DATA(HCONNECT hConn, _U32 len, const _U8* data)
	{
		CConnectionClient* pConn = (CConnectionClient*)KeyOf(hConn);
		ZION_ASSERT(pConn);
		pConn->OnRawData(len, data);
	}

}
