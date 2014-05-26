#include <ZionDefines.h>
#include <ZionSTL.h>
#include <ZionSocket.h>
#include <FastJson.h>
#include <uv.h>
#include "JsonRPC.h"
#include "string.h"
#include <time.h>

#ifdef _WIN32
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "psapi.lib")
#endif

#define STATE_CONNECTING		((_U32)0x01)
#define STATE_CONNECTED			((_U32)0x02)
#define STATE_SHUTDOWNING		((_U32)0x04)
#define STATE_SHUTDOWN			((_U32)0x08)

namespace Zion
{

	typedef struct _JSONRPC_RESPONSE_DATA
	{
		JSONRPC_RESPONSE_ID id;
		String val;
	} JSONRPC_RESPONSE_DATA;

	typedef struct _JSONRPC_CALL
	{
		uv_work_t work;
		JSONRPC_RESPONSE_ID id;
		String res;
		String method;
		char* args;
		_U32 args_size;
		List<JSONRPC_RESPONSE_DATA> other_res;
	} JSONRPC_CALL;

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

		_U32 GetState();

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

		bool Start(const char* ep, bool singlethread);
		void Stop();
		bool IsSingleThread();

		bool Send(const char* data);
		bool SetPending(JSONRPC_RESPONSE_ID& res);
		bool Send(_U32 conn, _U32 seq, const char* data);

		bool Call(const String& method, const char* data, _U32 len);
		void Call(CJsonRPCServerConnection* pConn, _U32 seq, const String& method, const char* data, _U32 len);
		void Call(const JSONRPC_CALL& data);
		void Detach(_U32 id, CJsonRPCServerConnection* pConn);

		static void OnConnect(uv_stream_t* server, int status);
		static void OnClose(uv_handle_t* server);
		static void ThreadProc(void*);
		static void QuitEvent(uv_async_t* handle);

	private:
		CJsonRPCServerConnection* m_pCurConn;
		_U32 m_CurSeq;

		bool m_bSingleThread;

		Map<String, JSON_RESPONSE_PROC> m_Methods;
		_U32 m_ClientSeq;
		Map<_U32, CJsonRPCServerConnection*> m_Clients;
		uv_loop_t* m_uv_loop;
		uv_thread_t m_uv_thread;
		uv_async_t m_uv_quit;
		uv_tcp_t m_server;

		uv_key_t m_res_key;
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
		virtual ~CJsonRPCClient();

		ASOCK_HANDLE NewHandle();
		ASOCK_HANDLE GetHandle();
		void FreeHandle(ASOCK_HANDLE sock);
		void CloseHandle(ASOCK_HANDLE sock);

		bool Connect();
		bool Send(const char* method, const char* data);
		bool Send(const char* method, const char* data, JSON_CALLBACK_PROC proc);

		virtual void OnDisconnect();
		virtual void OnData(_U32 seq, const char* data, _U32 len);

		static void OnConnect(uv_connect_t* req, int status);

	private:
		uv_mutex_t m_locker;
		Map<ASOCK_HANDLE, time_t> m_socks;

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

		static void DisconnectAll();
		static void Reconnect();

	private:
		CJsonRPCClientManager();
		~CJsonRPCClientManager();

