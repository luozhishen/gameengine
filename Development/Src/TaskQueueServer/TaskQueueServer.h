#ifndef __ZION_TASKQUEUE_SERVER__
#define __ZION_TASKQUEUE_SERVER__

namespace Zion
{
	namespace TaskQueue
	{

		extern String	CONFIG_RPCEP;
		extern bool		CONFIG_SINGLETHREAD;

		int Main(int argc, char* argv[]);

	}
}

#endif
