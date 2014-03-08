#ifndef __ZION_DATA_CACHE_DBAPI__
#define __ZION_DATA_CACHE_DBAPI__

namespace Zion
{
	namespace DataCache
	{

		bool InitDatabase();
		void FiniDatabase();

		bool LoadAvatar(_U32 avatar_id, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr);
		bool InsertAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data);
		bool UpdateAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* data);
		bool DeleteAvatarObject(_U32 avatar_id, const A_UUID* _uuids, _U32 count);

	}
}

#endif
