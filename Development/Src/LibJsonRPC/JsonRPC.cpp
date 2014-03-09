#include <ZionDefines.h>
#include <ZionSTL.h>

#include "uv.h"
#include "JsonRPC.h"

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "psapi.lib")

#define STATE_CONNECTING		((_U32)0x01)
#define STATE_READING			((_U32)0x02)
#define STATE_SHUTDOWNING		((_U32)0x04)
#define STATE_SHUTDOWN			((_U32)0x08)

namespace Zion
{

	class CJsonRPCConnection
	{
	public:
		CJsonRPCConnection();
		~CJsonRPCConnection();

		virtual void OnDisconnect() = 0;
		virtual void OnData(_U32 seq, const char* data) = 0;

		bool Send(_U32 seq, const char* method, const char* data);
		void Shutdown();

		static void OnConnect(uv_stream_t* stream, int status);
		static void OnBeforRead(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
		static void OnAfterRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
		static void OnAfterWrite(uv_write_t* req, int status);
		static void OnShutdown(uv_shutdown_t* req, int status);
		static void OnClose(uv_handle_t* handle);

	protected:
		_U32 m_state;

	private:
		uv_stream_t* m_stream;
		uv_shutdown_t m_shutdown;

		char m_RecvBuf[1024];
		_U32 m_RecvLen;

		_U32 m_PacketLen;
		_U32 m_PacketSeq;
		String m_PacketData;
	};

	class CJsonRPCServerConnection : public CJsonRPCConnection
	{
		friend class CJsonRPCServer;
	public:
		CJsonRPCServerConnection(_U32 id, CJsonRPCServer* pServer);
		_U32 GetID() { return m_id; }
		virtual void OnDisconnect();
		virtual void OnData(_U32 seq, const char* data);

	private:
		uv_tcp_t m_socket;
		_U32 m_id;
		CJsonRPCServer* m_pServer;
	};

	class CJsonRPCServer
	{
	public:
		CJsonRPCServer();
		~CJsonRPCServer();

		void Bind(const char* method, JSON_RESPONSE_PROC proc);

		bool Start(const char* ep);
		void Stop();
		bool Send(_U32 conn, _U32 seq, int errcode, const char* data);

		void Call(CJsonRPCServerConnection* pConn, _U32 seq, const char* method, const char* data);
		void Detach(_U32 id, CJsonRPCServerConnection* pConn);

		static void OnConnect(uv_stream_t* server, int status);
		static void OnClose(uv_handle_t* server);
		static void ThreadProc(void*);
		static void QuitEvent(uv_async_t* handle, int status);

	private:
		Map<String, JSON_RESPONSE_PROC> m_Methods;
		_U32 m_ClientSeq;
		Map<_U32, CJsonRPCServerConnection*> m_Clients;
		uv_loop_t* m_uv_loop;
		uv_thread_t m_uv_thread;
		uv_async_t m_uv_quit;
		uv_tcp_t m_server;
	};

	struct JSON_RPCCALL
	{
		String method;
		String data;
		JSON_CALLBACK_PROC proc;
	};

	class CJsonRPCClient : public CJsonRPCConnection
	{
	public:
		CJsonRPCClient(struct sockaddr* sa);

		bool Connect();

		bool Send(const char* method, const char* data);
		bool Send(const char* method, const char* data, JSON_CALLBACK_PROC proc);

		virtual void OnDisconnect();
		virtual void OnData(_U32 seq, const char* data);

		static void OnConnect(uv_connect_t* req, int status);

	private:
		_U32 m_seq;
		Map<_U32, JSON_RPCCALL> m_Requests;

		uv_tcp_t m_socket;
		uv_connect_t m_connect;
		union
		{
			struct sockaddr m_sa;
			struct sockaddr_in m_ip4;
			struct sockaddr_in6 m_ip6;
		};
	};

	class CJsonRPCClientManager
	{
		friend class CJsonRPCClient;
	public:
		static CJsonRPCClientManager& Get();
		static CJsonRPCClient* GetClient(const char* ep);
		static void ThreadProc(void *arg);

