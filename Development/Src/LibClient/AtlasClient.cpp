#include <stdarg.h>

#include "AtlasBase.h"
#include "AtlasCommon.h"
#include "AtlasClientApp.h"
#include "AtlasClient.h"
#include "AsyncIOConnection.h"

namespace Atlas
{

	CClient::CClient(CClientApp* pClientApp, _U32 recvsize) : m_pClientApp(pClientApp)
	{
		m_nLoginDataSize = 0;
		m_nState = STATE_NA;
		m_nErrCode = ERRCODE_SUCCESSED;
		m_pClientConnection = NULL;
		A_MUTEX_INIT(&m_mtxClient);
		m_pRecvBuff = (_U8*)ATLAS_ALLOC(recvsize);
		m_nRecvBuffLen = 0;
		m_nRecvBuffSize = recvsize;
		m_bNeedRedirect = false;

		m_pClientConnection = ATLAS_NEW CAsyncIOConnection(this);
		AddComponent(m_pClientConnection);
	}

	CClient::~CClient()
	{
		while(!m_Components.empty())
		{
			ATLAS_DELETE *m_Components.begin();
			m_Components.erase(m_Components.begin());
		}

		ATLAS_FREE(m_pRecvBuff);
		A_MUTEX_DESTROY(&m_mtxClient);
	}

	void CClient::SetErrorCode(_U32 errcode)
	{
		if(m_nErrCode==ERRCODE_SUCCESSED) m_nErrCode = errcode;
	}

	bool CClient::Login(const SOCKADDR& sa, _U32 nUID, const char* pToken)
	{
		if(m_nState!=STATE_NA && m_nState!=STATE_FAILED) return false;

		m_nErrCode = ERRCODE_SUCCESSED;
		_U16 len = (_U16)strlen(pToken)+1;
		*((_U16*)(m_LoginData)) = sizeof(nUID) + len;
		*((_U32*)(m_LoginData+sizeof(_U16))) = nUID;
		memcpy(m_LoginData+sizeof(_U16)+sizeof(nUID), pToken, len);
		m_nLoginDataSize = sizeof(_U16)+sizeof(nUID) + len;

		m_nState = STATE_LOGINING;
		if(!m_pClientConnection->Connect(sa))
		{
			m_nState = STATE_FAILED;
			SetErrorCode(ERRCODE_NETWORK);
			return false;
		}

		return true;
	}

	bool CClient::LoginForStress(_U32 id)
	{
		const char* addr = m_pClientApp->GetParam("svraddr", "127.0.0.1:1978");

		Atlas::SOCKADDR sa;
		if(!Atlas::STR2ADDR((char*)addr, sa)) return false;
		const char* uid_base = m_pClientApp->GetParam("uid_base", "0");
		return Login(sa, id+atoi(uid_base));
	}

	void CClient::Logout()
	{
		if(m_nState == STATE_LOGINED)
		{
			m_pClientConnection->Disconnect();
		}
	}

	void CClient::AddComponent(CClientComponent* pComponent)
	{
		m_Components.push_back(pComponent);
	}

	void CClient::InitializeComponents()
	{
	}
	
	void CClient::OnLoginDone()
	{
		_OnLoginDone();
	}

	void CClient::OnLoginFailed()
	{
		_OnLoginFailed();
	}

	void CClient::OnDisconnected()
	{
		_OnDisconnected();
	}

	void CClient::OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		_OnData(iid, fid, len, data);
	}

	void CClient::SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		_U16 code =  iid | (fid<<8);
		_U16 datalen = sizeof(code) + (_U16)len;
		m_pClientConnection->SendData(sizeof(datalen), (const _U8*)&datalen, true);
		m_pClientConnection->SendData(sizeof(code), (const _U8*)&code, true);
		m_pClientConnection->SendData(len, data);
	}

	void CClient::OnRawConnected()
	{
		m_pClientConnection->SendData(m_nLoginDataSize, m_LoginData);
	}

	void CClient::OnRawData(_U32 len, const _U8* data)
	{
		_U16 pkglen;
		while(len>0 && m_pClientApp->IsEnableTick())
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
					SetErrorCode(ERRCODE_UNKOWN);
					m_pClientConnection->Disconnect();
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

	void CClient::OnRawDisconnected()
	{
		if(m_bNeedRedirect)
		{
			m_bNeedRedirect = false;
			if(!m_pClientConnection->Connect(m_saRedirectAddr))
			{
				m_nState = STATE_FAILED;
				m_nErrCode = ERRCODE_NETWORK;
				return;	//connect session failed
			}
		}
		else
		{
			ATLAS_ASSERT(m_nState!=STATE_NA);
			if(m_nState==STATE_LOGINED)
			{
				m_nState = STATE_NA;
				GetClientApp()->QueueDisconnected(this);
			}
			else
			{
				SetErrorCode(ERRCODE_NETWORK);
				m_nState = STATE_FAILED;
				GetClientApp()->QueueLoginFailed(this);
			}
		}
	}

	void CClient::OnRawConnectFailed()
	{
		m_nState = STATE_FAILED;
		SetErrorCode(ERRCODE_NETWORK);
		m_pClientApp->QueueLoginFailed(this);
	}

	void CClient::ProcessPacket(_U32 len, const _U8* data)
	{
		if(m_nState==CClient::STATE_LOGINING)
		{
			_U16 code;
			if(len<sizeof(code))
			{
				SetErrorCode(CClient::ERRCODE_UNKOWN);
				m_pClientConnection->Disconnect();
				return;
			}

			code = *data;
			switch(code)
			{
			case 0x0: // login completed
				m_nState = CClient::STATE_LOGINED;
				GetClientApp()->QueueLoginDone(this);
				break;
			case 0x1: // redirect
				if(len<sizeof(code)+sizeof(SOCKADDR))
				{
					SetErrorCode(CClient::ERRCODE_UNKOWN);
				}
				else
				{
					m_saRedirectAddr = *((const SOCKADDR*)(data+sizeof(code)));
					m_nLoginDataSize = (_U16)len - sizeof(code) - sizeof(SOCKADDR) + sizeof(_U16);
					if(m_nLoginDataSize>sizeof(m_LoginData))
					{
						SetErrorCode(CClient::ERRCODE_UNKOWN);
					}
					else
					{
						*((_U16*)m_LoginData) = (_U16)m_nLoginDataSize - sizeof(_U16);
						memcpy(m_LoginData+sizeof(_U16), data+sizeof(code)+sizeof(SOCKADDR), (size_t)m_nLoginDataSize);
						m_bNeedRedirect = true;
					}
				}
				m_pClientConnection->Disconnect();
				break;
			default:
				ATLAS_ASSERT(0);
				SetErrorCode(CClient::ERRCODE_UNKOWN);
				m_pClientConnection->Disconnect();
				break;
			}
		}
		else if(m_nState==CClient::STATE_LOGINED)
		{
			_U16 id;
			if(len<sizeof(id))
			{
				SetErrorCode(ERRCODE_UNKOWN);
				m_pClientConnection->Disconnect();
			}
			else
			{
				id = *((const _U16*)data);
				GetClientApp()->QueueData(this, id&0xff, id>>8, len-sizeof(id), data+sizeof(id));
			}
		}
		else
		{
			ATLAS_ASSERT(!"invalid client state");
		}
	}

	CClientComponent::CClientComponent(CClient* pClient) : m_pClient(pClient)
	{
	}

	CClientComponent::~CClientComponent()
	{
	}

	CClientConnection::CClientConnection(CClient* pClient) : CClientComponent(pClient)
	{
	}

	CClientConnection::~CClientConnection()
	{
	}

}
