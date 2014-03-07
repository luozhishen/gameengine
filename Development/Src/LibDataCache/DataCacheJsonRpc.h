#ifndef __ZION_DATA_CACHE_JSONRPC__
#define __ZION_DATA_CACHE_JSONRPC__

namespace Zion
{
	namespace DataCache
	{

		class IServer
		{
		public:
			static IServer* Singleton();

			IServer();
			virtual ~IServer();

			virtual void GetAvatar(const JSONRPC_RESPONSE& res, _U32 avatar_id) = 0;
			virtual void KeepAlive(const JSONRPC_RESPONSE& res, _U32 avatar_id) = 0;
			virtual void ExecuteDDL(const JSONRPC_RESPONSE& res, _U32 avatar_id, const char* method, const char* data) = 0;

		};

		void JsonRPC_GetAvatar(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_KeepAlive(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_ExecuteDDL(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_SetAvatarObject(const JSONRPC_RESPONSE& res, const char* args);
		void JsonRPC_DelAvatarObject(const JSONRPC_RESPONSE& res, const char* args);

	}
}

#endif
