#ifndef __ZION_DATA_CACHE__
#define __ZION_DATA_CACHE__

namespace Zion
{
	namespace DataCache
	{

		extern bool CONFIG_SIMPLE_MODE;
		extern String CONFIG_DATABASE;
		extern String CONFIG_RPCEP;

		int Main(int argc, char* argv[]);

	}
}

#endif
