#ifndef __ZION_DATA_CACHE_RPCIMPL__
#define __ZION_DATA_CACHE_RPCIMPL__

namespace Zion
{
	namespace DataCache
	{

		void RPCIMPL_LoginUser(const char* token);
		// return errcode, user_id
		void RPCIMPL_CreateAvatar(
				_U32 user_id,
				_U32 server_id,
				const char* avatar_name,
				const char* avatar_desc,
				const Array<A_UUID>& uuids,
				const Array<String>& types,
				const Array<String>& datas);
		// return errcode, avatar_id
		void RPCIMPL_DeleteAvatar(_U32 avatar_id);
		// return errcode
		void RPCIMPL_GetAvatarList(_U32 user_id, _U32 server_id);
		// return errcode, avatars[]

		void RPCIMPL_GetAvatar(_U32 avatar_id);
		// return errcode, avatar_id, version, avatar_objects[]
		void RPCIMPL_SaveAvatar(_U32 avatar_id);
		// return errcode
		void RPCIMPL_ClearAvatar(_U32 avatar_id);
		// return errcode
		void RPCIMPL_KeepAlive(_U32 avatar_id);
		// return errcode, version
		void RPCIMPL_CreateObject(_U32 avatar_id, _U32 version, const A_UUID& _uuid, const char* type, const char* data);
		// return errcode, version
		void RPCIMPL_UpdateObject(_U32 avatar_id, _U32 version, const A_UUID& _uuid, const char* data);
		// return errcode, version
		void RPCIMPL_DeleteObject(_U32 avatar_id, _U32 version, const A_UUID* _uuids, _U32 count);
		// return errcode, version
		void RPCIMPL_LoadObjectFromDB(_U32 avatar_id, const A_UUID& _uuid);
		// return errcode, version, uuid, type, data

		void RPCIMPL_FlushAllData();

	}
}

#endif
