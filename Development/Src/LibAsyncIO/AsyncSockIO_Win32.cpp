#ifndef WITHOUT_ZION_ASYNCIO

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <mswsock.h>
#include <limits.h>
#include <process.h>
#pragma comment(lib, "ws2_32")

#include "ZionBase.h"
#include "AsyncSockIO.h"

#include <pshpack8.h>

#define	SIOP_IDLE		0
#define	SIOP_ACCEPT		1
#define	SIOP_CONNECT	2
#define	SIOP_DISCONNECT	3
#define	SIOP_RECV		4
#define	SIOP_SEND		5
#define	SIOP_RECVFROM	6
#define	SIOP_SENDTO		7

namespace Zion
{

	// Number of bytes allowed to be in the WSASend callback pending state before stalling sends
	static const _U32 DOE_ASOCKIO_HIGH_PENDING_BUFFER_SIZE = 60 * 1024 * 1024; // 60MiB
	// Data pending a WSASend callback must drop below this threshold before resuming sends
	static const _U32 DOE_ASOCKIO_LOW_PENDING_BUFFER_SIZE = 10 * 1024 * 1024; // 10MiB

	typedef BOOL (WINAPI *LPFN_CANCELIOEX)(HANDLE, LPOVERLAPPED);

	static LPFN_ACCEPTEX lpfnAcceptEx;
	static LPFN_CONNECTEX lpfnConnectEx;
	static LPFN_DISCONNECTEX lpfnDisconnectEx;
	static LPFN_CANCELIOEX lpCancelIoEx;

	struct AIO_CONTEXT;
	struct AIO_BUFFER_POOL;
	struct AIO_WORKERS;
	struct AIO_CONNECTION;
	struct AIO_TCP_END_POINT;
	struct AIO_UDP_END_POINT;

	struct AIO_CONTEXT : SLIST_ENTRY {
		WSAOVERLAPPED		overlapped;
		WSABUF				wsabuf;
		AIO_BUFFER_POOL*	buffer_pool;
		_U64				operation;
		BYTE				buffer[(sizeof(SOCKADDR_IN)+16)*2];
	};

	#define	CtxOfOlap(olap)		((AIO_CONTEXT*)((LPBYTE)(olap)-FIELD_OFFSET(AIO_CONTEXT, overlapped)))
	#define	CtxOfBuf(buf)		((AIO_CONTEXT*)((LPBYTE)(buf)-FIELD_OFFSET(AIO_CONTEXT, buffer)))
	#define	AIO_CONTEXT_SIZE	FIELD_OFFSET(AIO_CONTEXT, buffer)
	#define	CtxOfTBuf(buf)		((PTURBO_CONTEXT)((LPBYTE)(buf)-FIELD_OFFSET(TURBO_CONTEXT, buffer)))

	struct AIO_BUFFER_POOL {
		DWORD input_size, output_size, input_count, output_count;
		SLIST_HEADER input_buffers, output_buffers;

		bool init(DWORD isize, DWORD icount, DWORD osize, DWORD ocount) {
			input_size = isize;
			input_count = 0;
			output_size = osize;
			output_count = 0;
			InitializeSListHead(&input_buffers);
			InitializeSListHead(&output_buffers);
			while(input_count<icount) {
				AIO_CONTEXT* ctx = (AIO_CONTEXT*)ZION_ALIGN_ALLOC(AIO_CONTEXT_SIZE+isize);
				if(ctx) {
					ZeroMemory(&ctx->overlapped, sizeof(WSAOVERLAPPED));
					ctx->wsabuf.buf = (char *)ctx->buffer;
					ctx->buffer_pool = this;
					ctx->operation = SIOP_RECV;
					InterlockedPushEntrySList(&input_buffers, ctx);
					++input_count;
				} else {
					break;
				}
			}
			while(output_count<ocount) {
				AIO_CONTEXT* ctx = (AIO_CONTEXT*)ZION_ALIGN_ALLOC(AIO_CONTEXT_SIZE+osize);
				if(ctx) {
					ZeroMemory(&ctx->overlapped, sizeof(WSAOVERLAPPED));
					ctx->wsabuf.buf = (char *)ctx->buffer;
					ctx->buffer_pool = this;
					ctx->operation = SIOP_SEND;
					InterlockedPushEntrySList(&output_buffers, ctx);
					++output_count;
				} else {
					break;
				}
			}
			return true;
		}
		void fini() {
			PSLIST_ENTRY p;
			DWORD deleted = 0;
			while(deleted!=input_count) {
				p = InterlockedPopEntrySList(&input_buffers);
				if(p) {
					ZION_ALIGN_FREE(p);
					++deleted;
				} else {
					Sleep(1);
				}
			}
			deleted = 0;
			while(deleted!=output_count) {
				p = InterlockedPopEntrySList(&output_buffers);
				if(p) {
					ZION_ALIGN_FREE(p);
					++deleted;
				} else {
					Sleep(1);
				}
			}
		}
		AIO_CONTEXT* LockInputContext() {
			AIO_CONTEXT* ctx = (AIO_CONTEXT*)InterlockedPopEntrySList(&input_buffers);
			if(!ctx) {
				ctx = (AIO_CONTEXT*)ZION_ALIGN_ALLOC(AIO_CONTEXT_SIZE+input_size);
				if(!ctx) {
					return NULL;
				} else {
					ZeroMemory(&ctx->overlapped, sizeof(WSAOVERLAPPED));
					ctx->wsabuf.buf = (char *)ctx->buffer;
					ctx->buffer_pool = this;
					ctx->operation = SIOP_RECV;
					InterlockedIncrement((LONG *)&input_count);
				}
			}
			return ctx;;
		}
		AIO_CONTEXT* LockOutputContext() {
			AIO_CONTEXT* ctx = (AIO_CONTEXT*)InterlockedPopEntrySList(&output_buffers);
			if(!ctx) {
				ctx = (AIO_CONTEXT*)ZION_ALIGN_ALLOC(AIO_CONTEXT_SIZE+output_size);
				if(!ctx) {
					return NULL;
				} else {
					ZeroMemory(&ctx->overlapped, sizeof(WSAOVERLAPPED));
					ctx->wsabuf.buf = (char *)ctx->buffer;
					ctx->buffer_pool = this;
					ctx->operation = SIOP_SEND;
					InterlockedIncrement((LONG *)&output_count);
				}
			}
			return ctx;;
		}
	};

