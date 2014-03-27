#ifndef __ZION_DATA_CACHE_JSONRPC__
#define __ZION_DATA_CACHE_JSONRPC__

namespace Zion
{
	namespace DataCache
	{

		void JsonRPC_Echo(const JsonValue& args);
		void JsonRPC_LoginUser(const JsonValue& args);
		void JsonRPC_CreateAvatar(const JsonValue& args);
		void JsonRPC_DeleteAvatar(const JsonValue& args);
		void JsonRPC_GetAvatarList(const JsonValue& args);
		void JsonRPC_GetAvatar(const JsonValue& args);
		void JsonRPC_SaveAvatar(const JsonValue& args);
		void JsonRPC_ClearAvatar(const JsonValue& args);
		void JsonRPC_KeepAlive(const JsonValue& args);
		void JsonRPC_CreateObject(const JsonValue& args);
		void JsonRPC_UpdateObject(const JsonValue& args);
		void JsonRPC_DeleteObject(const JsonValue& args);
		void JsonRPC_LoadObjectFromDB(const JsonValue& args);

	}
}

#endif
