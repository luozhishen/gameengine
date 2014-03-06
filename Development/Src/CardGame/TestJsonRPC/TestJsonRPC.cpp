#include <uv.h>

void fib(uv_work_t *req) {
	int i = 0;
	i = 10;
}

void after_fib(uv_work_t *req, int status) {
	int i = 0;
	i = 10;
}

int main(int argc, char* argv[])
{
	uv_work_t req;
	uv_queue_work(uv_default_loop(), &req, fib, after_fib);
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	return 0;
}

/*
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

#if 0
	Zion::CJsonRPCClient* pClient = Zion::JsonRPC_GetClient("127.0.0.1:8000");
	printf("send data to xxxxxxx\n");
	JsonRPC_Send(pClient, "welcome", "\"welcome aaaaaaaaa\"");
	getchar();
#endif

	return 0;
}
*/