	static inline void UnlockInputContext(AIO_CONTEXT* ctx)
	{
		InterlockedPushEntrySList(&ctx->buffer_pool->input_buffers, ctx);
	}

	static inline void UnlockOutputContext(AIO_CONTEXT* ctx)
	{
		InterlockedPushEntrySList(&ctx->buffer_pool->output_buffers, ctx);
	}

	HIOPOOL AllocIoBufferPool(_U32 isize, _U32 osize, _U32 icount, _U32 ocount)
	{
		AIO_BUFFER_POOL* ret = (AIO_BUFFER_POOL*)ZION_ALIGN_ALLOC(sizeof(AIO_BUFFER_POOL));
		if(ret) {
			if(ret->init(isize, icount, osize, ocount)) {
				return ret;
			}
			ret->fini();
			ZION_ALIGN_FREE(ret);
		}
		return NULL;
	}

	void FreeIoBufferPool(HIOPOOL hpool)
	{
		((AIO_BUFFER_POOL*)hpool)->fini();
		ZION_ALIGN_FREE(hpool);
	}

	_U8* LockIoBuffer(HIOPOOL hpool)
	{
		AIO_CONTEXT* ctx = ((AIO_BUFFER_POOL*)hpool)->LockOutputContext();
		return(ctx ? ctx->buffer : NULL);
	}

	void UnlockIoBuffer(_U8* buf)
	{
		UnlockOutputContext(CtxOfBuf(buf));
	}

	_U32 GetIoBufferSize(HIOPOOL hpool)
	{
		return hpool->output_size;
	}

	_U32 STR2IP(const _STR str)
	{
		_U32 ret = inet_addr(str);
		if(ret==INADDR_NONE) {
			hostent *phe = gethostbyname(str);
			if(phe) {
				ret = *(_U32*)(phe->h_addr);
			}
		}
		return ret;
	}

	static void ThrottleSendBandwidth(AIO_CONNECTION* conn);

	struct AIO_CONNECTION : SLIST_ENTRY {
		SOCKET				sock;
		ASOCKIO_HANDLER		handler;
		AIO_BUFFER_POOL*	buffer_pool;
		AIO_WORKERS*		workers;
		LPVOID				key;
		LONG				iorefs;
		LONG				ioreflimit;
		LONG				iopends;
		LONG				iopendlimit;
		LONG				connected;
		HANDLE				fin_wait_timer;
		AIO_TCP_END_POINT*	end_point;
		AIO_CONTEXT			static_ctx;

		bool Connect(PSOCKADDR_IN addr, PVOID k) {
			key = k;
			static_ctx.operation = SIOP_CONNECT;
			return(lpfnConnectEx(sock, (const sockaddr *)addr, sizeof(*addr), NULL, 0, NULL, &static_ctx.overlapped) || WSAGetLastError()==ERROR_IO_PENDING);
		}
		void Disconnect() {
			if(InterlockedCompareExchange(&connected, 0, 1)) {
				static_ctx.operation = SIOP_DISCONNECT;
	#ifndef	NO_OS_BUG_FIX
				if(!lpfnDisconnectEx(sock, &static_ctx.overlapped, 0, 0)) {
	#else
				if(!lpfnDisconnectEx(sock, &static_ctx.overlapped, end_point ? TF_REUSE_SOCKET : 0, 0)) {
	#endif
					ZION_ASSERT(WSAGetLastError()==ERROR_IO_PENDING);
				}
			}
		}
		void Send(AIO_CONTEXT* ctx) {
			if(iopends>iopendlimit || iorefs>ioreflimit) {
				UnlockOutputContext(ctx);
				Disconnect();
				return;
			}
			ThrottleSendBandwidth(this);
			InterlockedIncrement(&iorefs);
			InterlockedExchangeAdd(&iopends, ctx->wsabuf.len);
			DWORD dwXfer;
			if(SOCKET_ERROR==WSASend(sock, &ctx->wsabuf, 1, &dwXfer, 0, &ctx->overlapped, NULL) && WSAGetLastError()!=ERROR_IO_PENDING) {
				InterlockedExchangeAdd(&iopends, -(LONG)ctx->wsabuf.len);
				InterlockedDecrement(&iorefs);
				UnlockOutputContext(ctx);
				Disconnect();
			}
		}
		void Recv(AIO_CONTEXT* ctx) {
			ctx->wsabuf.len = buffer_pool->input_size;
			InterlockedIncrement(&iorefs);
			DWORD dwXfer, dwFlag = 0;
			if(SOCKET_ERROR==WSARecv(sock, &ctx->wsabuf, 1, &dwXfer, &dwFlag, &ctx->overlapped, NULL) && WSAGetLastError()!=ERROR_IO_PENDING) {
				UnlockInputContext(ctx);
				Disconnect();
				InterlockedDecrement(&iorefs);
			}
		}
		BOOL Refresh();
	};

