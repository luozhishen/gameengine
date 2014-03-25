#include <ZionDefines.h>
#include <ZionSTL.h>
#include <json/jsoncpp.h>
#include "JsonRPC_2.h"

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

namespace JsonRPC2
{

	struct _SENDBUF;
	typedef struct _SENDBUF SENDBUF;
	struct _CONNECTION;
	typedef struct _CONNECTION CONNECTION;

	struct _SENDBUF
	{
		uv_write_t req;
		uv_buf_t buf;
		CONNECTION* conn;
		_U32 len;
		_U8 data[1];
	};

	struct _CONNECTION
	{
		uv_tcp_t socket; 

		_U32 index;
		CJsonRPCServer* server;

		CJsonRPCClient* client;
		Zion::Map<_U32, JSON_CALLBACK_PROC> callbacks;
		Zion::List<SENDBUF*> sendbufs;

		_U32 send_count;
		SENDBUF* sendbuf;

		_U32 recv_len;
		_U8 recv_buf[1];

		bool (*on_data)(CONNECTION* conn, _U32 seq, const char* data, _U32 len);
	};

	static void conn_connecct(uv_stream_t* server, int status);
	static void conn_before_read(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
	static void conn_after_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	static void conn_after_write(uv_write_t* req, int status);
	static void conn_close(uv_handle_t* handle);
	static bool conn_send(CONNECTION* conn, void* data, _U32 len, bool delay);
	static void conn_rawsend(CONNECTION* conn, SENDBUF* buf);

	static void server_accept(uv_stream_t* server, int status);
	static bool server_ondata(CONNECTION* conn, _U32 seq, const char* data, _U32 len);
	static bool client_ondata(CONNECTION* conn, _U32 seq, const char* data, _U32 len);
	static void server_disconnect(CONNECTION* conn);
	static void client_disconnect(CONNECTION* conn);

	void conn_connecct(uv_stream_t* stream, int status)
	{
		CONNECTION* conn = (CONNECTION*)stream->data;

		if(status==0)
		{
			if(uv_read_start((uv_stream_t*)&conn->socket, conn_before_read, conn_after_read)==0)
			{
				return;
			}
			else
			{
				ZION_ASSERT(0);
			}
		}
		else
		{
			ZION_ASSERT(0);
		}
		conn_close((uv_handle_t*)&conn->socket);
	}

	void conn_before_read(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{
		CONNECTION* conn = (CONNECTION*)handle;
		buf->base = (char*)(conn->recv_buf + conn->recv_len);
		buf->len = sizeof(conn->recv_buf) - conn->recv_len;

	}

	void conn_after_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
	{
		CONNECTION* conn = (CONNECTION*)stream;
		if(nread<=0)
		{
			uv_close((uv_handle_t*)stream, conn_close);
			return;
		}
		conn->recv_len += (_U32)nread;

		char* data = (char*)conn->recv_buf;
		_U32 len = conn->recv_len;
		while(len>=8)
		{
			_U32 pkg_len = *((_U32*)data + 0);
			_U32 pkg_seq = *((_U32*)data + 1);
			if(8+pkg_len>len) break;

			if(!conn->on_data(conn, pkg_seq, data, pkg_len))
			{
				ZION_FATAL("invalidate");
			}

			len -= 8 + pkg_len;
			data += 8 + pkg_len;
		}

		if(len==0)
		{
			conn->recv_len = 0;
		}
		else
		{
			ZION_ASSERT(len<=conn->recv_len);
			memmove(conn->recv_buf, data, len);
			conn->recv_len = len;
		}
	}

	void conn_after_write(uv_write_t* req, int status)
	{
		CONNECTION* conn = ((SENDBUF*)req)->conn;
		ZION_FREE(req);
		ZION_ASSERT(conn->send_count>0);
		conn->send_count -= 1;
		if(uv_is_writable((uv_stream_t*)&conn->socket) && conn->sendbuf && conn->send_count==0)
		{
			SENDBUF* sendbuf = conn->sendbuf;
			conn->sendbuf = NULL;
			conn_rawsend(conn, sendbuf);
		}
	}

	void conn_close(uv_handle_t* handle)
	{
		CONNECTION* conn = ((CONNECTION*)handle->data);
		if(conn->sendbuf) ZION_FREE(conn->sendbuf);
		if(conn->server)
		{
			server_disconnect(conn);
		}
		else
		{
			client_disconnect(conn);
		}
		conn->~CONNECTION();
		ZION_FREE(conn);
	}

	bool conn_send(CONNECTION* conn, void* data, _U32 len, bool delay)
	{
		if(!uv_is_writable((uv_stream_t*)&conn->socket))
		{
			return false;
		}

		SENDBUF* sendbuf = conn->sendbuf;
		while(len>0)
		{
			if(!sendbuf)
			{
				sendbuf = (SENDBUF*)ZION_ALLOC(sizeof(SENDBUF)+SENDBUF_SIZE);
				sendbuf->len = 0;
			}

			if(sendbuf->len+len<SENDBUF_SIZE)
			{
				memcpy(sendbuf->data+sendbuf->len, data, len);
				sendbuf->len += len;
				break;
			}
			else
			{
				_U32 sendlen = SENDBUF_SIZE - sendbuf->len;
				memcpy(sendbuf->data+sendbuf->len, data, sendlen);
				sendbuf->len += sendlen;
				conn_rawsend(conn, sendbuf);
				sendbuf = NULL;
				data = (char*)data + sendlen;
				len -= sendlen;
			}
		}

		if(sendbuf && conn->send_count==0 && !delay)
		{
			conn_rawsend(conn, sendbuf);
			conn->sendbuf = NULL;
		}
		else
		{
			conn->sendbuf = sendbuf;
		}

		return true;
	}

	void conn_rawsend(CONNECTION* conn, SENDBUF* buf)
	{
		buf->conn = conn;
		buf->buf = uv_buf_init((char*)buf->data, buf->len);
		if(uv_write(&buf->req, (uv_stream_t*)&conn->socket, &buf->buf, 1, conn_after_write))
		{
			ZION_FATAL("uv_write failed");
			ZION_FREE(buf);
		}
		else
		{
			conn->send_count += 1;
		}
	}

	class CJsonRPCServer
	{
	public:
		CJsonRPCServer()
		{
			client_seq = 1980;
		}

		Zion::Map<Zion::String, JSON_RESPONSE_PROC> methods;
		Zion::Map<_U32, CONNECTION*> clients;
		_U32 client_seq;
		uv_tcp_t m_server;
	};
	static CJsonRPCServer g_server;

	static CONNECTION*	g_cur_conn = NULL;
	static _U32			g_cur_seq = NULL;

	class CJsonRPCClient
	{
	public:
		CJsonRPCClient(struct sockaddr& _sa)
		{
			sa = _sa;
		}
		struct sockaddr sa;
	};
	class CJsonRPCClientManager
	{
	public:
		~CJsonRPCClientManager()
		{
			Zion::Map<Zion::String, CJsonRPCClient*>::iterator i;
			for(i=clients.begin(); i!=clients.end(); i++)
			{
				ZION_DELETE i->second;
			}
			clients.clear();
		}
		Zion::Map<Zion::String, CJsonRPCClient*> clients;
	};
	static CJsonRPCClientManager g_manager;

	void server_accept(uv_stream_t* server, int status)
	{
		CONNECTION* conn = (CONNECTION*)ZION_ALLOC(sizeof(CONNECTION)+RECVBUF_SIZE);
		conn = new(conn) CONNECTION;
		conn->socket.data = conn;
		conn->send_count = 0;
		conn->sendbuf = NULL;
		conn->recv_len = 0;
		conn->on_data = server_ondata;
		conn->server = &g_server;
		do
		{
			conn->index = g_server.client_seq++;
		} while(g_server.clients.find(conn->index)!=g_server.clients.end());
		g_server.clients[conn->index] = conn;
		if(uv_tcp_init(uv_default_loop(), &conn->socket))
		{
			ZION_ASSERT(!"uv_tcp_init failed");
			conn->~CONNECTION();
			ZION_FREE(conn);
			return;
		}
		if(uv_accept(server, (uv_stream_t*)&conn->socket))
		{
			ZION_ASSERT(!"uv_accept failed");
			conn->~CONNECTION();
			ZION_FREE(conn);
			return;
		}
		if(uv_read_start((uv_stream_t*)&conn->socket, conn_before_read, conn_after_read))
		{
			ZION_ASSERT(!"uv_read_start failed");
			conn->~CONNECTION();
			ZION_FREE(conn);
			return;
		}
	}

	bool server_ondata(CONNECTION* conn, _U32 seq, const char* data, _U32 len)
	{
		Zion::String method;
		_U32 pos;
		for(pos=0; pos<len && data[pos]!=':'; pos++);
		method.append(data, pos);

		Zion::Map<Zion::String, JSON_RESPONSE_PROC>::iterator i;
		i = g_server.methods.find(method);
		if(i==g_server.methods.end())
		{
			ZION_ASSERT(0);
			return false;
		}

		Json::Reader reader;
		Json::Value json;
		if(!reader.parse(data+pos+1, data+len-1, json))
		{
			ZION_ASSERT(0);
			return false;
		}

		g_cur_conn = conn;
		g_cur_seq = seq;
		i->second(json);
		ZION_ASSERT(!g_cur_conn);
		return true;
	}

	bool client_ondata(CONNECTION* conn, _U32 seq, const char* data, _U32 len)
	{
		Zion::Map<_U32, JSON_CALLBACK_PROC>::iterator i;
		i = conn->callbacks.find(seq);
		if(i==conn->callbacks.end())
		{
			ZION_ASSERT(0);
			return false;
		}

		Json::Reader reader;
		Json::Value json;
		if(!reader.parse(data, data+len-1, json))
		{
			ZION_ASSERT(0);
			return false;
		}

		i->second(&json);
		return true;
	}

	void server_disconnect(CONNECTION* conn)
	{
		g_server.clients.erase(conn->index);
	}

	void client_disconnect(CONNECTION* conn)
	{
		Zion::Map<_U32, JSON_CALLBACK_PROC>::iterator i;
		for(i=conn->callbacks.begin(); i!=conn->callbacks.end(); i++)
		{
			i->second(NULL);
		}
		conn->callbacks.clear();
		
		while(!conn->sendbufs.empty())
		{
			ZION_FREE(conn->sendbufs.front());
			conn->sendbufs.pop_front();
		}
	}

	void JsonRPC_Bind(const char* method, JSON_RESPONSE_PROC proc)
	{
		g_server.methods[method] = proc;
	}

	bool JsonRPC_Start(const char* ep)
	{
		const char* pos = strchr(ep, ':');
		if(!pos) return false;

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

		g_server.m_server.data = &g_server;
		if(uv_tcp_init(uv_default_loop(), &g_server.m_server))
		{
			ZION_ASSERT(0);
			return false;
		}
		if(uv_tcp_bind(&g_server.m_server, &sa))
		{
			ZION_ASSERT(0);
			return false;
		}
		if(uv_listen((uv_stream_t*)&g_server.m_server, SOMAXCONN, server_accept))
		{
			ZION_ASSERT(0);
			return false;
		}

		return true;
	}

	void JsonRPC_Stop()
	{
	}

	bool JsonRPC_Send(const char* args, _U32 len)
	{
		ZION_ASSERT(g_cur_conn);
		if(!g_cur_conn) return false;
		if(len==(_U32)-1) len = (_U32)strlen(args);
		if(!conn_send(g_cur_conn, &len, sizeof(len), true)) return false;
		if(!conn_send(g_cur_conn, &g_cur_seq, sizeof(g_cur_seq), true)) return false;
		if(!conn_send(g_cur_conn, (void*)args, len, true)) return false;
		return true;
	}

	bool JsonRPC_SetPending(JSONRPC_RESPONSE& res)
	{
		ZION_ASSERT(g_cur_conn);
		if(!g_cur_conn) return false;
		res.conn_id = g_cur_conn->index;
		res.req_seq = g_cur_seq;
		g_cur_conn = NULL;
		return true;
	}

	bool JsonRPC_Send(const JSONRPC_RESPONSE& res, const char* args, _U32 len)
	{
		Zion::Map<_U32, CONNECTION*>::iterator i;
		i = g_server.clients.find(res.conn_id);
		if(i==g_server.clients.end()) return false;
		if(len==(_U32)-1) len = (_U32)strlen(args);
		if(!conn_send(i->second, &len, sizeof(len), true)) return false;
		if(!conn_send(i->second, (void*)&res.req_seq, sizeof(res.req_seq), true)) return false;
		if(!conn_send(i->second, (void*)args, len, true)) return false;
		return true;
	}

	CJsonRPCClient* JsonRPC_GetClient(const char* ep)
	{
		Zion::Map<Zion::String, CJsonRPCClient*>::iterator i;
		i = g_manager.clients.find(ep);
		if(i!=g_manager.clients.end()) return i->second;

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

		CJsonRPCClient* client = ZION_NEW CJsonRPCClient(sa);
		g_manager.clients[ep] = client;
		return NULL;
	}

	void JsonRPC_StopClients()
	{
	}

	bool JsonRPC_Send(CJsonRPCClient* pClient, const char* method, const char* args)
	{
		return true;
	}

	bool JsonRPC_Send(CJsonRPCClient* pClient, const char* method, const char* args, JSON_CALLBACK_PROC proc)
	{
		return true;
	}

}
