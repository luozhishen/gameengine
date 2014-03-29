#include <ZionBase.h>
#include "DataCacheDBApi.h"
#include "DataCacheDBApiSqlite.h"
#include "DataCacheServer.h"

#include "sqlite/sqlite3.h"
#include <time.h>

namespace Zion
{
	namespace DataCache
	{

		class CSqliteDBApi : public IDBApi
		{
		public:
			CSqliteDBApi(const char* dbfile);
			virtual ~CSqliteDBApi();

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

		private:
			String m_sqlite_path;

			sqlite3* m_sqlite;
			sqlite3_stmt* m_sqlite_create;
			int m_sqlite_create_user_id;
			int m_sqlite_create_server_id;
			int m_sqlite_create_avatar_name;
			int m_sqlite_create_avatar_desc;
			sqlite3_stmt* m_sqlite_delete;
			int m_sqlite_delete_avatar_id;
			sqlite3_stmt* m_sqlite_check;
			int m_sqlite_check_avatar_id;
			sqlite3_stmt* m_sqlite_load;
			int m_sqlite_load_avatar_id;
			sqlite3_stmt* m_sqlite_insert;
			int m_sqlite_insert_avatar_id;
			int m_sqlite_insert_object_uuid;
			int m_sqlite_insert_object_type;
			int m_sqlite_insert_object_data;
			sqlite3_stmt* m_sqlite_update;
			int m_sqlite_update_avatar_id;
			int m_sqlite_update_object_uuid;
			int m_sqlite_update_object_data;
			sqlite3_stmt* m_sqlite_remove;
			int m_sqlite_remove_avatar_id;
			int m_sqlite_remove_object_uuid;
			sqlite3_stmt* m_sqlite_query;
			int m_sqlite_query_avatar_id;
			int m_sqlite_query_object_uuid;
			sqlite3_stmt* m_sqlite_lock_task;
			int m_sqlite_lock_task_id;
			int m_sqlite_lock_avatar_id;
			sqlite3_stmt* m_sqlite_mark_task;
			int m_sqlite_mark_task_id;
		};

		static const char* sqls[] = {
			"CREATE TABLE user_table (\n"
			"	user_id			INTEGER PRIMARY KEY AUTOINCREMENT,\n"
			"	token			TEXT NOT NULL,\n"
			"	state			INTEGER NOT NULL,\n"
			"	freeze_duetime	INTEGER NOT NULL\n"
			")",

			"CREATE UNIQUE INDEX user_table_token_index ON user_table(token)",

			"CREATE TABLE login_history_table ("
			"	user_id			INTEGER NOT NULL,"
			"	ip				TEXT NOT NULL,"
			"	dv_type			TEXT NOT NULL,"
			"	os_type			TEXT NOT NULL,"
			"	dv_id			TEXT NOT NULL,"
			"	create_ts		INTEGER NOT NULL"
			")",

			"CREATE INDEX login_history_table_user_id_index ON login_history_table(user_id)",

			"CREATE TABLE avatar_table (\n"
			"	avatar_id	INTEGER PRIMARY KEY AUTOINCREMENT,\n"
			"	user_id		INTEGER,\n"
			"	server_id	INTEGER,\n"
			"	state		INTEGER DEFAULT 0,\n"
			"	freeze_duetime INTEGER DEFAULT 0,\n"
			"	avatar_name	TEXT,\n"
			"	avatar_desc	TEXT\n"
			")",

			"CREATE UNIQUE INDEX avatar_table_name_index ON avatar_table(avatar_name)",

			"CREATE INDEX avatar_table_user_index ON avatar_table(user_id, server_id)",

			"CREATE TABLE avatar_object_table (\n"
			"	avatar_id INTEGER,\n"
			"	object_uuid TEXT,\n"
			"	object_type TEXT,\n"
			"	object_data TEXT\n"
			")",

			"CREATE UNIQUE INDEX avatar_object_table_index ON avatar_object_table(avatar_id, object_uuid)",

			"CREATE TABLE task_table (\n"
			"	task_id			INTEGER PRIMARY KEY AUTOINCREMENT,\n"
			"	avatar_id		INTEGER,\n"
			"	state			INTEGER\n"
			")",

			NULL
		};