	struct AIO_TCP_END_POINT {
		SOCKET				sock;
		ASOCKIO_HANDLER		handler;
		AIO_BUFFER_POOL*	buffer_pool;
		AIO_WORKERS*		workers;
		LPVOID				key;
		LONG				iorefs;
		LONG				running;
		DWORD				total_conns;
		PSLIST_HEADER		free_conns;
		AIO_CONNECTION*		current_conn;
		AIO_CONTEXT			static_ctx;

		void Start() {
			if(!InterlockedCompareExchange(&running, 1, 0)) {
				while(iorefs) Sleep(1);
				Accept(TRUE);
			}
		}
		void Stop() {
			running = 0;
			ZION_ASSERT(lpCancelIoEx);
			if(sock!=INVALID_SOCKET) lpCancelIoEx((HANDLE)sock, NULL);
		}
		bool init(LPSOCKADDR_IN addr, ASOCKIO_HANDLER& handler, AIO_BUFFER_POOL* hpool, AIO_WORKERS* hworkers, PVOID key, DWORD count);
		void fini();
		void Accept(BOOL newconn);
	};

	struct AIO_UDP_END_POINT {
		SOCKET			sock;
		PFN_ON_DATAGRAM	pfnOnDatagram;
		AIO_BUFFER_POOL*	buffer_pool;
		AIO_WORKERS*		workers;
		LPVOID			key;
		LONG			iorefs;
		LONG			running;
		INT				sainLen;
		SOCKADDR_IN		sainSrc;

		void Start() {
			if(!InterlockedCompareExchange(&running, 1, 0)) {
				while(iorefs) Sleep(1);
				Recv();
			}
		}
		void Stop() {
			running = 0;
			ZION_ASSERT(lpCancelIoEx);
			if(sock!=INVALID_SOCKET) lpCancelIoEx((HANDLE)sock, NULL);
		}
		bool init(PSOCKADDR_IN, PFN_ON_DATAGRAM, AIO_BUFFER_POOL*, AIO_WORKERS*, PVOID);
		void fini();
		void Send(PSOCKADDR_IN addr, AIO_CONTEXT* ctx) {
			InterlockedIncrement(&iorefs);
			DWORD dwXfer;
			if(SOCKET_ERROR==WSASendTo(sock, &ctx->wsabuf, 1, &dwXfer, 0, (PSOCKADDR)addr, sizeof(SOCKADDR_IN), &ctx->overlapped, NULL) && WSAGetLastError()!=ERROR_IO_PENDING) {
				ctx->operation = SIOP_SEND;
				UnlockOutputContext(ctx);
				InterlockedDecrement(&iorefs);
			}
		}
		void Recv() {
			if(!running) return;
			AIO_CONTEXT* ctx = buffer_pool->LockInputContext();
			if(ctx) {
				ctx->operation = SIOP_RECVFROM;
				ctx->wsabuf.len = buffer_pool->input_size;
				InterlockedIncrement(&iorefs);
				DWORD dwXfer, dwFlag = 0;
				sainLen = sizeof(sainSrc);
				if(SOCKET_ERROR==WSARecvFrom(sock, &ctx->wsabuf, 1, &dwXfer, &dwFlag, (PSOCKADDR)&sainSrc, &sainLen, &ctx->overlapped, NULL) && WSAGetLastError()!=ERROR_IO_PENDING) {
					ctx->operation = SIOP_RECV;
					UnlockInputContext(ctx);
					running = 0;
					InterlockedDecrement(&iorefs);
				}
			} else {
				running = 0;
			}
		}
	};

	static inline void DelConn(AIO_CONNECTION* conn)
	{
		closesocket(conn->sock);
		ZION_ALIGN_FREE(conn);
	}

	struct AIO_WORKERS {
		DWORD worker_count;
		DWORD worker_mark;
		HANDLE iocp;

