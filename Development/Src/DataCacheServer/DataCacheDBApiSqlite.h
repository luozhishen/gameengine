#ifndef __ZION_DATA_CACHE_DBAPI_SQLITE__
#define __ZION_DATA_CACHE_DBAPI_SQLITE__

namespace Zion
{
	namespace DataCache
	{

		IDBApi* CreateSqliteDatabase(const char* dbfile);

	}
}

#endif
