#ifndef __ZION_DATA_CACHE_DBAPI__
#define __ZION_DATA_CACHE_DBAPI__

namespace Zion
{
	namespace DataCache
	{

		bool InitDatabase();
		void FiniDatabase();

		_U32 LoginUser(const char* token);
		_U32 CreateAvatar(_U32 user_id, _U32 server_id, const char* avatar_name, const char* avatar_desc);
		bool DeleteAvatar(_U32 avatar_id);
		bool GetAvatarList(_U32 user_id, _U32 server_id, bool (*callback)(void*, _U32, _U32, const char*, const char*), void* userptr);
		bool LoadAvatar(_U32 avatar_id, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr);
		bool InsertAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data);
		bool UpdateAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* data);
		bool DeleteAvatarObject(_U32 avatar_id, const A_UUID* _uuids, _U32 count);
		bool QueryAvatarObject(_U32 avatar_id, const A_UUID& _uuid, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr);

	}
}

#endif
