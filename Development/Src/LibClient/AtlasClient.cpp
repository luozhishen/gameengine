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
		A_MUTEX_DESTROY(&m_mtxClient);
	}

	bool CClient::Login(const char* pUsername, const char* pPassword, const char* pUrl)
	{
		return m_pConnectionComponent->Login(pUsername, pPassword, pUrl);
	}

	bool CClient::Login(const char* pDevice, const char* pUrl)
	{
		return m_pConnectionComponent->Login(pDevice, pUrl);
	}

	bool CClient::LoginForStress(_U32 id, const char* pUrl)
	{
		std::string uname = StringFormat("U%u", id);
		return m_pConnectionComponent->Login(uname.c_str(), "password", pUrl);
	}

	bool CClient::Logout()
	{
		return m_pConnectionComponent->Logout();
	}

	CClient::CLIENT_STATE CClient::GetState()
	{
		return m_pConnectionComponent->GetState();
	}

	_U32 CClient::GetErrCode()
	{
		return m_pConnectionComponent->GetErrCode();
	}

	void CClient::SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		m_pConnectionComponent->SendData(iid, fid, len, data);
	}

	void CClient::AddComponent(CClientComponent* pComponent)
	{
		m_Components.push_back(pComponent);
	}

	void CClient::InitializeComponents()
	{
	}
	
	void CClient::OnConnected()
	{
		_OnConnect();
	}

	void CClient::OnDisconnected()
	{
		_OnDisconnected();
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

	CClientConnectionComponent::CClientConnectionComponent(CClient* pClient) : CClientComponent(pClient)
	{
	}

	CClientConnectionComponent::~CClientConnectionComponent()
	{
	}

	CClient::CLIENT_STATE CClientConnectionComponent::GetState()
	{
		return m_nState;
	}

	_U32 CClientConnectionComponent::GetErrCode()
	{
		return m_nErrCode;
	}

}
