#include <stdarg.h>

#include "AtlasBase.h"
#include "AtlasCommon.h"
#include "AtlasClientApp.h"
#include "AtlasClient.h"

namespace Atlas
{

	CClient::CClient(CClientApp* pClientApp, _U32 recvsize) : m_pClientApp(pClientApp)
	{
		A_MUTEX_INIT(&m_mtxClient);
	}

	CClient::~CClient()
	{
		while(!m_Components.empty())
		{
			ATLAS_DELETE *m_Components.begin();
			m_Components.erase(m_Components.begin());
		}

		A_MUTEX_DESTROY(&m_mtxClient);
	}

	bool CClient::Login(const char* pUsername, const char* pPassword, const char* pUrl)
	{
		return false;
	}

	bool CClient::Login(const char* pDevice, const char* pUrl)
	{
		return false;
	}

	bool CClient::LoginForStress(_U32 id, const char* pUrl)
	{
		return false;
	}

	bool CClient::Logout()
	{
		return m_pConnectionComponent->Disconnect();
	}

	void CClient::SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		m_pConnectionComponent->SendData(iid, fid, len, data);
	}

	CClient::CLIENT_STATE CClient::GetState()
	{
		return m_nState;
	}

	_U32 CClient::GetErrCode()
	{
		return m_nErrCode;
	}

	void CClient::SetErrCode(_U32 errcode)
	{
		if(m_nErrCode==0) m_nErrCode = errcode;
	}

	void CClient::AddComponent(CClientComponent* pComponent)
	{
		m_Components.push_back(pComponent);
	}

	void CClient::InitializeComponents()
	{
	}

	void CClient::ProcessPacket(_U32 len, const _U8* data)
	{
		if(m_nState==CClient::STATE_LOGINING)
		{
			_U8 code;
			if(len<sizeof(code))
			{
				m_nErrCode = CClient::ERRCODE_UNKOWN;
				m_pConnectionComponent->Disconnect();
				return;
			}

			code = *data;
			switch(code)
			{
			case 0x0: // login completed
				m_nState = CClient::STATE_LOGINED;
				break;
			case 0x1: // redirect
				if(len<1+sizeof(SOCKADDR)+1 || data[len-1]!=0)
				{
					SetErrCode(CClient::ERRCODE_UNKOWN);
				}
				else
				{
					m_pConnectionComponent->Redirect(*((const SOCKADDR*)(data+1)), (const char*)(data+1+sizeof(SOCKADDR)));
				}
				m_pConnectionComponent->Disconnect();
				break;
			default:
				ATLAS_ASSERT(0);
				SetErrCode(CClient::ERRCODE_UNKOWN);
				m_pConnectionComponent->Disconnect();
				break;
			}
		}
		else if(m_nState==CClient::STATE_LOGINED)
		{
			_U16 id;
			if(len<sizeof(id))
			{
				m_pConnectionComponent->Disconnect();
				return;
			}

			id = *((const _U16*)data);
			GetClientApp()->QueueData(this, id&0xff, id>>8, len-sizeof(id), data+sizeof(id));
		}
		else
		{
			ATLAS_ASSERT(!"invalid client state");
		}
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

	CClientComponent::CClientComponent(CClient* pClient) : m_pClient(pClient)
	{
	}

	CClientComponent::~CClientComponent()
	{
	}

	CClientConnection::CClientConnection(CClient* pClient) : CClientComponent(pClient)
	{
		m_bRedirect = false;
	}

	CClientConnection::~CClientConnection()
	{
	}

	void CClientConnection::Redirect(const SOCKADDR& sa, const char* token)
	{
		m_bRedirect = true;
		m_saRedirectAddr = sa;
		m_sRedirectToken = token;
	}

}