		bool init(DWORD count) {
			worker_count = 0;
			worker_mark = TlsAlloc();
			ZION_ASSERT(TLS_OUT_OF_INDEXES!=worker_mark);
			iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
			if(!iocp) return false;
			for(DWORD iter=0; iter<count; ++iter) {
				if(_beginthread((void(__cdecl *)(PVOID))WorkerProc, 0, this)==-1) {
					break;
				}
			}
			return true;
		}
		void fini() {
			while(worker_count) {
				PostQueuedCompletionStatus(iocp, 0, 0, NULL);
				Sleep(1);
			}
			if(iocp) CloseHandle(iocp);
			TlsFree(worker_mark);
		}
		bool IncWorker() {
			return(_beginthread((void(__cdecl *)(PVOID))WorkerProc, 0, this)!=-1);
		}
		BOOL WorkerProcImp(DWORD dwWaits) {
			BOOL res;
			DWORD nob;
			ULONG_PTR key;
			LPOVERLAPPED olap;
			AIO_CONTEXT* ctx;
			AIO_CONNECTION* conn;
			AIO_TCP_END_POINT* tcpep;
			AIO_UDP_END_POINT* udpep;

			res = GetQueuedCompletionStatus(iocp, &nob, &key, &olap, dwWaits);
			if(olap) {
				ctx = CtxOfOlap(olap);
				switch(ctx->operation) {
					case SIOP_ACCEPT:
						tcpep = (AIO_TCP_END_POINT*)key;
						conn = tcpep->current_conn;
						tcpep->Accept(res);
						if(res) {
							ZION_VERIFY(setsockopt(conn->sock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (const char *)&tcpep->sock, sizeof(tcpep->sock))==0);
							conn->connected = 1;
							InterlockedIncrement(&conn->iorefs);
							bool ret = conn->handler.OnConnected((HCONNECT)conn);
							if(ret) {
								ctx = conn->buffer_pool->LockInputContext();
								if(ctx) {
									conn->Recv(ctx);
								} else {
									conn->Disconnect();
								}
							} else {
								conn->Disconnect();
							}
							InterlockedDecrement(&conn->iorefs);
						}
						InterlockedDecrement(&tcpep->iorefs);
						break;
					case SIOP_CONNECT:
						conn = (AIO_CONNECTION*)key;
						if(res) {
							ZION_VERIFY(setsockopt(conn->sock, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0)==0);
							conn->connected = 1;
							InterlockedIncrement(&conn->iorefs);
							bool ret = conn->handler.OnConnected((HCONNECT)conn);
							if(ret)
							{
								ctx = conn->buffer_pool->LockInputContext();
								if(ctx)
								{
									conn->Recv(ctx);
								} else {
									conn->Disconnect();
								}
							} else {
								conn->Disconnect();
							}
							InterlockedDecrement(&conn->iorefs);
						} else {
							conn->handler.OnConnectFailed(conn->key);
							DelConn(conn);
						}
						break;
					case SIOP_DISCONNECT:
						conn = (AIO_CONNECTION*)key;
						if(conn->iorefs) {
							if(!nob) {
								PostQueuedCompletionStatus(iocp, GetTickCount(), key, olap);
							} else {
								if(GetTickCount()-nob<1000 || !CreateTimerQueueTimer(&conn->fin_wait_timer, NULL, (WAITORTIMERCALLBACK)FinWaitCallback, conn, 4000, 0, WT_EXECUTEONLYONCE)) {
									PostQueuedCompletionStatus(iocp, nob, key, olap);
								}
							}
						} else {
							conn->handler.OnDisconnected((HCONNECT)conn);
						}
						break;
					case SIOP_RECV:
						conn = (AIO_CONNECTION*)key;
						if(nob) {
							conn->handler.OnData((HCONNECT)conn, nob, ctx->buffer);
							conn->Recv(ctx);
						} else {
							UnlockInputContext(ctx);
							conn->Disconnect();
						}
						InterlockedDecrement(&conn->iorefs);
						break;
					case SIOP_SEND:
						conn = (AIO_CONNECTION*)key;
						InterlockedExchangeAdd(&conn->iopends, -(LONG)ctx->wsabuf.len);
						InterlockedDecrement(&conn->iorefs);
						UnlockOutputContext(ctx);
						break;
					case SIOP_RECVFROM:
						udpep = (AIO_UDP_END_POINT*)key;
						ASOCK_ADDR sa;
						sock_addr(&sa, udpep->sainSrc.sin_addr.S_un.S_addr, ntohs(udpep->sainSrc.sin_port));
						udpep->Recv();
						if(nob) {
							udpep->pfnOnDatagram((HUDPEP)udpep, sa, nob, ctx->buffer);
						}
						ctx->operation = SIOP_RECV;
						UnlockInputContext(ctx);
						InterlockedDecrement(&udpep->iorefs);
						break;
					case SIOP_SENDTO:
						udpep = (AIO_UDP_END_POINT*)key;
						ctx->operation = SIOP_SEND;
						UnlockOutputContext(ctx);
						InterlockedDecrement(&udpep->iorefs);
						break;
				}
			} else {
				if(WAIT_TIMEOUT!=GetLastError()) {
					return FALSE;
				}
			}
			return TRUE;
		}
		static void __cdecl WorkerProc(AIO_WORKERS* workers) {
			InterlockedIncrement((LONG *)&workers->worker_count);
			TlsSetValue(workers->worker_mark, workers);
			while(workers->WorkerProcImp(INFINITE));
			InterlockedDecrement((LONG *)&workers->worker_count);
		}
		static void CALLBACK FinWaitCallback(AIO_CONNECTION* conn, BOOLEAN) {
			HANDLE handle = conn->fin_wait_timer;
			PostQueuedCompletionStatus(conn->workers->iocp, 0, (ULONG_PTR)conn, &conn->static_ctx.overlapped);
			DeleteTimerQueueTimer(NULL, handle, NULL);
		}
		BOOL IsMyWorker() {
			return(TlsGetValue(worker_mark)==this);
		}
	};

