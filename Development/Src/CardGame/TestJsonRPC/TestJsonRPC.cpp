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
	_U8 data[2000*1024];
};

struct _CONNECTION
{
	uv_tcp_t socket;

	bool sending;
	SENDBUF* sendbuf;

	_U32 recv_len;
	_U8 recv_buf[2000*1024];
};

static void server_accept(uv_stream_t* server, int status);
static void client_connecct(uv_stream_t* server, int status);
static void client_before_read(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
static void client_after_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
static void client_after_write(uv_write_t* req, int status);
static void client_close(uv_handle_t* handle);
static void client_send(CONNECTION* conn, void* data, _U32 len);

void server_accept(uv_stream_t* server, int status)
{
	CONNECTION* conn = (CONNECTION*)ZION_ALLOC(sizeof(CONNECTION));
	conn->socket.data = conn;
	conn->sending = false;
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
	buf->len = sizeof(conn->recv_buf) - conn->recv_len;

}

void client_after_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	CONNECTION* conn = (CONNECTION*)stream;
	if(nread<=0)
	{
		uv_close((uv_handle_t*)stream, client_close);
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
	ZION_ASSERT(conn->sending);
	if(conn->sendbuf)
	{
		SENDBUF* sendbuf = conn->sendbuf;
		conn->sendbuf = NULL;
		sendbuf->buf = uv_buf_init((char*)sendbuf->data, sendbuf->len);
		if(uv_write(&sendbuf->req, (uv_stream_t*)&conn->socket, &sendbuf->buf, 1, client_after_write))
		{
			ZION_FREE(sendbuf);
			conn->sending = false;
			return;
		}
	}
	else
	{
		conn->sending = false;
	}
}

void client_close(uv_handle_t* handle)
{
	ZION_FREE(handle);
}

void client_send(CONNECTION* conn, void* data, _U32 len)
{
	SENDBUF* sendbuf = conn->sendbuf;
	if(!sendbuf)
	{
		sendbuf = (SENDBUF*)ZION_ALLOC(sizeof(SENDBUF));
		sendbuf->len = 0;
		sendbuf->conn = conn;
		conn->sendbuf = sendbuf;
	}

	ZION_ASSERT(sendbuf->len+len<sizeof(sendbuf->data));

	memcpy(sendbuf->data+sendbuf->len, data, len);
	sendbuf->len += len;
	if(!conn->sending)
	{
		conn->sendbuf = NULL;
		sendbuf->buf = uv_buf_init((char*)sendbuf->data, sendbuf->len);
		if(uv_write(&sendbuf->req, (uv_stream_t*)&conn->socket, &sendbuf->buf, 1, client_after_write))
		{
			ZION_FREE(sendbuf);
			conn->sending = false;
			return;
		}
		conn->sending = true;
	}
}

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
