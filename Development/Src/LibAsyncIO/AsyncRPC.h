#ifndef WITHOUT_ZION_ASYNCIO

#ifndef	__LWRPC__
#define	__LWRPC__

#ifndef	__cplusplus
#error	C++ compiler needed
#endif

namespace Zion {

	typedef struct RPC_SERVER* HSERVER;
	typedef struct RPC_CLIENT* HCLIENT;

	typedef _U32 RPC_RESULT;

	#define	RPC_RES_NONE	0
	#define	RPC_RES_OOM		1
	#define	RPC_RES_SNA		2
	#define	RPC_RES_TMP		3

	bool StartRPCService(_U32 ip, _U16 port, _U32 tcnt);
	void StopRPCService();
	_U32 GetRPCClientAddr(HCLIENT hClient);
	_U32 GetRPCServerAddr(HSERVER hServer);
	_U16 GetRPCServerPort(HSERVER hServer);
	HSERVER GetRPCServer(const _STR ep);
	HSERVER GetRPCServer(_U32 ip, _U16 port);
	HSERVER GetRPCServer(HCLIENT hClient);
	HSERVER GetRPCServer(HCLIENT hClient, _U16 port);
	HSERVER GetLocalRPCServer();
	_U32 GetRPCBindIp();
	_U16 GetRPCBindPort();

}

#endif

#endif
