#include <ZionBase.h>
#include "DataCacheDBApi.h"
#include "DataCacheDBApiMysql.h"

#include <mysql.h>

namespace Zion
{
	namespace DataCache
	{

		class CMysqlDBApi : public IDBApi
		{
		public:
			CMysqlDBApi(const char* host, unsigned int port, const char* username, const char* password, const char* db);
			virtual ~CMysqlDBApi();

			virtual _U32 LoginUser(const char* token);
			virtual _U32 CreateAvatar(_U32 user_id, _U32 server_id, const char* avatar_name, const char* avatar_desc);
			virtual bool DeleteAvatar(_U32 avatar_id);
			virtual bool GetAvatarList(_U32 user_id, _U32 server_id, bool (*callback)(void*, _U32, _U32, const char*, const char*), void* userptr);
			virtual bool LoadAvatar(_U32 avatar_id, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr);
			virtual bool InsertAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data);
			virtual bool UpdateAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* data);
			virtual bool DeleteAvatarObject(_U32 avatar_id, const A_UUID* _uuids, _U32 count);
			virtual bool QueryAvatarObject(_U32 avatar_id, const A_UUID& _uuid, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr);

		private:
			MYSQL		m_mysql_instance;
			MYSQL*		m_mysql;
			MYSQL_STMT* m_stmt_login;
			MYSQL_BIND	m_login_param[1];
			MYSQL_STMT* m_stmt_adduser;
			MYSQL_STMT* m_stmt_history;
			MYSQL_STMT* m_stmt_create;
			MYSQL_STMT* m_stmt_delete;
			MYSQL_STMT* m_stmt_list;
			MYSQL_STMT* m_stmt_check;
			MYSQL_STMT* m_stmt_load;
			MYSQL_STMT* m_stmt_insert;
			MYSQL_STMT* m_stmt_update;
			MYSQL_STMT* m_stmt_remove;
			MYSQL_STMT* m_stmt_query;
		};

		IDBApi* CreateMysqlDatabase(const char* host, unsigned int port, const char* username, const char* password, const char* db)
		{
			return ZION_NEW CMysqlDBApi(host, port, username, password, db);
		}

		CMysqlDBApi::CMysqlDBApi(const char* host, unsigned int port, const char* username, const char* password, const char* db)
		{
			m_stmt_login = NULL;
			m_stmt_adduser = NULL;
			m_stmt_history = NULL;
			m_stmt_create = NULL;
			m_stmt_delete = NULL;
			m_stmt_list = NULL;
			m_stmt_check = NULL;
			m_stmt_load = NULL;
			m_stmt_insert = NULL;
			m_stmt_update = NULL;
			m_stmt_remove = NULL;
			m_stmt_query = NULL;
			m_mysql = NULL;

			m_mysql = mysql_init(&m_mysql_instance);
			ZION_ASSERT(m_mysql);
			if(!m_mysql) return;

			m_mysql = mysql_real_connect(m_mysql, host, username, password, db, port, NULL, 0);
			ZION_ASSERT(m_mysql);
			if(!m_mysql) return;

			const char* sql = NULL;

			m_stmt_login = mysql_stmt_init(m_mysql);
			sql = "SELECT user_id, state, freeze_duetime FROM user_table WHERE token=?";
			if(0!=mysql_stmt_prepare(m_stmt_login, sql, (unsigned long)strlen(sql)) || mysql_stmt_param_count(m_stmt_login)!=sizeof(m_login_param)/sizeof(m_login_param[0]))
			{
				ZION_FATAL("error in mysql_stmt_prepare(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
			}
			memset(m_login_param, 0, sizeof(m_login_param));
			m_login_param[0].buffer_type = MYSQL_TYPE_STRING;
			/*
			m_login_param[0].buffer = (char *)str_data;
			m_login_param[0].buffer_length = STRING_SIZE;
			m_login_param[0].is_null = 0;
			m_login_param[0].length = &str_length;
			*/

			m_stmt_adduser = mysql_stmt_init(m_mysql);
			sql = "INSERT INTO user_table(token, state, freeze_duetime) VALUES(?, 0, 0)";
			if(0!=mysql_stmt_prepare(m_stmt_adduser, sql, (unsigned long)strlen(sql)))
			{
				ZION_FATAL("error in mysql_stmt_prepare(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
			}

			m_stmt_history = mysql_stmt_init(m_mysql);
			sql = "INSERT INTO login_history_table(user_id, ip, dv_type, os_type, dv_id, create_ts) VALUES(?, ?, ?, ?, ?, CURRENT_TIMESTAMP)";
			if(0!=mysql_stmt_prepare(m_stmt_history, sql, (unsigned long)strlen(sql)))
			{
				ZION_FATAL("error in mysql_stmt_prepare(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
			}

			m_stmt_create = mysql_stmt_init(m_mysql);
			sql = "INSERT INTO avatar_table(user_id, server_id, avatar_name, avatar_desc) VALUES(?, ?, ?, ?)";
			if(0!=mysql_stmt_prepare(m_stmt_create, sql, (unsigned long)strlen(sql)))
			{
				ZION_FATAL("error in mysql_stmt_prepare(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
			}

			m_stmt_delete = mysql_stmt_init(m_mysql);
			sql = "UPDATE avatar_table SET state = 1 WHERE avatar_id=?";
			if(0!=mysql_stmt_prepare(m_stmt_delete, sql, (unsigned long)strlen(sql)))
			{
				ZION_FATAL("error in mysql_stmt_prepare(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
			}

			m_stmt_list = mysql_stmt_init(m_mysql);
			sql = "SELECT avatar_id, state, freeze_duetime, avatar_name, avatar_desc FROM avatar_table WHERE user_id=? AND server_id=?";
			if(0!=mysql_stmt_prepare(m_stmt_list, sql, (unsigned long)strlen(sql)))
			{
				ZION_FATAL("error in mysql_stmt_prepare(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
			}

			m_stmt_check = mysql_stmt_init(m_mysql);
			sql = "SELECT state, freeze_duetime, avatar_name FROM avatar_table WHERE avatar_id=?";
			if(0!=mysql_stmt_prepare(m_stmt_check, sql, (unsigned long)strlen(sql)))
			{
				ZION_FATAL("error in mysql_stmt_prepare(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
			}

			m_stmt_load = mysql_stmt_init(m_mysql);
			sql = "SELECT object_uuid, object_type, object_data FROM avatar_object_table WHERE avatar_id=?";
			if(0!=mysql_stmt_prepare(m_stmt_load, sql, (unsigned long)strlen(sql)))
			{
				ZION_FATAL("error in mysql_stmt_prepare(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
			}

			m_stmt_insert = mysql_stmt_init(m_mysql);
			sql = "INSERT INTO avatar_object_table values(?, ?, ?, ?)";
			if(0!=mysql_stmt_prepare(m_stmt_insert, sql, (unsigned long)strlen(sql)))
			{
				ZION_FATAL("error in mysql_stmt_prepare(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
			}

			m_stmt_update = mysql_stmt_init(m_mysql);
			sql = "UPDATE avatar_object_table SET object_data=? WHERE avatar_id=? and object_uuid=?";
			if(0!=mysql_stmt_prepare(m_stmt_update, sql, (unsigned long)strlen(sql)))
			{
				ZION_FATAL("error in mysql_stmt_prepare(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
			}

			m_stmt_remove = mysql_stmt_init(m_mysql);
			sql = "DELETE FROM avatar_object_table WHERE avatar_id=? and object_uuid=?";
			if(0!=mysql_stmt_prepare(m_stmt_remove, sql, (unsigned long)strlen(sql)))
			{
				ZION_FATAL("error in mysql_stmt_prepare(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
			}

			m_stmt_query = mysql_stmt_init(m_mysql);
			sql = "SELECT object_type, object_data FROM avatar_object_table WHERE avatar_id=? and object_uuid=?";
			if(0!=mysql_stmt_prepare(m_stmt_query, sql, (unsigned long)strlen(sql)))
			{
				ZION_FATAL("error in mysql_stmt_prepare(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
			}
		}

		CMysqlDBApi::~CMysqlDBApi()
		{
			if(m_stmt_login)	mysql_stmt_close(m_stmt_login);
			if(m_stmt_adduser)	mysql_stmt_close(m_stmt_adduser);
			if(m_stmt_history)	mysql_stmt_close(m_stmt_history);
			if(m_stmt_create)	mysql_stmt_close(m_stmt_create);
			if(m_stmt_delete)	mysql_stmt_close(m_stmt_delete);
			if(m_stmt_list)		mysql_stmt_close(m_stmt_list);
			if(m_stmt_check)	mysql_stmt_close(m_stmt_check);
			if(m_stmt_load)		mysql_stmt_close(m_stmt_load);
			if(m_stmt_insert)	mysql_stmt_close(m_stmt_insert);
			if(m_stmt_update)	mysql_stmt_close(m_stmt_update);
			if(m_stmt_remove)	mysql_stmt_close(m_stmt_remove);
			if(m_stmt_query)	mysql_stmt_close(m_stmt_query);
			if(m_mysql)			mysql_close(m_mysql);

			m_stmt_login = NULL;
			m_stmt_adduser = NULL;
			m_stmt_history = NULL;
			m_stmt_create = NULL;
			m_stmt_delete = NULL;
			m_stmt_list = NULL;
			m_stmt_check = NULL;
			m_stmt_load = NULL;
			m_stmt_insert = NULL;
			m_stmt_update = NULL;
			m_stmt_remove = NULL;
			m_stmt_query = NULL;
			m_mysql = NULL;
		}

		_U32 CMysqlDBApi::LoginUser(const char* token)
		{
			if(mysql_stmt_reset(m_stmt_login)!=0)
			{
				printf("error in mysql_stmt_reset(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
				return (_U32)-1;
			}
			m_login_param[0].buffer = (char *)token;
			m_login_param[0].buffer_length = (unsigned long)strlen(token);
			if(mysql_stmt_bind_param(m_stmt_login, m_login_param)!=0)
			{
				printf("error in mysql_stmt_bind_param(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
				return (_U32)-1;
			}
			if(mysql_stmt_execute(m_stmt_login)!=0)
			{
				printf("error in mysql_stmt_execute(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
				return (_U32)-1;
			}
			while(mysql_stmt_fetch(m_stmt_login)==0)
			{
				MYSQL_BIND arg[3];
				memset(arg, 0, sizeof(arg));
				if(	mysql_stmt_fetch_column(m_stmt_login, arg, 0, 0)!=0 ||
					mysql_stmt_fetch_column(m_stmt_login, arg, 1, 1)!=0 ||
					mysql_stmt_fetch_column(m_stmt_login, arg, 2, 2)!=0)
				{
					printf("error in mysql_stmt_fetch_column(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
					return (_U32)-1;
				}
				int i = 10;
			}

			return (_U32)-1;
		}

		_U32 CMysqlDBApi::CreateAvatar(_U32 user_id, _U32 server_id, const char* avatar_name, const char* avatar_desc)
		{
			return (_U32)-1;
		}

		bool CMysqlDBApi::DeleteAvatar(_U32 avatar_id)
		{
			return false;
		}

		bool CMysqlDBApi::GetAvatarList(_U32 user_id, _U32 server_id, bool (*callback)(void*, _U32, _U32, const char*, const char*), void* userptr)
		{
			return false;
		}

		bool CMysqlDBApi::LoadAvatar(_U32 avatar_id, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr)
		{
			return false;
		}

		bool CMysqlDBApi::InsertAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data)
		{
			return false;
		}

		bool CMysqlDBApi::UpdateAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* data)
		{
			return false;
		}

		bool CMysqlDBApi::DeleteAvatarObject(_U32 avatar_id, const A_UUID* _uuids, _U32 count)
		{
			return false;
		}

		bool CMysqlDBApi::QueryAvatarObject(_U32 avatar_id, const A_UUID& _uuid, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr)
		{
			return false;
		}

	}
}
