#include <ZionBase.h>
#include <ZionCommon.h>
#include <JsonRPC.h>
#include "DataCache.h"
#include "DataCacheJsonRpc.h"
#include "DataCacheServer.h"
#include "SimpleDataCacheServer.h"

#include <stdio.h>

static char rpcep[1000] = "127.0.0.1:1980";
static bool g_simple_server = true;

namespace Zion
{
	namespace DataCache
	{

		bool CONFIG_SIMPLE_MODE = true;
		String CONFIG_DATABASE("sqlite:zion_db.sqlite");
		String CONFIG_RPCEP("0.0.0.0:1982");

		int Main(int argc, char* argv[])
		{
			// step 1: parse command line

			// step 2: load content
			if(!CONFIG_SIMPLE_MODE)
			{
				Zion::InitDDLStub();
				Zion::InitContentObjects();
				Zion::InitLiveObjects();
				if(!ContentObject::LoadContent())
				{
					ZION_FATAL("load content failed");
				}
			}

			// step 3: start rpc server
			JsonRPC_Bind("getAvatar",		JsonRPC_GetAvatar);
			JsonRPC_Bind("keepAlive",		JsonRPC_KeepAlive);
			JsonRPC_Bind("saveToDB",		JsonRPC_SaveToDB);
			JsonRPC_Bind("executeDDL",		JsonRPC_ExecuteDDL);
			JsonRPC_Bind("createObject",	JsonRPC_CreateObject);
			JsonRPC_Bind("updateObject",	JsonRPC_UpdateObject);
			JsonRPC_Bind("deleteObject",	JsonRPC_DeleteObject);
			if(!JsonRPC_Start(rpcep))
			{
				ZION_FATAL("start jsonrpc failed");
			}

			// step 4: wait process terminiate signal
			getchar();

			// step 5: stop rpc server
			JsonRPC_Stop();

			// step 6: wait all cache data flush to database
			if(CONFIG_SIMPLE_MODE)
			{
				RPCSIMPLE_FlushAllData();
			}
			else
			{
				RPCIMPL_FlushAllData();
			}

			// step 7: process exit
			return 0;
		}

	}
}
