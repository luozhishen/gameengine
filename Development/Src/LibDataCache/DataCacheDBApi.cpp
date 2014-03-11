#include <ZionBase.h>
#include "DataCacheDBApi.h"
#include "DataCache.h"

#include "sqlite\sqlite3.h"
#include <time.h>

namespace Zion
{
	namespace DataCache
	{

		static sqlite3* g_sqlite = NULL;
		static sqlite3_stmt* g_sqlite_login = NULL;
		static int g_sqlite_login_token = -1;
		static sqlite3_stmt* g_sqlite_adduser = NULL;
		static int g_sqlite_adduser_token = -1;
		static sqlite3_stmt* g_sqlite_history = NULL;
		static int g_sqlite_history_user_id = -1;
		static int g_sqlite_history_ip = -1;
		static int g_sqlite_history_dv_type = -1;
		static int g_sqlite_history_os_type = -1;
		static int g_sqlite_history_dv_id = -1;
		static sqlite3_stmt* g_sqlite_create = NULL;
		static int g_sqlite_create_user_id = -1;
		static int g_sqlite_create_server_id = -1;
		static int g_sqlite_create_avatar_name = -1;
		static int g_sqlite_create_avatar_desc = -1;
		static sqlite3_stmt* g_sqlite_delete = NULL;
		static int g_sqlite_delete_avatar_id = -1;
		static sqlite3_stmt* g_sqlite_list = NULL;
		static int g_sqlite_list_user_id = -1;
		static int g_sqlite_list_server_id = -1;
		static sqlite3_stmt* g_sqlite_check = NULL;
		static int g_sqlite_check_avatar_id = -1;
		static sqlite3_stmt* g_sqlite_load = NULL;
		static int g_sqlite_load_avatar_id = -1;
		static sqlite3_stmt* g_sqlite_insert = NULL;
		static int g_sqlite_insert_avatar_id = -1;
		static int g_sqlite_insert_object_uuid = -1;
		static int g_sqlite_insert_object_type = -1;
		static int g_sqlite_insert_object_data = -1;
		static sqlite3_stmt* g_sqlite_update = NULL;
		static int g_sqlite_update_avatar_id = -1;
		static int g_sqlite_update_object_uuid = -1;
		static int g_sqlite_update_object_data = -1;
		static sqlite3_stmt* g_sqlite_remove = NULL;
		static int g_sqlite_remove_avatar_id = -1;
		static int g_sqlite_remove_object_uuid = -1;

		static const char* sqls[] = {
			"CREATE TABLE user_table (\n"
			"	user_id			INTEGER PRIMARY KEY AUTOINCREMENT,\n"
			"	token			TEXT NOT NULL,\n"
			"	state			INTEGER NOT NULL,\n"
			"	freeze_duetime	INTEGER NOT NULL\n"
			")",

			"CREATE TABLE login_history_table (\n"
			"	seq_id			INTEGER PRIMARY KEY AUTOINCREMENT,\n"
			"	user_id			INTEGER NOT NULL,\n"
			"	ip				TEXT NOT NULL,\n"
			"	dv_type			TEXT NOT NULL,\n"
			"	os_type			TEXT NOT NULL,\n"
			"	dv_id			TEXT NOT NULL,\n"
			"	create_ts		INTEGER NOT NULL\n"
			")",

			"CREATE UNIQUE INDEX user_table_token_index ON user_table(token)",

			"CREATE INDEX login_history_table_user_id_index ON login_history_table(user_id)",

			"CREATE TABLE avatar_table (\n"
			"	avatar_id	INTEGER PRIMARY KEY AUTOINCREMENT,\n"
			"	user_id		INTEGER,\n"
			"	server_id	INTEGER,\n"
			"	flag		INTEGER,\n"
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
			NULL
		};

		static int sqlite_callback(void* userptr, int argc, char** argv,char** name)
		{
			*((int*)userptr) = atoi(argv[0]);
			return SQLITE_OK;
		}

