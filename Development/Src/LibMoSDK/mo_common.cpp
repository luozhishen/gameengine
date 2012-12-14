#include <string>
#include <map>
#include "mo_common.h"

static const char HEXCHARS[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

static char* urlencode(const char* in, char* out)
{
	char* ret = out;
	while(*in!='\0')
	{
		if(!(*in>='a' && *in<='z') && !(*in>='A' && *in<='Z') && !(*in>='0' && *in<='9'))
		{
			*(out++) = '%';
			*(out++) = HEXCHARS[((unsigned char)*in)>>4];
			*(out++) = HEXCHARS[((unsigned char)*in)&0x0F];
		}
		else
		{
			*(out++) = *in;
		}

		in++;
	}
	*out = '\0';
	return ret;
}

void build_http_param(std::string& value, const std::map<std::string, std::string>& params)
{
	std::map<std::string, std::string>::const_iterator i;
	char txt[1024*10];
	value = "";

	for(i=params.begin(); i!=params.end(); ++i)
	{
		if(value.size()>0) value += "&";

		value += urlencode(i->first.c_str(), txt);
		value += "=";
		value += urlencode(i->second.c_str(), txt);
	}
}