	void ThrottleSendBandwidth(AIO_CONNECTION* conn)
	{
		// If we're not configured to disconnect when the network backs up AND
		// we exceed our high water wark: we need to scale back the traffic
		// we're sending or we will overwhelm the netowrk link and either crash
		// or reset the connection.
		if(conn->iopendlimit==LONG_MAX && conn->iopends > DOE_ASOCKIO_HIGH_PENDING_BUFFER_SIZE) {
			BOOL towork = conn->workers->IsMyWorker();
			while(conn->iopends > DOE_ASOCKIO_LOW_PENDING_BUFFER_SIZE) {
				if(towork) {
					if(!conn->workers->WorkerProcImp(10)) {
						break;
					}
				} else {
					SwitchToThread();
				}
			}
		}
	}

	static AIO_CONNECTION* NewConn(ASOCKIO_HANDLER& handler, AIO_BUFFER_POOL* pp, AIO_WORKERS* pw, AIO_TCP_END_POINT* pep)
	{
		AIO_CONNECTION* conn = (AIO_CONNECTION*)ZION_ALIGN_ALLOC(sizeof(AIO_CONNECTION));
		if(conn) {
			conn->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if(conn->sock!=INVALID_SOCKET) {
				if(CreateIoCompletionPort((HANDLE)conn->sock, pw->iocp, (ULONG_PTR)conn, 0)) {
					DWORD val;
					val = 0;
					ZION_VERIFY(setsockopt(conn->sock, SOL_SOCKET, SO_SNDBUF, (const char *)&val, sizeof(val))==0);
					val = 256*1024;
					ZION_VERIFY(setsockopt(conn->sock, SOL_SOCKET, SO_RCVBUF, (const char *)&val, sizeof(val))==0);
					val = 1;
					ZION_VERIFY(setsockopt(conn->sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&val, sizeof(val))==0);
					ZION_VERIFY(setsockopt(conn->sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&val, sizeof(val))==0);
					conn->handler = handler;
					conn->buffer_pool = pp;
					conn->workers = pw;
					conn->iorefs = conn->iopends = 0;
					conn->ioreflimit = conn->iopendlimit = LONG_MAX;
					conn->connected = 0;
					conn->end_point = pep;
					ZeroMemory(&conn->static_ctx.overlapped, sizeof(WSAOVERLAPPED));
					if(!pep) {
						SOCKADDR_IN sain;
						ZeroMemory(&sain, sizeof(sain));
						sain.sin_family = AF_INET;
						if(bind(conn->sock, (const sockaddr *)&sain, sizeof(sain))==0) {
							return(conn);
						}
					} else {
						return(conn);
					}
				}
				closesocket(conn->sock);
			}
			ZION_ALIGN_FREE(conn);
		}
		return NULL;
	}

