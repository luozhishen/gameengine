#ifndef	__ASYNC_SOCK_IO__
#define	__ASYNC_SOCK_IO__

namespace Atlas
{

	typedef struct AIO_WORKERS*			HWORKERS;
	HWORKERS CreateWorkers(_U32 count);
	void KillWorkers(HWORKERS hworkers);
	bool IncWorker(HWORKERS hworkers);
	_U32 CountOf(HWORKERS hworkers);

	typedef struct AIO_BUFFER_POOL*		HIOPOOL;
	HIOPOOL AllocIoBufferPool(_U32 isize, _U32 osize, _U32 icount, _U32 ocount);
	void FreeIoBufferPool(HIOPOOL hpool);
	_U8* LockIoBuffer(HIOPOOL hpool);
	void UnlockIoBuffer(_U8* buf);
	_U32 GetIoBufferSize(HIOPOOL hpool);

	struct SOCKADDR
	{
		_U32 ip;
		_U16 port;
	};
	_U32 STR2IP(const _STR str);
	bool STR2ADDR(const _STR str, SOCKADDR& sa);
	bool ADDR2STR(const SOCKADDR& sa, _STR str, _U32 size);
	void ADDR(SOCKADDR& sa, _U32 ip, _U16 port);

	typedef struct AIO_CONNECTION*		HCONNECT;
	typedef struct AIO_TCP_END_POINT*	HTCPEP;

	typedef bool (*PFN_ON_CONNECT)(HCONNECT);
	typedef void (*PFN_ON_DISCONNECT)(HCONNECT);
	typedef void (*PFN_ON_DATA)(HCONNECT, _U32, const _U8*);
	typedef void (*PFN_ON_CONNECTFAILED)(void* key);

	struct ASOCKIO_HANDLER
	{
		PFN_ON_CONNECT			OnConnected;
		PFN_ON_DISCONNECT		OnDisconnected;
		PFN_ON_DATA				OnData;
		PFN_ON_CONNECTFAILED	OnConnectFailed;
	};

	HTCPEP NewEP(const SOCKADDR& sa, ASOCKIO_HANDLER& handler, HIOPOOL hpool, HWORKERS hworkers, void* key, _U32 count=0);
	void DelEP(HTCPEP hep);
	void StartEP(HTCPEP hep);
	void StopEP(HTCPEP hep);
	bool IsRunning(HTCPEP hep);
	_U32 GetIORefs(HTCPEP hep);
	void* KeyOf(HTCPEP hep);
	void SetKey(HTCPEP hep, void* key);
	bool GetEpAddr(HTCPEP hep, SOCKADDR& sa);
	HIOPOOL HIOPoolOf(HTCPEP hep);
	HWORKERS HWorkersOf(HTCPEP hep);

	bool Connect(const SOCKADDR& sa, ASOCKIO_HANDLER& handler, HIOPOOL hpool, HWORKERS hworkers, void* key);
	void Disconnect(HCONNECT hConn);
	bool IsConnected(HCONNECT hConn);
	void CloseConn(HCONNECT hConn);
	void Send(HCONNECT hConn, _U32 len, _U8* buf);
	void SetRefLimit(HCONNECT hConn, _U32 count);
	void SetPendLimit(HCONNECT hConn, _U32 bytes);
	_U32 GetIORefs(HCONNECT hConn);
	_U32 GetIOPends(HCONNECT hConn);
	HTCPEP HepOf(HCONNECT hConn);
	void* KeyOf(HCONNECT hConn);
	void SetKey(HCONNECT hConn, void* key);
	bool GetSelfAddr(HCONNECT hConn, SOCKADDR& sa);
	bool GetPeerAddr(HCONNECT hConn, SOCKADDR& sa);
	HIOPOOL HIOPoolOf(HCONNECT hConn);
	HWORKERS HWorkersOf(HCONNECT hConn);

	typedef struct AIO_UDP_END_POINT*	HUDPEP;
	typedef void (*PFN_ON_DATAGRAM)(HUDPEP hep, const SOCKADDR& sa, _U32 len, const _U8* data);

	HUDPEP NewEP(const SOCKADDR& sa, PFN_ON_DATAGRAM on, HIOPOOL hpool, HWORKERS hworkers, void* key);
	void DelEP(HUDPEP hep);
	void StartEP(HUDPEP hep);
	void StopEP(HUDPEP hep);
	void Send(HUDPEP hep, const SOCKADDR& sa, _U32 len, _U8* data);
	bool IsRunning(HUDPEP hep);
	void* KeyOf(HUDPEP hep);
	void SetKey(HUDPEP hep, void* key);
	bool GetEpAddr(HUDPEP hep, SOCKADDR& sa);
	HIOPOOL HIOPoolOf(HUDPEP hep);
	HWORKERS HWorkersOf(HUDPEP hep);

	void ASockIOInit();
	void ASockIOFini();

}

#endif
