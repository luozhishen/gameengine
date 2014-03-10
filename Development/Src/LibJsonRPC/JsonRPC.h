#ifndef	__ZION_JSONRPC__
#define	__ZION_JSONRPC__

#include <functional>

namespace Zion
{

	class CJsonRPCServer;
	class CJsonRPCClient;
	
	typedef struct JSONRPC_RESPONSE
	{
		CJsonRPCServer*	server;
		_U32			conn;
		_U32			seq;
	} JSONRPC_RESPONSE;

	/*
	typedef void(JSON_RESPONSE_PROC)(cosnt JSONRPC_RESPONSE& res, const Json::Value& data);
	typedef void(JSON_CALLBACK_PROC)(bool error, const char* args);
	*/
	typedef std::function<void(const JSONRPC_RESPONSE& res, const Json::Value& data)>	JSON_RESPONSE_PROC;
	typedef std::function<void(int errcode, const char*)>								JSON_CALLBACK_PROC;

	void JsonRPC_Bind(const char* method, JSON_RESPONSE_PROC proc);
	bool JsonRPC_Start(const char* ep);
	void JsonRPC_Stop();

	bool JsonRPC_Valid(const JSONRPC_RESPONSE& res);
	bool JsonRPC_Send(const JSONRPC_RESPONSE& res, const char* args=NULL);

	CJsonRPCClient* JsonRPC_GetClient(const char* ep);
	void JsonRPC_StopClients();
	bool JsonRPC_Send(CJsonRPCClient* pClient, const char* method, const char* args);
	bool JsonRPC_Send(CJsonRPCClient* pClient, const char* method, const char* args, JSON_CALLBACK_PROC proc);

}

#endif
