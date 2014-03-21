#include <ZionBase.h>
#include <ZionCommon.h>
#include <JsonRPC.h>
#include "DataCacheServer.h"
#include "DataCacheJsonRpc.h"
#include "DataCacheRpcImpl.h"
#include "DataCacheDBApi.h"

#include <stdio.h>

namespace Zion
{
	namespace DataCache
	{

		String	CONFIG_DATABASE("sqlite:zion_db.sqlite");
		_U32	CONFIG_DATABASE_COUNT_MAX = 100;
		_U32	CONFIG_DATABASE_COUNT_INIT = 10;
		String	CONFIG_RPCEP("0.0.0.0:1980");
		bool CONFIG_SINGLETHREAD = true;

		static bool ParseArgs(int argc, char* argv[])
		{
			for(int i=1; i<argc; i++)
			{
				String name, value;
				char* pos = strchr(argv[i], '=');
				if(!pos) pos = argv[i] + strlen(argv[i]);
				char* pos1 = strrchr(argv[i], ':');
				if(pos1 && pos1<pos) pos = pos1;
				name.append(argv[i], pos-argv[i]);
				if(*pos!='\0') value = pos + 1;

				if(name=="-db")
				{
					CONFIG_DATABASE = value;
					continue;
				}
				if(name=="-db_conn_max" && !value.empty())
				{
					CONFIG_DATABASE_COUNT_MAX = (_U32)atoi(value.c_str());
					continue;
				}
				if(name=="-db_conn_init" && !value.empty())
				{
					CONFIG_DATABASE_COUNT_INIT = (_U32)atoi(value.c_str());
					continue;
				}
				if(name=="-rpcep")
				{
					CONFIG_RPCEP = value;
					continue;
				}
				if(name=="-singlethread")
				{
					CONFIG_SINGLETHREAD = true;
					if(value=="false") CONFIG_SINGLETHREAD = false;
					continue;
				}

				return false;
			}

			if(CONFIG_DATABASE_COUNT_INIT<1) CONFIG_DATABASE_COUNT_INIT = 1;
			if(CONFIG_DATABASE_COUNT_MAX<CONFIG_DATABASE_COUNT_INIT) CONFIG_DATABASE_COUNT_MAX = CONFIG_DATABASE_COUNT_INIT;
			if(CONFIG_SINGLETHREAD)
			{
				CONFIG_DATABASE_COUNT_INIT = 1;
				CONFIG_DATABASE_COUNT_MAX = 1;
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
				printf("		-db=[database string]\n");
				printf("		-rpcep=[127.0.0.1:1980]\n");
				return -1;
			}

			printf("CONFIG :\n");
			printf("	DATABASE = %s\n", CONFIG_DATABASE.c_str());
			printf("	DATABASE_COUNT_MAX = %u\n", CONFIG_DATABASE_COUNT_MAX);
			printf("	DATABASE_COUNT_INIT = %u\n", CONFIG_DATABASE_COUNT_INIT);
			printf("	RPCEP = %s\n", CONFIG_RPCEP.c_str());
			printf("	RPC_THREAD_COUNT = %s\n", CONFIG_SINGLETHREAD?"1":"MULTI");
			printf("\n");

			// step 2: init database
			printf("init database\n");
			if(!InitDatabase())
			{
				ZION_FATAL("FAILED InitDatabase");
			}

			// step 3: start rpc server
			CJsonRPCServer* pServer = JsonRPC_Create(CONFIG_SINGLETHREAD);
			JsonRPC_Bind(pServer, "echo",			JsonRPC_Echo);
			JsonRPC_Bind(pServer, "loginUser",		JsonRPC_LoginUser);
			JsonRPC_Bind(pServer, "createAvatar",	JsonRPC_CreateAvatar);
			JsonRPC_Bind(pServer, "deleteAvatar",	JsonRPC_DeleteAvatar);
			JsonRPC_Bind(pServer, "listAvatar",		JsonRPC_GetAvatarList);
			JsonRPC_Bind(pServer, "getAvatar",		JsonRPC_GetAvatar);
			JsonRPC_Bind(pServer, "saveAvatar",		JsonRPC_SaveAvatar);
			JsonRPC_Bind(pServer, "clearAvatar",	JsonRPC_ClearAvatar);
			JsonRPC_Bind(pServer, "keepAlive",		JsonRPC_KeepAlive);
			JsonRPC_Bind(pServer, "createObject",	JsonRPC_CreateObject);
			JsonRPC_Bind(pServer, "updateObject",	JsonRPC_UpdateObject);
			JsonRPC_Bind(pServer, "deleteObject",	JsonRPC_DeleteObject);
			printf("start JsonRpc Server...\n");
			if(!JsonRPC_Start(pServer, CONFIG_RPCEP.c_str()))
			{
				ZION_FATAL("start jsonrpc failed");
			}

			// step 4: wait process terminiate signal
			printf("server running...\n");
			getchar();

			// step 5: stop rpc server
			printf("stoping JsonRpc server\n");
			JsonRPC_Stop(pServer);
			JsonRPC_Destory(pServer);

			// step 6: wait all cache data flush to database
			printf("flash all data to database\n");
			RPCIMPL_FlushAllData();

			// step 7: close database
			printf("fini database\n");
			FiniDatabase();

			// step 8: process exit
			printf("exit.\n");
			return 0;
		}

	}
}

int main(int argc, char* argv[])
{
	return Zion::DataCache::Main(argc, argv);
}
