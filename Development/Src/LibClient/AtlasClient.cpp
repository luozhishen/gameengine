#include <stdarg.h>

#include "AtlasBase.h"
#include "AtlasCommon.h"
#include "AtlasClientApp.h"
#include "AtlasClient.h"
#include "ClientConnection.h"
#include "AsyncIOConnection.h"
#include "NonblockConnection.h"

namespace Atlas
{

	CClient::CClient(CClientApp* pClientApp, _U32 recvsize) : m_pClientApp(pClientApp)
	{
		pClientApp->RegisterClient(this);
		A_MUTEX_INIT(&m_mtxClient);
#ifndef WITHOUT_ASYNCIO
		m_pClientConnection = ATLAS_NEW CNonblockConnection(this, recvsize);
//		m_pClientConnection = ATLAS_NEW CAsyncIOConnection(this, recvsize);
#else
		m_pClientConnection = ATLAS_NEW CNonblockConnection(this, recvsize);
#endif
		AddComponent(m_pClientConnection);
	}

	CClient::~CClient()
	{
		while(!m_Components.empty())
		{
			ATLAS_DELETE *m_Components.begin();
			m_Components.erase(m_Components.begin());
		}
		A_MUTEX_DESTROY(&m_mtxClient);
		m_pClientApp->UnregisterClient(this);
	}

	CClient::CLIENT_STATE CClient::GetState()
	{
		return m_pClientConnection->GetState();
	}

	_U32 CClient::GetErrorCode()
	{
		return m_pClientConnection->GetErrorCode();
	}

	bool CClient::Login(const SOCK_ADDR& sa, _U32 nUID, const char* pToken)
	{
		if(m_pClientConnection->GetState()!=STATE_NA && m_pClientConnection->GetState()!=STATE_FAILED) return false;
		return m_pClientConnection->Login(sa, nUID, pToken);
	}

	bool CClient::LoginForStress(_U32 id)
	{
		const char* addr = m_pClientApp->GetParam("svraddr", "127.0.0.1:1978");

		SOCK_ADDR sa;
		if(!sock_str2addr(addr, &sa)) return false;
		const char* uid_base = m_pClientApp->GetParam("uid_base", "0");
		return Login(sa, id+atoi(uid_base));
	}

	void CClient::Logout()
	{
		m_pClientConnection->Logout();
	}

	void CClient::AddComponent(CClientComponent* pComponent)
	{
		m_Components.push_back(pComponent);
	}

	void CClient::InitializeComponents()
	{
	}

	void CClient::Tick()
	{
		std::list<CClientComponent*>::iterator i;
		for(i=m_Components.begin(); i!=m_Components.end(); i++)
		{
			(*i)->Tick();
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

	void CClient::SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		m_pClientConnection->SendData(iid, fid, len, data);
	}

	CClientComponent::CClientComponent(CClient* pClient) : m_pClient(pClient)
	{
	}

	CClientComponent::~CClientComponent()
	{
	}

	CClientConnectionBase::CClientConnectionBase(CClient* pClient) : CClientComponent(pClient)
	{
		m_nErrCode = CClient::ERRCODE_SUCCESSED;
		m_nState = CClient::STATE_NA;
	}

	CClientConnectionBase::~CClientConnectionBase()
	{
	}

	CClient::CLIENT_STATE CClientConnectionBase::GetState()
	{
		return m_nState;
	}

	_U32 CClientConnectionBase::GetErrorCode()
	{
		return m_nErrCode;
	}

	void CClientConnectionBase::SetErrorCode(_U32 errcode)
	{
		if(m_nErrCode!=CClient::ERRCODE_SUCCESSED) m_nErrCode = errcode;
	}

}
