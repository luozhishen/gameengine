#ifndef	__ZION_JSONRPC__
#define	__ZION_JSONRPC__

#include <functional>

namespace Zion
{

	class CJsonRPCServer;
	class CJsonRPCClient;
	
	struct JSONRPC_RESPONSE;
	typedef struct JSONRPC_RESPONSE JSONRPC_RESPONSE;

	/*
	typedef void(JSON_RESPONSE_PROC)(cosnt JSONRPC_RESPONSE& res, const Json::Value& data);
	typedef void(JSON_CALLBACK_PROC)(bool error, const char* args);
	*/
	typedef std::function<void(const JSONRPC_RESPONSE* res, const Json::Value& data)>	JSON_RESPONSE_PROC;
	typedef std::function<void(int errcode, const char*)>								JSON_CALLBACK_PROC;

	CJsonRPCServer* JsonRPC_Create(bool bSingleThread);
	void JsonRPC_Destory(CJsonRPCServer* pServer);

	void JsonRPC_Bind(CJsonRPCServer* pServer, const char* method, JSON_RESPONSE_PROC proc);
	bool JsonRPC_Start(CJsonRPCServer* pServer, const char* ep);
	void JsonRPC_Stop(CJsonRPCServer* pServer);

	void JsonRPC_SetPending(const JSONRPC_RESPONSE* res);
	bool JsonRPC_Send(const JSONRPC_RESPONSE* res, const char* args);

	CJsonRPCClient* JsonRPC_GetClient(const char* ep);
	void JsonRPC_StopClients();
	bool JsonRPC_Send(CJsonRPCClient* pClient, const char* method, const char* args);
	bool JsonRPC_Send(CJsonRPCClient* pClient, const char* method, const char* args, JSON_CALLBACK_PROC proc);

}

#endif
