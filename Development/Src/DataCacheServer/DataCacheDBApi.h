#ifndef __ZION_DATA_CACHE_DBAPI__
#define __ZION_DATA_CACHE_DBAPI__

namespace Zion
{
	namespace DataCache
	{

		class IDBApi
		{
		public:
			virtual ~IDBApi() { }

			virtual _U32 LoginUser(const char* token) = 0;
			virtual _U32 CreateAvatar(_U32 user_id, _U32 server_id, const char* avatar_name, const char* avatar_desc) = 0;
			virtual bool DeleteAvatar(_U32 avatar_id) = 0;
			virtual bool GetAvatarList(_U32 user_id, _U32 server_id, bool (*callback)(void*, _U32, _U32, const char*, const char*), void* userptr) = 0;
			virtual bool LoadAvatar(_U32 avatar_id, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr) = 0;
			virtual bool InsertAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data) = 0;
			virtual bool UpdateAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* data) = 0;
			virtual bool DeleteAvatarObject(_U32 avatar_id, const A_UUID* _uuids, _U32 count) = 0;
			virtual bool QueryAvatarObject(_U32 avatar_id, const A_UUID& _uuid, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr) = 0;
		};

		bool InitDatabase();
		void FiniDatabase();
		IDBApi* AllocDataBase();
		void FreeDatabase(IDBApi* db);

	}
}

#endif
