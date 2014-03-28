#include <ZionBase.h>
#include "DataCacheDBApi.h"
#include "DataCacheDBApiFake.h"
#include "DataCacheDBApiSqlite.h"
#include "DataCacheDBApiMysql.h"
#include "DataCacheServer.h"

#include <time.h>
#include <uv.h>

namespace Zion
{
	namespace DataCache
	{

		static List<IDBApi*> g_Conns;
		static uv_mutex_t g_ConnsMutex;
		static uv_cond_t g_ConnsCond;
		_U32 current_count = 0;
		static bool g_bFake = false;
		static bool g_bSqlite = false;
		static bool g_bMysql = false;
		static String g_SqliteFile;
		static String g_MysqlHost;
		static _U32 g_MysqlPort;
		static String g_MysqlUserName;
		static String g_MysqlPassword;
		static String g_MysqlDatabase;

		static IDBApi* NewDatabase()
		{
			if(g_bFake)
			{
				return CreateFakeDatabase();
			}

			if(g_bSqlite)
			{
				return CreateSqliteDatabase(g_SqliteFile.c_str());
			}

			if(g_bMysql)
			{
				return CreateMysqlDatabase(g_MysqlHost.c_str(), g_MysqlPort, g_MysqlUserName.c_str(), g_MysqlPassword.c_str(), g_MysqlDatabase.c_str());
			}

			return NULL;
		}

		bool InitDatabase()
		{
			if(CONFIG_DATABASE=="fake")
			{
				g_bFake = true;
			}
			else
			{
				size_t offset = CONFIG_DATABASE.find_first_of(':');
				if(offset<1) return false;
				String type = CONFIG_DATABASE.substr(0, offset);
				if(type=="sqlite")
				{
					g_bSqlite = true;
					g_SqliteFile = CONFIG_DATABASE.substr(offset+1);
				}
				else
				{
					Map<String, String> values;
					Array<String> pairs;
					StringSplit(CONFIG_DATABASE.substr(offset+1), ';', pairs);
					for(_U32 i=0; i<(_U32)pairs.size(); i++)
					{
						offset = pairs[i].find_first_of('=');
						if(offset<1) return false;
						values[pairs[i].substr(0, offset)] = pairs[i].substr(offset+1);
					}
					if(type=="mysql")
					{
						g_bMysql = true;
						if(values.find("host")==values.end()) return false;
						if(values.find("port")==values.end()) return false;
						if(values.find("username")==values.end()) return false;
						if(values.find("password")==values.end()) return false;
						if(values.find("database")==values.end()) return false;
						g_MysqlHost		= values["host"];
						g_MysqlPort		= (_U32)atoi(values["port"].c_str());
						g_MysqlUserName	= values["username"];
						g_MysqlPassword	= values["password"];
						g_MysqlDatabase	= values["database"];
					}
					else
					{
						return false;
					}
				}
			}

			for(_U32 i=0; i<CONFIG_DATABASE_COUNT_INIT; i++)
			{
				IDBApi* db = NewDatabase();
				if(!db->Connect())
				{
					ZION_DELETE db;
					return false;
				}
				g_Conns.push_back(db);
				current_count++;
			}

			uv_mutex_init(&g_ConnsMutex);
			uv_cond_init(&g_ConnsCond);

			return true;
		}

		void FiniDatabase()
		{
			while(!g_Conns.empty())
			{
				g_Conns.front()->Disconnect();
				ZION_DELETE g_Conns.front();
				g_Conns.pop_front();
			}
			uv_mutex_destroy(&g_ConnsMutex);
			uv_cond_destroy(&g_ConnsCond);
		}

		//uv_mutex_init(&g_ConnsMutex);
		//uv_cond_init(&g_ConnsCond);

		IDBApi* AllocDataBase()
		{
			IDBApi* db = NULL;
			uv_mutex_lock(&g_ConnsMutex);
			for(;;)
			{
				if(!g_Conns.empty())
				{
					db = g_Conns.front();
					g_Conns.pop_front();
					break;
				}

				if(current_count<CONFIG_DATABASE_COUNT_MAX)
				{
					current_count++;
					db = NewDatabase();
					break;
				}

				uv_cond_wait(&g_ConnsCond, &g_ConnsMutex);
			}
			uv_mutex_unlock(&g_ConnsMutex);

			if(!db->Check())
			{
				if(!db->Connect())
				{
					uv_mutex_lock(&g_ConnsMutex);
					g_Conns.push_back(db);
					uv_mutex_unlock(&g_ConnsMutex);
					return NULL;
				}
			}

			return db;
		}

		void FreeDatabase(IDBApi* db)
		{
			uv_mutex_lock(&g_ConnsMutex);
			g_Conns.push_back(db);
			uv_mutex_unlock(&g_ConnsMutex);
			uv_cond_signal(&g_ConnsCond);
		}

	}
}
