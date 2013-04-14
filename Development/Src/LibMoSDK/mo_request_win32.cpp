#ifdef _WIN32

#pragma warning(disable:4100)
#pragma warning(disable:4996)
#pragma warning(disable:4121)

#include <AtlasSTL.h>
#include <crtdbg.h>
#include "mosdk.h"
#include "mo_common.h"

#include <windows.h>
#include <wininet.h>

static bool http_request(MOREQUEST* request, const char* url, const char* postdata);
static char g_TypeSpec[] = "Content-Type: application/x-www-form-urlencoded\r\n";
static char g_AcceptType[] = "*/*";

struct MOREQUEST
{
	HINTERNET _connect;
	HINTERNET _request;
	Atlas::String _result;
	FILE* _file;
	MOREQUESTSTATE _state;
	Atlas::String _postdata;
	char buf[1000];
};

static void CALLBACK request_callback(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
	if(INTERNET_STATUS_REQUEST_COMPLETE!=dwInternetStatus) return;
	LPINTERNET_ASYNC_RESULT res = (LPINTERNET_ASYNC_RESULT)lpvStatusInformation;
	MOREQUEST* request = (MOREQUEST*)dwContext;

	if(!res->dwResult)
	{
		request->_state = MOREQUESTSTATE_FAILED;
		_RPT1(_CRT_WARN, "callback return error = %d", res->dwError);
		return;
	}

	DWORD len = 0;
//	char buf[1000];
	for(;;)
	{
		INTERNET_BUFFERS ib = { sizeof(INTERNET_BUFFERS) };
		ib.lpvBuffer = request->buf;
		ib.dwBufferLength = sizeof(request->buf) - 1;
		if(!InternetReadFileEx(request->_request, &ib, IRF_ASYNC, (LPARAM)request))
		{
			if(GetLastError()!=ERROR_IO_PENDING)
			{
				request->_state = MOREQUESTSTATE_FAILED;
				_RPT1(_CRT_WARN, "InternetReadFileEx return error = %d", GetLastError());
			}
			return;
		}
		len = ib.dwBufferLength;
		if(len==0) break;
		/*
		if(!InternetReadFile(request->_request, buf, sizeof(buf)-1, &len))
		{
			if(GetLastError()!=ERROR_IO_PENDING)
			{
				request->_state = MOREQUESTSTATE_FAILED;
			}
			return;
		}
		if(len==0) break;
		*/

		if(request->_file)
		{
			fwrite(request->buf, 1, len, request->_file);
		}
		else
		{
			request->buf[len] = '\0';
			request->_result += request->buf;
		}
	}

	request->_state = MOREQUESTSTATE_DONE;
}

class MO_REQUEST_WIN32_INIT
{
public:
	MO_REQUEST_WIN32_INIT()
	{
		_session = InternetOpenA("MO_CLIENT", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, INTERNET_FLAG_ASYNC);
		InternetSetStatusCallback(_session, request_callback);
	}
	~MO_REQUEST_WIN32_INIT()
	{
		InternetSetStatusCallback(_session, NULL);
		CloseHandle(_session);
	}
	HINTERNET _session;
};
static MO_REQUEST_WIN32_INIT g_MO_REQUEST_WIN32_INIT;

MOREQUEST* MORequestString(const char* url, const Atlas::Map<Atlas::String, Atlas::String>& params)
{
	Atlas::String postdata;
	build_http_param(postdata, params);
	return MORequestString(url, postdata.c_str());
}

MOREQUEST* MODownloadFile(const char* url, const Atlas::Map<Atlas::String, Atlas::String>& params, const char* path)
{
	Atlas::String postdata;
	build_http_param(postdata, params);
	return MODownloadFile(url, postdata.c_str(), path);
}

MOREQUEST* MOUploadFiles(const char* url, const Atlas::Map<Atlas::String, Atlas::String>& params, const Atlas::Map<Atlas::String, Atlas::String>& files)
{
	Atlas::String postdata;
	build_http_param(postdata, params);
	return MOUploadFiles(url, postdata.c_str(), files);
}

MOREQUEST* MORequestString(const char* url, const char* postdata)
{
	MOREQUEST* request = new MOREQUEST;
	request->_file = NULL;
	if(http_request(request, url, postdata))
	{
		return request;
	}
	else
	{
		delete request;
		return NULL;
	}
}

