#ifndef __ZION_SIMPLE_DATA_CACHE_SERVER__
#define __ZION_SIMPLE_DATA_CACHE_SERVER__

namespace Zion
{
	namespace DataCache
	{

		void RPCSIMPLE_CreateAvatar(
				const JSONRPC_RESPONSE& res,
				_U32 user_id,
				_U32 server_id,
				const char* avatar_name,
				const char* avatar_desc,
				const Array<A_UUID>& uuids,
				const Array<String>& types,
				const Array<String>& datas);
		void RPCSIMPLE_DeleteAvatar(const JSONRPC_RESPONSE& res, _U32 avatar_id);
		void RPCSIMPLE_GetAvatarList(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 server_id);

		void RPCSIMPLE_GetAvatar(const JSONRPC_RESPONSE& res, _U32 avatar_id);
		void RPCSIMPLE_SaveAvatar(const JSONRPC_RESPONSE& res, _U32 avatar_id);
		void RPCSIMPLE_ClearAvatar(const JSONRPC_RESPONSE& res, _U32 avatar_id);
		void RPCSIMPLE_KeepAlive(const JSONRPC_RESPONSE& res, _U32 avatar_id);
		void RPCSIMPLE_ExecuteDDL(const JSONRPC_RESPONSE& res, _U32 avatar_id, const char* method_name, const _U8* buf, _U32 len);
		void RPCSIMPLE_CreateObject(const JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data);
		void RPCSIMPLE_UpdateObject(const JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID& _uuid, const char* data);
		void RPCSIMPLE_DeleteObject(const JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID* _uuids, _U32 count);
		void RPCSIMPLE_FlushAllData();

	}
}

#endif
