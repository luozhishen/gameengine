#ifndef __ZION_DATA_CACHE_JSONRPC__
#define __ZION_DATA_CACHE_JSONRPC__

namespace Zion
{
	namespace DataCache
	{

		void JsonRPC_CreateAvatar(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_DeleteAvatar(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_GetAvatarList(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_GetAvatar(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_SaveAvatar(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_ClearAvatar(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_KeepAlive(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_ExecuteDDL(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_CreateObject(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_UpdateObject(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_DeleteObject(const JSONRPC_RESPONSE& res, const char* args);

	}
}

#endif
