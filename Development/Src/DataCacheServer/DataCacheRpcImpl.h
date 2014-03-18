#ifndef __ZION_DATA_CACHE_RPCIMPL__
#define __ZION_DATA_CACHE_RPCIMPL__

namespace Zion
{
	namespace DataCache
	{

		void RPCIMPL_LoginUser(const JSONRPC_RESPONSE* res, const char* token);
		// return errcode, user_id
		void RPCIMPL_CreateAvatar(
				const JSONRPC_RESPONSE* res,
				_U32 user_id,
				_U32 server_id,
				const char* avatar_name,
				const char* avatar_desc,
				const Array<A_UUID>& uuids,
				const Array<String>& types,
				const Array<String>& datas);
		// return errcode, avatar_id
		void RPCIMPL_DeleteAvatar(const JSONRPC_RESPONSE* res, _U32 avatar_id);
		// return errcode
		void RPCIMPL_GetAvatarList(const JSONRPC_RESPONSE* res, _U32 user_id, _U32 server_id);
		// return errcode, avatars[]

		void RPCIMPL_GetAvatar(const JSONRPC_RESPONSE* res, _U32 avatar_id);
		// return errcode, avatar_objects[]
		void RPCIMPL_SaveAvatar(const JSONRPC_RESPONSE* res, _U32 avatar_id);
		// return errcode
		void RPCIMPL_ClearAvatar(const JSONRPC_RESPONSE* res, _U32 avatar_id);
		// return errcode
		void RPCIMPL_KeepAlive(const JSONRPC_RESPONSE* res, _U32 avatar_id);
		// return errcode
		void RPCIMPL_CreateObject(const JSONRPC_RESPONSE* res, _U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data);
		// return errcode
		void RPCIMPL_UpdateObject(const JSONRPC_RESPONSE* res, _U32 avatar_id, const A_UUID& _uuid, const char* data);
		// return errcode
		void RPCIMPL_DeleteObject(const JSONRPC_RESPONSE* res, _U32 avatar_id, const A_UUID* _uuids, _U32 count);
		// return errcode
		void RPCIMPL_LoadObjectFromDB(const JSONRPC_RESPONSE* res, _U32 avatar_id, const A_UUID& _uuid);
		// return errcode, uuid, type, data
		void RPCIMPL_FlushAllData();

	}
}

#endif
