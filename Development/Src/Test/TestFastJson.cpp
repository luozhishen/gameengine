#include <ZionBase.h>
#include <FastJson.h>
#include <stdio.h>
#include <uv.h>

static const char* test_cases[] =
{
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

#if 0
static Zion::JsonValue jsons[10];
static Json::Value s_jsons	[10];
#else
static Zion::JsonValue jsons[1000000];
static Json::Value s_jsons	[1000000];
#endif

int main(int argc, char* argv[])
{
	for(_U32 i=0; i<sizeof(test_cases)/sizeof(test_cases[0]); i++)
	{
		for(_U32 t=0; t<sizeof(jsons)/sizeof(jsons[0]); t++)
		{
			jsons[t].SetType(Zion::JsonValue::TYPE_NULL);
			s_jsons[t] = Json::nullValue;
		}

		uint64_t start, end;
		printf("%s\n", test_cases[i]);
		
		start = uv_hrtime();
		for(_U32 t=0; t<sizeof(jsons)/sizeof(jsons[0]); t++)
		{
			Zion::JsonValue node;
			if(!Zion::JsonValue::Parse(test_cases[i], NULL, node))
			{
				printf("ERROR F, %s\n", test_cases[i]);
				break;
			}
		}
		end = uv_hrtime();
		printf("case [FA]: %fs\n", (float)(end-start)/1000000);

		start = uv_hrtime();
		for(_U32 t=0; t<sizeof(jsons)/sizeof(jsons[0]); t++)
		{
			if(!Zion::JsonValue::Parse(test_cases[i], NULL, jsons[i]))
			{
				printf("ERROR F, %s\n", test_cases[i]);
				break;
			}
		}
		end = uv_hrtime();
		printf("case [FX]: %fs\n", (float)(end-start)/1000000);

		start = uv_hrtime();
		Json::Reader reader1;
		for(_U32 t=0; t<sizeof(jsons)/sizeof(jsons[0]); t++)
		{
			Json::Value node;
			if(!reader1.parse(test_cases[i], test_cases[i] + strlen(test_cases[i]), node))
			{
				printf("ERROR S, %s\n", test_cases[i]);
				break;
			}
		}
		end = uv_hrtime();
		printf("case [SA]: %fs\n", (float)(end-start)/1000000);

		start = uv_hrtime();
		Json::Reader reader2;
		for(_U32 t=0; t<sizeof(jsons)/sizeof(jsons[0]); t++)
		{
			if(!reader2.parse(test_cases[i], test_cases[i] + strlen(test_cases[i]), s_jsons[i]))
			{
				printf("ERROR S, %s\n", test_cases[i]);
				break;
			}
		}
		end = uv_hrtime();
		printf("case [SX]: %fs\n", (float)(end-start)/1000000);
	}

	return 0;
}
