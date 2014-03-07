#ifndef __ZION_DATA_CACHE_DBAPI__
#define __ZION_DATA_CACHE_DBAPI__

namespace Zion
{
	namespace DataCache
	{

		class IDatabase
		{
		public:
			~IDatabase() { }

		};

		bool InitDatabase();
		void FiniDatabase();
		IDatabase* GetDatabase();

	}
}

#endif