		IDBApi* CreateSqliteDatabase(const char* dbfile)
		{
			return ZION_NEW CSqliteDBApi(dbfile);
		}

		static int sqlite_callback(void* userptr, int argc, char** argv,char** name)
		{
			*((int*)userptr) = atoi(argv[0]);
			return SQLITE_OK;
		}

		CSqliteDBApi::CSqliteDBApi(const char* dbfile)
		{
			sqlite3_os_init();
			sqlite3_initialize();
			m_sqlite_path = dbfile;

			m_sqlite_create = NULL;
			m_sqlite_delete = NULL;
			m_sqlite_check = NULL;
			m_sqlite_load = NULL;
			m_sqlite_insert = NULL;
			m_sqlite_update = NULL;
			m_sqlite_remove = NULL;
			m_sqlite_query = NULL;
			m_sqlite_lock_task = NULL;
			m_sqlite_mark_task = NULL;

			m_sqlite = NULL;
		}

		CSqliteDBApi::~CSqliteDBApi()
		{
			Disconnect();

			sqlite3_shutdown();
			sqlite3_os_end();
		}

		bool CSqliteDBApi::Check()
		{
			if(!m_sqlite_create)	return false;
			if(!m_sqlite_delete)	return false;
			if(!m_sqlite_check)		return false;
			if(!m_sqlite_load)		return false;
			if(!m_sqlite_insert)	return false;
			if(!m_sqlite_update)	return false;
			if(!m_sqlite_remove)	return false;
			if(!m_sqlite_query)		return false;
			if(!m_sqlite_lock_task)	return false;
			if(!m_sqlite_mark_task)	return false;
			if(!m_sqlite)			return false;
			return true;
		}

