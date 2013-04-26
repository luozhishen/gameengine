#pragma warning(disable:4100)
#pragma warning(disable:4996)

#include <AtlasSTL.h>
#include "mosdk.h"
#include "mo_common.h"

struct MOCLIENT
{
	Atlas::String baseurl;
	Atlas::String prefix;
	Atlas::String session_key;
	MOREQUEST* login_request;
	MOCLIENTSTATE state;
};

MOCLIENT* MOCreateClient(const char* baseurl, const char* prefix)
{
	MOCLIENT* client = new MOCLIENT;
	client->baseurl = baseurl;
	if(prefix) client->prefix = prefix;
	client->session_key = "";
	client->login_request = NULL;
	return client;
}

void MODestoryClient(MOCLIENT* client)
{
	if(client->login_request) MORequestDestory(client->login_request);
	delete client;
}

bool MOLoginByDeviceID(MOCLIENT* client)
{
	if(client->login_request) return false;
	Atlas::Map<Atlas::String, Atlas::String> params;
	params["app"] = MOGetAppName();
	params["os"] = MOGetOSName();
	params["device"] = MOGetDeviceUDID();
	Atlas::String url;
	url = client->baseurl + "login" + client->prefix;
	client->login_request = MORequestString(url.c_str(), params);
	if(client->login_request==NULL)
	{
		client->state = MOCLIENTSTATE_FAILED;
		return false;
	}
	client->state = MOCLIENTSTATE_LOGINING;
	return true;
}

bool MOLoginByToken(MOCLIENT* client, const char* token)
{
	return false;
}

bool MOLoginByUsername(MOCLIENT* client, const char* username, const char* password)
{
	if(client->login_request) return false;
	Atlas::Map<Atlas::String, Atlas::String> params;
	params["app"] = MOGetAppName();
	params["os"] = MOGetOSName();
	params["username"] = username;
	params["password"] = password;
	Atlas::String url;
	url = client->baseurl + "login" + client->prefix;
	client->login_request = MORequestString(url.c_str(), params);
	if(client->login_request)
	{
		client->state = MOCLIENTSTATE_LOGINING;
		return true;
	}
	else
	{
		client->state = MOCLIENTSTATE_FAILED;
		return false;
	}
}

void MOLogout(MOCLIENT* client)
{
	if(client->login_request) MORequestDestory(client->login_request);
	client->login_request = NULL;
	if(client->session_key.empty()) return;
	Atlas::Map<Atlas::String, Atlas::String> params;
	params["token"] = client->session_key;
	Atlas::String url;
	url = client->baseurl + "login" + client->prefix;
	client->login_request = MORequestString(url.c_str(), params);
	client->session_key = "";
	if(client->login_request)
	{
		client->state = MOCLIENTSTATE_LOGOUTING;
	}
	else
	{
		client->state = MOCLIENTSTATE_NA;
	}
}

MOCLIENTSTATE MOGetClientState(MOCLIENT* client)
{
	if(client->login_request)
	{
		switch(MORequestStatus(client->login_request))
		{
		case MOREQUESTSTATE_PENDING:
			return client->state;
		case MOREQUESTSTATE_FAILED:
			MORequestDestory(client->login_request);
			client->login_request = NULL;
			client->state = MOCLIENTSTATE_FAILED;
			return client->state;
		case MOREQUESTSTATE_DONE:
			int code = MOClientGetResultCode(client->login_request);
			if(code!=0)
			{
				client->state = MOCLIENTSTATE_FAILED;
			}
			else
			{
				client->state = MOCLIENTSTATE_AUTH;
				client->session_key = MOClientGetResultString(client->login_request);
			}
			MORequestDestory(client->login_request);
			client->login_request = NULL;
			break;
		}
	}

	return client->state;
}

const char* MOGetSessionKey(MOCLIENT* client)
{
	if(MOGetClientState(client)!=MOCLIENTSTATE_AUTH) return NULL;
	return client->session_key.c_str();
}

const char* MOGetClientBaseUrl(MOCLIENT* client)
{
	return client->baseurl.c_str();
}

MOREQUEST* MOClientRequestString(MOCLIENT* client, const char* method, const Atlas::Map<Atlas::String, Atlas::String>& params)
{
	if(MOGetClientState(client)!=MOCLIENTSTATE_AUTH) return NULL;

	Atlas::String postdata;
	build_http_param(postdata, params);
	if(!postdata.empty()) postdata += "&";
	postdata += "token=";
	postdata += client->session_key;
	Atlas::String url;
	url = client->baseurl + method + client->prefix;
	return MORequestString(url.c_str(), postdata.c_str());
}

MOREQUEST* MOClientDownloadFile(MOCLIENT* client, const char* method, const Atlas::Map<Atlas::String, Atlas::String>& params, const char* path)
{
	if(MOGetClientState(client)!=MOCLIENTSTATE_AUTH) return NULL;

	Atlas::String postdata;
	build_http_param(postdata, params);
	if(!postdata.empty()) postdata += "&";
	postdata += "token=";
	postdata += client->session_key;
	Atlas::String url;
	url = client->baseurl + method + client->prefix;
	return MODownloadFile(url.c_str(), postdata.c_str(), path, false);
}

int MOClientGetResultCode(MOREQUEST* request)
{
	switch(MORequestStatus(request))
	{
	case MOREQUESTSTATE_PENDING:
		return MOERROR_PENDING;
	case MOREQUESTSTATE_DONE:
		break;
	case MOREQUESTSTATE_FAILED:
	default:
		return MOERROR_NETWORK;
	}

	const char* result = MORequestGetResult(request);
	if(!result) return MOERROR_NOERROR;
	if(memcmp(result, "ERROR=", 6)!=0) return MOERROR_UNKNOWN;
	const char* pos = strchr(result+6, ';');
	if(!pos) pos = result + strlen(result);
	char code[100];
	if(pos-result-6>=sizeof(code)) return MOERROR_UNKNOWN;
	memcpy(code, result+6, pos-result-6);
	code[pos-result-6] = '\0';
	return MOError(code);
}

const char* MOClientGetResultString(MOREQUEST* request)
{
	if(MORequestStatus(request)!=MOREQUESTSTATE_DONE) return NULL;
	const char* result = MORequestGetResult(request);
	if(!result) return NULL;
	result = strchr(result, ';');
	if(!result) return "";
	return result+1;
}
