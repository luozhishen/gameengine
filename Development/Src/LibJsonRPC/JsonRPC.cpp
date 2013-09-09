#include <ZionDefines.h>
#include <ZionSTL.h>

#include "uv.h"
#include "JsonRPC.h"

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "psapi.lib")

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

		static void OnConnect(uv_stream_t* stream, int status);
		static void OnBeforRead(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
		static void OnAfterRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
		static void OnAfterWrite(uv_write_t* req, int status);
		static void OnShutdown(uv_shutdown_t* req, int status);

	private:
		uv_mutex_t m_Mutex;
		_U32 m_wrt_count;
		uv_stream_t* m_steam;
		uv_shutdown_t m_shutdown;

		char m_RecvBuf[1024];
		_U32 m_RecvLen;

		_U32 m_PacketLen;
		_U32 m_PacketSeq;
		String m_PacketData;
	};

	class CJsonRPCServer
	{
	public:
		CJsonRPCServer();

		bool Start(const char* ep);
		void Stop();

	private:
		uv_mutex_t m_Mutex;
		Set<CJsonRPCClient*> m_Clients;
		uv_loop_t* m_uv_loop;
		bool m_is_stop;
		uv_thread_t m_uv_thread;
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

		void Send(const char* method, const char* data);
		void Send(const char* method, const char* data, JSON_CALLBACK_PROC proc);

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
		bool m_enable;
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

	CJsonRPCServer* JsonRPC_StartServer(const char* ep)
	{
		CJsonRPCServer* pServer = ZION_NEW CJsonRPCServer();
		if(!pServer->Start(ep)) {
			ZION_DELETE pServer;
			return NULL;
		}
		return pServer;
	}

	void JsonRPC_StopServer(CJsonRPCServer* pServer)
	{
		pServer->Stop();
	}

	void JsonRPC_BindMethod(const char* method, JSON_RESPONSE_PROC proc)
	{
	}

	CJsonRPCClient* JsonRPC_GetClient(const char* ep)
	{
		return CJsonRPCClientManager::GetClient(ep);
	}

	void JsonRPC_StopClients()
	{
	}

	void JsonRPC_Send(CJsonRPCClient* pClient, const char* method, const char* args)
	{
		pClient->Send(method, args);
	}

	void JsonRPC_Send(CJsonRPCClient* pClient, const char* method, const char* args, JSON_CALLBACK_PROC proc)
	{
		pClient->Send(method, args, proc);
	}

	CJsonRPCConnection::CJsonRPCConnection()
	{
		uv_mutex_init(&m_Mutex);
		m_wrt_count = 0;
		m_steam = NULL;
	}

	CJsonRPCConnection::~CJsonRPCConnection()
	{
		uv_mutex_destroy(&m_Mutex);
	}

	typedef struct
	{
		uv_write_t req;
		uv_buf_t buf;
	} write_req_t;

	bool CJsonRPCConnection::Send(_U32 seq, const char* method, const char* data)
	{
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


		uv_mutex_lock(&m_Mutex);
		if(m_steam && !uv_write(&req->req, m_steam, &req->buf, 1, &CJsonRPCConnection::OnAfterWrite))
		{
			req = NULL;
		}
		uv_mutex_unlock(&m_Mutex);

		if(req) ZION_FREE(req);
		return req==NULL;
	}

	void CJsonRPCConnection::OnConnect(uv_stream_t* stream, int status)
	{

		CJsonRPCConnection* pConn = (CJsonRPCConnection*)stream->data;

		pConn->m_steam = stream;
		pConn->m_RecvLen = 0;
		pConn->m_PacketLen = (_U32)-1;
		pConn->m_PacketSeq = 0;
		pConn->m_PacketData = "";

		int result = uv_read_start(stream, &CJsonRPCConnection::OnBeforRead, &CJsonRPCConnection::OnAfterRead);
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
			pConn->m_shutdown.data = (void*)pConn;
			uv_shutdown(&pConn->m_shutdown, pConn->m_steam, &CJsonRPCConnection::OnShutdown);
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

		int result = uv_read_start(stream, &CJsonRPCConnection::OnBeforRead, &CJsonRPCConnection::OnAfterRead);
	}

	void CJsonRPCConnection::OnAfterWrite(uv_write_t* req, int status)
	{
		CJsonRPCConnection* pConn = (CJsonRPCConnection*)req->data;
		ZION_FREE(req);

		bool bEnd = false;
		uv_mutex_lock(&pConn->m_Mutex);
		pConn->m_wrt_count -= 1;
		if(pConn->m_steam==NULL && pConn->m_wrt_count==0) bEnd = true;
		uv_mutex_unlock(&pConn->m_Mutex);

		if(bEnd) pConn->OnDisconnect();
	}

	void CJsonRPCConnection::OnShutdown(uv_shutdown_t* req, int status)
	{
		CJsonRPCConnection* pConn = (CJsonRPCConnection*)req->data;

		bool bEnd = false;
		uv_mutex_lock(&pConn->m_Mutex);
		pConn->m_steam = NULL;
		if(pConn->m_steam==NULL && pConn->m_wrt_count==0) bEnd = true;
		uv_mutex_unlock(&pConn->m_Mutex);

		if(bEnd) pConn->OnDisconnect();
	}

	CJsonRPCServer::CJsonRPCServer()
	{
	}

	bool CJsonRPCServer::Start(const char* ep)
	{
		/*
int main() {
    loop = uv_default_loop();

    uv_tcp_t server;
    uv_tcp_init(loop, &server);

    struct sockaddr_in bind_addr = uv_ip4_addr("0.0.0.0", 7000);
    uv_tcp_bind(&server, bind_addr);
    int r = uv_listen((uv_stream_t*) &server, 128, on_new_connection);
    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_err_name(uv_last_error(loop)));
        return 1;
    }
    return uv_run(loop, UV_RUN_DEFAULT);
	*/
		return true;
	}

	void CJsonRPCServer::Stop()
	{
	}

	CJsonRPCClient::CJsonRPCClient(struct sockaddr* sa)
	{
		ZION_ASSERT(sizeof(struct sockaddr_in6)>=sizeof(struct sockaddr_in));

		m_seq = 1;

		size_t len = sizeof(struct sockaddr_in6);
		memcpy(&m_sa, sa, len);
		m_connect.data = this;
		m_enable = false;

		uv_tcp_init(CJsonRPCClientManager::Get().m_uv_loop, &m_socket);
		uv_tcp_connect(&m_connect, &m_socket, &m_sa, &CJsonRPCClient::OnConnect);
	}

	void CJsonRPCClient::Send(const char* method, const char* data)
	{
		CJsonRPCConnection::Send(0, method, data);
	}

	void CJsonRPCClient::Send(const char* method, const char* data, JSON_CALLBACK_PROC proc)
	{
		//Map<_U32, JSON_RPCCALL>::iterator i; m_Requests;
		for(;;)
		{
		}
	}

	void CJsonRPCClient::OnDisconnect()
	{
	}

	void CJsonRPCClient::OnData(_U32 seq, const char* data)
	{
	}

	void CJsonRPCClient::OnConnect(uv_connect_t* req, int status)
	{
		req->handle->data = req->data;
		CJsonRPCConnection::OnConnect(req->handle, status);
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
			Get().m_Clients[ep] = pClient;
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