	private:
		CJsonRPCClientManager();
		~CJsonRPCClientManager();

	protected:
		uv_mutex_t m_Mutex;
		Map<String, CJsonRPCClient*> m_Clients;
		uv_loop_t* m_uv_loop;
		bool m_is_stop;
		uv_thread_t m_uv_thread;
	};

	static CJsonRPCServer g_JsonRPCServer;

	void JsonRPC_Bind(const char* method, JSON_RESPONSE_PROC proc)
	{
		g_JsonRPCServer.Bind(method, proc);
	}

	bool JsonRPC_Start(const char* ep)
	{
		return g_JsonRPCServer.Start(ep);
	}

	void JsonRPC_Stop()
	{
		g_JsonRPCServer.Stop();
	}

	bool JsonRPC_Valid(const JSONRPC_RESPONSE& res)
	{
		return res.seq!=0;
	}

	bool JsonRPC_Send(const JSONRPC_RESPONSE& res, int errcode, const char* args)
	{
		if(res.seq==0) return true;
		ZION_ASSERT(res.server!=NULL && res.conn!=(_U32)-1);
		if(res.server==NULL || res.conn!=(_U32)-1) return false;
		return ((CJsonRPCServer*)res.server)->Send(res.conn, res.seq, errcode, args);
	}

	CJsonRPCClient* JsonRPC_GetClient(const char* ep)
	{
		return CJsonRPCClientManager::GetClient(ep);
	}

	void JsonRPC_StopClients()
	{
	}

	bool JsonRPC_Send(CJsonRPCClient* pClient, const char* method, const char* args)
	{
		return pClient->Send(method, args);
	}

	bool JsonRPC_Send(CJsonRPCClient* pClient, const char* method, const char* args, JSON_CALLBACK_PROC proc)
	{
		return pClient->Send(method, args, proc);
	}

	CJsonRPCConnection::CJsonRPCConnection()
	{
		m_state = 0;
	}

	CJsonRPCConnection::~CJsonRPCConnection()
	{
	}

	typedef struct
	{
		uv_write_t req;
		uv_buf_t buf;
	} write_req_t;

	bool CJsonRPCConnection::Send(_U32 seq, const char* method, const char* data)
	{
		if(m_state&(STATE_SHUTDOWNING|STATE_SHUTDOWN))
		{
			return false;
		}

		_U32 data_len;
		write_req_t* req;
		if(method)
		{
			_U32 method_len = strlen(method);
			data_len = strlen(data);
			req = (write_req_t*)ZION_ALLOC(sizeof(write_req_t) + 8 + method_len + 1 + data_len);
			if(!req) return false;

			memcpy((char*)(req+1) + 8, method, method_len);
			memcpy((char*)(req+1) + 8 + method_len, ":", 1);
			memcpy((char*)(req+1) + 8 + method_len + 1, data, data_len);
			data_len += method_len + 1;
		}
		else
		{
			data_len = strlen(data);
			req = (write_req_t*)ZION_ALLOC(sizeof(write_req_t) + 8 + data_len);
			if(!req) return false;

			memcpy((char*)(req+1) + 8, data, data_len);
		}

		*((_U32*)(req+1) + 0) = data_len;
		*((_U32*)(req+1) + 1) = seq;

		req->buf = uv_buf_init((char*)(req+1), 8 + data_len);
		if(uv_write(&req->req, m_stream, &req->buf, 1, &CJsonRPCConnection::OnAfterWrite))
		{
			ZION_FREE(req);
			return false;
		}
		else
		{
			return true;
		}
	}

	void CJsonRPCConnection::Shutdown()
	{
		if((m_state&STATE_SHUTDOWNING)==0)
		{
			m_state |= STATE_SHUTDOWNING;
			m_shutdown.data = this;
			if(uv_shutdown(&m_shutdown, m_stream, &CJsonRPCConnection::OnShutdown))
			{
				ZION_ASSERT(0);
				m_state |= STATE_SHUTDOWN;
			}
		}

		if(m_state==(STATE_SHUTDOWNING|STATE_SHUTDOWN))
		{
			uv_close((uv_handle_t*)m_stream, &CJsonRPCConnection::OnClose);
		}
	}

