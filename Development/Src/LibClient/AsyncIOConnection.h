#ifndef __ASYNCIO_CONNECTION__
#define __ASYNCIO_CONNECTION__

#ifndef WITHOUT_ASYNCIO

namespace Atlas
{

	class CAsyncIOConnection : public CClientConnection
	{
	public:
		static void Init(_U32 tcount=1);
		static void Fini();

		CAsyncIOConnection(CClient* pClient);
		virtual ~CAsyncIOConnection();

		virtual bool Connect(const SOCKADDR& sa, const char* token);
		virtual bool Disconnect();
		virtual void SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data);

		void OnRawConnected(HCONNECT hConn);
		void OnRawDisconnected();
		void OnRawData(_U32 len, const _U8* data);
		void OnRawConnectFailed();

	private:
		HCONNECT m_hConn;
		_U8* m_pRecvBuff;
		_U32 m_nRecvBuffLen;
		_U32 m_nRecvBuffSize;
	};

}

#endif

#endif

/*

#include <stdarg.h>

#include "AtlasBase.h"
#include "AtlasCommon.h"
#include "AtlasClientApp.h"
#include "AtlasClient.h"

namespace Atlas
{

	CClient::CClient(CClientApp* pClientApp, _U32 recvsize) : m_pClientApp(pClientApp)
	{
		m_nLoginDataSize = 0;
		m_nClientState = CLIENT_NA;
		m_hConnect = NULL;
		A_MUTEX_INIT(&m_mtxClient);
		if(recvsize)
		{
			m_pRecvBuff = (_U8*)ATLAS_ALLOC(recvsize);
			m_nRecvBuffLen = 0;
			m_nRecvBuffSize = recvsize;
		}
		else
		{
			m_pRecvBuff = NULL;
		}
		m_pLogOutput = NULL;

		m_bNeedRedirect = true;
		m_strSessionAddr.clear();

		m_pDataSyncComponent = NULL;
	}

	CClient::~CClient()
	{
		ATLAS_FREE(m_pRecvBuff);
		A_MUTEX_DESTROY(&m_mtxClient);
	}

	bool CClient::Login(const SOCKADDR& sa, _U32 nUID, const char* pToken)
	{
		if(m_nClientState!=CLIENT_NA) return false;

		_U16 len = (_U16)strlen(pToken)+1;
		*((_U32*)m_LoginData) = nUID;
		memcpy(m_LoginData+sizeof(nUID), pToken, len);
		m_nLoginDataSize = sizeof(nUID) + len;

		ASOCKIO_HANDLER handler =
		{
			CLT_ON_CONNECT,
			CLT_ON_DISCONNECT,
			CLT_ON_DATA,
			CLT_ON_CONNECTFAILED,
		};
		m_nClientState = CLIENT_LOGINING;
		if(!Connect(sa, handler, m_pClientApp->GetIOPool(), m_pClientApp->GetHWorkers(), this))
		{
			m_nClientState = CLIENT_NA;
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
		if(m_hConnect && m_nClientState == CLIENT_LOGINED)
		{
			Disconnect(m_hConnect);
		}
		else
		{
			m_nClientState = CLIENT_NA;
		}
	}

	void CClient::AddComponent(CClientComponent* pComponent)
	{
		m_Components.push_back(pComponent);
	}

	void CClient::InitializeComponents()
	{
	}

	CClientDataSyncComponent* CClient::GetDataSyncComponent()
	{
		return m_pDataSyncComponent;
	}

	void CClient::OnConnect()
	{
		_OnConnect();
	}

	void CClient::OnDisconnect()
	{
		_OnDisconnect();
	}

	void CClient::OnData(_U16 id, _U32 len, const _U8* data)
	{
		_OnData(id, len, data);
	}

	void CClient::OnConnectFailed()
	{
		_OnConnectFailed();
	}

	void CClient::OnLoginDone()
	{
		_OnLoginDone();
	}


	void CClient::SendData(_U16 id, _U32 len, const _U8* data)
	{
		HIOPOOL pool = HIOPoolOf(m_hConnect);
		_U32 blen = GetIoBufferSize(pool);
		_U8* buf = LockIoBuffer(pool);
		*((_U16*)(buf+0)) = (_U16)len + sizeof(id);
		*((_U16*)(buf+2)) = id;
		if(len<=blen-sizeof(_U16)*2)
		{
			memcpy(buf+4, data, len);
			Atlas::Send(m_hConnect, len+sizeof(_U16)*2, buf);
			return;
		}
		else
		{
			Atlas::Send(m_hConnect, blen, buf);
			len -= blen - sizeof(_U16)*2;
			data += blen - sizeof(_U16)*2;
		}

		for(;;)
		{
			buf = LockIoBuffer(pool);
			if(len<=blen)
			{
				memcpy(buf, data, len);
				Atlas::Send(m_hConnect, len, buf);
				return;
			}
			else
			{
				memcpy(buf, data, blen);
				Atlas::Send(m_hConnect, blen, buf);
				len -= blen;
				buf += blen;
			}
		}
	}

	void CClient::OnRawConnect(HCONNECT hConn)
	{
		m_hConnect = hConn;
		IOBUFFER_STREAM<10> stream(HIOPoolOf(hConn), 0);
		stream.Write(m_nLoginDataSize);
		stream.WriteData(m_LoginData, m_nLoginDataSize);
		stream.Send(hConn);
		m_nRecvBuffLen = 0;
		OnConnect();
	}

	void CClient::OnRawData(_U32 len, const _U8* data)
	{
	}

	void CClient::OnRawDisconnect()
	{
		if(m_bNeedRedirect)
		{
			if(m_strSessionAddr.empty())
			{
				m_nClientState = CLIENT_AUTH_FAILED;
				return;//auth falied;
			}

			Atlas::SOCKADDR sa;
			Atlas::STR2ADDR((_STR)m_strSessionAddr.c_str(), sa);	//login server port
			//Login(sa, m_nUID, "welcome");
			ASOCKIO_HANDLER handler =
			{
				CLT_ON_CONNECT,
				CLT_ON_DISCONNECT,
				CLT_ON_DATA,
				CLT_ON_CONNECTFAILED,
			};

			if(!Connect(sa, handler, m_pClientApp->GetIOPool(), m_pClientApp->GetHWorkers(), this))
			{
				OnDisconnect();
				m_nClientState = CLIENT_AUTH_FAILED;
				return;	//connect session failed
			}

			m_bNeedRedirect = false;
		}
		else
		{
			m_nClientState = CLIENT_NA;
			m_hConnect = NULL;
			m_strSessionAddr.clear();
			m_bNeedRedirect = true;
			OnDisconnect();
		}
	}

	void CClient::OnRawConnectFailed()
	{
		m_nClientState = CLIENT_NA;
		OnConnectFailed();
	}

}
*/
