#if 0

#include <ZionDefines.h>
#include <ZionSTL.h>
#include <uv.h>

#include "FastCGIServer.h"
#include "fastcgi.h"

namespace Zion
{

	static void server_accept(uv_stream_t* server, int status);
	static void client_connecct(uv_stream_t* server, int status);
	static void client_read_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
	static void client_read_done(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	static void client_write_done(uv_write_t* req, int status);
	static void client_close(uv_handle_t* handle);
	// static bool client_send(CONNECTION* conn, void* data, _U32 len);
	// static void client_rawsend(CONNECTION* conn, SENDBUF* buf);

	static uv_tcp_t m_fcgi_server;

	bool FCGI_StartServer(const char* ep)
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

		if(uv_tcp_init(uv_default_loop(), &m_fcgi_server))
		{
			ZION_ASSERT(0);
			return false;
		}

		if(uv_tcp_bind(&m_fcgi_server, &sa))
		{
			ZION_ASSERT(!"uv_tcp_bind failed");
			return false;
		}

		if(uv_listen((uv_stream_t*)&m_fcgi_server, SOMAXCONN, &server_accept))
		{
			ZION_ASSERT(!"uv_listen failed");
			return false;
		}

		return true;
	}

	void FCGI_StopServer()
	{
	}

	bool FCGI_Bind(const char* url, FCGI_RESPONSE_PROC proc)
	{
		return true;
	}

}

#endif
