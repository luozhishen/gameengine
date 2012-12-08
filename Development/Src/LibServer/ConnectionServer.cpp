#include <AtlasBase.h>

#include "ServerApp.h"
#include "ServerBase.h"
#include "ConnectionServer.h"

namespace Atlas
{

	CConnectionServer::CConnectionServer(CServerApp* pServerApp) : CServerBase(pServerApp)
	{
	}

	CConnectionServer::~CConnectionServer()
	{
	}

	void CConnectionServer::SetEP(const SOCKADDR& sa)
	{
		m_saAddr = sa;
	}

	bool CConnectionServer::Start()
	{
		m_hPool = AllocIoBufferPool(500, 500, 0, 0);
		return true;
	}

	void CConnectionServer::Stop()
	{
		if(m_hPool) FreeIoBufferPool(m_hPool);
		m_hPool = NULL;
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
	}

	CConnectionClient::~CConnectionClient()
	{
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
		if(m_hConn) Atlas::Disconnect(m_hConn);
	}

	void CConnectionClient::OnRawConnected()
	{
	}

	void CConnectionClient::OnRawDisconnected()
	{
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

}
