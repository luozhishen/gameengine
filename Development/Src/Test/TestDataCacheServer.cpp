#include <ZionBase.h>
#include <JsonRPC.h>
#include <uv.h>
#include <stdio.h>

static _U32 completed_count = 0;
static _U32 error_count = 0;
static Zion::CJsonRPCClient* client;
static _U32 seq = 0;

class CTestGetAvatar
{
public:
	CTestGetAvatar(_U32 avatar_id)
	{
		_avatar_id = avatar_id;
		_args = Zion::StringFormat("[%u]", avatar_id);
	}

	void Start()
	{
		Zion::JsonRPC_Call(client, "getAvatar", _args.c_str(), std::bind (&CTestGetAvatar::OnReturn, this,std::placeholders::_1));
	}

	void OnReturn(const Zion::JsonValue* val)
	{
		if(val)
		{
			completed_count += 1;
		}
		else
		{
			error_count += 1;
		}
		Zion::JsonRPC_Call(client, "getAvatar", _args.c_str(), std::bind (&CTestGetAvatar::OnReturn, this,std::placeholders::_1));
	}

private:
	_U32 _avatar_id;
	Zion::String _args;
};

class CTestAutoAvatar
{
public:
	CTestAutoAvatar();
	~CTestAutoAvatar();

	void Start()
	{
	}

	void OnGetAvatar(const Zion::JsonValue* json)
	{
	}

	void OnCreateAvatar(const Zion::JsonValue* json)
	{
	}

	void OnCreateObject(const Zion::JsonValue* json)
	{
	}

	void OnDeleteObject(const Zion::JsonValue* json)
	{
	}

	void OnExecuteBatch(const Zion::JsonValue* json)
	{
	}

};

void timer_callback(uv_timer_t* handle, int status)
{
	static _U32 last_completed = 0;
	printf("%8u %15u %15u %15u\n", seq++, completed_count-last_completed, error_count, completed_count+error_count);
	last_completed = completed_count;
}

int main(int argc, char* argv[])
{
	if(argc<2) return -1;

	client = Zion::JsonRPC_GetClient(argv[1]);
	uv_timer_t timer;
	uv_timer_init(uv_default_loop(), &timer);
	uv_timer_start(&timer, timer_callback, 1000, 1000);

	for(_U32 i=0; i<100000; i++)
	{
		CTestGetAvatar* test = new CTestGetAvatar(i);
		test->Start();
	}

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	return 0;
}
