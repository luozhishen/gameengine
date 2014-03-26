#include <ZionDefines.h>
#include <ZionSTL.h>
#include <json/jsoncpp.h>
#include "JsonRPC.h"

#include "uv.h"

#ifdef _WIN32
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "psapi.lib")
#endif

#define STATE_CONNECTING		((_U32)0x01)
#define STATE_READING			((_U32)0x02)
#define STATE_SHUTDOWNING		((_U32)0x04)
#define STATE_SHUTDOWN			((_U32)0x08)

namespace Zion
{

	class CJsonRPCConnection;

	typedef struct _SENDBUF
	{
		uv_write_t req;
		uv_buf_t buf;
		CJsonRPCConnection* conn;
	} SENDBUF;

	class CJsonRPCConnection
	{
	public:
		CJsonRPCConnection();
		~CJsonRPCConnection();

		virtual void OnDisconnect() = 0;
		virtual void OnData(_U32 seq, const char* data, _U32 len) = 0;

		bool Send(_U32 seq, const char* method, const char* data);
		bool Send(const void* data, _U32 len, bool delay);
		bool Send(SENDBUF* sendbuf, _U32 len);
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

		_U32 m_send_count;
		SENDBUF* m_sendbuf;
		_U32 m_sendbuf_len;

		char m_RecvBuf[RECVBUF_SIZE];
		_U32 m_RecvLen;
	};

	class CJsonRPCServerConnection : public CJsonRPCConnection
	{
		friend class CJsonRPCServer;
	public:
		CJsonRPCServerConnection(_U32 id, CJsonRPCServer* pServer);
		_U32 GetID() { return m_id; }
		virtual void OnDisconnect();
		virtual void OnData(_U32 seq, const char* data, _U32 len);

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
		bool Send(const char* data);
		bool Send(_U32 conn, _U32 seq, const char* data);

		void Call(CJsonRPCServerConnection* pConn, _U32 seq, const char* method, const char* data, _U32 len);
		void Detach(_U32 id, CJsonRPCServerConnection* pConn);

		static void OnConnect(uv_stream_t* server, int status);
		static void OnClose(uv_handle_t* server);
		static void ThreadProc(void*);
		static void QuitEvent(uv_async_t* handle, int status);

	private:
		CJsonRPCServerConnection* m_pCurConn;
		_U32 m_CurSeq;

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
		virtual void OnData(_U32 seq, const char* data, _U32 len);

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

	static CJsonRPCServer g_server;

	void JsonRPC_Bind(const char* method, JSON_RESPONSE_PROC proc)
	{
		g_server.Bind(method, proc);
	}

	bool JsonRPC_Start(const char* ep)
	{
		return g_server.Start(ep);
	}

	void JsonRPC_Stop()
	{
		g_server.Stop();
	}
	/*
	void JsonRPC_SetPending(const JSONRPC_RESPONSE* res)
	{
	}
	*/
	bool JsonRPC_Send(const char* args)
	{
		return g_server.Send(args);
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
		m_send_count = 0;
		m_sendbuf = NULL;
		m_sendbuf_len = 0;
	}

	CJsonRPCConnection::~CJsonRPCConnection()
	{
		if(m_sendbuf)
		{
			ZION_FREE(m_sendbuf);
			m_sendbuf = NULL;
		}
	}

	bool CJsonRPCConnection::Send(_U32 seq, const char* method, const char* data)
	{
		if(m_state&(STATE_SHUTDOWNING|STATE_SHUTDOWN))
		{
			return false;
		}
		if(method)
		{
			_U32 method_len = strlen(method);
			_U32 data_len = strlen(data);
			_U32 len = method_len + 1 + data_len;
			if(!Send(&len, sizeof(len), true)) return false;
			if(!Send(&seq, sizeof(seq), true)) return false;
			if(!Send(method, method_len, true)) return false;
			if(!Send(":", 1, true)) return false;
			if(!Send(data, data_len, false)) return false;
		}
		else
		{
			_U32 data_len = strlen(data);
			if(!Send(&data_len, sizeof(data_len), true)) return false;
			if(!Send(&seq, sizeof(seq), true)) return false;
			if(!Send(data, data_len, false)) return false;
		}
		return true;
	}

	bool CJsonRPCConnection::Send(const void* data, _U32 len, bool delay)
	{
		SENDBUF* sendbuf = m_sendbuf;
		while(len>0)
		{
			if(!sendbuf)
			{
				sendbuf = (SENDBUF*)ZION_ALLOC(sizeof(SENDBUF)+SENDBUF_SIZE);
				m_sendbuf_len = 0;
			}

			if(m_sendbuf_len+len<SENDBUF_SIZE)
			{
				memcpy((char*)(sendbuf+1)+m_sendbuf_len, data, len);
				m_sendbuf_len += len;
				break;
			}
			else
			{
				_U32 sendlen = SENDBUF_SIZE - m_sendbuf_len;
				memcpy((char*)(sendbuf+1) + m_sendbuf_len, data, sendlen);
				m_sendbuf_len += sendlen;
				Send(sendbuf, m_sendbuf_len);
				sendbuf = NULL;
				data = (char*)data + sendlen;
				len -= sendlen;
			}
		}

		if(sendbuf && m_send_count==0 && !delay)
		{
			Send(sendbuf, m_sendbuf_len);
			m_sendbuf = NULL;
		}
		else
		{
			m_sendbuf = sendbuf;
		}

		return true;
	}

