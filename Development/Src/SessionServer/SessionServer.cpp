#include <ZionBase.h>
#include <ZionCommon.h>
#include <JsonRPC.h>
#include <uv.h>

#include "SessionServer.h"
#include "SessionJsonRpc.h"

#include <stdio.h>

namespace Zion
{
	namespace Session
	{

		String CONFIG_RPCEP("0.0.0.0:1981");
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

		static uv_signal_t sig_exit;
		static void sig_exit_cb(uv_signal_t* handle, int signum)
		{
			uv_signal_stop(&sig_exit);
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
			JsonRPC_Bind("echo",				JsonRPC_Echo);
			JsonRPC_Bind("loginUser",			JsonRpc_LoginUser);
			JsonRPC_Bind("logoutUser",			JsonRpc_LogoutUser);
			JsonRPC_Bind("kickUser",			JsonRpc_KickUser);
			JsonRPC_Bind("lockSession",			JsonRpc_LockSession);
			JsonRPC_Bind("unlockSession",		JsonRpc_UnlockSession);
			JsonRPC_Bind("getSessionInfo",		JsonRpc_GetSessionInfo);
			JsonRPC_Bind("bindAvatar",			JsonRpc_BindAvatar);
			JsonRPC_Bind("unbindAvatar",		JsonRpc_UnbindAvatar);
			JsonRPC_Bind("sendToUserID",		JsonRpc_SendToUserID);
			JsonRPC_Bind("sendToAvatarID",		JsonRpc_SendToAvatarID);
			JsonRPC_Bind("sendToAvatarName",	JsonRpc_SendToAvatarName);
			JsonRPC_Bind("joinDomain",			JsonRpc_JoinDomain);
			JsonRPC_Bind("leaveDomain",			JsonRpc_LeaveDomain);
			JsonRPC_Bind("sendToDomain",		JsonRpc_SendToDomain);
			JsonRPC_Bind("domainMemberList",	JsonRpc_GetDomainMemberList);
			JsonRPC_Bind("domainMemberCount",	JsonRpc_GetDomainMemberCount);
			JsonRPC_Bind("waitForMessage",		JsonRpc_WaitForMessage);
			JsonRPC_Bind("GetMessage",			JsonRpc_GetMessage);
			printf("start JsonRpc Server...\n");
			if(!JsonRPC_Start(CONFIG_RPCEP.c_str(), CONFIG_SINGLETHREAD))
			{
				ZION_FATAL("start jsonrpc failed");
			}

			// step 3: wait process terminiate signal
			printf("server running...\n");
			uv_signal_init(uv_default_loop(), &sig_exit);
			uv_signal_start(&sig_exit, sig_exit_cb, SIGINT);
			uv_run(uv_default_loop(), UV_RUN_DEFAULT);

			// step 4: stop rpc server
			printf("stoping JsonRpc server\n");
			JsonRPC_Stop();

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