	BOOL AIO_CONNECTION::Refresh()
	{
		ZION_ASSERT(0==iorefs);
		ZION_ASSERT(0==iopends);
		ioreflimit = iopendlimit = LONG_MAX;
	#ifndef	NO_OS_BUG_FIX
		closesocket(sock);
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(sock!=INVALID_SOCKET) {
			if(CreateIoCompletionPort((HANDLE)sock, workers->iocp, (ULONG_PTR)this, 0)) {
				DWORD val;
				val = 0;
				ZION_VERIFY(setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (const char *)&val, sizeof(val))==0);
				val = 256*1024;
				ZION_VERIFY(setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const char *)&val, sizeof(val))==0);
				val = 1;
				ZION_VERIFY(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&val, sizeof(val))==0);
				ZION_VERIFY(setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&val, sizeof(val))==0);
				return TRUE;
			}
			closesocket(sock);
		}
		return FALSE;
	#else
		return TRUE;
	#endif
	}

	bool AIO_TCP_END_POINT::init(LPSOCKADDR_IN addr, ASOCKIO_HANDLER& h, AIO_BUFFER_POOL* pp, AIO_WORKERS* pw, PVOID k, DWORD count)
	{
		iorefs = 0;
		running = 0;
		total_conns = 0;
		if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))==INVALID_SOCKET) return false;
		DWORD val;
		val = 0;
		ZION_VERIFY(setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (const char *)&val, sizeof(val))==0);
		val = 256*1024;
		ZION_VERIFY(setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const char *)&val, sizeof(val))==0);
		val = 1;
		ZION_VERIFY(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&val, sizeof(val))==0);
		ZION_VERIFY(setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&val, sizeof(val))==0);
		free_conns = (PSLIST_HEADER)ZION_ALIGN_ALLOC(sizeof(SLIST_HEADER));
		if(!free_conns) return false;
		InitializeSListHead(free_conns);
		ZeroMemory(&static_ctx.overlapped, sizeof(WSAOVERLAPPED));
		static_ctx.operation = SIOP_ACCEPT;
		if(CreateIoCompletionPort((HANDLE)sock, pw->iocp, (ULONG_PTR)this, 0) && bind(sock, (const sockaddr *)addr, sizeof(*addr))==0 && listen(sock, SOMAXCONN)==0) {
			handler = h;
			buffer_pool = pp;
			workers = pw;
			key = k;
			while(total_conns<count) {
				AIO_CONNECTION* conn = NewConn(handler, pp, pw, this);
				if(conn) {
					InterlockedPushEntrySList(free_conns, conn);
					++total_conns;
				} else {
					break;
				}
			}
			return true;
		}
		return false;
	}

	bool AIO_UDP_END_POINT::init(PSOCKADDR_IN addr, PFN_ON_DATAGRAM pfn, AIO_BUFFER_POOL* pp, AIO_WORKERS* pw, PVOID k)
	{
		iorefs = 0;
		running = 0;
		if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==INVALID_SOCKET) return false;
		DWORD dwBytesReturned;
		DWORD val;
		val = 0;
		ZION_VERIFY(WSAIoctl(sock, SIO_UDP_CONNRESET, &val, sizeof(val), NULL, 0, &dwBytesReturned, NULL, NULL)==0);
		ZION_VERIFY(setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (const char *)&val, sizeof(val))==0);
		val = 1024*1024;
		ZION_VERIFY(setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const char *)&val, sizeof(val))==0);
		val = 1;
		ZION_VERIFY(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (const char *)&val, sizeof(val))==0);
		if(CreateIoCompletionPort((HANDLE)sock, pw->iocp, (ULONG_PTR)this, 0) && bind(sock, (const sockaddr *)addr, sizeof(*addr))==0) {
			pfnOnDatagram = pfn;
			buffer_pool = pp;
			workers = pw;
			key = k;
			return true;
		}
		return false;
	}

	void AIO_TCP_END_POINT::fini()
	{
		Stop();
		while(iorefs) Sleep(1);
		if(free_conns) {
			while(QueryDepthSList(free_conns)!=total_conns) Sleep(1);
			while(QueryDepthSList(free_conns)) {
				DelConn((AIO_CONNECTION*)InterlockedPopEntrySList(free_conns));
			}
			ZION_ALIGN_FREE(free_conns);
		}
		if(sock!=INVALID_SOCKET) closesocket(sock);
	}

	void AIO_UDP_END_POINT::fini()
	{
		Stop();
		while(iorefs) Sleep(1);
		if(sock!=INVALID_SOCKET) closesocket(sock);
	}

	void AIO_TCP_END_POINT::Accept(BOOL newconn)
	{
		if(!running) {
			if(!newconn) {
				InterlockedPushEntrySList(free_conns, current_conn);
			}
			return;
		}
		if(newconn) {
			current_conn = (AIO_CONNECTION*)InterlockedPopEntrySList(free_conns);
			if(!current_conn) {
				current_conn = NewConn(handler, buffer_pool, workers, this);
				if(!current_conn) {
					running = 0;
					return;
				}
				InterlockedIncrement((LONG *)&total_conns);
			}
			current_conn->key = NULL;
		}
		InterlockedIncrement(&iorefs);
		DWORD dwXfer;
		if(!lpfnAcceptEx(sock, current_conn->sock, static_ctx.buffer, 0, sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, &dwXfer, &static_ctx.overlapped) && WSAGetLastError()!=ERROR_IO_PENDING) {
			AIO_CONNECTION* save_curr_conn = current_conn;
			if(WSAGetLastError()==WSAEINVAL) {
				Accept(TRUE);
			} else {
				running = 0;
			}
			if(save_curr_conn->Refresh()) {
				InterlockedPushEntrySList(free_conns, save_curr_conn);
			} else {
				InterlockedDecrement((LONG *)&total_conns);
				ZION_ALIGN_FREE(save_curr_conn);
			}
			InterlockedDecrement(&iorefs);
		}
	}

	HWORKERS CreateWorkers(_U32 count)
	{
		AIO_WORKERS* ret = (AIO_WORKERS*)ZION_ALIGN_ALLOC(sizeof(AIO_WORKERS));
		if(ret) {
			if(ret->init(count)) {
				return((HWORKERS)ret);
			}
			ret->fini();
			ZION_ALIGN_FREE(ret);
		}
		return NULL;
	}

	void KillWorkers(HWORKERS hworkers)
	{
		((AIO_WORKERS*)hworkers)->fini();
		ZION_ALIGN_FREE(hworkers);
	}

	bool IncWorker(HWORKERS hworkers)
	{
		return(((AIO_WORKERS*)hworkers)->IncWorker());
	}

	_U32 CountOf(HWORKERS hworkers)
	{
		return(((AIO_WORKERS*)hworkers)->worker_count);
	}

	HIOPOOL HIOPoolOf(HTCPEP hep)
	{
		return((HIOPOOL)((AIO_TCP_END_POINT*)hep)->buffer_pool);
	}

	HIOPOOL HIOPoolOf(HUDPEP hep)
	{
		return((HIOPOOL)((AIO_UDP_END_POINT*)hep)->buffer_pool);
	}

	HIOPOOL HIOPoolOf(HCONNECT hConn)
	{
		return((HIOPOOL)((AIO_CONNECTION*)hConn)->buffer_pool);
	}

	HWORKERS HWorkersOf(HTCPEP hep)
	{
		return((HWORKERS)((AIO_TCP_END_POINT*)hep)->workers);
	}

	HWORKERS HWorkersOf(HUDPEP hep)
	{
		return((HWORKERS)((AIO_UDP_END_POINT*)hep)->workers);
	}

	HWORKERS HWorkersOf(HCONNECT hConn)
	{
		return((HWORKERS)((AIO_CONNECTION*)hConn)->workers);
	}

	HTCPEP HepOf(HCONNECT hConn)
	{
		return((HTCPEP)((AIO_CONNECTION*)hConn)->end_point);
	}

	PVOID KeyOf(HTCPEP hep)
	{
		return(((AIO_TCP_END_POINT*)hep)->key);
	}

	PVOID KeyOf(HUDPEP hep)
	{
		return(((AIO_UDP_END_POINT*)hep)->key);
	}

	PVOID KeyOf(HCONNECT hConn)
	{
		return(((AIO_CONNECTION*)hConn)->key);
	}

	void SetKey(HTCPEP hep, PVOID key)
	{
		((AIO_TCP_END_POINT*)hep)->key = key;
	}

	void SetKey(HUDPEP hep, PVOID key)
	{
		((AIO_UDP_END_POINT*)hep)->key = key;
	}

	void SetKey(HCONNECT hConn, PVOID key)
	{
		((AIO_CONNECTION*)hConn)->key = key;
	}

	bool GetSelfAddr(HCONNECT hConn, ASOCK_ADDR& sa)
	{
		SOCKADDR_IN addr;
		int len = sizeof(addr);
		if(getsockname(hConn->sock, (sockaddr *)&addr, &len)!=0) return false;
		sa.ip = addr.sin_addr.S_un.S_addr;
		sa.port = ntohs(addr.sin_port);
		return true;
	}

	bool GetPeerAddr(HCONNECT hConn, ASOCK_ADDR& sa)
	{
		SOCKADDR_IN addr;
		int len = sizeof(addr);
		if(getpeername(hConn->sock, (sockaddr *)&addr, &len)!=0) return false;
		sa.ip = addr.sin_addr.S_un.S_addr;
		sa.port = ntohs(addr.sin_port);
		return true;
	}

	bool GetEpAddr(HTCPEP hep, ASOCK_ADDR& sa)
	{
		SOCKADDR_IN addr;
		int len = sizeof(addr);
		if(getsockname(hep->sock, (sockaddr *)&addr, &len)!=0) return false;
		sa.ip = addr.sin_addr.S_un.S_addr;
		sa.port = ntohs(addr.sin_port);
		return true;
	}

	bool GetEpAddr(HUDPEP hep, ASOCK_ADDR& sa)
	{
		SOCKADDR_IN addr;
		int len = sizeof(addr);
		if(getsockname(hep->sock, (sockaddr *)&addr, &len)!=0) return false;
		sa.ip = addr.sin_addr.S_un.S_addr;
		sa.port = ntohs(addr.sin_port);
		return true;
	}

	HTCPEP NewEP(const ASOCK_ADDR& sa, ASOCKIO_HANDLER& handler, HIOPOOL hpool, HWORKERS hworkers, PVOID key, _U32 count)
	{
		AIO_TCP_END_POINT* ep = (AIO_TCP_END_POINT*)ZION_ALIGN_ALLOC(sizeof(AIO_TCP_END_POINT));
		if(ep) {
			SOCKADDR_IN addr;
			addr.sin_family = AF_INET;
			addr.sin_addr.S_un.S_addr = sa.ip;
			addr.sin_port = htons(sa.port);
			if(ep->init(&addr, handler, (AIO_BUFFER_POOL*)hpool, (AIO_WORKERS*)hworkers, key, count)) {
				return((HTCPEP)ep);
			}
			ep->fini();
			ZION_ALIGN_FREE(ep);
		}
		return NULL;
	}

	HUDPEP NewEP(const ASOCK_ADDR& sa, PFN_ON_DATAGRAM pfn, HIOPOOL hpool, HWORKERS hworkers, PVOID key)
	{
		AIO_UDP_END_POINT* ep = (AIO_UDP_END_POINT*)ZION_ALIGN_ALLOC(sizeof(AIO_UDP_END_POINT));
		if(ep) {
			SOCKADDR_IN addr;
			addr.sin_family = AF_INET;
			addr.sin_addr.S_un.S_addr = sa.ip;
			addr.sin_port = htons(sa.port);
			if(ep->init(&addr, pfn, (AIO_BUFFER_POOL*)hpool, (AIO_WORKERS*)hworkers, key)) {
				return((HUDPEP)ep);
			}
			ep->fini();
			ZION_ALIGN_FREE(ep);
		}
		return NULL;
	}

	void DelEP(HTCPEP hep)
	{
		((AIO_TCP_END_POINT*)hep)->fini();
		ZION_ALIGN_FREE(hep);
	}

	void DelEP(HUDPEP hep)
	{
		((AIO_UDP_END_POINT*)hep)->fini();
		ZION_ALIGN_FREE(hep);
	}

	void StartEP(HTCPEP hep)
	{
		((AIO_TCP_END_POINT*)hep)->Start();
	}

	void StartEP(HUDPEP hep)
	{
		((AIO_UDP_END_POINT*)hep)->Start();
	}

	void StopEP(HTCPEP hep)
	{
		((AIO_TCP_END_POINT*)hep)->Stop();
	}

	void StopEP(HUDPEP hep)
	{
		((AIO_UDP_END_POINT*)hep)->Stop();
	}

	bool IsRunning(HTCPEP hep)
	{
		return(((AIO_TCP_END_POINT*)hep)->running ? true : false);
	}

	bool IsRunning(HUDPEP hep)
	{
		return(((AIO_UDP_END_POINT*)hep)->running ? true : false);
	}

	bool Connect(const ASOCK_ADDR& sa, ASOCKIO_HANDLER& handler, HIOPOOL hpool, HWORKERS hworkers, PVOID key)
	{
		AIO_CONNECTION* conn = NewConn(handler, (AIO_BUFFER_POOL*)hpool, (AIO_WORKERS*)hworkers, NULL);
		if(conn) {
			SOCKADDR_IN addr;
			addr.sin_family = AF_INET;
			addr.sin_addr.S_un.S_addr = sa.ip;
			addr.sin_port = htons(sa.port);
			if(conn->Connect(&addr, key)) {
				return true;
			} else {
				DelConn(conn);
			}
		}
		return false;
	}

	void Disconnect(HCONNECT hConn)
	{
		((AIO_CONNECTION*)hConn)->Disconnect();
	}

	bool IsConnected(HCONNECT hConn)
	{
		return(((AIO_CONNECTION*)hConn)->connected ? true : false);
	}

	void CloseConn(HCONNECT hConn)
	{
		AIO_CONNECTION* conn = (AIO_CONNECTION*)hConn;
		ZION_ASSERT(!conn->iorefs);
		ZION_ASSERT(!conn->connected);
		if(conn->iorefs || conn->connected) return;
		if(conn->end_point) {
			if(conn->Refresh()) {
				InterlockedPushEntrySList(conn->end_point->free_conns, conn);
			} else {
				InterlockedDecrement((LONG *)&conn->end_point->total_conns);
				ZION_ALIGN_FREE(conn);
			}
		} else {
			DelConn(conn);
		}
	}

	void Send(HCONNECT hConn, _U32 len, _U8* buf)
	{
		AIO_CONTEXT* ctx = CtxOfBuf(buf);
		ZION_ASSERT(ctx->operation==SIOP_SEND);
		ZION_ASSERT(len<=ctx->buffer_pool->output_size);
		ctx->wsabuf.len = len;
		((AIO_CONNECTION*)hConn)->Send(ctx);
	}

	void Send(HUDPEP hep, PSOCKADDR_IN addr, _U32 len, _U8* buf)
	{
		AIO_CONTEXT* ctx = CtxOfBuf(buf);
		ZION_ASSERT(ctx->operation==SIOP_SEND);
		ZION_ASSERT(len<=ctx->buffer_pool->output_size);
		ctx->wsabuf.len = len;
		ctx->operation = SIOP_SENDTO;
		((AIO_UDP_END_POINT*)hep)->Send(addr, ctx);
	}

	void SetRefLimit(HCONNECT hConn, _U32 limit)
	{
		((AIO_CONNECTION*)hConn)->ioreflimit = limit;
	}

	void SetPendLimit(HCONNECT hConn, _U32 limit)
	{
		((AIO_CONNECTION*)hConn)->iopendlimit = limit;
	}

	_U32 GetIORefs(HTCPEP hep)
	{
		return(((AIO_TCP_END_POINT*)hep)->iorefs);
	}

	_U32 GetIORefs(HUDPEP hep)
	{
		return(((AIO_UDP_END_POINT*)hep)->iorefs);
	}

	_U32 GetIORefs(HCONNECT hConn)
	{
		return(((AIO_CONNECTION*)hConn)->iorefs);
	}

	_U32 GetIOPends(HCONNECT hConn)
	{
		return(((AIO_CONNECTION*)hConn)->iopends);
	}

	static _U32 g_asockio_init_count = 0;

	void ASockIOInit()
	{
		if((g_asockio_init_count++)==0)
		{
			WSADATA wsaData;
			ZION_VERIFY(WSAStartup(0x0202, &wsaData)==0);
			SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			ZION_ASSERT(s!=INVALID_SOCKET);
			DWORD cb;
			GUID GuidAcceptEx = WSAID_ACCEPTEX, GuidConnectEx = WSAID_CONNECTEX, GuidDisconnectEx = WSAID_DISCONNECTEX;
			ZION_VERIFY(WSAIoctl(s, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidAcceptEx, sizeof(GuidAcceptEx), &lpfnAcceptEx, sizeof(lpfnAcceptEx), &cb, NULL, NULL)==0);
			ZION_VERIFY(WSAIoctl(s, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidConnectEx, sizeof(GuidConnectEx), &lpfnConnectEx, sizeof(lpfnConnectEx), &cb, NULL, NULL)==0);
			ZION_VERIFY(WSAIoctl(s, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidDisconnectEx, sizeof(GuidDisconnectEx), &lpfnDisconnectEx, sizeof(lpfnDisconnectEx), &cb, NULL, NULL)==0);
			closesocket(s);
			lpCancelIoEx = (LPFN_CANCELIOEX)GetProcAddress(GetModuleHandle(TEXT("KERNEL32.DLL")), "CancelIoEx");
		}
	}

	void ASockIOFini()
	{
		if((--g_asockio_init_count)==0)
		{
			WSACleanup();
		}
	}

}

#include <poppack.h>

#endif

#endif
