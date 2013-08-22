#include <stdarg.h>

#include "ZionBase.h"
#include "ZionCommon.h"
#include "ZionClientApp.h"
#include "ZionClient.h"
#include "ClientConnection.h"
#include "AsyncIOConnection.h"
#include "NonblockConnection.h"
#include "HttpConnection.h"

namespace Zion
{

	CClient::CClient(CClientApp* pClientApp, _U32 recvsize) : m_pClientApp(pClientApp)
	{
		pClientApp->RegisterClient(this);
		A_MUTEX_INIT(&m_mtxClient);

		m_ClientConnectionType = m_pClientApp->GetParam("ConnectType");
		if(m_ClientConnectionType=="nonblock")
		{
			m_pClientConnection = ZION_NEW CNonblockConnection(this, recvsize);
		}
		else if(m_ClientConnectionType=="http")
		{
			m_pClientConnection = ZION_NEW CHttpConnection(this);
		}
		else if(m_ClientConnectionType=="async")
		{
			m_pClientConnection = ZION_NEW CAsyncIOConnection(this, recvsize);
		}
		else
		{
			ZION_ASSERT(0);
		}

		AddComponent(m_pClientConnection);
	}

	CClient::~CClient()
	{
		while(!m_Components.empty())
		{
			ZION_DELETE *m_Components.begin();
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

	const Zion::String& CClient::GetClientConnectionType()
	{
		return m_ClientConnectionType;
	}
	
	CClientConnectionBase* CClient::GetClientConnection()
	{
		return m_pClientConnection;
	}

	void CClient::SetLogCallback(CClient::LOG_CALLBACK logproc)
	{
		m_LogCallback = logproc;
	}

	CClient::LOG_CALLBACK CClient::GetLogCallback()
	{
		return m_LogCallback;
	}

	bool CClient::Login(const char* pUrl, const char* pToken)
	{
		if(m_pClientConnection->GetState()!=STATE_NA && m_pClientConnection->GetState()!=STATE_FAILED) return false;
		if(!pUrl) pUrl = m_pClientApp->GetParam("ServerUrl");
		if(!pUrl) return false;
		return m_pClientConnection->Login(pUrl, pToken);
	}

	bool CClient::LoginForStress(_U32 id)
	{
		const char* pServerUrl = m_pClientApp->GetParam("ServerUrl");
		if(!pServerUrl) return false;
		const char* uid_base = m_pClientApp->GetParam("UidBase", "0");
		char token[1000];
		sprintf(token, "%d", id+atoi(uid_base));
		return Login(pServerUrl, token);
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
		Zion::List<CClientComponent*>::iterator i;
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
		if(GetState()!=STATE_LOGINED) return;
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
		if(m_nErrCode==CClient::ERRCODE_SUCCESSED) m_nErrCode = errcode;
	}

}
