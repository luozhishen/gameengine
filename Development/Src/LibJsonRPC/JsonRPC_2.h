#ifndef	__ZION_JSONRPC2__
#define	__ZION_JSONRPC2__

#include <functional>

namespace JsonRPC2
{

	const _U32 SENDBUF_SIZE = 10*1024;
	const _U32 RECVBUF_SIZE = 1*1024*1024;

	class CJsonRPCServer;
	class CJsonRPCClient;
	
	typedef struct _JSONRPC_RESPONSE
	{
		_U32 conn_id;
		_U32 req_seq;
	} JSONRPC_RESPONSE;

	/*
	typedef void(JSON_RESPONSE_PROC)(cosnt JSONRPC_RESPONSE& res, const Json::Value& data);
	typedef void(JSON_CALLBACK_PROC)(bool error, const char* args);
	*/
	typedef std::function<void(const Json::Value& data)>	JSON_RESPONSE_PROC;
	typedef std::function<void(const Json::Value* data)>	JSON_CALLBACK_PROC;

	void JsonRPC_Bind(const char* method, JSON_RESPONSE_PROC proc);
	bool JsonRPC_Start(const char* ep);
	void JsonRPC_Stop();

	bool JsonRPC_Send(const char* args, _U32 len=(_U32)-1);
	bool JsonRPC_SetPending(JSONRPC_RESPONSE& res);
	bool JsonRPC_Send(const JSONRPC_RESPONSE& res, const char* args, _U32 len=(_U32)-1);

	CJsonRPCClient* JsonRPC_GetClient(const char* ep);
	void JsonRPC_StopClients();
	bool JsonRPC_Send(CJsonRPCClient* client, const char* method, const char* args, JSON_CALLBACK_PROC proc);

}

#endif
