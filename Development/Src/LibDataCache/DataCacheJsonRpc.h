#ifndef __ZION_DATA_CACHE_JSONRPC__
#define __ZION_DATA_CACHE_JSONRPC__

namespace Zion
{
	namespace DataCache
	{

		void JsonRPC_GetAvatar(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_KeepAlive(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_SaveToDB(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_ExecuteDDL(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_CreateObject(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_UpdateObject(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_DeleteObject(const JSONRPC_RESPONSE& res, const char* args);

	}
}

#endif
