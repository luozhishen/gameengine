#include <ZionBase.h>
#include <ZionCommon.h>
#include <JsonRPC.h>
#include "DataCacheJsonRpc.h"
#include "DataCacheDBApi.h"
#include <stdio.h>

static char rpcep[1000] = "127.0.0.1:1980";

namespace Zion
{
	namespace DataCache
	{

		int Main(int argc, char* argv[])
		{
			// step 1: parse command line

			// step 2: load content

			// step 3: start rpc server
			Zion::CJsonRPCServer* pServer = Zion::JsonRPC_CreateServer();
			JsonRPC_Bind(pServer, "getAvatar", DataCache::JsonRPC_GetAvatar);
			JsonRPC_Bind(pServer, "keepAlive", DataCache::JsonRPC_KeepAlive);
			JsonRPC_Bind(pServer, "executeDDL", DataCache::JsonRPC_ExecuteDDL);
			JsonRPC_Bind(pServer, "setAvatarObject", DataCache::JsonRPC_SetAvatarObject);
			JsonRPC_Bind(pServer, "delAvatarObject", DataCache::JsonRPC_DelAvatarObject);

			if(!JsonRPC_Start(pServer, rpcep))
			{
				ZION_FATAL("start jsonrpc failed");
			}

			// step 4: wait process terminiate signal
			getchar();

			// step 5: stop rpc server
			JsonRPC_Stop(pServer);

			// step 6: wait all cache data flush to database

			// step 7: process exit
			return 0;
		}

	}
}

int main(int argc, char* argv[])
{
	return Zion::DataCache::Main(argc, argv);
}
