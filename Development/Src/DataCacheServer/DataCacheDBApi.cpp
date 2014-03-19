#include <ZionBase.h>
#include "DataCacheDBApi.h"
#include "DataCacheDBApiSqlite.h"
#include "DataCacheDBApiMysql.h"
#include "DataCacheServer.h"

#include "sqlite\sqlite3.h"
#include <time.h>

namespace Zion
{
	namespace DataCache
	{

		static IDBApi* g_Sqlite = NULL;
		static A_MUTEX g_Mutex;

		bool InitDatabase()
		{
			g_Sqlite = CreateSqliteDatabase(CONFIG_DATABASE.c_str());
			A_MUTEX_INIT(&g_Mutex);
			return true;
		}

		void FiniDatabase()
		{
			ZION_DELETE g_Sqlite;
			A_MUTEX_DESTROY(&g_Mutex);
		}

		IDBApi* AllocDataBase()
		{
			A_MUTEX_LOCK(&g_Mutex);
			return g_Sqlite;
		}

		void FreeDatabase(IDBApi* db)
		{
			A_MUTEX_UNLOCK(&g_Mutex);
		}

	}
}
