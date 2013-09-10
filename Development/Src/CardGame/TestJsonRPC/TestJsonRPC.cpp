#include <ZionBase.h>
#include <JsonRPC.h>

void welcome_proc(const Zion::JSONRPC_RESPONSE& res, const char* args)
{
	Zion::JsonRPC_Send(res, args);
}

int main(int argc, char* argv[])
{

	Zion::CJsonRPCServer* pServer = Zion::JsonRPC_CreateServer();
	Zion::JsonRPC_Bind(pServer, "welcome", welcome_proc);
	Zion::JsonRPC_Bind(pServer, "back", welcome_proc);
	Zion::JsonRPC_Start(pServer, "127.0.0.1:8001");

	getchar();

	Zion::JsonRPC_Stop(pServer);
	Zion::JsonRPC_DestroyServer(pServer);

/*
	Zion::CJsonRPCClient* pClient = Zion::JsonRPC_GetClient("127.0.0.1:8000");
	printf("send data to xxxxxxx\n");
	JsonRPC_Send(pClient, "welcome", "\"welcome aaaaaaaaa\"");
	getchar();
*/

	return 0;
}