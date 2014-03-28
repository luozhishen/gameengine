#include <ZionBase.h>
#include "DataCacheDBApi.h"
#include "DataCacheDBApiFake.h"
#include "DataCacheServer.h"

namespace Zion
{
	namespace DataCache
	{

		class CFakeDBApi : public IDBApi
		{
		public:
			CFakeDBApi();
			virtual ~CFakeDBApi();

			virtual bool Check();
			virtual bool Connect();
			virtual bool Disconnect();

			virtual bool BeginTranscation();
			virtual bool RollbackTransaction();
			virtual bool CommitTransaction();

			virtual _U32 CreateAvatar(_U32 user_id, _U32 server_id, const char* avatar_name, const char* avatar_desc);
			virtual bool DeleteAvatar(_U32 avatar_id);
			virtual bool LoadAvatar(_U32 avatar_id, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr);
			virtual bool InsertAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data);
			virtual bool UpdateAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* data);
			virtual bool DeleteAvatarObject(_U32 avatar_id, const A_UUID* _uuids, _U32 count);
			virtual bool QueryAvatarObject(_U32 avatar_id, const A_UUID& _uuid, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr);
			virtual bool LockTask(_U32 avatar_id, _U32 task_id);
			virtual bool MarkTask(_U32 avatar_id, _U32 task_id);
		};

		IDBApi* CreateFakeDatabase()
		{
			return ZION_NEW CFakeDBApi();
		}

		CFakeDBApi::CFakeDBApi()
		{
		}

		CFakeDBApi::~CFakeDBApi()
		{
		}

		bool CFakeDBApi::Check()
		{
			return true;
		}

		bool CFakeDBApi::Connect()
		{
			return true;
		}

		bool CFakeDBApi::Disconnect()
		{
			return true;
		}

		bool CFakeDBApi::BeginTranscation()
		{
			return true;
		}

		bool CFakeDBApi::RollbackTransaction()
		{
			return true;
		}

		bool CFakeDBApi::CommitTransaction()
		{
			return true;
		}

		_U32 CFakeDBApi::CreateAvatar(_U32 user_id, _U32 server_id, const char* avatar_name, const char* avatar_desc)
		{
			return user_id;
		}

		bool CFakeDBApi::DeleteAvatar(_U32 avatar_id)
		{
			return true;
		}

		bool CFakeDBApi::LoadAvatar(_U32 avatar_id, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr)
		{
			for(_U32 i=0; i<1; i++)
			{
				A_UUID _uuid;
				AUuidGenerate(_uuid);
				if(!callback(userptr, _uuid, "faketype", "{\"name\":\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\"}")) return false;
			}
			return true;
		}

		bool CFakeDBApi::InsertAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data)
		{
			return true;
		}

		bool CFakeDBApi::UpdateAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* data)
		{
			return true;
		}

		bool CFakeDBApi::DeleteAvatarObject(_U32 avatar_id, const A_UUID* _uuids, _U32 count)
		{
			return true;
		}

		bool CFakeDBApi::QueryAvatarObject(_U32 avatar_id, const A_UUID& _uuid, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr)
		{
			if(!callback(userptr, _uuid, "faketype", "{\"name\":\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\"}")) return false;
			return true;
		}

		bool CFakeDBApi::LockTask(_U32 avatar_id, _U32 task_id)
		{
			return true;
		}

		bool CFakeDBApi::MarkTask(_U32 avatar_id, _U32 task_id)
		{
			return true;
		}

	}
}
