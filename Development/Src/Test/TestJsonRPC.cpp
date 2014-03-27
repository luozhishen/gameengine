#include <ZionBase.h>
#include <JsonRPC.h>
#include <uv.h>
#include <stdio.h>

static _U32 completed_count = 0;
static _U32 error_count = 0;
static Zion::CJsonRPCClient* client;
static _U32 seq = 0;

void timer_callback(uv_timer_t* handle, int status)
{
	static _U32 last_completed = 0;
	printf("%8u %15u %15u %15u\n", seq++, completed_count-last_completed, error_count, completed_count+error_count);
	last_completed = completed_count;
}

void jsonrpc_callback(const Zion::JsonValue* val)
{
	if(val)
	{
		completed_count += 1;
	}
	else
	{
		error_count += 1;
	}
	Zion::JsonRPC_Send(client, "echo", "[0]", jsonrpc_callback);
}

int main(int argc, char* argv[])
{
	if(argc!=2) return -1;
	client = Zion::JsonRPC_GetClient(argv[1]);
	uv_timer_t timer;
	uv_timer_init(uv_default_loop(), &timer);
	uv_timer_start(&timer, timer_callback, 1000, 1000);

	for(_U32 i=0; i<100000; i++)
	{
		Zion::JsonRPC_Send(client, "echo", "[0]", jsonrpc_callback);
	}

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	return 0;
}

#if 0
#include <ZionDefines.h>
#include <uv.h>

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

	_U32 send_count;
	SENDBUF* sendbuf;

	_U32 recv_len;
	_U8 recv_buf[1];
};

#define JSONRPC_SENDBUF_SIZE		(10*1024)
#define JSONRPC_RECVBUF_SIZE		(1*1024*1024)

static void server_accept(uv_stream_t* server, int status);
static void client_connecct(uv_stream_t* server, int status);
static void client_before_read(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
static void client_after_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
static void client_after_write(uv_write_t* req, int status);
static void client_close(uv_handle_t* handle);
static bool client_send(CONNECTION* conn, void* data, _U32 len);
static void client_rawsend(CONNECTION* conn, SENDBUF* buf);

void server_accept(uv_stream_t* server, int status)
{
	CONNECTION* conn = (CONNECTION*)ZION_ALLOC(sizeof(CONNECTION)+JSONRPC_RECVBUF_SIZE);
	conn->socket.data = conn;
	conn->send_count = 0;
	conn->sendbuf = NULL;
	conn->recv_len = 0;
	if(uv_tcp_init(uv_default_loop(), &conn->socket))
	{
		ZION_ASSERT(!"uv_tcp_init failed");
		ZION_FREE(conn);
		return;
	}
	if(uv_accept(server, (uv_stream_t*)&conn->socket))
	{
		ZION_ASSERT(!"uv_accept failed");
		ZION_FREE(conn);
		return;
	}
	client_connecct((uv_stream_t*)conn, 0);
}

void client_connecct(uv_stream_t* stream, int status)
{
	CONNECTION* conn = (CONNECTION*)stream->data;
	if(uv_read_start((uv_stream_t*)&conn->socket, client_before_read, client_after_read))
	{
		ZION_ASSERT(!"uv_read_start failed");
		ZION_FREE(conn);
		return;
	}
}

void client_before_read(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	CONNECTION* conn = (CONNECTION*)handle;
	buf->base = (char*)(conn->recv_buf + conn->recv_len);
	buf->len = JSONRPC_RECVBUF_SIZE - conn->recv_len;

}

void client_after_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	CONNECTION* conn = (CONNECTION*)stream;
	if(nread<=0)
	{
		uv_close((uv_handle_t*)stream, client_close);
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
		char* out_txt = "[0]";
		_U32 out_len = 3;
		client_send(conn, &out_len, sizeof(out_len));
		client_send(conn, &pkg_seq, sizeof(pkg_seq));
		client_send(conn, out_txt, out_len);
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

void client_after_write(uv_write_t* req, int status)
{
	CONNECTION* conn = ((SENDBUF*)req)->conn;
	ZION_FREE(req);
	ZION_ASSERT(conn->send_count>0);
	conn->send_count -= 1;
	if(uv_is_writable((uv_stream_t*)&conn->socket) && conn->sendbuf && conn->send_count==0)
	{
		SENDBUF* sendbuf = conn->sendbuf;
		conn->sendbuf = NULL;
		client_rawsend(conn, sendbuf);
	}
}

void client_close(uv_handle_t* handle)
{
	CONNECTION* conn = ((CONNECTION*)handle->data);
	if(conn->sendbuf) ZION_FREE(conn->sendbuf);
	ZION_FREE(conn);
}

bool client_send(CONNECTION* conn, void* data, _U32 len)
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
			sendbuf = (SENDBUF*)ZION_ALLOC(sizeof(SENDBUF)+JSONRPC_SENDBUF_SIZE);
			sendbuf->len = 0;
		}

		if(sendbuf->len+len<JSONRPC_SENDBUF_SIZE)
		{
			memcpy(sendbuf->data+sendbuf->len, data, len);
			sendbuf->len += len;
			break;
		}
		else
		{
			_U32 sendlen = JSONRPC_SENDBUF_SIZE - sendbuf->len;
			memcpy(sendbuf->data+sendbuf->len, data, sendlen);
			sendbuf->len += sendlen;
			client_rawsend(conn, sendbuf);
			sendbuf = NULL;
			data = (char*)data + sendlen;
			len -= sendlen;
		}
	}

	if(sendbuf && conn->send_count==0)
	{
		client_rawsend(conn, sendbuf);
		conn->sendbuf = NULL;
	}
	else
	{
		conn->sendbuf = sendbuf;
	}

	return true;
}

