#ifndef __ZION_DATA_CACHE__
#define __ZION_DATA_CACHE__

namespace Zion
{
	namespace DataCache
	{

		extern String	CONFIG_DATABASE;
		extern String	CONFIG_RPCEP;
		extern bool		CONFIG_SINGLETHREAD;

		int Main(int argc, char* argv[]);

	}
}

#endif
