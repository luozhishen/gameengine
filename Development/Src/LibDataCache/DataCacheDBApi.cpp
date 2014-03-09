#include <ZionBase.h>
#include "DataCacheDBApi.h"
#include "DataCache.h"

#include "sqlite\sqlite3.h"

namespace Zion
{
	namespace DataCache
	{

		static sqlite3* g_sqlite = NULL;
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
		static sqlite3_stmt* g_sqlite_delete = NULL;
		static int g_sqlite_delete_avatar_id = -1;
		static int g_sqlite_delete_object_uuid = -1;

		static const char* sqls[] = {
			"-- 角色表\n"
			"CREATE TABLE avatar_table (\n"
			"	avatar_id integer AUTOINCERMENT,	-- COMMENT '角色ID',\n"
			"	user_id integer,					-- COMMENT '用户ID',\n"
			"	server_id integer,					-- COMMENT '角色所在服务器ID',\n"
			"	avatar_name text,					-- COMMENT '角色名称',\n"
			"	avatar_desc text					-- COMMENT '角色基本信息'\n"
			")",
			"CREATE UNIQUE INDEX avatar_table_id_index ON avatar_table(avatar_id)",
			"CREATE UNIQUE INDEX avatar_table_name_index ON avatar_table(avatar_name)",
			"CREATE INDEX avatar_table_user_index ON avatar_table(user_id)",
			"CREATE INDEX avatar_table_server_index ON avatar_table(server_id)",
			"-- 角色对象表\n",
			"CREATE TABLE avatar_object_table (\n"
			"	avatar_id integer,				-- COMMENT '角色ID',\n"
			"	object_uuid text,					-- COMMENT '对象UUID;,\n"
			"	object_type text,					-- COMMENT '对象类型', \n"
			"	object_data text					-- COMMENT '对象数据'\n"
			")\n",
			"CREATE UNIQUE INDEX avatar_object_table_index ON avatar_object_table(avatar_id, object_uuid)\n",
			NULL
		};

		static int sqlite_callback(void* userptr, int argc, char** argv,char** name)
		{
			*((int*)userptr) = atoi(argv[0]);
			return SQLITE_OK;
		}

		bool InitDatabase()
		{
			if(SQLITE_OK!=sqlite3_open(CONFIG_DATABASE.c_str(), &g_sqlite))
			{
				printf("error in sqlite3_open(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}

			int table_count = -1;
			if(SQLITE_OK!=sqlite3_exec(g_sqlite, "SELECT count(*) FROM sqlite_master WHERE type='table' AND (name='avatar_table' OR name='avatar_object_table')", sqlite_callback, &table_count, NULL))
			{
				printf("error in sqlite3_exec(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			if(table_count==0)
			{
				for(_U32 i=0; sqls[i]!=NULL; i++)
				{
					if(SQLITE_OK!=sqlite3_exec(g_sqlite, sqls[i], NULL, NULL, NULL))
					{
						printf("error in sqlite3_exec(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
						return false;
					}
				}
			}
			else if(table_count!=2)
			{
				printf("error");
				return false;
			}

			if(SQLITE_OK!=sqlite3_prepare(g_sqlite, "SELECT object_uuid, object_type, object_data FROM avatar_object_table where avatar_id=:avatar_id", -1, &g_sqlite_load, NULL))
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

			if(SQLITE_OK!=sqlite3_prepare(g_sqlite, "DELETE FROM avatar_object_table WHERE avatar_id=:avatar_id and object_uuid=:object_uuid", -1, &g_sqlite_delete, NULL))
			{
				printf("error in sqlite3_prepare(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
				return false;
			}
			g_sqlite_delete_avatar_id = sqlite3_bind_parameter_index(g_sqlite_delete, ":avatar_id");
			g_sqlite_delete_object_uuid = sqlite3_bind_parameter_index(g_sqlite_delete, ":object_uuid");

			return true;
		}

		void FiniDatabase()
		{
			sqlite3_finalize(g_sqlite_load);
			sqlite3_finalize(g_sqlite_insert);
			sqlite3_finalize(g_sqlite_update);
			sqlite3_finalize(g_sqlite_delete);
			sqlite3_close(g_sqlite);
			g_sqlite_load = NULL;
			g_sqlite_insert = NULL;
			g_sqlite_update = NULL;
			g_sqlite_delete = NULL;
			g_sqlite = NULL;
		}

		bool LoadAvatar(_U32 avatar_id, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr)
		{
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

			if(SQLITE_DONE!=sqlite3_step(g_sqlite_insert))
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

				if(SQLITE_OK!=sqlite3_reset(g_sqlite_delete))
				{
					printf("error in sqlite3_reset(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
					return false;
				}

				if(SQLITE_OK!=sqlite3_bind_int(g_sqlite_delete, g_sqlite_delete_avatar_id, (int)avatar_id))
				{
					printf("error in sqlite3_bind_int(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
					return false;
				}
				if(SQLITE_OK!=sqlite3_bind_text(g_sqlite_delete, g_sqlite_delete_object_uuid, suuid, (int)strlen(suuid), NULL))
				{
					printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
					return false;
				}

				if(SQLITE_DONE!=sqlite3_step(g_sqlite_insert))
				{
					printf("error in sqlite3_bind_text(%d), %s", sqlite3_errcode(g_sqlite), sqlite3_errmsg(g_sqlite));
					return false;
				}
			}
			return true;
		}

	}
}
