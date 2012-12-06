#include <stdarg.h>

#include "AtlasBase.h"
#include "AtlasCommon.h"
#include "AtlasClient.h"

namespace Atlas
{

	CClient::CClient(CClientApp* pClientApp, _U32 recvsize) : m_pClientApp(pClientApp)
	{
		m_pConnectionComponent = NULL;
	}

	CClient::~CClient()
	{
		while(!m_Components.empty())
		{
			ATLAS_DELETE *m_Components.begin();
			m_Components.erase(m_Components.begin());
		}
	}

	bool CClient::Login(const char* pURL, const char* pUsername, const char* pPassword)
	{
		return m_pConnectionComponent->Login(pURL, pUsername, pPassword);
	}

	bool CClient::Login(const char* pURL, const char* pDevice)
	{
		return m_pConnectionComponent->Login(pURL, pDevice);
	}

	bool CClient::LoginForStress(const char* pURL, _U32 id)
	{
		std::string username = StringFormat("U%u", id);
		return m_pConnectionComponent->Login(pURL, username.c_str(), "password");
	}

	bool CClient::Logout()
	{
		return m_pConnectionComponent->Logout();
	}

	void CClient::SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		m_pConnectionComponent->SendData(iid, fid, len, data);
	}

	CClient::CLIENT_STATE CClient::GetClientState()
	{
		return m_pConnectionComponent->GetState();
	}

	_U32 CClient::GetLoginErrorCode()
	{
		return m_pConnectionComponent->GetErrorCode();
	}

	void CClient::AddComponent(CClientComponent* pComponent)
	{
		m_Components.push_back(pComponent);
	}

	void CClient::InitializeComponents()
	{
	}

	void CClient::SetConnectionComponent(CClientConnectionComponent* pConnectionComponent)
	{
		m_pConnectionComponent = pConnectionComponent;
	}

	void CClient::OnConnected()
	{
		_OnConnected();
	}

	void CClient::OnDisconnect()
	{
		_OnDisconnect();
	}

	void CClient::OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		_OnData(iid, fid, len, data);
	}

	void CClient::OnConnectFailed()
	{
		_OnConnectFailed();
	}

	void CClient::OnLoginDone()
	{
		_OnLoginDone();
	}

	CClientComponent::CClientComponent(CClient* pClient) : m_pClient(pClient)
	{
	}

	CClientComponent::~CClientComponent()
	{
	}

	CClientConnectionComponent::CClientConnectionComponent(CClient* pClient)
	{
	}

	CClientConnectionComponent::~CClientConnectionComponent()
	{
	}

}