void client_rawsend(CONNECTION* conn, SENDBUF* buf)
{
	buf->conn = conn;
	buf->buf = uv_buf_init((char*)buf->data, buf->len);
	if(uv_write(&buf->req, (uv_stream_t*)&conn->socket, &buf->buf, 1, client_after_write))
	{
		ZION_FATAL("uv_write failed");
		ZION_FREE(buf);
	}
	else
	{
		conn->send_count += 1;
	}
}

/*
uv_signal_t sig;
    uv_signal_init(loop, &sig);
    uv_signal_start(&sig, signal_handler, SIGINT);
*/

int main(int argc, char* argv[])
{
	struct sockaddr_in ip4;
	uv_ip4_addr("0.0.0.0", 1980, &ip4);
	uv_tcp_t m_server;

	if(uv_tcp_init(uv_default_loop(), &m_server))
	{
		ZION_ASSERT(0);
		return -1;
	}

	if(uv_tcp_bind(&m_server, (const sockaddr*)&ip4))
	{
		ZION_ASSERT(0);
		return -1;
	}

	if(uv_listen((uv_stream_t*)&m_server, SOMAXCONN, server_accept))
	{
		ZION_ASSERT(0);
		return -1;
	}
	
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	return 0;
}
#endif
/*
#include <ZionBase.h>
#include <JsonRPC.h>

void welcome_proc(const Zion::JSONRPC_RESPONSE& res, const char* args)
{
	Zion::JsonRPC_Send(res, args);
}

int main(int argc, char* argv[])
{

	Zion::CJsonRPCServer* pServer = Zion::JsonRPC_CreateServer();
	Zion::JsonRPC_Bind(pServer, "welcome", welcome_proc);
	Zion::JsonRPC_Bind(pServer, "back", welcome_proc);
	Zion::JsonRPC_Start(pServer, "127.0.0.1:8001");

	getchar();

	Zion::JsonRPC_Stop(pServer);
	Zion::JsonRPC_DestroyServer(pServer);

#if 0
	Zion::CJsonRPCClient* pClient = Zion::JsonRPC_GetClient("127.0.0.1:8000");
	printf("send data to xxxxxxx\n");
	JsonRPC_Send(pClient, "welcome", "\"welcome aaaaaaaaa\"");
	getchar();
#endif

	return 0;
}
*/
