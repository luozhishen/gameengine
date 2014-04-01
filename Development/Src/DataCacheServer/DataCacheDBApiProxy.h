#ifndef __ZION_DATA_CACHE_DBAPI_PROXY__
#define __ZION_DATA_CACHE_DBAPI_PROXY__

namespace Zion
{
	namespace DataCache
	{

		IDBApi* CreateProxyDatabase(IDBApi* db);
		IDBApi* CreateProxyDatabase(const char* filename);

	}
}

#endif
