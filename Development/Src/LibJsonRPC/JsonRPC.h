#ifndef	__ZION_JSONRPC__
#define	__ZION_JSONRPC__

#include <functional>

namespace Zion
{

	class CJsonRPCServer;
	class CJsonRPCClient;
	class CJsonRPCResponse;

	/*
	typedef void(JSON_RESPONSE_PROC)(CJsonRPCResponse* res, const char* args);
	typedef void(JSON_CALLBACK_PROC)(bool error, const char* args);
	*/

	typedef std::function<void(CJsonRPCResponse*, const char*)>	JSON_RESPONSE_PROC;
	typedef std::function<void(bool, const char*)>				JSON_CALLBACK_PROC;

	CJsonRPCServer* JsonRPC_StartServer(const char* ep);
	void JsonRPC_StopServer(CJsonRPCServer* pServer);
	void JsonRPC_BindMethod(const char* method, JSON_RESPONSE_PROC proc);

	CJsonRPCClient* JsonRPC_GetClient(const char* ep);
	void JsonRPC_StopClients();
	bool JsonRPC_Send(CJsonRPCClient* pClient, const char* method, const char* args);
	bool JsonRPC_Send(CJsonRPCClient* pClient, const char* method, const char* args, JSON_CALLBACK_PROC proc);

}

#endif
