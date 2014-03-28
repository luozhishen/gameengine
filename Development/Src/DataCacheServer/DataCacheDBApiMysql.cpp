#include <ZionBase.h>
#include "DataCacheDBApi.h"
#include "DataCacheDBApiMysql.h"

#include <mysql.h>
#include <time.h>

#define MY(x)		( CMysqlString(m_mysql, x).data )

namespace Zion
{
	namespace DataCache
	{

		class CMysqlString
		{
		public:
			CMysqlString(MYSQL*	_mysql, const char* val);
			CMysqlString(MYSQL*	_mysql, const String& val);
			void Allocate(MYSQL* _mysql, const char* val);
			~CMysqlString();
			char* data;
		};

		CMysqlString::CMysqlString(MYSQL* mysql, const char* val)
		{
			data = NULL;
			Allocate(mysql, val);
		}

		CMysqlString::CMysqlString(MYSQL* mysql, const String& val)
		{
			data = NULL;
			Allocate(mysql, val.c_str());
		}

		void CMysqlString::Allocate(MYSQL* mysql, const char* val)
		{
			if(data) ZION_FREE(data);
			_U32 len = (_U32)strlen(val);
			data = (char*)ZION_ALLOC(len*2+1);;
			mysql_real_escape_string(mysql, data, val, (unsigned long)len);
		}

		CMysqlString::~CMysqlString()
		{
			if(data) ZION_FREE(data);
		}

		class CMysqlDBApi : public IDBApi
		{
		public:
			CMysqlDBApi(const char* host, unsigned int port, const char* username, const char* password, const char* db);
			virtual ~CMysqlDBApi();

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

		private:
			String	m_host;
			_U32	m_port;
			String	m_username;
			String	m_password;
			String	m_db;
			MYSQL	m_mysql_instance;
			MYSQL*	m_mysql;
		};

		IDBApi* CreateMysqlDatabase(const char* host, unsigned int port, const char* username, const char* password, const char* db)
		{
			return ZION_NEW CMysqlDBApi(host, port, username, password, db);
		}

		CMysqlDBApi::CMysqlDBApi(const char* host, unsigned int port, const char* username, const char* password, const char* db)
		{
			m_host		= host;
			m_port		= port;
			m_username	= username;
			m_password	= password;
			m_db		= db;
			m_mysql		= NULL;
		}

		CMysqlDBApi::~CMysqlDBApi()
		{
			if(m_mysql)
			{
				mysql_close(m_mysql);
				m_mysql = NULL;
			}
		}

		bool CMysqlDBApi::Check()
		{
			if(m_mysql==NULL) return false;
			if(mysql_ping(m_mysql)!=0) return false;
			return true;
		}