		bool InitDatabase()
		{
			sqlite3_os_init();
			sqlite3_initialize();

			if(SQLITE_OK!=sqlite3_open(CONFIG_DATABASE.c_str(), &g_sqlite))
			{
				printf("error in sqlite3_open(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}

			int table_count = -1;
			if(SQLITE_OK!=sqlite3_exec(g_sqlite, "SELECT count(*) FROM sqlite_master WHERE type='table' AND (name='avatar_table' OR name='avatar_object_table' OR name='user_table' OR name='login_history_table')", sqlite_callback, &table_count, NULL))
			{
				printf("error in sqlite3_exec(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			if(table_count==0)
			{
				printf("table not found. create tables...\n");
				for(_U32 i=0; sqls[i]!=NULL; i++)
				{
					if(SQLITE_OK!=sqlite3_exec(g_sqlite, sqls[i], NULL, NULL, NULL))
					{
						printf("error in sqlite3_exec(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
						return false;
					}
				}
			}
			else if(table_count!=4)
			{
				printf("error");
				return false;
			}

			if(SQLITE_OK!=sqlite3_prepare(g_sqlite, "SELECT user_id, state, freeze_duetime FROM user_table WHERE token=:token", -1, &g_sqlite_login, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			g_sqlite_login_token = sqlite3_bind_parameter_index(g_sqlite_login, ":token");

			if(SQLITE_OK!=sqlite3_prepare(g_sqlite, "INSERT INTO user_table(token, state, freeze_duetime) VALUES(:token, 0, 0)", -1, &g_sqlite_adduser, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			g_sqlite_adduser_token = sqlite3_bind_parameter_index(g_sqlite_adduser, ":token");

			if(SQLITE_OK!=sqlite3_prepare(g_sqlite, "INSERT INTO login_history_table(user_id, ip, dv_type, os_type, dv_id, create_ts) VALUES(:user_id, :ip, :dv_type, :os_type, :dv_id, datetime('now', 'unixepoch'))", -1, &g_sqlite_history, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			g_sqlite_history_user_id = sqlite3_bind_parameter_index(g_sqlite_history, ":user_id");
			g_sqlite_history_ip = sqlite3_bind_parameter_index(g_sqlite_history, ":ip");
			g_sqlite_history_dv_type = sqlite3_bind_parameter_index(g_sqlite_history, ":dv_type");
			g_sqlite_history_os_type = sqlite3_bind_parameter_index(g_sqlite_history, ":os_type");
			g_sqlite_history_dv_id = sqlite3_bind_parameter_index(g_sqlite_history, ":dv_id");

			if(SQLITE_OK!=sqlite3_prepare(g_sqlite, "INSERT INTO avatar_table(user_id, server_id, flag, avatar_name, avatar_desc) VALUES(:user_id, :server_id, 0, :avatar_name, :avatar_desc)", -1, &g_sqlite_create, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			g_sqlite_create_user_id = sqlite3_bind_parameter_index(g_sqlite_create, ":user_id");
			g_sqlite_create_server_id = sqlite3_bind_parameter_index(g_sqlite_create, ":server_id");
			g_sqlite_create_avatar_name = sqlite3_bind_parameter_index(g_sqlite_create, ":avatar_name");
			g_sqlite_create_avatar_desc = sqlite3_bind_parameter_index(g_sqlite_create, ":avatar_desc");

			if(SQLITE_OK!=sqlite3_prepare(g_sqlite, "UPDATE avatar_table SET flag = 1 WHERE avatar_id=:avatar_id", -1, &g_sqlite_delete, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			g_sqlite_delete_avatar_id = sqlite3_bind_parameter_index(g_sqlite_delete, ":avatar_id");

			if(SQLITE_OK!=sqlite3_prepare(g_sqlite, "SELECT avatar_id, flag, avatar_name, avatar_desc FROM avatar_table WHERE user_id=:user_id AND server_id=:server_id", -1, &g_sqlite_list, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			g_sqlite_list_user_id = sqlite3_bind_parameter_index(g_sqlite_list, ":user_id");
			g_sqlite_list_server_id = sqlite3_bind_parameter_index(g_sqlite_list, ":server_id");

			if(SQLITE_OK!=sqlite3_prepare(g_sqlite, "SELECT flag, avatar_name FROM avatar_table WHERE avatar_id=:avatar_id", -1, &g_sqlite_check, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			g_sqlite_check_avatar_id = sqlite3_bind_parameter_index(g_sqlite_check, ":avatar_id");

			if(SQLITE_OK!=sqlite3_prepare(g_sqlite, "SELECT object_uuid, object_type, object_data FROM avatar_object_table WHERE avatar_id=:avatar_id", -1, &g_sqlite_load, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			g_sqlite_load_avatar_id = sqlite3_bind_parameter_index(g_sqlite_load, ":avatar_id");

			if(SQLITE_OK!=sqlite3_prepare(g_sqlite, "INSERT INTO avatar_object_table values(:avatar_id, :object_uuid, :object_type, :object_data)", -1, &g_sqlite_insert, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			g_sqlite_insert_avatar_id = sqlite3_bind_parameter_index(g_sqlite_insert, ":avatar_id");
			g_sqlite_insert_object_uuid = sqlite3_bind_parameter_index(g_sqlite_insert, ":object_uuid");
			g_sqlite_insert_object_type = sqlite3_bind_parameter_index(g_sqlite_insert, ":object_type");
			g_sqlite_insert_object_data = sqlite3_bind_parameter_index(g_sqlite_insert, ":object_data");

			if(SQLITE_OK!=sqlite3_prepare(g_sqlite, "UPDATE avatar_object_table SET object_data=:object_data WHERE avatar_id=:avatar_id and object_uuid=:object_uuid", -1, &g_sqlite_update, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			g_sqlite_update_avatar_id = sqlite3_bind_parameter_index(g_sqlite_update, ":avatar_id");
			g_sqlite_update_object_uuid = sqlite3_bind_parameter_index(g_sqlite_update, ":object_uuid");
			g_sqlite_update_object_data = sqlite3_bind_parameter_index(g_sqlite_update, ":object_data");

			if(SQLITE_OK!=sqlite3_prepare(g_sqlite, "DELETE FROM avatar_object_table WHERE avatar_id=:avatar_id and object_uuid=:object_uuid", -1, &g_sqlite_remove, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			g_sqlite_remove_avatar_id = sqlite3_bind_parameter_index(g_sqlite_remove, ":avatar_id");
			g_sqlite_remove_object_uuid = sqlite3_bind_parameter_index(g_sqlite_remove, ":object_uuid");

			return true;
		}

		void FiniDatabase()
		{
			sqlite3_finalize(g_sqlite_login);
			sqlite3_finalize(g_sqlite_adduser);
			sqlite3_finalize(g_sqlite_history);
			sqlite3_finalize(g_sqlite_create);
			sqlite3_finalize(g_sqlite_delete);
			sqlite3_finalize(g_sqlite_list);
			sqlite3_finalize(g_sqlite_check);
			sqlite3_finalize(g_sqlite_load);
			sqlite3_finalize(g_sqlite_insert);
			sqlite3_finalize(g_sqlite_update);
			sqlite3_finalize(g_sqlite_remove);
			sqlite3_close(g_sqlite);

			g_sqlite_login = NULL;
			g_sqlite_adduser = NULL;
			g_sqlite_history = NULL;
			g_sqlite_create = NULL;
			g_sqlite_delete = NULL;
			g_sqlite_list = NULL;
			g_sqlite_check = NULL;
			g_sqlite_load = NULL;
			g_sqlite_insert = NULL;
			g_sqlite_update = NULL;
			g_sqlite_delete = NULL;
			g_sqlite = NULL;

			sqlite3_shutdown();
			sqlite3_os_end();
		}

		_U32 LoginUser(const char* token)
		{
			if(SQLITE_OK!=sqlite3_reset(g_sqlite_login))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return (_U32)-1;
			}
			if(SQLITE_OK!=sqlite3_bind_text(g_sqlite_login, g_sqlite_login_token, token, -1, NULL))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return (_U32)-1;
			}
			_U32 user_id = (_U32)-1;
			bool disable = false;
			while(1)
			{
				int ret = sqlite3_step(g_sqlite_login);
				if(ret==SQLITE_OK || ret==SQLITE_DONE) break;
				if(ret!=SQLITE_ROW)
				{
					printf("error in sqlite3_step(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
					return false;
				}

				user_id = (_U32)sqlite3_column_int(g_sqlite_login, 0);
				int state = sqlite3_column_int(g_sqlite_list, 1);
				time_t freeze = (time_t)sqlite3_column_int(g_sqlite_list, 2);
				disable = (state!=0 || freeze>=time(NULL));
			}

			if(user_id==(_U32)-1)
			{
				if(SQLITE_OK!=sqlite3_reset(g_sqlite_adduser))
				{
					printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
					return (_U32)-1;
				}
				if(SQLITE_OK!=sqlite3_bind_text(g_sqlite_adduser, g_sqlite_adduser_token, token, -1, NULL))
				{
					printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
					return (_U32)-1;
				}
				int ret = sqlite3_step(g_sqlite_adduser);
				if(ret!=SQLITE_OK && ret!=SQLITE_DONE)
				{
					printf("error in sqlite3_step(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
					return false;
				}
				user_id = (_U32)sqlite3_last_insert_rowid(g_sqlite);
			}
			else if(disable)
			{
				return (_U32)-1;
			}

			if(SQLITE_OK!=sqlite3_reset(g_sqlite_history))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return (_U32)-1;
			}
			if(SQLITE_OK!=sqlite3_bind_int(g_sqlite_history, g_sqlite_history_user_id, (int)user_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return (_U32)-1;
			}
			if(SQLITE_OK!=sqlite3_bind_text(g_sqlite_history, g_sqlite_history_ip, "ip", -1, NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return (_U32)-1;
			}
			if(SQLITE_OK!=sqlite3_bind_text(g_sqlite_history, g_sqlite_history_dv_type, "dv_type", -1, NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return (_U32)-1;
			}
			if(SQLITE_OK!=sqlite3_bind_text(g_sqlite_history, g_sqlite_history_os_type, "os_type", -1, NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return (_U32)-1;
			}
			if(SQLITE_OK!=sqlite3_bind_text(g_sqlite_history, g_sqlite_history_dv_id, "dv_id", -1, NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return (_U32)-1;
			}
			int ret;
			ret = sqlite3_step(g_sqlite_history);
			if(ret!=SQLITE_OK && ret!=SQLITE_DONE)
			{
				printf("error in sqlite3_step(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return (_U32)-1;
			}

			return user_id;
		}

		_U32 CreateAvatar(_U32 user_id, _U32 server_id, const char* avatar_name, const char* avatar_desc)
		{
			if(SQLITE_OK!=sqlite3_reset(g_sqlite_create))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return (_U32)-1;
			}

			if(SQLITE_OK!=sqlite3_bind_int(g_sqlite_create, g_sqlite_create_user_id, (int)user_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return (_U32)-1;
			}
			if(SQLITE_OK!=sqlite3_bind_int(g_sqlite_create, g_sqlite_create_server_id, (int)server_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return (_U32)-1;
			}
			if(SQLITE_OK!=sqlite3_bind_text(g_sqlite_create, g_sqlite_create_avatar_name, avatar_name, -1, NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return (_U32)-1;
			}
			if(SQLITE_OK!=sqlite3_bind_text(g_sqlite_create, g_sqlite_create_avatar_desc, avatar_desc, -1, NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return (_U32)-1;
			}

			int ret;
			ret = sqlite3_step(g_sqlite_create);
			if(ret!=SQLITE_OK && ret!=SQLITE_DONE)
			{
				printf("error in sqlite3_step(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return (_U32)-1;
			}

			return (_U32)sqlite3_last_insert_rowid(g_sqlite);
		}

		bool DeleteAvatar(_U32 avatar_id)
		{
			if(SQLITE_OK!=sqlite3_reset(g_sqlite_delete))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}

			if(SQLITE_OK!=sqlite3_bind_int(g_sqlite_delete, g_sqlite_load_avatar_id, (int)avatar_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}

			int ret;
			ret = sqlite3_step(g_sqlite_delete);
			if(ret!=SQLITE_OK && ret!=SQLITE_DONE)
			{
				printf("error in sqlite3_step(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			return true;
		}

		bool GetAvatarList(_U32 user_id, _U32 server_id, bool (*callback)(void*, _U32, _U32, const char*, const char*), void* userptr)
		{
			if(SQLITE_OK!=sqlite3_reset(g_sqlite_list))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}

			if(SQLITE_OK!=sqlite3_bind_int(g_sqlite_list, g_sqlite_list_user_id, (int)user_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			if(SQLITE_OK!=sqlite3_bind_int(g_sqlite_list, g_sqlite_list_server_id, (int)server_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}

			int ret;
			while(1)
			{
				ret = sqlite3_step(g_sqlite_list);
				if(ret==SQLITE_OK || ret==SQLITE_DONE) break;
				if(ret!=SQLITE_ROW)
				{
					printf("error in sqlite3_step(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
					return false;
				}

				if(!callback(userptr,
					(_U32)sqlite3_column_int(g_sqlite_list, 0),
					(_U32)sqlite3_column_int(g_sqlite_list, 1),
					(const char*)sqlite3_column_text(g_sqlite_list, 3),
					(const char*)sqlite3_column_text(g_sqlite_list, 4)))
				{
					return false;
				}
			}
			return true;
		}

		bool LoadAvatar(_U32 avatar_id, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr)
		{
			if(SQLITE_OK!=sqlite3_reset(g_sqlite_check))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}

			if(SQLITE_OK!=sqlite3_bind_int(g_sqlite_check, g_sqlite_check_avatar_id, (int)avatar_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}

			bool flag = false;
			String avatar_name;
			while(1)
			{
				int ret = sqlite3_step(g_sqlite_check);
				if(ret==SQLITE_OK || ret==SQLITE_DONE) break;
				if(ret!=SQLITE_ROW)
				{
					printf("error in sqlite3_step(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
					return false;
				}

				if(sqlite3_column_int(g_sqlite_check, 0)==0)
				{
					flag = true;
				}
				avatar_name = (const char*)sqlite3_column_text(g_sqlite_check, 1);
			}
			if(!flag)
			{
				printf("avatar(%u) not found or disable", avatar_id);
				return false;
			}

			if(SQLITE_OK!=sqlite3_reset(g_sqlite_load))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}

			if(SQLITE_OK!=sqlite3_bind_int(g_sqlite_load, g_sqlite_load_avatar_id, (int)avatar_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}

			int ret;
			A_UUID uuid;
			while(1)
			{
				ret = sqlite3_step(g_sqlite_load);
				if(ret==SQLITE_OK || ret==SQLITE_DONE) break;
				if(ret!=SQLITE_ROW)
				{
					printf("error in sqlite3_step(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
					return false;
				}

				if(!AUuidFromString((const char*)sqlite3_column_text(g_sqlite_load, 0), uuid))
				{
					printf("invalid uuid %d", (const char*)sqlite3_column_text(g_sqlite_load, 0));
					return false;
				}

				if(!callback(userptr, uuid, (const char*)sqlite3_column_text(g_sqlite_load, 1), (const char*)sqlite3_column_text(g_sqlite_load, 2)))
				{
					return false;
				}
			}

			return true;
		}

		bool InsertAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data)
		{
			char suuid[100];
			AUuidToString(_uuid, suuid);

			if(SQLITE_OK!=sqlite3_reset(g_sqlite_insert))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}

			if(SQLITE_OK!=sqlite3_bind_int(g_sqlite_insert, g_sqlite_insert_avatar_id, (int)avatar_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			if(SQLITE_OK!=sqlite3_bind_text(g_sqlite_insert, g_sqlite_insert_object_uuid, suuid, (int)strlen(suuid), NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			if(SQLITE_OK!=sqlite3_bind_text(g_sqlite_insert, g_sqlite_insert_object_type, type, (int)strlen(type), NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			if(SQLITE_OK!=sqlite3_bind_text(g_sqlite_insert, g_sqlite_insert_object_data, data, (int)strlen(data), NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}

			if(SQLITE_DONE!=sqlite3_step(g_sqlite_insert))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}

			return true;
		}

		bool UpdateAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* data)
		{
			char suuid[100];
			AUuidToString(_uuid, suuid);

			if(SQLITE_OK!=sqlite3_reset(g_sqlite_update))
			{
				printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}

			if(SQLITE_OK!=sqlite3_bind_int(g_sqlite_update, g_sqlite_update_avatar_id, (int)avatar_id))
			{
				printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			if(SQLITE_OK!=sqlite3_bind_text(g_sqlite_update, g_sqlite_update_object_uuid, suuid, (int)strlen(suuid), NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			if(SQLITE_OK!=sqlite3_bind_text(g_sqlite_update, g_sqlite_update_object_data, data, (int)strlen(data), NULL))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}

			if(SQLITE_DONE!=sqlite3_step(g_sqlite_update))
			{
				printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}

			return true;
		}

		bool DeleteAvatarObject(_U32 avatar_id, const A_UUID* _uuids, _U32 count)
		{
			char suuid[100];
			for(_U32 i=0; i<count; i++)
			{
				AUuidToString(_uuids[i], suuid);

				if(SQLITE_OK!=sqlite3_reset(g_sqlite_remove))
				{
					printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
					return false;
				}

				if(SQLITE_OK!=sqlite3_bind_int(g_sqlite_remove, g_sqlite_remove_avatar_id, (int)avatar_id))
				{
					printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
					return false;
				}
				if(SQLITE_OK!=sqlite3_bind_text(g_sqlite_remove, g_sqlite_remove_object_uuid, suuid, (int)strlen(suuid), NULL))
				{
					printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
					return false;
				}

				if(SQLITE_DONE!=sqlite3_step(g_sqlite_remove))
				{
					printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
					return false;
				}
			}
			return true;
		}

	}
}
