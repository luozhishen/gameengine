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

		String CONFIG_DATABASE("zion_db.sqlite");
		String CONFIG_RPCEP("0.0.0.0:1980");

		static bool ParseArgs(int argc, char* argv[])
		{
			for(int i=1; i<argc; i++)
			{
				String name, value;
				char* pos = strchr(argv[i], '=');
				if(!pos) pos = argv[i] + strlen(argv[i]);
				char* pos1 = strrchr(argv[i], ':');
				if(!pos1 && pos1<pos) pos = pos1;
				name.copy(argv[i], pos1-argv[i]);
				if(*pos!='\0') value = pos + 1;

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
				printf("		-db=[database string]\n");
				printf("		-rpcep=[127.0.0.1:1980]\n");
				return -1;
			}

			printf("CONFIG :\n");
			printf("	DATABASE = %s\n", CONFIG_DATABASE.c_str());
			printf("	RPCEP = %s\n", CONFIG_RPCEP.c_str());
			printf("\n");

			// step 2: init database
			printf("init database\n");
			if(!InitDatabase())
			{
				ZION_FATAL("");
			}

			// step 3: start rpc server
			JsonRPC_Bind("loginUser",		JsonRPC_LoginUser);
			JsonRPC_Bind("createAvatar",	JsonRPC_CreateAvatar);
			JsonRPC_Bind("deleteAvatar",	JsonRPC_DeleteAvatar);
			JsonRPC_Bind("listAvatar",		JsonRPC_GetAvatarList);
			JsonRPC_Bind("getAvatar",		JsonRPC_GetAvatar);
			JsonRPC_Bind("saveAvatar",		JsonRPC_SaveAvatar);
			JsonRPC_Bind("clearAvatar",		JsonRPC_ClearAvatar);
			JsonRPC_Bind("keepAlive",		JsonRPC_KeepAlive);
			JsonRPC_Bind("createObject",	JsonRPC_CreateObject);
			JsonRPC_Bind("updateObject",	JsonRPC_UpdateObject);
			JsonRPC_Bind("deleteObject",	JsonRPC_DeleteObject);
			printf("start JsonRpc Server...\n");
			if(!JsonRPC_Start(CONFIG_RPCEP.c_str()))
			{
				ZION_FATAL("start jsonrpc failed");
			}

			// step 4: wait process terminiate signal
			printf("server running...\n");
			getchar();

			// step 5: stop rpc server
			printf("stoping JsonRpc server\n");
			JsonRPC_Stop();

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
