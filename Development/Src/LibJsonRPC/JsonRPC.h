#ifndef	__ZION_JSONRPC__
#define	__ZION_JSONRPC__

#include <functional>

namespace Zion
{

	const _U32 SENDBUF_SIZE = 10*1024;
	const _U32 RECVBUF_SIZE = 1*1024*1024;

	class CJsonRPCServer;
	class CJsonRPCClient;

	typedef struct _JSONRPC_RESPONSE_ID
	{
		_U32 conn;
		_U32 seq;
	} JSONRPC_RESPONSE_ID;
	void SetInvalidResponseID(JSONRPC_RESPONSE_ID& id);
	bool IsValidResponseID(const JSONRPC_RESPONSE_ID& id);

	/*
	struct JSONRPC_RESPONSE_ID;
	typedef struct JSONRPC_RESPONSE_ID JSONRPC_RESPONSE_ID;

	typedef void(JSON_RESPONSE_PROC)(const Json::Value& data);
	typedef void(JSON_CALLBACK_PROC)(bool error, const char* args);
	*/
	typedef std::function<void(const JsonValue&)>	JSON_RESPONSE_PROC;
	typedef std::function<void(const JsonValue*)>	JSON_CALLBACK_PROC;

	void JsonRPC_Bind(const char* method, JSON_RESPONSE_PROC proc);
	bool JsonRPC_Start(const char* ep, bool singlethread);
	void JsonRPC_Stop();

	bool JsonRPC_Send(const char* args);
	bool JsonRPC_SetPending(JSONRPC_RESPONSE_ID& res);
	bool JsonRPC_Send(const JSONRPC_RESPONSE_ID& res, const char* args);


	CJsonRPCClient* JsonRPC_GetClient(const char* ep);
	void JsonRPC_StopClients();
	bool JsonRPC_Call(CJsonRPCClient* pClient, const char* method, const char* args, JSON_CALLBACK_PROC proc);
	bool JsonRPC_Call(CJsonRPCClient* pClient, const char* method, const JsonValue& args, JSON_CALLBACK_PROC proc);
	bool JsonRpc_Call(CJsonRPCClient* pClient, const char* method, const JsonValue& args, JsonValue& ret);

}

#endif
