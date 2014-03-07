#include <ZionBase.h>
#include <ZionCommon.h>
#include <JsonRPC.h>
#include "DataCacheJsonRpc.h"
#include "DataCacheServer.h"

namespace Zion
{
	namespace DataCache
	{

		CServer::CServer()
		{
		}

		CServer::~CServer()
		{
		}

		void CServer::GetAvatar(const JSONRPC_RESPONSE& res, _U32 avatar_id)
		{
		}

		void CServer::KeepAlive(const JSONRPC_RESPONSE& res, _U32 avatar_id)
		{
		}

		void CServer::ExecuteDDL(const JSONRPC_RESPONSE& res, _U32 avatar_id, const char* method, const char* data)
		{
		}

	}
}
