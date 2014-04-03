#ifndef __ZION_DATA_CACHE_RPCIMPL__
#define __ZION_DATA_CACHE_RPCIMPL__

namespace Zion
{
	namespace DataCache
	{

		const _U32 MAX_AVATAR =  100*10000;

		enum TASK_TYPE
		{
			TASK_CREATE_OBJECT,
			TASK_DELETE_OBJECT,
			TASK_UPDATE_OBJECT,
			TASK_DELETE_TASK,
		};

		typedef struct _TASK
		{
			TASK_TYPE _task_type;
			A_UUID _obj_uuid;
			String _obj_type;
			String _obj_data;
			_U32 _task_id;
		} TASK;

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
		void RPCIMPL_GetAvatar(_U32 avatar_id);
		// return errcode, avatar_id, version, avatar_objects[]
		void RPCIMPL_SaveAvatar(_U32 avatar_id);
		// return errcode
		void RPCIMPL_KeepAlive(_U32 avatar_id);
		// return errcode, version
		void RPCIMPL_CreateObject(_U32 avatar_id, _U32 version, const A_UUID& _uuid, const char* type, const char* data);
		// return errcode, version
		void RPCIMPL_UpdateObject(_U32 avatar_id, _U32 version, const A_UUID& _uuid, const char* data);
		// return errcode, version
		void RPCIMPL_DeleteObject(_U32 avatar_id, _U32 version, const A_UUID* _uuids, _U32 count);
		// return errcode, version
		void RPCIMPL_LoadObject(_U32 avatar_id, const A_UUID& _uuid);
		// return errcode, uuid, type, data
		void RPCIMPL_ExecuteBatch(_U32 avatar_id, _U32 version, const Array<TASK>& tasks);
		// return errcode, version

		void RPCIMPL_GetChangeList(Set<_U32>& list);
		void RPCIMPL_Init();
		void RPCIMPL_FlushAllData();

	}
}

#endif
