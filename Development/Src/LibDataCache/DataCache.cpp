#include <ZionBase.h>
#include <ZionCommon.h>
#include <JsonRPC.h>
#include "DataCache.h"
#include "DataCacheJsonRpc.h"
#include "DataCacheServer.h"
#include "SimpleDataCacheServer.h"
#include "DataCacheDBApi.h"

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

		static bool ParseArgs(int argc, char* argv[])
		{
			for(int i=1; i<argc; i++)
			{
				String name, value;
				char* pos = strchr(argv[i], '=');
				if(!pos) pos = argv[i] + strlen(argv[i]);
				char* pos1 = strchr(argv[i], ':');
				if(!pos1 && pos1<pos) pos = pos1;
				name.copy(argv[i], pos1-argv[i]);
				if(*pos!='\0') value = pos + 1;

				if(name=="-simple" && value=="off")
				{
					CONFIG_SIMPLE_MODE = false;
					continue;
				}
				if(name=="-db")
				{
					CONFIG_DATABASE = value;
					continue;
				}
				if(name=="-rpcep")
				{
					CONFIG_RPCEP = value;
					continue;
				}

				return false;
			}
			return true;
		}

		int Main(int argc, char* argv[])
		{
			// step 1: parse command line
			if(!ParseArgs(argc, argv))
			{
				printf("invalid parameter\n");
				printf("\n");
				printf("Usage : %s\n", argv[0]);
				printf("		-simple=[on|off]\n");
				printf("		-db=[database string]\n");
				printf("		-rpcep=[127.0.0.1:1982]\n");
				return -1;
			}

			printf("CONFIG :\n");
			printf("	SIMPLE_MODE = %s\n", CONFIG_SIMPLE_MODE?"true":"false");
			printf("	DATABASE = %s\n", CONFIG_DATABASE.c_str());
			printf("	RPCEP = %s\n", CONFIG_RPCEP.c_str());
			printf("\n");

			// step 2: load content
			if(!CONFIG_SIMPLE_MODE)
			{
				Zion::InitDDLStub();
				Zion::InitContentObjects();
				Zion::InitLiveObjects();
				printf("loading content..\n");
				if(!ContentObject::LoadContent())
				{
					ZION_FATAL("load content failed");
				}
				printf("all content loaded.\n");
			}

			// step 3: init database
			printf("init database\n");
			if(!InitDatabase())
			{
				ZION_FATAL("");
			}

			// step 4: start rpc server
			JsonRPC_Bind("getAvatar",		JsonRPC_GetAvatar);
			JsonRPC_Bind("saveAvatar",		JsonRPC_SaveAvatar);
			JsonRPC_Bind("clearAvatar",		JsonRPC_ClearAvatar);
			JsonRPC_Bind("keepAlive",		JsonRPC_KeepAlive);
			JsonRPC_Bind("executeDDL",		JsonRPC_ExecuteDDL);
			JsonRPC_Bind("createObject",	JsonRPC_CreateObject);
			JsonRPC_Bind("updateObject",	JsonRPC_UpdateObject);
			JsonRPC_Bind("deleteObject",	JsonRPC_DeleteObject);
			printf("start JsonRpc Server...\n");
			if(!JsonRPC_Start(rpcep))
			{
				ZION_FATAL("start jsonrpc failed");
			}

			// step 5: wait process terminiate signal
			getchar();

			// step 6: stop rpc server
			printf("stoping JsonRpc server\n");
			JsonRPC_Stop();

			// step 7: wait all cache data flush to database
			printf("flash all data to database\n");
			if(CONFIG_SIMPLE_MODE)
			{
				RPCSIMPLE_FlushAllData();
			}
			else
			{
				RPCIMPL_FlushAllData();
			}

			// step 8: close database
			printf("fini database\n");
			FiniDatabase();

			// step 9: process exit
			printf("exit.\n");
			return 0;
		}

	}
}
