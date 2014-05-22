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
			CProxyDBApi(FILE* fp);
			CProxyDBApi(IDBApi* db);
			virtual ~CProxyDBApi();

			bool Read(const char* method, JsonValue& json);

			virtual bool Check();
			virtual bool Connect();
			virtual bool Disconnect();

			virtual bool BeginTranscation();
			virtual bool RollbackTransaction();
			virtual bool CommitTransaction();

			virtual _U32 CreateAvatar(_U32 user_id, _U32 avatar_scope, const char* avatar_name, const char* avatar_desc);
			virtual bool DeleteAvatar(_U32 user_id, _U32 avatar_scope, _U32 avatar_id);
			virtual bool LoadAvatar(_U32 avatar_id, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr);
			virtual bool InsertAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data);
			virtual bool UpdateAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* data);
			virtual bool DeleteAvatarObject(_U32 avatar_id, const A_UUID* _uuids, _U32 count);
			virtual bool QueryAvatarObject(_U32 avatar_id, const A_UUID& _uuid, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr);
			virtual bool LockTask(_U32 avatar_id, _U32 task_id);
			virtual bool DeleteTask(_U32 avatar_id, _U32 task_id);

			FILE* m_fp;
			IDBApi* m_db;
			JsonValue* m_json;
			void* m_userptr;
			bool (*m_callback)(void*, const A_UUID&, const char*, const char*);
		};

		IDBApi* CreateProxyDatabase(IDBApi* db)
		{
			return ZION_NEW CProxyDBApi(db);
		}

		IDBApi* CreateProxyDatabase(const char* filename)
		{
			return NULL;
		}

		CProxyDBApi::CProxyDBApi(FILE* fp)
		{
			m_fp = fp;
			m_db = NULL;
		}

		CProxyDBApi::CProxyDBApi(IDBApi* db)
		{
			m_fp = NULL;
			m_db = db;
		}

		CProxyDBApi::~CProxyDBApi()
		{
			if(m_fp)
			{
				fclose(m_fp);
				m_fp = NULL;
			}
			if(m_db)
			{
				ZION_DELETE m_db;
				m_db = NULL;
			}
		}

		bool CProxyDBApi::Read(const char* method, JsonValue& json)
		{
			static char buf[1000000];
			if(!fgets(buf, sizeof(buf), m_fp)) return false;
			char* pos = strchr(buf, ' ');
			if(!pos) return false;
			if((size_t)(pos-buf)!=strlen(method) || memcmp(method, buf, pos-buf)!=0) return false;
			if(!json.Parse(pos+1, pos+strlen(pos+1))) return false;
			return true;
		}

		bool CProxyDBApi::Check()
		{
			if(m_fp)
			{
				JsonValue json;
				if(!Read("Check", json)) ZION_FATAL("failed to read db log file\n");
				return json.Get((_U32)0).AsBool();
			}
			bool ret = m_db->BeginTranscation();
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("Check", json);
			return ret;
		}

		bool CProxyDBApi::Connect()
		{
			if(m_fp)
			{
				JsonValue json;
				if(!Read("Connect", json)) ZION_FATAL("failed to read db log file\n");
				return json.Get((_U32)0).AsBool();
			}
			bool ret = m_db->BeginTranscation();
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("Connect", json);
			return ret;
		}

		bool CProxyDBApi::Disconnect()
		{
			if(m_fp)
			{
				JsonValue json;
				if(!Read("Disconnect", json)) ZION_FATAL("failed to read db log file\n");
				return json.Get((_U32)0).AsBool();
			}
			bool ret = m_db->BeginTranscation();
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("Disconnect", json);
			return ret;
		}

		bool CProxyDBApi::BeginTranscation()
		{
			if(m_fp)
			{
				JsonValue json;
				if(!Read("BeginTranscation", json)) ZION_FATAL("failed to read db log file\n");
				return json.Get((_U32)0).AsBool();
			}
			bool ret = m_db->BeginTranscation();
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("BeginTranscation", json);
			return ret;
		}

		bool CProxyDBApi::RollbackTransaction()
		{
			if(m_fp)
			{
				JsonValue json;
				if(!Read("RollbackTransaction", json)) ZION_FATAL("failed to read db log file\n");
				return json.Get((_U32)0).AsBool();
			}
			bool ret = m_db->RollbackTransaction();
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("RollbackTransaction", json);
			return ret;
		}

		bool CProxyDBApi::CommitTransaction()
		{
			if(m_fp)
			{
				JsonValue json;
				if(!Read("CommitTransaction", json)) ZION_FATAL("failed to read db log file\n");
				return json.Get((_U32)0).AsBool();
			}
			bool ret = m_db->CommitTransaction();
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("CommitTransaction", json);
			return ret;
		}

		_U32 CProxyDBApi::CreateAvatar(_U32 user_id, _U32 avatar_scope, const char* avatar_name, const char* avatar_desc)
		{
			if(m_fp)
			{
				JsonValue json;
				if(!Read("CreateAvatar", json)) ZION_FATAL("failed to read db log file\n");
				return json.Get((_U32)0).AsU32();
			}
			_U32 ret = m_db->CreateAvatar(user_id, avatar_scope, avatar_name, avatar_desc);
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("CreateAvatar", json);
			return ret;
		}

		bool CProxyDBApi::DeleteAvatar(_U32 user_id, _U32 avatar_scope, _U32 avatar_id)
		{
			if(m_fp)
			{
				JsonValue json;
				if(!Read("DeleteAvatar", json)) ZION_FATAL("failed to read db log file\n");
				return json.Get((_U32)0).AsBool();
			}
			bool ret = m_db->DeleteAvatar(user_id, avatar_scope, avatar_id);
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
			if(m_fp)
			{
				JsonValue json;
				if(!Read("LoadAvatar", json)) ZION_FATAL("failed to read db log file\n");
				const JsonValue& objs = json.Get((_U32)0);
				for(_U32 i=0; i<objs.GetSize(); i++)
				{
					const JsonValue& obj = objs.Get(i);
					A_UUID _uuid;
					AUuidFromString(obj.Get((_U32)0).AsCSTR(), _uuid);
					callback(userptr, _uuid, obj.Get((_U32)1).AsCSTR(), obj.Get((_U32)2).AsCSTR());
				}
				return json.Get((_U32)1).AsBool();
			}
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
			if(m_fp)
			{
				JsonValue json;
				if(!Read("InsertAvatarObject", json)) ZION_FATAL("failed to read db log file\n");
				return json.Get((_U32)0).AsBool();
			}
			bool ret = m_db->InsertAvatarObject(avatar_id, _uuid, type, data);
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("InsertAvatarObject", json);
			return ret;
		}

		bool CProxyDBApi::UpdateAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* data)
		{
			if(m_fp)
			{
				JsonValue json;
				if(!Read("UpdateAvatarObject", json)) ZION_FATAL("failed to read db log file\n");
				return json.Get((_U32)0).AsBool();
			}
			bool ret = m_db->UpdateAvatarObject(avatar_id, _uuid, data);
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("UpdateAvatarObject", json);
			return ret;
		}

		bool CProxyDBApi::DeleteAvatarObject(_U32 avatar_id, const A_UUID* _uuids, _U32 count)
		{
			if(m_fp)
			{
				JsonValue json;
				if(!Read("DeleteAvatarObject", json)) ZION_FATAL("failed to read db log file\n");
				return json.Get((_U32)0).AsBool();
			}
			bool ret = m_db->DeleteAvatarObject(avatar_id, _uuids, count);
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("DeleteAvatarObject", json);
			return ret;
		}

		bool CProxyDBApi::QueryAvatarObject(_U32 avatar_id, const A_UUID& _uuid, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr)
		{
			if(m_fp)
			{
				JsonValue json;
				if(!Read("LoadAvatar", json)) ZION_FATAL("failed to read db log file\n");
				const JsonValue& obj = json.Get((_U32)0);
				A_UUID _uuid;
				AUuidFromString(obj.Get((_U32)0).AsCSTR(), _uuid);
				callback(userptr, _uuid, obj.Get((_U32)1).AsCSTR(), obj.Get((_U32)2).AsCSTR());
				return json.Get((_U32)1).AsBool();
			}
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
			if(m_fp)
			{
				JsonValue json;
				if(!Read("LockTask", json)) ZION_FATAL("failed to read db log file\n");
				return json.Get((_U32)0).AsBool();
			}
			bool ret = m_db->LockTask(avatar_id, task_id);
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("LockTask", json);
			return ret;
		}

		bool CProxyDBApi::DeleteTask(_U32 avatar_id, _U32 task_id)
		{
			if(m_fp)
			{
				JsonValue json;
				if(!Read("DeleteTask", json)) ZION_FATAL("failed to read db log file\n");
				return json.Get((_U32)0).AsBool();
			}
			bool ret = m_db->DeleteTask(avatar_id, task_id);
			JsonValue json(JsonValue::TYPE_ARRAY);
			json.Append(JsonValue(ret));
			WriteDBLog("DeleteTask", json);
			return ret;
		}

	}
}
