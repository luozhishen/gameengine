#include <ZionBase.h>
#include <ZionCommon.h>
#include <JsonRPC.h>
#include "DataCacheJsonRPC.h"

namespace Zion
{
	namespace DataCache
	{

		static IServer* g_singleton_server = NULL;
		IServer* IServer::Singleton()
		{
			return g_singleton_server;
		}

		IServer::IServer()
		{
			g_singleton_server = this;
		}

		IServer::~IServer()
		{
			g_singleton_server = NULL;
		}

		void JsonRPC_GetAvatar(const JSONRPC_RESPONSE& res, const char* args)
		{
		}

		void JsonRPC_KeepAlive(const JSONRPC_RESPONSE& res, const char* args)
		{
		}

		void JsonRPC_ExecuteDDL(const JSONRPC_RESPONSE& res, const char* args)
		{
		}

		void JsonRPC_SetAvatarObject(const JSONRPC_RESPONSE& res, const char* args)
		{
		}

		void JsonRPC_DelAvatarObject(const JSONRPC_RESPONSE& res, const char* args)
		{
		}

	}
}