		bool CSqliteDBApi::Connect()
		{
			Disconnect();

			if(SQLITE_OK!=sqlite3_open(m_sqlite_path.c_str(), &m_sqlite))
			{
				printf("error in sqlite3_open(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			int table_count = -1;
			if(SQLITE_OK!=sqlite3_exec(m_sqlite, "SELECT count(*) FROM sqlite_master WHERE type='table' AND (name='avatar_table' OR name='avatar_object_table' OR name='user_table' OR name='login_history_table' OR name='task_table')", sqlite_callback, &table_count, NULL))
			{
				printf("error in sqlite3_exec(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			if(table_count==0)
			{
				printf("table not found. create tables...\n");
				for(_U32 i=0; sqls[i]!=NULL; i++)
				{
					if(SQLITE_OK!=sqlite3_exec(m_sqlite, sqls[i], NULL, NULL, NULL))
					{
						printf("error in sqlite3_exec(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
						return false;
					}
				}
			}
			else if(table_count!=5)
			{
				printf("error");
				return false;
			}

			if(SQLITE_OK!=sqlite3_prepare(m_sqlite, "INSERT INTO avatar_table(user_id, server_id, avatar_name, avatar_desc) VALUES(:user_id, :server_id, :avatar_name, :avatar_desc)", -1, &m_sqlite_create, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			m_sqlite_create_user_id = sqlite3_bind_parameter_index(m_sqlite_create, ":user_id");
			m_sqlite_create_server_id = sqlite3_bind_parameter_index(m_sqlite_create, ":server_id");
			m_sqlite_create_avatar_name = sqlite3_bind_parameter_index(m_sqlite_create, ":avatar_name");
			m_sqlite_create_avatar_desc = sqlite3_bind_parameter_index(m_sqlite_create, ":avatar_desc");

			if(SQLITE_OK!=sqlite3_prepare(m_sqlite, "UPDATE avatar_table SET state = 1 WHERE avatar_id=:avatar_id and state=0", -1, &m_sqlite_delete, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			m_sqlite_delete_avatar_id = sqlite3_bind_parameter_index(m_sqlite_delete, ":avatar_id");

			if(SQLITE_OK!=sqlite3_prepare(m_sqlite, "SELECT state, freeze_duetime, avatar_name FROM avatar_table WHERE avatar_id=:avatar_id", -1, &m_sqlite_check, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			m_sqlite_check_avatar_id = sqlite3_bind_parameter_index(m_sqlite_check, ":avatar_id");

			if(SQLITE_OK!=sqlite3_prepare(m_sqlite, "SELECT object_uuid, object_type, object_data FROM avatar_object_table WHERE avatar_id=:avatar_id", -1, &m_sqlite_load, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			m_sqlite_load_avatar_id = sqlite3_bind_parameter_index(m_sqlite_load, ":avatar_id");

			if(SQLITE_OK!=sqlite3_prepare(m_sqlite, "INSERT INTO avatar_object_table values(:avatar_id, :object_uuid, :object_type, :object_data)", -1, &m_sqlite_insert, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			m_sqlite_insert_avatar_id = sqlite3_bind_parameter_index(m_sqlite_insert, ":avatar_id");
			m_sqlite_insert_object_uuid = sqlite3_bind_parameter_index(m_sqlite_insert, ":object_uuid");
			m_sqlite_insert_object_type = sqlite3_bind_parameter_index(m_sqlite_insert, ":object_type");
			m_sqlite_insert_object_data = sqlite3_bind_parameter_index(m_sqlite_insert, ":object_data");

			if(SQLITE_OK!=sqlite3_prepare(m_sqlite, "UPDATE avatar_object_table SET object_data=:object_data WHERE avatar_id=:avatar_id and object_uuid=:object_uuid", -1, &m_sqlite_update, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			m_sqlite_update_avatar_id = sqlite3_bind_parameter_index(m_sqlite_update, ":avatar_id");
			m_sqlite_update_object_uuid = sqlite3_bind_parameter_index(m_sqlite_update, ":object_uuid");
			m_sqlite_update_object_data = sqlite3_bind_parameter_index(m_sqlite_update, ":object_data");

			if(SQLITE_OK!=sqlite3_prepare(m_sqlite, "DELETE FROM avatar_object_table WHERE avatar_id=:avatar_id AND object_uuid=:object_uuid", -1, &m_sqlite_remove, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			m_sqlite_remove_avatar_id = sqlite3_bind_parameter_index(m_sqlite_remove, ":avatar_id");
			m_sqlite_remove_object_uuid = sqlite3_bind_parameter_index(m_sqlite_remove, ":object_uuid");

			if(SQLITE_OK!=sqlite3_prepare(m_sqlite, "SELECT object_type, object_data FROM avatar_object_table WHERE avatar_id=:avatar_id AND object_uuid=:object_uuid", -1, &m_sqlite_query, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			m_sqlite_query_avatar_id = sqlite3_bind_parameter_index(m_sqlite_remove, ":avatar_id");
			m_sqlite_query_object_uuid = sqlite3_bind_parameter_index(m_sqlite_remove, ":object_uuid");

			if(SQLITE_OK!=sqlite3_prepare(m_sqlite, "SELECT state FROM task_table WHERE task_id=:task_id AND avatar_id=:avatar_id", -1, &m_sqlite_lock_task, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			m_sqlite_lock_task_id = sqlite3_bind_parameter_index(m_sqlite_lock_task, ":task_id");
			m_sqlite_lock_avatar_id = sqlite3_bind_parameter_index(m_sqlite_lock_task, ":avatar_id");

			if(SQLITE_OK!=sqlite3_prepare(m_sqlite, "UPDATE task_table SET state=1 WHERE task_id=:task_id", -1, &m_sqlite_mark_task, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			m_sqlite_mark_task_id = sqlite3_bind_parameter_index(m_sqlite_mark_task, ":task_id");

			return true;
		}

		bool CSqliteDBApi::Disconnect()
		{
			if(m_sqlite_create)		{	sqlite3_finalize(m_sqlite_create);	m_sqlite_create = NULL;		}
			if(m_sqlite_delete)		{	sqlite3_finalize(m_sqlite_delete);	m_sqlite_delete = NULL;		}
			if(m_sqlite_check)		{	sqlite3_finalize(m_sqlite_check);	m_sqlite_check = NULL;		}
			if(m_sqlite_load)		{	sqlite3_finalize(m_sqlite_load);	m_sqlite_load = NULL;		}
			if(m_sqlite_insert)		{	sqlite3_finalize(m_sqlite_insert);	m_sqlite_insert = NULL;		}
			if(m_sqlite_update)		{	sqlite3_finalize(m_sqlite_update);	m_sqlite_update = NULL;		}
			if(m_sqlite_remove)		{	sqlite3_finalize(m_sqlite_remove);	m_sqlite_remove = NULL;		}
			if(m_sqlite_query)		{	sqlite3_finalize(m_sqlite_query);	m_sqlite_query = NULL;		}
			if(m_sqlite_lock_task)	{	sqlite3_finalize(m_sqlite_lock_task);	m_sqlite_lock_task = NULL;		}
			if(m_sqlite_mark_task)	{	sqlite3_finalize(m_sqlite_mark_task);	m_sqlite_mark_task = NULL;		}
			if(m_sqlite)			{	sqlite3_close(m_sqlite);			m_sqlite = NULL;			}
			return true;
		}

		bool CSqliteDBApi::BeginTranscation()
		{
			return sqlite3_exec(m_sqlite, "BEGIN TRANSACTION;", NULL, NULL, NULL)==SQLITE_OK;
		}

		bool CSqliteDBApi::RollbackTransaction()
		{
			return sqlite3_exec(m_sqlite, "COMMIT;", NULL, NULL, NULL)==SQLITE_OK;
		}

		bool CSqliteDBApi::CommitTransaction()
		{
			return sqlite3_exec(m_sqlite, "ROLLBACK;", NULL, NULL, NULL)==SQLITE_OK;
		}

		_U32 CSqliteDBApi::CreateAvatar(_U32 user_id, _U32 server_id, const char* avatar_name, const char* avatar_desc)
		{
			if(SQLITE_OK!=sqlite3_reset(m_sqlite_create))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return (_U32)-1;
			}

			if(SQLITE_OK!=sqlite3_bind_int(m_sqlite_create, m_sqlite_create_user_id, (int)user_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return (_U32)-1;
			}
			if(SQLITE_OK!=sqlite3_bind_int(m_sqlite_create, m_sqlite_create_server_id, (int)server_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return (_U32)-1;
			}
			if(SQLITE_OK!=sqlite3_bind_text(m_sqlite_create, m_sqlite_create_avatar_name, avatar_name, -1, NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return (_U32)-1;
			}
			if(SQLITE_OK!=sqlite3_bind_text(m_sqlite_create, m_sqlite_create_avatar_desc, avatar_desc, -1, NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return (_U32)-1;
			}

			int ret;
			ret = sqlite3_step(m_sqlite_create);
			if(ret!=SQLITE_OK && ret!=SQLITE_DONE)
			{
				printf("error in sqlite3_step(m_sqlite_create, %d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return (_U32)-1;
			}

			return (_U32)sqlite3_last_insert_rowid(m_sqlite);
		}

		bool CSqliteDBApi::DeleteAvatar(_U32 avatar_id)
		{
			if(SQLITE_OK!=sqlite3_reset(m_sqlite_delete))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			if(SQLITE_OK!=sqlite3_bind_int(m_sqlite_delete, m_sqlite_load_avatar_id, (int)avatar_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			int ret;
			ret = sqlite3_step(m_sqlite_delete);
			if(ret!=SQLITE_OK && ret!=SQLITE_DONE)
			{
				printf("error in sqlite3_step(m_sqlite_delete, %d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			return true;
		}

		bool CSqliteDBApi::LoadAvatar(_U32 avatar_id, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr)
		{
			if(SQLITE_OK!=sqlite3_reset(m_sqlite_check))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			if(SQLITE_OK!=sqlite3_bind_int(m_sqlite_check, m_sqlite_check_avatar_id, (int)avatar_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			bool flag = false;
			String avatar_name;
			while(1)
			{
				int ret = sqlite3_step(m_sqlite_check);
				if(ret==SQLITE_OK || ret==SQLITE_DONE) break;
				if(ret!=SQLITE_ROW)
				{
					printf("error in sqlite3_step(m_sqlite_check, %d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
					return false;
				}

				if(sqlite3_column_int(m_sqlite_check, 0)==0)
				{
					if((time_t)sqlite3_column_int(m_sqlite_check, 1)<time(NULL))
					{
						flag = true;
					}
				}
				avatar_name = (const char*)sqlite3_column_text(m_sqlite_check, 2);
			}
			if(!flag)
			{
				printf("avatar(%u) not found or disable", avatar_id);
				return false;
			}

			if(SQLITE_OK!=sqlite3_reset(m_sqlite_load))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			if(SQLITE_OK!=sqlite3_bind_int(m_sqlite_load, m_sqlite_load_avatar_id, (int)avatar_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			int ret;
			A_UUID uuid;
			while(1)
			{
				ret = sqlite3_step(m_sqlite_load);
				if(ret==SQLITE_OK || ret==SQLITE_DONE) break;
				if(ret!=SQLITE_ROW)
				{
					printf("error in sqlite3_step(m_sqlite_load, %d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
					return false;
				}

				if(!AUuidFromString((const char*)sqlite3_column_text(m_sqlite_load, 0), uuid))
				{
					printf("invalid uuid %s", (const char*)sqlite3_column_text(m_sqlite_load, 0));
					return false;
				}

				if(!callback(userptr, uuid, (const char*)sqlite3_column_text(m_sqlite_load, 1), (const char*)sqlite3_column_text(m_sqlite_load, 2)))
				{
					return false;
				}
			}

			return true;
		}

		bool CSqliteDBApi::InsertAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data)
		{
			char suuid[100];
			AUuidToString(_uuid, suuid);

			if(SQLITE_OK!=sqlite3_reset(m_sqlite_insert))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			if(SQLITE_OK!=sqlite3_bind_int(m_sqlite_insert, m_sqlite_insert_avatar_id, (int)avatar_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			if(SQLITE_OK!=sqlite3_bind_text(m_sqlite_insert, m_sqlite_insert_object_uuid, suuid, -1, NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			if(SQLITE_OK!=sqlite3_bind_text(m_sqlite_insert, m_sqlite_insert_object_type, type, -1, NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			if(SQLITE_OK!=sqlite3_bind_text(m_sqlite_insert, m_sqlite_insert_object_data, data, -1, NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			if(SQLITE_DONE!=sqlite3_step(m_sqlite_insert))
			{
				printf("error in sqlite3_step(m_sqlite_insert, %d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			return true;
		}

		bool CSqliteDBApi::UpdateAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* data)
		{
			char suuid[100];
			AUuidToString(_uuid, suuid);

			if(SQLITE_OK!=sqlite3_reset(m_sqlite_update))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			if(SQLITE_OK!=sqlite3_bind_int(m_sqlite_update, m_sqlite_update_avatar_id, (int)avatar_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			if(SQLITE_OK!=sqlite3_bind_text(m_sqlite_update, m_sqlite_update_object_uuid, suuid, -1, NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			if(SQLITE_OK!=sqlite3_bind_text(m_sqlite_update, m_sqlite_update_object_data, data, -1, NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			if(SQLITE_DONE!=sqlite3_step(m_sqlite_update))
			{
				printf("error in sqlite3_step(m_sqlite_update, %d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			return true;
		}

		bool CSqliteDBApi::DeleteAvatarObject(_U32 avatar_id, const A_UUID* _uuids, _U32 count)
		{
			char suuid[100];
			for(_U32 i=0; i<count; i++)
			{
				AUuidToString(_uuids[i], suuid);

				if(SQLITE_OK!=sqlite3_reset(m_sqlite_remove))
				{
					printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
					return false;
				}

				if(SQLITE_OK!=sqlite3_bind_int(m_sqlite_remove, m_sqlite_remove_avatar_id, (int)avatar_id))
				{
					printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
					return false;
				}
				if(SQLITE_OK!=sqlite3_bind_text(m_sqlite_remove, m_sqlite_remove_object_uuid, suuid, -1, NULL))
				{
					printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
					return false;
				}

				if(SQLITE_DONE!=sqlite3_step(m_sqlite_remove))
				{
					printf("error in sqlite3_step(m_sqlite_remove, %d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
					return false;
				}
			}
			return true;
		}

		bool CSqliteDBApi::QueryAvatarObject(_U32 avatar_id, const A_UUID& _uuid, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr)
		{
			char suuid[100];
			AUuidToString(_uuid, suuid);

			if(SQLITE_OK!=sqlite3_reset(m_sqlite_query))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			if(SQLITE_OK!=sqlite3_bind_int(m_sqlite_query, m_sqlite_query_avatar_id, (int)avatar_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			if(SQLITE_OK!=sqlite3_bind_text(m_sqlite_query, m_sqlite_query_object_uuid, suuid, -1, NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			int ret, count = 0;
			while(1)
			{
				ret = sqlite3_step(m_sqlite_query);
				if(ret==SQLITE_OK || ret==SQLITE_DONE) break;
				if(ret!=SQLITE_ROW)
				{
					printf("error in sqlite3_step(m_sqlite_query, %d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
					return false;
				}

				ZION_ASSERT(count==0);
				count++;

				if(!callback(userptr,
					_uuid,
					(const char*)sqlite3_column_text(m_sqlite_query, 0),
					(const char*)sqlite3_column_text(m_sqlite_query, 1)))
				{
					return false;
				}

			}
			return count==1;
		}

		bool CSqliteDBApi::LockTask(_U32 avatar_id, _U32 task_id)
		{
			if(SQLITE_OK!=sqlite3_reset(m_sqlite_lock_task))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			if(SQLITE_OK!=sqlite3_bind_int(m_sqlite_lock_task, m_sqlite_lock_task_id, (int)avatar_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			if(SQLITE_OK!=sqlite3_bind_int(m_sqlite_lock_task, m_sqlite_lock_avatar_id, (int)task_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			bool is_error = true;
			while(1)
			{
				int ret = sqlite3_step(m_sqlite_lock_task);
				if(ret==SQLITE_OK || ret==SQLITE_DONE) break;
				if(ret!=SQLITE_ROW)
				{
					printf("error in sqlite3_step(m_sqlite_check, %d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
					return false;
				}

				if(sqlite3_column_int(m_sqlite_lock_task, 0)==0)
				{
					is_error = false;
				}
			}

			return !is_error;
		}

		bool CSqliteDBApi::DeleteTask(_U32 avatar_id, _U32 task_id)
		{
			if(SQLITE_OK!=sqlite3_reset(m_sqlite_mark_task))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			if(SQLITE_OK!=sqlite3_bind_int(m_sqlite_mark_task, m_sqlite_mark_task_id, (int)task_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}

			int ret;
			ret = sqlite3_step(m_sqlite_mark_task);
			if(ret!=SQLITE_OK && ret!=SQLITE_DONE)
			{
				printf("error in sqlite3_step(m_sqlite_mark_task, %d), %s", sqlite3_errcode(m_sqlite), sqlite3_errmsg(m_sqlite));
				return false;
			}
			return true;
		}

	}
}
