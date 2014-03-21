#include <ZionBase.h>
#include <ZionCommon.h>
#include "ZionClient.h"
#include "ZionClientApp.h"
#include "ClientConnection.h"
#include "NonblockConnection.h"

namespace Zion
{

	CNonblockConnection::CNonblockConnection(CClient* pClient, _U32 recvsize) : CClientConnection(pClient, recvsize)
	{
		m_hSocket = ASOCK_INVALID_HANDLE;
	}

	CNonblockConnection::~CNonblockConnection()
	{
	}

	void CNonblockConnection::Tick()
	{
		if(m_hSocket==ASOCK_INVALID_HANDLE) return;

		if(m_bConnecting)
		{
			if(sock_wait_error(m_hSocket, 0)==1)
			{
				Clear();
				OnRawConnectFailed();
				return;
			}

			if(sock_wait_write(m_hSocket, 0)==1)
			{
				m_bConnecting = false;
				OnRawConnected();
			}
		}

		if(!m_bConnecting)
		{
			int len;
			_U8 buf[1000];
			while(!m_bNeedDisconnect)
			{
				len = sock_read(m_hSocket, buf, sizeof(buf));
				if(len==0) break;
				if(len>0)
				{
					OnRawData((_U32)len, buf);
				}
				else
				{
					m_bNeedDisconnect = true;
				}
			}

			while(!m_bNeedDisconnect && !m_SendQueue.empty() && sock_wait_write(m_hSocket, 0)==1)
			{
				len = sock_write(m_hSocket, m_SendQueue.front().first+m_nSendLen, m_SendQueue.front().second-m_nSendLen);
				if(len<=0) break;
				m_nSendLen += (_U32)len;
				if(m_SendQueue.front().second==m_nSendLen)
				{
					ZION_FREE(m_SendQueue.front().first);
					m_SendQueue.erase(m_SendQueue.begin());
					m_nSendLen = 0;
				}								
			}
		}

		if(m_bNeedDisconnect)
		{
			Clear();
			OnRawDisconnected();
		}
	}

	bool CNonblockConnection::Connect(const ASOCK_ADDR& sa)
	{
		ZION_ASSERT(m_hSocket==ASOCK_INVALID_HANDLE);
		if(m_hSocket!=ASOCK_INVALID_HANDLE) return false;
		Clear();
		m_hSocket = sock_connect(&sa, ASOCK_NONBLOCK);
		if(m_hSocket==ASOCK_INVALID_HANDLE) return false;
		m_bNeedDisconnect = false;
		m_bConnecting = true;
		return true;
	}

	void CNonblockConnection::Disconnect()
	{
		if(m_hSocket==ASOCK_INVALID_HANDLE) return;
		if(m_bConnecting) return;
		m_bNeedDisconnect = true;
	}

	void CNonblockConnection::SendData(_U32 len, const _U8* data, bool bPending)
	{
		if(len==0) return;
		_U8* mdata = (_U8*)ZION_ALLOC(len);
		memcpy(mdata, data, len);
		m_SendQueue.push_back(std::pair<_U8*, _U32>(mdata, len));
	}

	void CNonblockConnection::Clear()
	{
		while(!m_SendQueue.empty())
		{
			ZION_FREE((*m_SendQueue.begin()).first);
			m_SendQueue.erase(m_SendQueue.begin());
		}
		m_nSendLen = 0;

		if(m_hSocket!=ASOCK_INVALID_HANDLE)
		{
			sock_close(m_hSocket);
			m_hSocket = ASOCK_INVALID_HANDLE;
		}
	}

}