		bool CMysqlDBApi::Connect()
		{
			Disconnect();

			m_mysql = mysql_init(&m_mysql_instance);
			ZION_ASSERT(m_mysql);
			if(!m_mysql)
			{
				printf("error in mysql_init(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
				return false;
			}
			my_bool val = 1;
			if(mysql_options(m_mysql, MYSQL_OPT_RECONNECT, (char*)&val)!=0)
			{
				printf("error in mysql_init(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
				return false;
			}
			m_mysql = mysql_real_connect(m_mysql, m_host.c_str(), m_username.c_str(), m_password.c_str(), m_db.c_str(), m_port, NULL, 0);
			ZION_ASSERT(m_mysql);
			if(!m_mysql)
			{
				printf("error in mysql_real_connect(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
				return false;
			}
			return true;
		}

		bool CMysqlDBApi::Disconnect()
		{
			if(m_mysql)
			{
				mysql_close(m_mysql);
				m_mysql = NULL;
			}
			return true;
		}

		bool CMysqlDBApi::BeginTranscation()
		{
			return true;
		}

		bool CMysqlDBApi::RollbackTransaction()
		{
			return true;
		}

		bool CMysqlDBApi::CommitTransaction()
		{
			return true;
		}

		_U32 CMysqlDBApi::CreateAvatar(_U32 user_id, _U32 server_id, const char* avatar_name, const char* avatar_desc)
		{
			String sql = StringFormat("INSERT INTO avatar_table(user_id, server_id, avatar_name, avatar_desc) VALUES(%u, %u, '%s', '%s')",
				user_id, server_id, MY(avatar_name), MY(avatar_desc));
			if(mysql_real_query(m_mysql, sql.c_str(), (unsigned long)sql.size())!=0)
			{
				printf("error in mysql_real_query(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
				return (_U32)-1;
			}
			return (_U32)mysql_insert_id(m_mysql);
		}

		bool CMysqlDBApi::DeleteAvatar(_U32 avatar_id)
		{
			String sql = StringFormat("UPDATE avatar_table SET state = 1 WHERE avatar_id=%u and state=0", avatar_id);
			if(mysql_real_query(m_mysql, sql.c_str(), (unsigned long)sql.size())!=0)
			{
				printf("error in mysql_real_query(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
				return false;
			}
			else
			{
				return true;
			}
		}

		bool CMysqlDBApi::LoadAvatar(_U32 avatar_id, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr)
		{
			String sql = StringFormat("SELECT state, freeze_duetime FROM avatar_table WHERE avatar_id=%u AND state=0", avatar_id);
			if(mysql_real_query(m_mysql, sql.c_str(), (unsigned long)sql.size())!=0)
			{
				printf("error in mysql_real_query(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
				return false;
			}
			MYSQL_RES *result = mysql_store_result(m_mysql);
			if(!result)
			{
				printf("error in mysql_store_result(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
				return false;
			}
			ZION_ASSERT(mysql_num_fields(result)==2);
			MYSQL_ROW row;
			bool disable = true;
			while((row=mysql_fetch_row(result))!=NULL)
			{
				int state = (_U32)atoi(row[0]);
				time_t freeze = (time_t)atoi(row[1]);
				disable = (state!=0 || freeze>=time(NULL));
			}
			mysql_free_result(result);
			if(disable) return false;

			sql = StringFormat("SELECT object_uuid, object_type, object_data FROM avatar_object_table WHERE avatar_id=%u", avatar_id);
			if(mysql_real_query(m_mysql, sql.c_str(), (unsigned long)sql.size())!=0)
			{
				printf("error in mysql_real_query(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
				return false;
			}
			result = mysql_store_result(m_mysql);
			if(!result)
			{
				printf("error in mysql_store_result(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
				return false;
			}
			ZION_ASSERT(mysql_num_fields(result)==3);
			A_UUID uuid;
			while((row=mysql_fetch_row(result))!=NULL)
			{
				if(!AUuidFromString(row[0], uuid))
				{
					mysql_free_result(result);
					return false;
				}

				if(!callback(userptr, uuid, row[1], row[2]))
				{
					mysql_free_result(result);
					return false;
				}
			}
			mysql_free_result(result);
			return true;
		}

		bool CMysqlDBApi::InsertAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data)
		{
			char suuid[100];
			AUuidToString(_uuid, suuid);
			String sql = StringFormat("INSERT INTO avatar_object_table values(%u, '%s', '%s', '%s')", -1, suuid, MY(type), MY(data));
			if(mysql_real_query(m_mysql, sql.c_str(), (unsigned long)sql.size())!=0)
			{
				printf("error in mysql_real_query(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
				return false;
			}
			return true;
		}

		bool CMysqlDBApi::UpdateAvatarObject(_U32 avatar_id, const A_UUID& _uuid, const char* data)
		{
			char suuid[100];
			AUuidToString(_uuid, suuid);
			String sql = StringFormat("UPDATE avatar_object_table SET object_data='%s' WHERE avatar_id=%u and object_uuid='%s'", data, avatar_id, suuid);
			if(mysql_real_query(m_mysql, sql.c_str(), (unsigned long)sql.size())!=0)
			{
				printf("error in mysql_real_query(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
				return false;
			}
			return true;
		}

		bool CMysqlDBApi::DeleteAvatarObject(_U32 avatar_id, const A_UUID* _uuids, _U32 count)
		{
			char suuid[100];
			for(_U32 i=0; i<count; i++)
			{
				AUuidToString(_uuids[i], suuid);
				String sql = StringFormat("DELETE FROM avatar_object_table WHERE avatar_id=%u and object_uuid='%s'", avatar_id, suuid);
				if(mysql_real_query(m_mysql, sql.c_str(), (unsigned long)sql.size())!=0)
				{
					printf("error in mysql_real_query(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
					return false;
				}
			}
			return true;
		}

		bool CMysqlDBApi::QueryAvatarObject(_U32 avatar_id, const A_UUID& _uuid, bool (*callback)(void*, const A_UUID&, const char*, const char*), void* userptr)
		{
			char suuid[100];
			AUuidToString(_uuid, suuid);
			String sql = StringFormat("SELECT object_type, object_data FROM avatar_object_table WHERE avatar_id=%u and object_uuid='%s'", avatar_id, suuid);
			if(mysql_real_query(m_mysql, sql.c_str(), (unsigned long)sql.size())!=0)
			{
				printf("error in mysql_real_query(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
				return false;
			}
			MYSQL_RES *result = mysql_store_result(m_mysql);
			if(!result)
			{
				printf("error in mysql_store_result(%d), %s", mysql_errno(m_mysql), mysql_error(m_mysql));
				return false;
			}
			ZION_ASSERT(mysql_num_fields(result)==2);
			MYSQL_ROW row;
			int count = 0;
			while((row=mysql_fetch_row(result))!=NULL)
			{
				if(!callback(userptr, _uuid, row[0], row[1]))
				{
					mysql_free_result(result);
					return false;
				}
				count += 1;
			}
			mysql_free_result(result);
			return count==1;
		}

		bool CMysqlDBApi::LockTask(_U32 avatar_id, _U32 task_id)
		{
			return true;
		}

		bool CMysqlDBApi::MarkTask(_U32 avatar_id, _U32 task_id)
		{
			return true;
		}

	}
}
