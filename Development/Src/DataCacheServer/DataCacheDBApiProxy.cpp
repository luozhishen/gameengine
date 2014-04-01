#include <ZionBase.h>
#include "DataCacheDBApi.h"
#include "DataCacheDBApiProxy.h"
#include "DataCacheServer.h"

namespace Zion
{
	namespace DataCache
	{

		class CProxyDBApi : public IDBApi
		{
		public:
			CProxyDBApi(IDBApi* db);
			virtual ~CProxyDBApi();

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
			virtual bool DeleteTask(_U32 avatar_id, _U32 task_id);

			IDBApi* m_db;
			JsonValue* m_json;
			void* m_userptr;
			bool (*m_callback)(void*, const A_UUID&, const char*, const char*);
		};

		IDBApi* CreateProxyDatabase(IDBApi* db)
		{
			return ZION_NEW CProxyDBApi(db);
		}

		CProxyDBApi::CProxyDBApi(IDBApi* db)
		{
			m_db = db;
		}

		CProxyDBApi::~CProxyDBApi()
		{
			ZION_DELETE m_db;
			m_db = NULL;
		}

		bool CProxyDBApi::Check()
		{
			return true;
		}

		bool CProxyDBApi::Connect()
		{
			return true;
		}

		bool CProxyDBApi::Disconnect()
		{
			return true;
		}

		bool CProxyDBApi::BeginTranscation()
		{
			bool ret = m_db->BeginTranscation();
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("BeginTranscation", json);
			return ret;
		}

		bool CProxyDBApi::RollbackTransaction()
		{
			bool ret = m_db->RollbackTransaction();
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("RollbackTransaction", json);
			return ret;
		}

		bool CProxyDBApi::CommitTransaction()
		{
			bool ret = m_db->CommitTransaction();
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("CommitTransaction", json);
			return ret;
		}

		_U32 CProxyDBApi::CreateAvatar(_U32 user_id, _U32 server_id, const char* avatar_name, const char* avatar_desc)
		{
			_U32 ret = m_db->CreateAvatar(user_id, server_id, avatar_name, avatar_desc);
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("CreateAvatar", json);
			return ret;
		}

		bool CProxyDBApi::DeleteAvatar(_U32 avatar_id)
		{
			bool ret = m_db->DeleteAvatar(avatar_id);
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("DeleteAvatar", json);
			return ret;
		}

		static bool loadavatar_callback(void* userptr, const A_UUID& uuid, const char* type, const char* data)
		{
			CProxyDBApi* db = (CProxyDBApi*)userptr;
			bool ret = db->m_callback(db->m_userptr, uuid, type, data);
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			char str[100];
			json.Append(JsonValue(ret));
			AUuidToString(uuid, str);
			json.Append(JsonValue(str));
			json.Append(JsonValue(type));
			json.Append(JsonValue(data));
			db->m_json->Append(json);
			return ret;
		}

		bool CProxyDBApi::LoadAvatar(_U32 avatar_id, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr)
		{
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(JsonValue::TYPE_ARRAY));
			m_userptr = userptr;
			m_json = (JsonValue*)&json.Get((_U32)0);
			m_callback = callback;
			bool ret = m_db->LoadAvatar(avatar_id, loadavatar_callback, this);
			json.Append(JsonValue(ret));
			WriteDBLog("LoadAvatar", json);
			return ret;
		}

		bool CProxyDBApi::InsertAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data)
		{
			bool ret = m_db->DeleteAvatar(avatar_id);
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("InsertAvatarObject", json);
			return ret;
		}

		bool CProxyDBApi::UpdateAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* data)
		{
			bool ret = m_db->DeleteAvatar(avatar_id);
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("UpdateAvatarObject", json);
			return ret;
		}

		bool CProxyDBApi::DeleteAvatarObject(_U32 avatar_id, const A_UUID* _uuids, _U32 count)
		{
			bool ret = m_db->DeleteAvatar(avatar_id);
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("DeleteAvatarObject", json);
			return ret;
		}

		bool CProxyDBApi::QueryAvatarObject(_U32 avatar_id, const A_UUID& _uuid, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr)
		{
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(JsonValue::TYPE_ARRAY));
			m_userptr = userptr;
			m_json = &json;
			m_callback = callback;
			bool ret = m_db->QueryAvatarObject(avatar_id, _uuid, loadavatar_callback, this);
			json.Append(JsonValue(ret));
			WriteDBLog("QueryAvatarObject", json);
			return ret;
		}

		bool CProxyDBApi::LockTask(_U32 avatar_id, _U32 task_id)
		{
			bool ret = m_db->LockTask(avatar_id, task_id);
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("LockTask", json);
			return ret;
		}

		bool CProxyDBApi::DeleteTask(_U32 avatar_id, _U32 task_id)
		{
			bool ret = m_db->DeleteTask(avatar_id, task_id);
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("DeleteTask", json);
			return ret;
		}

	}
}
