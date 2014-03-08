#ifndef __ZION_SIMPLE_DATA_CACHE_SERVER__
#define __ZION_SIMPLE_DATA_CACHE_SERVER__

namespace Zion
{
	namespace DataCache
	{

		void RPCSIMPLE_GetAvatar(const JSONRPC_RESPONSE& res, _U32 avatar_id);
		void RPCSIMPLE_KeepAlive(const JSONRPC_RESPONSE& res, _U32 avatar_id);
		void RPCSIMPLE_SaveToDB(const JSONRPC_RESPONSE& res, _U32 avatar_id);
		void RPCSIMPLE_ExecuteDDL(const JSONRPC_RESPONSE& res, _U32 avatar_id, const char* method_name, const _U8* buf, _U32 len);
		void RPCSIMPLE_CreateObject(const JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data);
		void RPCSIMPLE_UpdateObject(const JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID& _uuid, const char* data);
		void RPCSIMPLE_DeleteObject(const JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID* _uuids, _U32 count);
		void RPCSIMPLE_FlushAllData();

	}
}

#endif
