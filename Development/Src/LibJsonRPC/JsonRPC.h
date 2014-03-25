#ifndef	__ZION_JSONRPC__
#define	__ZION_JSONRPC__

#include <functional>

namespace Zion
{

	const _U32 SENDBUF_SIZE = 10*1024;
	const _U32 RECVBUF_SIZE = 1*1024*1024;

	class CJsonRPCServer;
	class CJsonRPCClient;

	/*
	struct JSONRPC_RESPONSE;
	typedef struct JSONRPC_RESPONSE JSONRPC_RESPONSE;

	typedef void(JSON_RESPONSE_PROC)(cosnt JSONRPC_RESPONSE& res, const Json::Value& data);
	typedef void(JSON_CALLBACK_PROC)(bool error, const char* args);
	*/
	typedef std::function<void(const Json::Value&)>	JSON_RESPONSE_PROC;
	typedef std::function<void(const Json::Value*)>	JSON_CALLBACK_PROC;

	void JsonRPC_Bind(const char* method, JSON_RESPONSE_PROC proc);
	bool JsonRPC_Start(const char* ep);
	void JsonRPC_Stop();

	bool JsonRPC_Send(const char* args);
//	void JsonRPC_SetPending(const JSONRPC_RESPONSE* res);

	CJsonRPCClient* JsonRPC_GetClient(const char* ep);
	void JsonRPC_StopClients();
	bool JsonRPC_Send(CJsonRPCClient* pClient, const char* method, const char* args, JSON_CALLBACK_PROC proc);

}

#endif