MOREQUEST* MODownloadFile(const char* url, const char* postdata, const char* path)
{
	FILE* file = fopen(path, "wb");
	if(file==NULL) return NULL;

	MOREQUEST* request = new MOREQUEST;
	request->_file = file;
	if(http_request(request, url, postdata))
	{
		return request;
	}
	else
	{
		delete request;
		fclose(file);
		return NULL;
	}
}

MOREQUEST* MOUploadFiles(const char* url, const char* postdata, const Atlas::Map<Atlas::String, Atlas::String>& files)
{
	return NULL;
}

void MORequestDestory(MOREQUEST* request)
{
	while(request->_state==MOREQUESTSTATE_PENDING) SwitchToThread();
//	InternetSetStatusCallback(request->_session, NULL);
	InternetCloseHandle(request->_request);
	InternetCloseHandle(request->_connect);
//	InternetCloseHandle(request->_session);
	delete request;
}

MOREQUESTSTATE MORequestStatus(MOREQUEST* request)
{
	return request->_state;
}

const char* MORequestGetResult(MOREQUEST* request)
{
	return request->_result.c_str();
}

int MORequestGetResultLength(MOREQUEST* request)
{
	return (int)(request->_result.size());
}

bool http_request(MOREQUEST* request, const char* url, const char* postdata)
{
	URL_COMPONENTSA urlcomps;
	CHAR buf0[256], buf1[256], buf2[256], buf3[256], buf4[1024], buf5[1024];
	ZeroMemory(&urlcomps, sizeof(urlcomps));
	urlcomps.dwStructSize = sizeof(urlcomps);
	urlcomps.lpszScheme = buf0;
	urlcomps.dwSchemeLength = sizeof(buf0);
	urlcomps.lpszHostName = buf1;
	urlcomps.dwHostNameLength = sizeof(buf1);
	urlcomps.lpszUserName = buf2;
	urlcomps.dwUserNameLength = sizeof(buf2);
	urlcomps.lpszPassword = buf3;
	urlcomps.dwPasswordLength = sizeof(buf3);
	urlcomps.lpszUrlPath = buf4;
	urlcomps.dwUrlPathLength = sizeof(buf4);
	urlcomps.lpszExtraInfo = buf5;
	urlcomps.dwExtraInfoLength = sizeof(buf5);
	if(!InternetCrackUrlA(url, (DWORD)strlen(url), 0, &urlcomps)) return false;

	request->_connect = InternetConnectA(g_MO_REQUEST_WIN32_INIT._session, urlcomps.lpszHostName, urlcomps.nPort, urlcomps.lpszUserName, urlcomps.lpszPassword, INTERNET_SERVICE_HTTP, INTERNET_FLAG_EXISTING_CONNECT, (DWORD_PTR)request);
	if(request->_connect)
	{
		request->_state = MOREQUESTSTATE_PENDING;
		request->_result = "";
		request->_postdata = postdata;
		DWORD flag = INTERNET_FLAG_NO_UI|INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_RELOAD;
		if(urlcomps.nScheme==INTERNET_SCHEME_HTTPS) flag |= INTERNET_FLAG_SECURE;
		Atlas::String urlpath;
		urlpath = urlcomps.lpszUrlPath;
		urlpath += urlcomps.lpszExtraInfo;
		request->_request = HttpOpenRequestA(request->_connect, request->_postdata.empty()?"GET":"POST", urlpath.c_str(), NULL, NULL, NULL, flag, (DWORD_PTR)request);
		if(request->_request)
		{
			BOOL result;
			if(request->_postdata.empty())
			{
				result = HttpSendRequestA(request->_request, NULL, 0, NULL, 0);
			}
			else
			{
				result = HttpSendRequestA(request->_request, g_TypeSpec, (DWORD)strlen(g_TypeSpec), (LPVOID)request->_postdata.c_str(), (DWORD)request->_postdata.size());
			}

			if(result || GetLastError()==ERROR_IO_PENDING) return true;
			{
				INTERNET_BUFFERS ib = { sizeof(INTERNET_BUFFERS) };
				ib.lpvBuffer = request->buf;
				ib.dwBufferLength = 0;//sizeof(request->buf);
				result = InternetReadFileEx(request->_request, &ib, IRF_ASYNC, (LPARAM)request);
				if(result || GetLastError()==ERROR_IO_PENDING) return true;
			}

			InternetCloseHandle(request->_request);
		}
		InternetCloseHandle(request->_connect);
	}
	return false;
}

#endif
