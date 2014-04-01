#ifndef __ZION_DATA_CACHE__
#define __ZION_DATA_CACHE__

namespace Zion
{
	namespace DataCache
	{

		extern String	CONFIG_DATABASE;
		extern _U32		CONFIG_DATABASE_COUNT_MAX;
		extern _U32		CONFIG_DATABASE_COUNT_INIT;
		extern String	CONFIG_RPCEP;
		extern bool		CONFIG_SINGLETHREAD;
		extern bool		CONFIG_ENABLE_REPLAYLOG;

		int Main(int argc, char* argv[]);
		void WriteRPCLog(const char* method, const JsonValue& args);
		void WriteDBLog(const char* method, const JsonValue& args);

	}
}

#endif
