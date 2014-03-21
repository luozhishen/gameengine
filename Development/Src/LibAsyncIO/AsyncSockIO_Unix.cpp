#ifndef WITHOUT_ZION_ASYNCIO

#ifndef WIN32

#include "ZionDefines.h"
#include "ZionSocket.h"
#include "AsyncSockIO.h"

namespace Zion
{

	HWORKERS CreateWorkers(_U32 count)
	{
		return NULL;
	}

	void KillWorkers(HWORKERS hworkers)
	{
	}

	bool IncWorker(HWORKERS hworkers)
	{
		return false;
	}

	_U32 CountOf(HWORKERS hworkers)
	{
		return 0;
	}

	HIOPOOL AllocIoBufferPool(_U32 isize, _U32 osize, _U32 icount, _U32 ocount)
	{
		return NULL;
	}

	void FreeIoBufferPool(HIOPOOL hpool)
	{
	}

	_U8* LockIoBuffer(HIOPOOL hpool)
	{
		return NULL;
	}

	void UnlockIoBuffer(_U8* buf)
	{
	}

	_U32 GetIoBufferSize(HIOPOOL hpool)
	{
		return 0;
	}

	_U32 STR2IP(const _STR str)
	{
		return 0;
	}

	HTCPEP NewEP(const ASOCK_ADDR& sa, ASOCKIO_HANDLER& handler, HIOPOOL hpool, HWORKERS hworkers, void* key, _U32 count)
	{
		return NULL;
	}

	void DelEP(HTCPEP hep)
	{
	}

	void StartEP(HTCPEP hep)
	{
	}

	void StopEP(HTCPEP hep)
	{
	}

	bool IsRunning(HTCPEP hep)
	{
		return true;
	}

	_U32 GetIORefs(HTCPEP hep)
	{
		return 0;
	}

	void* KeyOf(HTCPEP hep)
	{
		return NULL;
	}

	void SetKey(HTCPEP hep, void* key)
	{
	}

	bool GetEpAddr(HTCPEP hep, ASOCK_ADDR& sa)
	{
		return false;
	}

	HIOPOOL HIOPoolOf(HTCPEP hep)
	{
		return NULL;
	}

	HWORKERS HWorkersOf(HTCPEP hep)
	{
		return NULL;
	}

	bool Connect(const ASOCK_ADDR& sa, ASOCKIO_HANDLER& handler, HIOPOOL hpool, HWORKERS hworkers, void* key)
	{
		return false;
	}

	void Disconnect(HCONNECT hConn)
	{
	}

	bool IsConnected(HCONNECT hConn)
	{
		return false;
	}

	void CloseConn(HCONNECT hConn)
	{
	}

	void Send(HCONNECT hConn, _U32 len, _U8* buf)
	{
	}

	void SetRefLimit(HCONNECT hConn, _U32 count)
	{
	}

	void SetPendLimit(HCONNECT hConn, _U32 bytes)
	{
	}

	_U32 GetIORefs(HCONNECT hConn)
	{
		return 0;
	}

	_U32 GetIOPends(HCONNECT hConn)
	{
		return 0;
	}

	HTCPEP HepOf(HCONNECT hConn)
	{
		return NULL;
	}

	void* KeyOf(HCONNECT hConn)
	{
		return NULL;
	}

	void SetKey(HCONNECT hConn, void* key)
	{
	}

	bool GetSelfAddr(HCONNECT hConn, ASOCK_ADDR& sa)
	{
		return false;
	}

	bool GetPeerAddr(HCONNECT hConn, ASOCK_ADDR& sa)
	{
		return false;
	}

	HIOPOOL HIOPoolOf(HCONNECT hConn)
	{
		return NULL;
	}

	HWORKERS HWorkersOf(HCONNECT hConn)
	{
		return NULL;
	}

	HUDPEP NewEP(const ASOCK_ADDR& sa, PFN_ON_DATAGRAM on, HIOPOOL hpool, HWORKERS hworkers, void* key)
	{
		return NULL;
	}

	void DelEP(HUDPEP hep)
	{
	}

	void StartEP(HUDPEP hep)
	{
	}

	void StopEP(HUDPEP hep)
	{
	}

	void Send(HUDPEP hep, const ASOCK_ADDR& sa, _U32 len, _U8* data)
	{
	}

	bool IsRunning(HUDPEP hep)
	{
		return false;
	}

	void* KeyOf(HUDPEP hep)
	{
		return NULL;
	}

	void SetKey(HUDPEP hep, void* key)
	{
	}

	bool GetEpAddr(HUDPEP hep, ASOCK_ADDR& sa)
	{
		return false;
	}

	HIOPOOL HIOPoolOf(HUDPEP hep)
	{
		return NULL;
	}

	HWORKERS HWorkersOf(HUDPEP hep)
	{
		return NULL;
	}

	void ASockIOInit()
	{
	}

	void ASockIOFini()
	{
	}

};

#endif

#endif