	bool CJsonRPCConnection::Send(SENDBUF* sendbuf, _U32 len)
	{
		sendbuf->conn = this;
		sendbuf->buf = uv_buf_init((char*)(sendbuf+1), len);
		if(uv_write(&sendbuf->req, m_stream, &sendbuf->buf, 1, &CJsonRPCConnection::OnAfterWrite))
		{
			ZION_FATAL("uv_write failed");
			ZION_FREE(sendbuf);
		}
		else
		{
			m_send_count += 1;
		}
		return true;
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
		while(data_len>=8)
		{
			_U32 pkg_len = *((_U32*)data + 0);
			_U32 pkg_seq = *((_U32*)data + 1);
			if(data_len<pkg_len+8) break;
#if 0
			char* ret = "[0]";
			_U32 ret_len = 3;
			pConn->Send(&ret_len, sizeof(ret_len), true);
			pConn->Send(&pkg_seq, sizeof(pkg_seq), true);
			pConn->Send(ret, ret_len, false);
#else
			pConn->OnData(pkg_seq, data+8, pkg_len);
#endif
			data += pkg_len + 8;
			data_len -= pkg_len + 8;
		}

		if(data_len==0)
		{
			pConn->m_RecvLen = 0;
		}
		else
		{
			pConn->m_RecvLen = data_len;
			memmove(pConn->m_RecvBuf, data, (size_t)data_len);
		}
	}

	void CJsonRPCConnection::OnAfterWrite(uv_write_t* req, int status)
	{
		CJsonRPCConnection* conn = ((SENDBUF*)req)->conn;
		ZION_FREE(req);
		ZION_ASSERT(conn->m_send_count>0);
		conn->m_send_count -= 1;
		if(uv_is_writable((uv_stream_t*)&conn->m_stream) && conn->m_sendbuf && conn->m_send_count==0)
		{
			SENDBUF* sendbuf = conn->m_sendbuf;
			conn->m_sendbuf = NULL;
			conn->Send(sendbuf, conn->m_sendbuf_len);
		}
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

	void CJsonRPCServerConnection::OnData(_U32 seq, const char* data, _U32 len)
	{
		String method;
		_U32 pos;
		for(pos=0; pos<len && data[pos]!=':'; pos++);
		if(pos>=len-1)
		{
			ZION_ASSERT(!"invalid data format");
			Shutdown();
			return;
		}
		method.append(data, pos);
		m_pServer->Call(this, seq, method.c_str(), data+pos+1, len-pos-1);
	}

	CJsonRPCServer::CJsonRPCServer()
	{
		m_ClientSeq = 0;
		m_pCurConn = NULL;
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
		if(*((_U32*)&ip4.sin_addr)==INADDR_NONE)
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

	bool CJsonRPCServer::Send(const char* data)
	{
		if(!m_pCurConn) return false;
		m_pCurConn->Send(m_CurSeq, NULL, data);
		m_pCurConn = NULL;
		return true;
	}

	bool CJsonRPCServer::Send(_U32 conn, _U32 seq, const char* data)
	{
		Map<_U32, CJsonRPCServerConnection*>::iterator i;
		i = m_Clients.find(conn);
		if(i==m_Clients.end()) return false;
		return i->second->Send(seq, NULL, data);
	}

	void CJsonRPCServer::Call(CJsonRPCServerConnection* pConn, _U32 seq, const char* method, const char* data, _U32 len)
	{
		Map<String, JSON_RESPONSE_PROC>::iterator i;
		i = m_Methods.find(method);
		if(i==m_Methods.end())
		{
			ZION_ASSERT(!"method not found");
			return;
		}

		Json::Reader reader;
		Json::Value json;
		if(!reader.parse(data, data+len, json) || !json.isArray())
		{
			ZION_ASSERT(!"invalid data format");
			return;
		}

		m_pCurConn = pConn;
		m_CurSeq = seq;
		i->second(json);
		ZION_ASSERT(!m_pCurConn);
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
		uv_close((uv_handle_t*)handle, NULL);

		CJsonRPCServer* pServer = (CJsonRPCServer*)((char*)handle - ZION_OFFSETOF(CJsonRPCServer, m_uv_quit));
		uv_close((uv_handle_t*)&pServer->m_server, &CJsonRPCServer::OnClose);
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
				call.proc(NULL);
			}
			m_Requests.erase(m_Requests.begin());
		}
	}

	void CJsonRPCClient::OnData(_U32 seq, const char* data, _U32 len)
	{
		Map<_U32, JSON_RPCCALL>::iterator it;

		it = m_Requests.find(seq);
		if(it==m_Requests.end())
		{
			ZION_ASSERT(!"seq not found");
			return;
		}

		if(it->second.proc)
		{
			Json::Reader reader;
			Json::Value json;
			if(reader.parse(data, data+strlen(data), json))
			{
				it->second.proc(&json);
			}
			else
			{
				it->second.proc(NULL);
			}
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
				it->second.proc(NULL);
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
		if(*((_U32*)&ip4.sin_addr)==INADDR_NONE)
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
		m_uv_loop = uv_default_loop();
//		m_uv_loop = uv_loop_new();
//		uv_thread_create(&m_uv_thread, &CJsonRPCClientManager::ThreadProc, NULL);
	}

	CJsonRPCClientManager::~CJsonRPCClientManager()
	{
		m_is_stop = true;
//		uv_stop(m_uv_loop);
//		uv_thread_join(&m_uv_thread);
//		uv_loop_delete(m_uv_loop);
		uv_mutex_destroy(&m_Mutex);
	}

}