	void CJsonRPCConnection::OnConnect(uv_stream_t* stream, int status)
	{
		CJsonRPCConnection* pConn = (CJsonRPCConnection*)stream->data;

		pConn->m_state = STATE_READING;
		pConn->m_stream = stream;
		pConn->m_RecvLen = 0;
		pConn->m_PacketLen = (_U32)-1;
		pConn->m_PacketSeq = 0;
		pConn->m_PacketData = "";

		if(uv_read_start(stream, &CJsonRPCConnection::OnBeforRead, &CJsonRPCConnection::OnAfterRead))
		{
			pConn->m_state &= ~STATE_READING;
			pConn->Shutdown();
			ZION_ASSERT(0);
		}
	}

	void CJsonRPCConnection::OnBeforRead(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{
		CJsonRPCConnection* pConn = (CJsonRPCConnection*)handle->data;
		buf->base = pConn->m_RecvBuf + pConn->m_RecvLen;
		buf->len = sizeof(pConn->m_RecvBuf)-pConn->m_RecvLen;
	}

	void CJsonRPCConnection::OnAfterRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
	{
		CJsonRPCConnection* pConn = (CJsonRPCConnection*)stream->data;

		if(nread<0)
		{
			pConn->m_state &= ~STATE_READING;
			pConn->Shutdown();
			return;
		}

		pConn->m_RecvLen += nread;

		char* data = pConn->m_RecvBuf;
		_U32 data_len = pConn->m_RecvLen;
		for(;;)
		{
			_U32 plen;

			if(pConn->m_PacketLen!=(_U32)-1)
			{
				if(data_len + pConn->m_PacketData.size()<pConn->m_PacketLen)
				{
					plen = data_len;
				}
				else
				{
					plen = pConn->m_PacketLen - pConn->m_PacketData.size();
				}
				pConn->m_PacketData.append(data, plen);
				if(pConn->m_PacketLen==pConn->m_PacketData.size())
				{
					pConn->m_PacketLen = (_U32)-1;
					pConn->OnData(pConn->m_PacketSeq, pConn->m_PacketData.c_str());
				}
			}
			else
			{
				if(pConn->m_RecvLen<8) break;
				pConn->m_PacketLen = *((const _U32*)(data+0));
				pConn->m_PacketSeq = *((const _U32*)(data+4));
				pConn->m_PacketData = "";
				plen = 8;
			}

			ZION_ASSERT(plen<=data_len);
			if(plen==data_len)
			{
				pConn->m_RecvLen = 0;
				break;
			}
			memmove(data, data+plen, data_len-plen);
			data_len -= plen;
		}
	}

	void CJsonRPCConnection::OnAfterWrite(uv_write_t* req, int status)
	{
		ZION_FREE(req);
	}

	void CJsonRPCConnection::OnShutdown(uv_shutdown_t* req, int status)
	{
		CJsonRPCConnection* pConn = (CJsonRPCConnection*)req->data;

		pConn->m_state |= STATE_SHUTDOWN;

		pConn->Shutdown();
	}

	void CJsonRPCConnection::OnClose(uv_handle_t* handle)
	{
		CJsonRPCConnection* pConn = (CJsonRPCConnection*)handle->data;
		pConn->m_stream = NULL;
		pConn->OnDisconnect();
	}
	
	CJsonRPCServerConnection::CJsonRPCServerConnection(_U32 id, CJsonRPCServer* pServer)
	{
		m_id = id;
		m_pServer = pServer;
	}

	void CJsonRPCServerConnection::OnDisconnect()
	{
		m_pServer->Detach(m_id, this);
	}

	void CJsonRPCServerConnection::OnData(_U32 seq, const char* data)
	{
		const char* pos = strchr(data, ':');
		if(!pos)
		{
			ZION_ASSERT(!"invalid data format");
			return;
		}

		String method;
		method.append(data, pos-data);
		m_pServer->Call(this, seq, method.c_str(), pos+1);
	}

	CJsonRPCServer::CJsonRPCServer()
	{
		m_ClientSeq = 0;
	}

	CJsonRPCServer::~CJsonRPCServer()
	{
	}

	void CJsonRPCServer::Bind(const char* method, JSON_RESPONSE_PROC proc)
	{
		ZION_ASSERT(m_Methods.find(method)==m_Methods.end());
		m_Methods[method] = proc;
	}

	bool CJsonRPCServer::Start(const char* ep)
	{
		const char* pos = strchr(ep, ':');
		if(!pos) return NULL;

		union
		{
			struct sockaddr sa;
			struct sockaddr_in ip4;
			struct sockaddr_in6 ip6;
		};
		char host[100];
		memcpy(host, ep, pos-ep);
		host[pos-ep] = '\0';
		uv_ip4_addr(host, atoi(pos+1), &ip4);
		if(ip4.sin_addr.S_un.S_addr==0)
		{
			uv_ip6_addr(host, atoi(pos+1), &ip6);
		}

		m_uv_loop = uv_loop_new();
		if(!m_uv_loop)
		{
			ZION_ASSERT(0);
			return false;
		}

		if(uv_tcp_init(m_uv_loop, &m_server))
		{
			ZION_ASSERT(0);
			return false;
		}
		m_server.data = this;

		if(uv_tcp_bind(&m_server, &sa))
		{
			ZION_ASSERT(!"uv_tcp_bind failed");
			uv_loop_delete(m_uv_loop);
			return false;
		}

		if(uv_listen((uv_stream_t*)&m_server, SOMAXCONN, &CJsonRPCServer::OnConnect))
		{
			ZION_ASSERT(!"uv_listen failed");
			uv_loop_delete(m_uv_loop);
			return false;
		}

		uv_thread_create(&m_uv_thread, &CJsonRPCServer::ThreadProc, this);
		return true;
	}

	void CJsonRPCServer::Stop()
	{
		uv_async_init(m_uv_loop, &m_uv_quit, CJsonRPCServer::QuitEvent);
		uv_async_send(&m_uv_quit);
		uv_thread_join(&m_uv_thread);
		uv_loop_delete(m_uv_loop);
	}

	bool CJsonRPCServer::Send(_U32 conn, _U32 seq, int errcode, const char* data)
	{
		Map<_U32, CJsonRPCServerConnection*>::iterator i;
		i = m_Clients.find(conn);
		if(i==m_Clients.end()) return false;
		return i->second->Send(seq, NULL, data);
	}

	void CJsonRPCServer::Call(CJsonRPCServerConnection* pConn, _U32 seq, const char* method, const char* data)
	{
		Map<String, JSON_RESPONSE_PROC>::iterator i;
		i = m_Methods.find(method);
		if(i==m_Methods.end())
		{
			ZION_ASSERT(!"method not found");
			return;
		}

		JSONRPC_RESPONSE res = { this, pConn->GetID(), seq };

		i->second(res, data);
	}

	void CJsonRPCServer::Detach(_U32 id, CJsonRPCServerConnection* pConn)
	{
		m_Clients.erase(id);
		ZION_DELETE pConn;
	}

	void CJsonRPCServer::OnConnect(uv_stream_t* server, int status)
	{
		CJsonRPCServer* pServer = (CJsonRPCServer*)server->data;
		_U32 seq = pServer->m_ClientSeq++;
		CJsonRPCServerConnection* pConn = ZION_NEW CJsonRPCServerConnection(seq, pServer);

		if(uv_tcp_init(pServer->m_uv_loop, &pConn->m_socket))
		{
			ZION_ASSERT(!"uv_tcp_init failed");
			ZION_DELETE pConn;
			return;
		}
		if(uv_accept(server, (uv_stream_t*)&pConn->m_socket))
		{
			ZION_ASSERT(!"uv_accept failed");
			ZION_DELETE pConn;
			return;
		}

		pServer->m_Clients[seq] = pConn;
		pConn->m_socket.data = pConn;
		CJsonRPCConnection::OnConnect((uv_stream_t*)&pConn->m_socket, 0);
	}

	void CJsonRPCServer::OnClose(uv_handle_t* server)
	{
		CJsonRPCServer* pServer = (CJsonRPCServer*)server->data;

		Map<_U32, CJsonRPCServerConnection*>::iterator i;
		for(i=pServer->m_Clients.begin(); i!=pServer->m_Clients.end(); i++)
		{
			i->second->Shutdown();
		}
	}

	void CJsonRPCServer::ThreadProc(void* data)
	{
		CJsonRPCServer* pServer = (CJsonRPCServer*)data;
		uv_run(pServer->m_uv_loop, UV_RUN_DEFAULT);
	}

	void CJsonRPCServer::QuitEvent(uv_async_t* handle, int status)
	{
		CJsonRPCServer* pServer = (CJsonRPCServer*)((char*)handle - ZION_OFFSETOF(CJsonRPCServer, m_uv_quit));

		uv_close((uv_handle_t*)&pServer->m_server, &CJsonRPCServer::OnClose);

		Map<_U32, CJsonRPCServerConnection*>::iterator i;
		for(i=pServer->m_Clients.begin(); i!=pServer->m_Clients.end(); i++)
		{
			i->second->Shutdown();
		}
	}

	CJsonRPCClient::CJsonRPCClient(struct sockaddr* sa)
	{
		ZION_ASSERT(sizeof(struct sockaddr_in6)>=sizeof(struct sockaddr_in));

		m_seq = 1;
		memcpy(&m_sa, sa, sizeof(struct sockaddr_in6));
	}

	bool CJsonRPCClient::Connect()
	{
		if(m_state!=0)
		{
			ZION_ASSERT(0);
			return false;
		}

		m_state = STATE_CONNECTING;
		m_connect.data = this;

		if(uv_tcp_init(CJsonRPCClientManager::Get().m_uv_loop, &m_socket))
		{
			m_state = 0;
			ZION_ASSERT(0);
			return false;
		}

		if(uv_tcp_connect(&m_connect, &m_socket, &m_sa, &CJsonRPCClient::OnConnect))
		{
			m_state = 0;
			ZION_ASSERT(0);
			return false;
		}

		return true;
	}

	bool CJsonRPCClient::Send(const char* method, const char* data)
	{
		if(m_state&(STATE_SHUTDOWN|STATE_SHUTDOWNING))
		{
			return false;
		}

		if(m_state==0)
		{
			if(!Connect()) return false;
		}

		if(m_state&STATE_CONNECTING)
		{
			struct JSON_RPCCALL call;
			call.method = method;
			call.data = data;

			while(m_Requests.find(m_seq)!=m_Requests.end())
			{
				m_seq++;
			}

			m_Requests[m_seq++] = call;
			return true;
		}

		ZION_ASSERT(m_state&STATE_READING);
		return CJsonRPCConnection::Send(0, method, data);
	}

	bool CJsonRPCClient::Send(const char* method, const char* data, JSON_CALLBACK_PROC proc)
	{
		if(m_state&(STATE_SHUTDOWN|STATE_SHUTDOWNING))
		{
			return false;
		}

		if(m_state==0)
		{
			if(!Connect()) return false;
		}

		while(m_Requests.find(m_seq)!=m_Requests.end())
		{
			m_seq++;
		}

		if(m_state&STATE_CONNECTING)
		{
			struct JSON_RPCCALL call;
			call.method = method;
			call.data = data;
			call.proc = proc;
			m_Requests[m_seq++] = call;
			return true;
		}

		ZION_ASSERT(m_state&STATE_READING);

		struct JSON_RPCCALL call;
		call.method = method;
		call.data = data;
		call.proc = proc;
		m_Requests[m_seq++] = call;

		if(!CJsonRPCConnection::Send(m_seq, method, data))
		{
			m_Requests.erase(m_seq-1);
			return false;
		}

		return true;
	}

	void CJsonRPCClient::OnDisconnect()
	{
		while(!m_Requests.empty())
		{
			struct JSON_RPCCALL& call = m_Requests.begin()->second;
			if(call.proc)
			{
				call.proc(-1, NULL);
			}
			m_Requests.erase(m_Requests.begin());
		}
	}

	void CJsonRPCClient::OnData(_U32 seq, const char* data)
	{
		Map<_U32, JSON_RPCCALL>::iterator it;

		it = m_Requests.find(seq);
		if(it==m_Requests.end())
		{
			//ZION_ASSERT(!"seq not found");
			return;
		}

		int errcode = -1;
		if(data)
		{
			char tmp[100];
			char* pos = strchr((char*)data, ';');
			if(pos && (pos-data)<sizeof(tmp))
			{
				memcpy(tmp, data, pos-data);
				tmp[pos-data] = '\0';
				errcode = atoi(tmp);
				data = pos+1;
			}
			else
			{
				errcode = atoi(tmp);
				data = NULL;
			}
		}

		if(it->second.proc)
		{
			it->second.proc(errcode, data);
		}
		m_Requests.erase(it);
	}

	void CJsonRPCClient::OnConnect(uv_connect_t* req, int status)
	{
		CJsonRPCClient* pClient = (CJsonRPCClient*)req->data;

		if(status!=0)
		{
			pClient->m_state = 0;
			return;
		}

		req->handle->data = pClient;
		CJsonRPCConnection::OnConnect(req->handle, status);

		if((pClient->m_state&STATE_READING)==0)
		{
			return;
		}

		Map<_U32, JSON_RPCCALL>::iterator it, it2;

		for(it=pClient->m_Requests.begin(); it!=pClient->m_Requests.end();)
		{
			if(it->second.proc)
			{
				if(((CJsonRPCConnection*)pClient)->Send(it->first, it->second.method.c_str(), it->second.data.c_str()))
				{
					++it;
					continue;
				}
				it->second.proc(-1, NULL);
			}
			else
			{
				((CJsonRPCConnection*)pClient)->Send(1, it->second.method.c_str(), it->second.data.c_str());
			}
			it2 = it++;
			pClient->m_Requests.erase(it2);
		}
	}

	CJsonRPCClientManager& CJsonRPCClientManager::Get()
	{
		static CJsonRPCClientManager _Manager;
		return _Manager;
	}

	CJsonRPCClient* CJsonRPCClientManager::GetClient(const char* ep)
	{
		const char* pos = strchr(ep, ':');
		if(!pos) return NULL;

		union
		{
			struct sockaddr sa;
			struct sockaddr_in ip4;
			struct sockaddr_in6 ip6;
		};
		char host[100];
		memcpy(host, ep, pos-ep);
		host[pos-ep] = '\0';
		uv_ip4_addr(host, atoi(pos+1), &ip4);
		if(ip4.sin_addr.S_un.S_addr==0)
		{
			uv_ip6_addr(host, atoi(pos+1), &ip6);
		}

		CJsonRPCClientManager& _Manager = Get();
		CJsonRPCClient* pClient;
		Map<String, CJsonRPCClient*>::iterator i;

		uv_mutex_lock(&_Manager.m_Mutex);
		i = Get().m_Clients.find(ep);
		if(i!=Get().m_Clients.end())
		{
			pClient = i->second;
		}
		else
		{
			pClient = ZION_NEW CJsonRPCClient(&sa);
			CJsonRPCClientManager::Get().m_Clients[ep] = pClient;
		}
		uv_mutex_unlock(&_Manager.m_Mutex);
		return pClient;
	}

	void CJsonRPCClientManager::ThreadProc(void *arg)
	{
		while(!Get().m_is_stop) {
			uv_run(Get().m_uv_loop, UV_RUN_DEFAULT);
		}
	}

	CJsonRPCClientManager::CJsonRPCClientManager()
	{
		m_is_stop = false;
		uv_mutex_init(&m_Mutex);
		m_uv_loop = uv_loop_new();
		uv_thread_create(&m_uv_thread, &CJsonRPCClientManager::ThreadProc, NULL);
	}

	CJsonRPCClientManager::~CJsonRPCClientManager()
	{
		m_is_stop = true;
		uv_stop(m_uv_loop);
		uv_thread_join(&m_uv_thread);
		uv_loop_delete(m_uv_loop);
		uv_mutex_destroy(&m_Mutex);
	}

}
