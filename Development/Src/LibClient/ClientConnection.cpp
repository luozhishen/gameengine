#include <ZionBase.h>
#include <ZionCommon.h>
#include "ZionClientApp.h"
#include "ZionClient.h"
#include "ClientConnection.h"
#include "ZionClientLogin.h"

namespace Zion
{

	CClientConnection::CClientConnection(CClient* pClient, _U32 recvsize) : CClientConnectionBase(pClient)
	{
		m_nLoginDataSize = 0;
		m_nState = CClient::STATE_NA;
		m_nErrCode = CClient::ERRCODE_SUCCESSED;
		m_pRecvBuff = (_U8*)ZION_ALLOC(recvsize);
		m_nRecvBuffLen = 0;
		m_nRecvBuffSize = recvsize;
		m_bNeedRedirect = false;
	}

	CClientConnection::~CClientConnection()
	{
		ZION_FREE(m_pRecvBuff);
	}

	bool CClientConnection::Login(const char* pUrl, const CClientLoginMethod* pMethod)
	{
		ASOCK_ADDR sa;
		if(!sock_str2addr(pUrl, &sa))
		{
			m_nErrCode = CClient::ERRCODE_UNKOWN;
			return false;
		}
		if(pMethod->GetType()!=CClientLoginMethodByToken::METHOD_TYPE)
		{
			m_nErrCode = CClient::ERRCODE_UNKOWN;
			return false;
		}

		const Zion::String& token = ((const CClientLoginMethodByToken*)pMethod)->GetToken();

		m_nErrCode = CClient::ERRCODE_SUCCESSED;
		_U16 len = (_U16)token.size()+1;
		*((_U16*)(m_LoginData)) = len;
		memcpy(m_LoginData+sizeof(_U16), token.c_str(), len);
		m_nLoginDataSize = sizeof(_U16) + len;

		m_nState = CClient::STATE_LOGINING;
		if(!Connect(sa))
		{
			m_nState = CClient::STATE_FAILED;
			SetErrorCode(CClient::ERRCODE_NETWORK);
			return false;
		}
		return true;
	}

	void CClientConnection::Logout()
	{
		switch(m_nState)
		{
		case CClient::STATE_NA:
		case CClient::STATE_FAILED:
			return;
		case CClient::STATE_LOGINED:
		case CClient::STATE_LOGINING:
			Disconnect();
			break;
		default:
			ZION_ASSERT(0);
		}
	}

	void CClientConnection::SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		_U8 header[4];
		*((_U16*)(header+0)) = (_U16)(sizeof(_U16) + len);
		*((_U16*)(header+2)) = iid | (fid<<8);
		SendData(sizeof(header), header, true);
		SendData(len, data, false);
	}

	void CClientConnection::OnRawConnected()
	{
		SendData(m_nLoginDataSize, m_LoginData);
	}

	void CClientConnection::OnRawData(_U32 len, const _U8* data)
	{
		_U16 pkglen;
		while(len>0 && GetClient()->GetClientApp()->IsEnableTick())
		{
			_U32 copylen = len;
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
					SetErrorCode(CClient::ERRCODE_UNKOWN);
					Disconnect();
					return;
				}

				if(pkglen+sizeof(pkglen)>m_nRecvBuffLen) break;

				ProcessPacket(pkglen, m_pRecvBuff+sizeof(pkglen));

				_U32 ulen = pkglen + sizeof(pkglen);
				m_nRecvBuffLen -= ulen;
				memmove(m_pRecvBuff, m_pRecvBuff+ulen, m_nRecvBuffLen);
			}
		}
	}

	void CClientConnection::OnRawDisconnected()
	{
		if(m_bNeedRedirect)
		{
			m_bNeedRedirect = false;
			if(!Connect(m_saRedirectAddr))
			{
				m_nState = CClient::STATE_FAILED;
				m_nErrCode = CClient::ERRCODE_NETWORK;
				return;	//connect session failed
			}
		}
		else
		{
			ZION_ASSERT(m_nState!=CClient::STATE_NA);
			if(m_nState==CClient::STATE_LOGINED)
			{
				m_nState = CClient::STATE_NA;
				GetClient()->GetClientApp()->QueueDisconnected(GetClient());
			}
			else
			{
				SetErrorCode(CClient::ERRCODE_NETWORK);
				m_nState = CClient::STATE_FAILED;
				GetClient()->GetClientApp()->QueueLoginFailed(GetClient());
			}
		}
	}

	void CClientConnection::OnRawConnectFailed()
	{
		m_nState = CClient::STATE_FAILED;
		SetErrorCode(CClient::ERRCODE_NETWORK);
		GetClient()->GetClientApp()->QueueLoginFailed(GetClient());
	}

	void CClientConnection::ProcessPacket(_U32 len, const _U8* data)
	{
		if(m_nState==CClient::STATE_LOGINING)
		{
			_U16 code;
			if(len<sizeof(code))
			{
				SetErrorCode(CClient::ERRCODE_UNKOWN);
				Disconnect();
				return;
			}

			code = *data;
			switch(code)
			{
			case 0x0: // login completed
				m_nState = CClient::STATE_LOGINED;
				GetClient()->GetClientApp()->QueueLoginDone(GetClient());
				break;
			case 0x1: // redirect
				if(len<sizeof(code)+sizeof(ASOCK_ADDR))
				{
					SetErrorCode(CClient::ERRCODE_UNKOWN);
				}
				else
				{
					m_saRedirectAddr = *((const ASOCK_ADDR*)(data+sizeof(code)));
					m_nLoginDataSize = (_U16)len - sizeof(code) - sizeof(ASOCK_ADDR) + sizeof(_U16);
					if(m_nLoginDataSize>sizeof(m_LoginData))
					{
						SetErrorCode(CClient::ERRCODE_UNKOWN);
					}
					else
					{
						*((_U16*)m_LoginData) = (_U16)m_nLoginDataSize - sizeof(_U16);
						memcpy(m_LoginData+sizeof(_U16), data+sizeof(code)+sizeof(ASOCK_ADDR), (size_t)m_nLoginDataSize);
						m_bNeedRedirect = true;
					}
				}
				Disconnect();
				break;
			default:
				ZION_ASSERT(0);
				SetErrorCode(CClient::ERRCODE_UNKOWN);
				Disconnect();
				break;
			}
		}
		else if(m_nState==CClient::STATE_LOGINED)
		{
			_U16 id;
			if(len<sizeof(id))
			{
				SetErrorCode(CClient::ERRCODE_UNKOWN);
				Disconnect();
			}
			else
			{
				id = *((const _U16*)data);
				GetClient()->GetClientApp()->QueueData(GetClient(), id&0xff, id>>8, len-sizeof(id), data+sizeof(id));
			}
		}
		else
		{
			ZION_ASSERT(!"invalid client state");
		}
	}

}
