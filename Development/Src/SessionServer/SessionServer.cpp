#include <ZionBase.h>
#include <ZionCommon.h>
#include <JsonRPC.h>
#include "SessionServer.h"
#include "SessionJsonRpc.h"

#include <stdio.h>

namespace Zion
{
	namespace Session
	{

		String CONFIG_RPCEP("0.0.0.0:1980");
		bool CONFIG_SINGLETHREAD = false;

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
				printf("		-rpcep=[127.0.0.1:1980]\n");
				return -1;
			}

			printf("CONFIG :\n");
			printf("	RPCEP = %s\n", CONFIG_RPCEP.c_str());
			printf("\n");

			// step 2: start rpc server
			CJsonRPCServer* pServer = JsonRPC_Create(CONFIG_SINGLETHREAD);
			JsonRPC_Bind(pServer, "loginUser",			JsonRpc_LoginUser);
			JsonRPC_Bind(pServer, "logoutUser",			JsonRpc_LogoutUser);
			JsonRPC_Bind(pServer, "lockUser",			JsonRpc_LockUser);
			JsonRPC_Bind(pServer, "unlockUser",			JsonRpc_UnlockUser);
			JsonRPC_Bind(pServer, "bindAvatar",			JsonRpc_BindAvatar);
			JsonRPC_Bind(pServer, "unbindAvatar",		JsonRpc_UnbindAvatar);
			JsonRPC_Bind(pServer, "sendToUserID",		JsonRpc_SendToUserID);
			JsonRPC_Bind(pServer, "sendToAvatarID",		JsonRpc_SendToAvatarID);
			JsonRPC_Bind(pServer, "sendToAvatarName",	JsonRpc_SendToAvatarName);
			JsonRPC_Bind(pServer, "joinDomain",			JsonRpc_JoinDomain);
			JsonRPC_Bind(pServer, "leaveDomain",		JsonRpc_LeaveDomain);
			JsonRPC_Bind(pServer, "sendToDomain",		JsonRpc_SendToDomain);
			JsonRPC_Bind(pServer, "waitForMessage",		JsonRpc_WaitForMessage);
			JsonRPC_Bind(pServer, "GetMessage",			JsonRpc_GetMessage);
			printf("start JsonRpc Server...\n");
			if(!JsonRPC_Start(pServer, CONFIG_RPCEP.c_str()))
			{
				ZION_FATAL("start jsonrpc failed");
			}

			// step 3: wait process terminiate signal
			printf("server running...\n");
			getchar();

			// step 4: stop rpc server
			printf("stoping JsonRpc server\n");
			JsonRPC_Stop(pServer);
			JsonRPC_Destory(pServer);

			// step 5: process exit
			printf("exit.\n");
			return 0;
		}

	}
}

int main(int argc, char* argv[])
{
	return Zion::Session::Main(argc, argv);
}
