#include <ZionBase.h>
#include <ZionCommon.h>
#include <mosdk.h>
#include "ZionClientApp.h"
#include "ZionClient.h"
#include "ClientConnection.h"
#include "AsyncIOConnection.h"
#include "NonblockConnection.h"
#include "HttpConnection.h"
#include "ZionClientLogin.h"
#include "DataSyncClient.h"

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

		m_pDataSync = new CDataSyncClient(this);

		AddComponent(m_pClientConnection);
		AddComponent(m_pDataSync);
	}

	CClient::~CClient()
	{
		Zion::Map<_U16, DDLStub::IStub*>::iterator i;
		for(i=m_DDLStubs.begin(); i!=m_DDLStubs.end(); i++)
		{
			delete i->second;
		}
		m_DDLStubs.clear();

		m_pClientConnection = NULL;
		m_pDataSync = NULL;
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
	
	CDataSyncClient* CClient::GetDataSync()
	{
		return m_pDataSync;
	}

	void CClient::SetLogCallback(CClient::LOG_CALLBACK logproc)
	{
		m_LogCallback = logproc;
	}

	CClient::LOG_CALLBACK CClient::GetLogCallback()
	{
		return m_LogCallback;
	}

	bool CClient::Login(const char* pUrl, const CClientLoginMethod* pMethod)
	{
		if(m_pClientConnection->GetState()!=STATE_NA && m_pClientConnection->GetState()!=STATE_FAILED) return false;
		if(!pUrl) pUrl = m_pClientApp->GetParam("ServerUrl");
		if(!pUrl) return false;
		return m_pClientConnection->Login(pUrl, pMethod);
	}

	bool CClient::LoginByDevice(const char* pUrl)
	{
		CClientLoginMethodByDevice Method;
		return Login(pUrl, &Method);
	}

	bool CClient::LoginForStress(_U32 id)
	{
		const char* pServerUrl = m_pClientApp->GetParam("ServerUrl");
		if(!pServerUrl) return false;
		const char* uid_base = m_pClientApp->GetParam("UidBase", "0");

		char token[1000];
#ifdef _WIN32
		char comname[100];
		DWORD len = sizeof(comname);
		GetComputerName(comname, &len);
		sprintf(token, "@%d@%s", id+atoi(uid_base), comname);
#else
		sprintf(token, "@%d@XXXX", id+atoi(uid_base));
#endif

		CClientLoginMethodByDevice Method;
		Method.SetDeviceID(token);
		return Login(pServerUrl, &Method);
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

	void CClient::RegisterStub(DDLStub::IStub* pStub)
	{
		_U16 iid = GetClientStubID(pStub->GetClassInfo());
		ZION_ASSERT(iid!=(_U16)-1);
		DDLStub::CDispatcher::RegisterStub(iid, pStub);
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
		DDLStub::CDispatcher::Dispatch(iid, fid, len, data);
	}

	bool CClient::SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		if(GetState()!=STATE_LOGINED) return false;
		m_pClientConnection->SendData(iid, fid, len, data);
		return true;
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
