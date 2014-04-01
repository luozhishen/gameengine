#include <ZionBase.h>
#include <FastJson.h>
#include <stdio.h>
#include <uv.h>

static const char* test_cases[] =
{
	"{\"_name\":\"kkk\",\"_uuid\":\"8a0f7438-1228-4401-aed7-b93e0c05f312\",\"v1\":0,\"v2\":0,\"v3\":0.0}",
	"0",
	"[0]",
	"{\"name\":0}",
	"{\"name\":\"0\"}",
	"\"ABCDEFG\"",
	"\"\\uABCD\"",
	"-0.1E10",
	"0.1E10",
	"-0.1E-10",
	"0.1E-10",
};

#define RUN_TIMES		10000000

int main(int argc, char* argv[])
{
	Zion::JsonValue o;
	Zion::String vv;
	o.Stringify(vv);

	for(_U32 i=0; i<sizeof(test_cases)/sizeof(test_cases[0]); i++)
	{
		uint64_t start, end;
		printf("%s\n", test_cases[i]);

		start = uv_hrtime();
		for(_U32 t=0; t<RUN_TIMES; t++)
		{
			Zion::JsonValue node;
			if(!node.Parse(test_cases[i], NULL))
			{
				printf("ERROR F, %s\n", test_cases[i]);
				break;
			}
		}
		end = uv_hrtime();
		printf("case [F]: %fs\n", (float)(end-start)/1000000000);

		start = uv_hrtime();
		for(_U32 t=0; t<RUN_TIMES; t++)
		{
			Json::Reader reader;
			Json::Value node;
			if(!reader.parse(test_cases[i], test_cases[i] + strlen(test_cases[i]), node))
			{
				printf("ERROR S, %s\n", test_cases[i]);
				break;
			}
		}
		end = uv_hrtime();
		printf("case [S]: %fs\n", (float)(end-start)/1000000000);
	}

	return 0;
}
