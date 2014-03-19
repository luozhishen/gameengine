#ifndef __ZION_DATA_CACHE_DBAPI_MYSQL__
#define __ZION_DATA_CACHE_DBAPI_MYSQL__

namespace Zion
{
	namespace DataCache
	{

		IDBApi* CreateMysqlDatabase(const char* host, unsigned int port, const char* username, const char* password, const char* db);

	}
}

#endif
