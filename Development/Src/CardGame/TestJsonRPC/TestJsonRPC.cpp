#include <ZionBase.h>
#include <JsonRPC.h>

int main(int argc, char* argv[])
{
	Zion::CJsonRPCClient* pClient = Zion::JsonRPC_GetClient("127.0.0.1:8000");
	getchar();
	printf("send data to xxxxxxx\n");
	JsonRPC_Send(pClient, "welcome", "\"welcome aaaaaaaaa\"");
	getchar();
	return 0;
}
