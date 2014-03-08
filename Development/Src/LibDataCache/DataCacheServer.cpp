#include <ZionBase.h>
#include <ZionCommon.h>
#include <JsonRPC.h>
#include "DataCacheServer.h"

namespace Zion
{
	namespace DataCache
	{

		void RPCIMPL_GetAvatar(const Zion::JSONRPC_RESPONSE& res, _U32 avatar_id)
		{
		}

		void RPCIMPL_KeepAlive(const Zion::JSONRPC_RESPONSE& res, _U32 avatar_id)
		{
		}

		void RPCIMPL_SaveToDB(const Zion::JSONRPC_RESPONSE& res, _U32 avatar_id)
		{
		}

		void RPCIMPL_ExecuteDDL(const Zion::JSONRPC_RESPONSE& res, _U32 avatar_id, const char* method_name, const _U8* buf, _U32 len)
		{
		}

		void RPCIMPL_CreateObject(const Zion::JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data)
		{
		}

		void RPCIMPL_UpdateObject(const Zion::JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID& _uuid, const char* data)
		{
		}

		void RPCIMPL_DeleteObject(const Zion::JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID* _uuids, _U32 count)
		{
		}

		void RPCIMPL_FlushAllData()
		{
		}

	}
}
