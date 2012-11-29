#include <stdarg.h>

#include "AtlasBase.h"
#include "AtlasCommon.h"
#include "AtlasClientApp.h"
#include "AtlasClient.h"

namespace Atlas
{
	static bool CLT_ON_CONNECT(HCONNECT);
	static void CLT_ON_DISCONNECT(HCONNECT);
	static void CLT_ON_DATA(HCONNECT, _U32, const _U8*);
	static void CLT_ON_CONNECTFAILED(void* key);

	void CClientLogOutput::I(const char* fmt, ...)
	{
		va_list args;
		int	len;
		char buf[2*1024];
		va_start(args, fmt);
		len = vsprintf(buf, fmt, args);
		if(len<0) return;
		Write(TYPE_INFO, buf);
	}

	void CClientLogOutput::E(const char* fmt, ...)
	{
		va_list args;
		int	len;
		char buf[2*1024];
		va_start(args, fmt);
		len = vsprintf(buf, fmt, args);
		if(len<0) return;
		Write(TYPE_ERROR, buf);
	}

	void CClientLogOutput::W(const char* fmt, ...)
	{
		va_list args;
		int	len;
		char buf[2*1024];
		va_start(args, fmt);
		len = vsprintf(buf, fmt, args);
		if(len<0) return;
		Write(TYPE_WANNING, buf);
	}

	void CClientLogOutput::D(const char* fmt, ...)
	{
		va_list args;
		int	len;
		char buf[2*1024];
		va_start(args, fmt);
		len = vsprintf(buf, fmt, args);
		if(len<0) return;
		Write(TYPE_DEBUG, buf);
	}

	void CClientLogOutput::Write(int type, const char* pLine)
	{
	}

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

	static CClientLogOutput _global_log_output;
	CClientLogOutput& CClient::GetLogOutput()
	{
		if(m_pLogOutput) return *m_pLogOutput;
		return _global_log_output;
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

	bool CClient::ExistSyncDomain(_U16 domain)
	{
		return true;
	}

	bool CClient::ExistSyncObject(_U16 domain, _U32 id, _U16 index)
	{
		return true;
	}

	void CClient::GetSyncDomainList(std::vector<_U16>& list)
	{
	}

	void CClient::GetSyncObjectList(_U16 domain, std::vector<std::pair<_U32, _U16>>& list)
	{
	}

	const DDLReflect::STRUCT_INFO* CClient::GetSyncObjectType(_U16 domain, _U32 id, _U16 index)
	{
		return NULL;
	}

	void* CClient::GetSyncObjectType(_U16 domain, _U32 id, _U16 index, const DDLReflect::STRUCT_INFO* info)
	{
		return NULL;
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
		_U16 pkglen;
		_U16 id;
		while(len>0 && m_pClientApp->IsEnableTick())
		{
			_U32 copylen = len;
			if(m_nRecvBuffLen+copylen>m_nRecvBuffSize) copylen = m_nRecvBuffSize - m_nRecvBuffLen;
			memcpy(m_pRecvBuff+m_nRecvBuffLen, data, copylen);
			m_nRecvBuffLen += copylen;
			len -= copylen;
			data = data + copylen;
			while(m_nRecvBuffLen>=sizeof(pkglen)+sizeof(id))
			{
				memcpy(&pkglen, m_pRecvBuff, sizeof(pkglen));

				if(pkglen>m_nRecvBuffSize-sizeof(pkglen))
				{
					Logout();
					return;
				}

				if(pkglen+sizeof(pkglen) > m_nRecvBuffLen)
				{
					break;
				}

				if(pkglen<sizeof(id))
				{
					Logout();
					return;
				}
				
				memcpy(&id, m_pRecvBuff+sizeof(pkglen), sizeof(id));
				if(m_nClientState==CLIENT_LOGINING)
				{
					if(m_strSessionAddr.empty())
					{
						m_strSessionAddr = (char*)(m_pRecvBuff+sizeof(pkglen)+sizeof(id));
						Disconnect(m_hConnect);
						return;
					}
					else
					{
						m_nClientState = CLIENT_LOGINED;
						OnLoginDone();
					}
				}
				
				OnData(id, pkglen-sizeof(id), m_pRecvBuff+sizeof(pkglen)+sizeof(id));

				_U32 ulen = pkglen + sizeof(pkglen);
				m_nRecvBuffLen -= ulen;
				memmove(m_pRecvBuff, m_pRecvBuff+ulen, m_nRecvBuffLen);
			}
		}
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

	CClientComponent::CClientComponent(CClient* pClient) : m_pClient(pClient)
	{
	}

	CClientComponent::~CClientComponent()
	{
	}

	CClientDataSyncComponent::CClientDataSyncComponent(Atlas::CClient* pClient) : CClientComponent(pClient)
	{
	}

	CClientDataSyncComponent::~CClientDataSyncComponent()
	{
	}

	bool CClientDataSyncComponent::AppendObject(const DDLReflect::STRUCT_INFO* info, const void* data)
	{
		ATLAS_ASSERT(m_Objects.find(data)!=m_Objects.end());
		if(m_Objects.find(data)==m_Objects.end()) return false;
		m_Objects[data] = info;
		return true;
	}
	
	bool CClientDataSyncComponent::RemoveObject(const DDLReflect::STRUCT_INFO* info, const void* data)
	{
		std::map<const void*, const DDLReflect::STRUCT_INFO*>::iterator i;
		i = m_Objects.find(data);
		if(i==m_Objects.end()) return false;
		ATLAS_ASSERT(i->second==info);
		m_Objects.erase(i);
		return true;
	}
	
	void CClientDataSyncComponent::RemoveAllObjects()
	{
		m_Objects.clear();
	}

	void CClientDataSyncComponent::GetObjects(std::vector<std::pair<const void*, const DDLReflect::STRUCT_INFO*>>& objects)
	{
		objects.clear();
		objects.resize(m_Objects.size());

		std::map<const void*, const DDLReflect::STRUCT_INFO*>::iterator i;
		_U32 index = 0;
		for(i=m_Objects.begin(); i!=m_Objects.end(); i++)
		{
			objects[index].first = i->first;
			objects[index].second = i->second;
			index++;
		}
	}

	bool CLT_ON_CONNECT(HCONNECT hConn)
	{
		CClient* pClient = (CClient*)KeyOf(hConn);
		if(pClient)
		{
			pClient->GetClientApp()->OnConnect(pClient, hConn);
		}
		return true;
	}

	void CLT_ON_DISCONNECT(HCONNECT hConn)
	{
		CClient* pClient = (CClient*)KeyOf(hConn);
		if(pClient)
		{
			pClient->GetClientApp()->OnDisconnect(pClient, hConn);
		}
	}

	void CLT_ON_DATA(HCONNECT hConn, _U32 len, const _U8* data)
	{
		CClient* pClient = (CClient*)KeyOf(hConn);
		if(pClient)
		{
			pClient->GetClientApp()->OnData(pClient, len, data);
		}
	}

	void CLT_ON_CONNECTFAILED(void* key)
	{
		CClient* pClient = (CClient*)key;
		if(!pClient) return;
		pClient->GetClientApp()->OnConnectFailed(pClient);
	}

}
