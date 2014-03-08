#ifndef __ZION_DATA_CACHE_SERVER__
#define __ZION_DATA_CACHE_SERVER__

namespace Zion
{
	namespace DataCache
	{

		void RPCIMPL_GetAvatar(const JSONRPC_RESPONSE& res, _U32 avatar_id);
		void RPCIMPL_KeepAlive(const JSONRPC_RESPONSE& res, _U32 avatar_id);
		void RPCIMPL_SaveToDB(const JSONRPC_RESPONSE& res, _U32 avatar_id);
		void RPCIMPL_ExecuteDDL(const JSONRPC_RESPONSE& res, _U32 avatar_id, const char* method_name, const _U8* buf, _U32 len);
		void RPCIMPL_CreateObject(const JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data);
		void RPCIMPL_UpdateObject(const JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID& _uuid, const char* data);
		void RPCIMPL_DeleteObject(const JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID* _uuids, _U32 count);
		void RPCIMPL_FlushAllData();

	}
}

#endif
