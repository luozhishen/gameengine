#ifndef __ZION_DATA_CACHE_RPCIMPL__
#define __ZION_DATA_CACHE_RPCIMPL__

namespace Zion
{
	namespace DataCache
	{

		void RPCIMPL_LoginUser(const JSONRPC_RESPONSE& res, const char* token);
		void RPCIMPL_CreateAvatar(
				const JSONRPC_RESPONSE& res,
				_U32 user_id,
				_U32 server_id,
				const char* avatar_name,
				const char* avatar_desc,
				const Array<A_UUID>& uuids,
				const Array<String>& types,
				const Array<String>& datas);
		void RPCIMPL_DeleteAvatar(const JSONRPC_RESPONSE& res, _U32 avatar_id);
		void RPCIMPL_GetAvatarList(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 server_id);

		void RPCIMPL_GetAvatar(const JSONRPC_RESPONSE& res, _U32 avatar_id);
		void RPCIMPL_SaveAvatar(const JSONRPC_RESPONSE& res, _U32 avatar_id);
		void RPCIMPL_ClearAvatar(const JSONRPC_RESPONSE& res, _U32 avatar_id);
		void RPCIMPL_KeepAlive(const JSONRPC_RESPONSE& res, _U32 avatar_id);
		void RPCIMPL_CreateObject(const JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data);
		void RPCIMPL_UpdateObject(const JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID& _uuid, const char* data);
		void RPCIMPL_DeleteObject(const JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID* _uuids, _U32 count);
		void RPCIMPL_LoadObjectFromDB(const JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID& _uuid);
		void RPCIMPL_FlushAllData();

	}
}

#endif