	protected:
		uv_mutex_t m_Mutex;
		Map<String, CJsonRPCClient*> m_Clients;
		uv_loop_t* m_uv_loop;
		uv_timer_t m_uv_timer;
		bool m_is_stop;
		uv_thread_t m_uv_thread;
	};

	static CJsonRPCServer g_server;

	static void jsonrpc_work_cb(uv_work_t* req)
	{
		JSONRPC_CALL& c = *((JSONRPC_CALL*)req);
		g_server.Call(c);
	}

	static void jsonrpc_after_work_cb(uv_work_t* req, int status)
	{
		JSONRPC_CALL& c = *((JSONRPC_CALL*)req);
		if(!c.res.empty())
		{
			g_server.Send(c.id.conn, c.id.seq, c.res.c_str());
		}

		List<JSONRPC_RESPONSE_DATA>& datas = ((JSONRPC_CALL*)req)->other_res;
		List<JSONRPC_RESPONSE_DATA>::iterator i;
		for(i=datas.begin(); i!=datas.end(); i++)
		{
			g_server.Send((*i).id.conn, (*i).id.seq, (*i).val.c_str());
		}
		((JSONRPC_CALL*)req)->~JSONRPC_CALL();
		ZION_FREE(req);
	}

	void SetInvalidResponseID(JSONRPC_RESPONSE_ID& id)
	{
		id.conn = (_U32)-1;
		id.seq = (_U32)-1;
	}

	bool IsValidResponseID(const JSONRPC_RESPONSE_ID& id)
	{
		return !(id.conn==(_U32)-1 && id.seq==(_U32)-1);
	}

	void JsonRPC_Bind(const char* method, JSON_RESPONSE_PROC proc)
	{
		g_server.Bind(method, proc);
	}

	bool JsonRPC_Start(const char* ep, bool singlethread)
	{
		return g_server.Start(ep, singlethread);
	}

	void JsonRPC_Stop()
	{
		g_server.Stop();
	}

	bool JsonRPC_Send(const char* args)
	{
		return g_server.Send(args);
	}

	bool JsonRPC_SetPending(JSONRPC_RESPONSE_ID& res)
	{
		return g_server.SetPending(res);
	}

	bool JsonRPC_Send(const JSONRPC_RESPONSE_ID& res, const char* args)
	{
		return g_server.Send(res.conn, res.seq, args);
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

	bool JsonRPC_Call(CJsonRPCClient* pClient, const char* method, const char* args, JSON_CALLBACK_PROC proc)
	{
		return pClient->Send(method, args, proc);
	}

	bool JsonRPC_Call(CJsonRPCClient* pClient, const char* method, const JsonValue& args, JSON_CALLBACK_PROC proc)
	{
		if(!args.IsArray()) return false;
		String json;
		args.Stringify(json);
		return JsonRPC_Call(pClient, method, json.c_str(), proc);
	}

	static bool makecall(ASOCK_HANDLE sock, const char* method, const String& args, String& ret)
	{
		char header[8];
		*((_U32*)header + 0) = (_U32)args.size();
		*((_U32*)header + 1) = 0;

		if(sock_writebuf(sock, header, 8)!=0) return false;
		if(sock_writebuf(sock, args.c_str(), (int)args.size())!=0) return false;
		if(sock_readbuf(sock, header, 8)!=0) return false;
		_U32 recvlen = 0, tlen = *((_U32*)header);
		ret = "";
		while(recvlen<tlen)
		{
			char buf[10000];
			_U32 len = tlen - recvlen;
			if(len>sizeof(buf)) len = sizeof(buf);
			if(sock_readbuf(sock, buf, len)!=0) return false;
			ret.append(buf, (size_t)len);
			recvlen += len;
		}
		return true;
	}

	bool JsonRpc_Call(CJsonRPCClient* pClient, const char* method, const JsonValue& args, JsonValue& ret)
	{
		if(!args.IsArray()) return false;
		String json, ret_json;
		args.Stringify(json);

		ASOCK_HANDLE sock = pClient->GetHandle();
		if(sock!=ASOCK_INVALID_HANDLE)
		{
			if(!makecall(sock, method, json, ret_json))
			{
				pClient->CloseHandle(sock);
				sock = ASOCK_INVALID_HANDLE;
			}
		}
		if(sock==ASOCK_INVALID_HANDLE)
		{
			sock = pClient->NewHandle();
			if(sock==ASOCK_INVALID_HANDLE) return false;
			if(!makecall(sock, method, json, ret_json))
			{
				pClient->CloseHandle(sock);
				return false;
			}
		}
		pClient->FreeHandle(sock);

		if(!ret.Parse(ret_json.c_str(), ret_json.c_str() + ret_json.size()))
		{
			return false;
		}

		return true;
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

	_U32 CJsonRPCConnection::GetState()
	{
		return m_state;
	}

	bool CJsonRPCConnection::Send(_U32 seq, const char* method, const char* data)
	{
		if(m_state!=STATE_CONNECTED)
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
		if(m_state!=STATE_CONNECTED) return false;

		sendbuf->conn = this;
		sendbuf->buf = uv_buf_init((char*)(sendbuf+1), len);
		m_send_count += 1;
		if(uv_write(&sendbuf->req, m_stream, &sendbuf->buf, 1, &CJsonRPCConnection::OnAfterWrite))
		{
			ZION_FATAL("uv_write failed");
			ZION_FREE(sendbuf);
			m_send_count -= 1;
		}
		return true;
	}

	void CJsonRPCConnection::Shutdown()
	{
		if(m_state==STATE_CONNECTED)
		{
			m_state = STATE_SHUTDOWNING;
			m_shutdown.data = this;
			if(uv_shutdown(&m_shutdown, m_stream, &CJsonRPCConnection::OnShutdown))
			{
				ZION_ASSERT(0);
			}
		}
	}

	void CJsonRPCConnection::OnConnect(uv_stream_t* stream, int status)
	{
		CJsonRPCConnection* pConn = (CJsonRPCConnection*)stream->data;

		pConn->m_state = STATE_CONNECTED;
		pConn->m_stream = stream;
		pConn->m_RecvLen = 0;

		if(uv_read_start(stream, &CJsonRPCConnection::OnBeforRead, &CJsonRPCConnection::OnAfterRead))
		{
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
			pConn->m_state = STATE_SHUTDOWNING;
			uv_close((uv_handle_t*)pConn->m_stream, &CJsonRPCConnection::OnClose);
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
			pConn->OnData(pkg_seq, data+8, pkg_len);
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
		if(conn->m_state!=STATE_CONNECTED) return;

		if(conn->m_sendbuf && conn->m_send_count==0)
		{
			SENDBUF* sendbuf = conn->m_sendbuf;
			conn->m_sendbuf = NULL;
			conn->Send(sendbuf, conn->m_sendbuf_len);
		}
	}

	void CJsonRPCConnection::OnShutdown(uv_shutdown_t* req, int status)
	{
		CJsonRPCConnection* pConn = (CJsonRPCConnection*)req->data;
//		pConn->Shutdown();
	}

	void CJsonRPCConnection::OnClose(uv_handle_t* handle)
	{
		CJsonRPCConnection* pConn = (CJsonRPCConnection*)handle->data;
		pConn->m_stream = NULL;
		pConn->m_state = STATE_SHUTDOWN;
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
		uv_key_create(&m_res_key);
}

	CJsonRPCServer::~CJsonRPCServer()
	{
		uv_key_delete(&m_res_key);
	}

	void CJsonRPCServer::Bind(const char* method, JSON_RESPONSE_PROC proc)
	{
		ZION_ASSERT(m_Methods.find(method)==m_Methods.end());
		m_Methods[method] = proc;
	}

	bool CJsonRPCServer::Start(const char* ep, bool singlethread)
	{
		m_bSingleThread = singlethread;

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

		if(uv_tcp_bind(&m_server, &sa, 0))
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
		uv_async_init(m_uv_loop, &m_uv_quit, CJsonRPCServer::QuitEvent);
		uv_thread_create(&m_uv_thread, &CJsonRPCServer::ThreadProc, this);
		return true;
	}

	void CJsonRPCServer::Stop()
	{
		uv_async_send(&m_uv_quit);
		uv_thread_join(&m_uv_thread);
		uv_loop_delete(m_uv_loop);
	}
	
	bool CJsonRPCServer::IsSingleThread()
	{
		return m_bSingleThread;
	}

	bool CJsonRPCServer::Send(const char* data)
	{
		if(m_bSingleThread)
		{
			ZION_ASSERT(m_pCurConn);
			if(!m_pCurConn) return false;
			m_pCurConn->Send(m_CurSeq, NULL, data);
			m_pCurConn = NULL;
		}
		else
		{
			JSONRPC_CALL* c = (JSONRPC_CALL*)uv_key_get(&m_res_key);
			ZION_ASSERT(c);
			if(!c) return false;
			c->res = data;
			uv_key_set(&m_res_key, NULL);
		}
		return true;
	}
	
	bool CJsonRPCServer::SetPending(JSONRPC_RESPONSE_ID& res)
	{
		if(m_bSingleThread)
		{
			ZION_ASSERT(m_pCurConn);
			if(!m_pCurConn) return false;
			res.conn = m_pCurConn->GetID();
			res.seq = m_CurSeq;
			m_pCurConn = NULL;
		}
		else
		{
			JSONRPC_CALL* c = (JSONRPC_CALL*)uv_key_get(&m_res_key);
			ZION_ASSERT(c);
			if(!c) return false;
			res = c->id;
			uv_key_set(&m_res_key, NULL);
		}
		return true;
	}

	bool CJsonRPCServer::Send(_U32 conn, _U32 seq, const char* data)
	{
		Map<_U32, CJsonRPCServerConnection*>::iterator i;
		i = m_Clients.find(conn);
		if(i==m_Clients.end()) return false;
		return i->second->Send(seq, NULL, data);
	}

	bool CJsonRPCServer::Call(const String& method, const char* data, _U32 len)
	{
		Map<String, JSON_RESPONSE_PROC>::iterator i;
		i = m_Methods.find(method);
		if(i==m_Methods.end())
		{
			ZION_ASSERT(!"method not found");
			return false;
		}

		JsonValue json;
		if(!json.Parse(data, data+len) || !json.IsArray())
		{
			ZION_ASSERT(!"invalid data format");
			return false;
		}

		i->second(json);
		return true;
	}
	void CJsonRPCServer::Call(CJsonRPCServerConnection* pConn, _U32 seq, const String& method, const char* data, _U32 len)
	{
		if(m_bSingleThread)
		{
			m_pCurConn = pConn;
			m_CurSeq = seq;
			if(Call(method, data, len))
			{
				ZION_ASSERT(m_pCurConn==NULL);
			}
		}
		else
		{
			JSONRPC_CALL* c = new (ZION_ALLOC(sizeof(JSONRPC_CALL)+len)) JSONRPC_CALL();
			c->id.conn = pConn->GetID();
			c->id.seq = seq;
			c->method = method;
			c->args = (char*)(c+1);
			memcpy(c->args, data, len);
			c->args_size = len;
			if(uv_queue_work(m_uv_loop, &c->work, jsonrpc_work_cb, jsonrpc_after_work_cb))
			{
				c->~JSONRPC_CALL();
				ZION_FREE(c);
			}
		}
	}

	void CJsonRPCServer::Call(const JSONRPC_CALL& data)
	{
		uv_key_set(&m_res_key, (void*)&data);
		if(Call(data.method, data.args, data.args_size))
		{
			ZION_ASSERT(uv_key_get(&m_res_key)==NULL);
		}
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

	void CJsonRPCServer::QuitEvent(uv_async_t* handle)
	{
		uv_close((uv_handle_t*)handle, NULL);

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
		uv_mutex_init(&m_locker);

		ZION_ASSERT(sizeof(struct sockaddr_in6)>=sizeof(struct sockaddr_in));

		m_seq = 1;
		memcpy(&m_sa, sa, sizeof(struct sockaddr_in6));
	}

	CJsonRPCClient::~CJsonRPCClient()
	{
		while(!m_socks.empty())
		{
			ASOCK_HANDLE sock = m_socks.begin()->first;
			m_socks.erase(m_socks.begin());
			CloseHandle(sock);
		}
		uv_mutex_destroy(&m_locker);
	}

	ASOCK_HANDLE CJsonRPCClient::NewHandle()
	{
		ASOCK_ADDR sa;
		sa.ip = *((unsigned int*)&m_ip4.sin_addr);
		sa.port = ntohs(m_ip4.sin_port);
		return sock_connect(&sa, 0);
	}

	ASOCK_HANDLE CJsonRPCClient::GetHandle()
	{
		ASOCK_HANDLE sock = ASOCK_INVALID_HANDLE;
		uv_mutex_lock(&m_locker);
		if(!m_socks.empty())
		{
			sock = m_socks.begin()->first;
			m_socks.erase(m_socks.begin());
		}
		m_socks[sock] = time(NULL);
		uv_mutex_unlock(&m_locker);
		return sock;
	}
	
	void CJsonRPCClient::FreeHandle(ASOCK_HANDLE sock)
	{
		uv_mutex_lock(&m_locker);
		m_socks[sock] = time(NULL);
		uv_mutex_unlock(&m_locker);
	}

	void CJsonRPCClient::CloseHandle(ASOCK_HANDLE sock)
	{
		sock_close(sock);
	}

	bool CJsonRPCClient::Connect()
	{
		if(CJsonRPCClientManager::Get().m_is_stop)
		{
			return false;
		}

		if(m_state!=0)
		{
			ZION_ASSERT(0);
			return false;
		}

		m_state = STATE_CONNECTING;

		if(uv_tcp_init(CJsonRPCClientManager::Get().m_uv_loop, &m_socket))
		{
			m_state = 0;
			ZION_ASSERT(0);
			return false;
		}

		m_connect.data = this;
		m_socket.data = this;
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
		if(m_state!=STATE_CONNECTED)
		{
			struct JSON_RPCCALL call;
			call.method = method;
			call.data = data;
			while(m_Requests.find(m_seq)!=m_Requests.end())
			{
				m_seq++;
			}
			m_Requests[m_seq++] = call;

			if(m_state==0 && !Connect())
			{
				ZION_ASSERT(0);
				return false;
			}

			return true;
		}

		return CJsonRPCConnection::Send(0, method, data);
	}

	bool CJsonRPCClient::Send(const char* method, const char* data, JSON_CALLBACK_PROC proc)
	{
		while(m_Requests.find(m_seq)!=m_Requests.end())
		{
			m_seq++;
		}
		struct JSON_RPCCALL call;
		call.method = method;
		call.data = data;
		call.proc = proc;
		m_Requests[m_seq++] = call;

		if(m_state!=STATE_CONNECTED)
		{
			if(m_state==0)
			{
				if(!Connect())
				{
					ZION_ASSERT(0);
					return false;
				}
			}
			return true;
		}

		if(!CJsonRPCConnection::Send(m_seq-1, method, data))
		{
			m_Requests.erase(m_seq-1);
			return false;
		}

		return true;
	}

	void CJsonRPCClient::OnDisconnect()
	{
		Map<_U32, JSON_RPCCALL> Requests = m_Requests;
		m_Requests.clear();

		while(!Requests.empty())
		{
			struct JSON_RPCCALL& call = Requests.begin()->second;
			if(call.proc)
			{
				call.proc(NULL);
			}
			Requests.erase(Requests.begin());
		}
		m_state = 0;
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
			JsonValue json;
			if(json.Parse(data, data+len))
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
			pClient->OnDisconnect();
			return;
		}

		req->handle->data = pClient;
		CJsonRPCConnection::OnConnect(req->handle, status);
		if(pClient->m_state!=STATE_CONNECTED) return;

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
				((CJsonRPCConnection*)pClient)->Send(0, it->second.method.c_str(), it->second.data.c_str());
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

	static void on_client_timer(uv_timer_t* handle)
	{
		CJsonRPCClientManager::Reconnect();
	}

	CJsonRPCClientManager::CJsonRPCClientManager()
	{
		m_is_stop = false;
		uv_mutex_init(&m_Mutex);
		m_uv_loop = uv_default_loop();
		uv_timer_init(uv_default_loop(), &m_uv_timer);
		uv_timer_start(&m_uv_timer, on_client_timer, 1000, 1000);
//		m_uv_loop = uv_loop_new();
//		uv_thread_create(&m_uv_thread, &CJsonRPCClientManager::ThreadProc, NULL);
	}

	CJsonRPCClientManager::~CJsonRPCClientManager()
	{
//		uv_stop(m_uv_loop);
//		uv_thread_join(&m_uv_thread);
//		uv_loop_delete(m_uv_loop);
		uv_mutex_destroy(&m_Mutex);
	}

	void CJsonRPCClientManager::DisconnectAll()
	{
		Get().m_is_stop = true;
		uv_timer_stop(&Get().m_uv_timer);
		Map<String, CJsonRPCClient*>::iterator i;
		for(i=Get().m_Clients.begin(); i!=Get().m_Clients.end(); i++)
		{
			i->second->Shutdown();
		}
	}

	void CJsonRPCClientManager::Reconnect()
	{
		Map<String, CJsonRPCClient*>::iterator i;
		for(i=Get().m_Clients.begin(); i!=Get().m_Clients.end(); i++)
		{
			if(i->second->GetState()==0)
			{
				i->second->Connect();
			}
		}
	}

}
