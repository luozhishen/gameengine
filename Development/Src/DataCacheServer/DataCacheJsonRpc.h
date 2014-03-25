#ifndef __ZION_DATA_CACHE_JSONRPC__
#define __ZION_DATA_CACHE_JSONRPC__

namespace Zion
{
	namespace DataCache
	{

		void JsonRPC_Echo(const Json::Value& args);
		void JsonRPC_LoginUser(const Json::Value& args);
		void JsonRPC_CreateAvatar(const Json::Value& args);
		void JsonRPC_DeleteAvatar(const Json::Value& args);
		void JsonRPC_GetAvatarList(const Json::Value& args);
		void JsonRPC_GetAvatar(const Json::Value& args);
		void JsonRPC_SaveAvatar(const Json::Value& args);
		void JsonRPC_ClearAvatar(const Json::Value& args);
		void JsonRPC_KeepAlive(const Json::Value& args);
		void JsonRPC_CreateObject(const Json::Value& args);
		void JsonRPC_UpdateObject(const Json::Value& args);
		void JsonRPC_DeleteObject(const Json::Value& args);
		void JsonRPC_LoadObjectFromDB(const Json::Value& args);

	}
}

#endif
