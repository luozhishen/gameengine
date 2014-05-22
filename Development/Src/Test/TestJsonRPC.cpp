
#include <ZionBase.h>
#include <JsonRPC.h>
#include <uv.h>
#include <stdio.h>

static _U32 completed_count = 0;
static _U32 error_count = 0;
static Zion::CJsonRPCClient* client;
static _U32 seq = 0;
static const char* method = "echo";
static const char* args = "[0]";
static _U32 retry_count = 0;

static void timer_callback(uv_timer_t* handle, int status);
static void jsonrpc_callback(const Zion::JsonValue* val);

void timer_callback(uv_timer_t* handle, int status)
{
	static _U32 last_completed = 0;
	printf("%8u %15u %15u %15u\n", seq++, completed_count-last_completed, error_count, completed_count+error_count);
	last_completed = completed_count;
	_U32 count = retry_count;
	retry_count = 0;
	for(;count>0; count--)
	{
		if(!Zion::JsonRPC_Call(client, method, args, jsonrpc_callback))
		{
			retry_count += 1;
			error_count += 1;
		}
	}
}

void jsonrpc_callback(const Zion::JsonValue* val)
{
	if(val)
	{
		completed_count += 1;
	}
	else
	{
		error_count += 1;
	}
	if(!Zion::JsonRPC_Call(client, method, args, jsonrpc_callback))
	{
		retry_count += 1;
		error_count += 1;
	}
}

int main(int argc, char* argv[])
{
	if(argc<2 || argc==3) return -1;
	if(argc>=4)
	{
		method = argv[2];
		args = argv[3];
	}

	client = Zion::JsonRPC_GetClient(argv[1]);
	uv_timer_t timer;
	uv_timer_init(uv_default_loop(), &timer);
	uv_timer_start(&timer, timer_callback, 1000, 1000);

	for(_U32 i=0; i<1; i++)
	{
		Zion::JsonRPC_Call(client, method, args, jsonrpc_callback);
	}

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	return 0;
}
