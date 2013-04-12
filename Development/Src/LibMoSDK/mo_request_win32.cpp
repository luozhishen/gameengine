#ifdef _WIN32

#pragma warning(disable:4100)
#pragma warning(disable:4996)
#pragma warning(disable:4121)

#include <AtlasSTL.h>
#include "mosdk.h"
#include "mo_common.h"

#include <windows.h>
#include <wininet.h>

static bool http_request(MOREQUEST* request, INTERNET_STATUS_CALLBACK callback, const char* url, const char* postdata);

struct MOREQUEST
{
	HINTERNET _session;
	HINTERNET _connect;
	HINTERNET _request;
	Atlas::String _result;
	FILE* _file;
	MOREQUESTSTATE _state;
	Atlas::String _postdata;
};

static void CALLBACK requeststring_callback(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
	char tmp[1000];
	static DWORD prev_status_0 = 0, prev_status_1 = 0;
	sprintf(tmp, "state=%d %d %p\n", dwInternetStatus, GetCurrentThreadId(), hInternet);
	OutputDebugStringA(tmp);
	if(dwInternetStatus==41 && prev_status_1==40 && prev_status_0==100) {
//		DebugBreak();
	}
	prev_status_0 = prev_status_1;
	prev_status_1 = dwInternetStatus;
	if(INTERNET_STATUS_REQUEST_COMPLETE!=dwInternetStatus) return;
	LPINTERNET_ASYNC_RESULT res = (LPINTERNET_ASYNC_RESULT)lpvStatusInformation;
	MOREQUEST* request = (MOREQUEST*)dwContext;

	if(!res->dwResult)
	{
		request->_state = MOREQUESTSTATE_FAILED;
		return;
	}

	DWORD len;
	char buf[1000];

	for(;;)
	{
		if(!InternetReadFile(request->_request, buf, sizeof(buf)-1, &len))
		{
			request->_state = MOREQUESTSTATE_FAILED;
			return;
		}
		if(len==0) break;
		buf[len] = '\0';
		request->_result += buf;
	}

	request->_state = MOREQUESTSTATE_DONE;
}

static void CALLBACK requestfile_callback(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
	if(INTERNET_STATUS_REQUEST_COMPLETE!=dwInternetStatus && INTERNET_STATUS_RESPONSE_RECEIVED!=dwInternetStatus) return;

	MOREQUEST* request = (MOREQUEST*)dwContext;
	if(request->_state!=MOREQUESTSTATE_PENDING) return;

	if(INTERNET_STATUS_REQUEST_COMPLETE==dwInternetStatus)
	{
		LPINTERNET_ASYNC_RESULT res = (LPINTERNET_ASYNC_RESULT)lpvStatusInformation;
		if(!res->dwResult)
		{
			request->_state = MOREQUESTSTATE_FAILED;
			return;
		}
	}

	DWORD len, got;
	char buf[10*1024];
	for(;;)
	{
		if(!InternetQueryDataAvailable(request->_request, &len, NULL, NULL))
		{
			request->_state = MOREQUESTSTATE_FAILED;
			return;
		}
		if(INTERNET_STATUS_REQUEST_COMPLETE!=dwInternetStatus && len==0) return;

		if(!InternetReadFile(request->_request, buf, sizeof(buf), &got))
		{
			request->_state = MOREQUESTSTATE_FAILED;
			return;
		}
		if(got==0) break;

		fwrite(buf, 1, got, request->_file);
	}

	request->_state = MOREQUESTSTATE_DONE;
}

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
	if(http_request(request, requeststring_callback, url, postdata))
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
	if(http_request(request, requestfile_callback, url, postdata))
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
	InternetSetStatusCallback(request->_session, NULL);
	InternetCloseHandle(request->_request);
	InternetCloseHandle(request->_connect);
	InternetCloseHandle(request->_session);
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

bool http_request(MOREQUEST* request, INTERNET_STATUS_CALLBACK callback, const char* url, const char* postdata)
{
	static char g_TypeSpec[] = "Content-Type: application/x-www-form-urlencoded\r\n";
	static char g_AcceptType[] = "*/*";

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

	request->_session = InternetOpenA("MO_CLIENT", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_ASYNC);
	if(request->_session)
	{
		InternetSetStatusCallback(request->_session, callback);
		char tmp[1000];
		sprintf(tmp, "current thread id = %d %p\n", GetCurrentThreadId(), request->_session);
		OutputDebugStringA(tmp);
		request->_connect = InternetConnectA(request->_session, urlcomps.lpszHostName, urlcomps.nPort, urlcomps.lpszUserName, urlcomps.lpszPassword, INTERNET_SERVICE_HTTP, INTERNET_FLAG_EXISTING_CONNECT, (DWORD_PTR)request);

		sprintf(tmp, "InternetConnectA %d %p\n", GetCurrentThreadId(), request->_connect);
		OutputDebugStringA(tmp);
		if(request->_connect)
		{
			request->_state = MOREQUESTSTATE_PENDING;
			request->_result = "";
			DWORD flag = INTERNET_FLAG_NO_UI|INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_RELOAD;
			if(urlcomps.nScheme==INTERNET_SCHEME_HTTPS) flag |= INTERNET_FLAG_SECURE;
			Atlas::String urlpath;
			urlpath = urlcomps.lpszUrlPath;
			urlpath += urlcomps.lpszExtraInfo;
			request->_request = HttpOpenRequestA(request->_connect, postdata?"POST":"GET", urlpath.c_str(), NULL, NULL, NULL, flag, (DWORD_PTR)request);
			sprintf(tmp, "HttpOpenRequestA %d %p\n", GetCurrentThreadId(), request->_request);
			OutputDebugStringA(tmp);
			if(request->_request)
			{
				BOOL result;
				if(postdata)
				{
					request->_postdata = postdata;
					postdata = request->_postdata.c_str();
					result = HttpSendRequestA(request->_request, g_TypeSpec, (DWORD)strlen(g_TypeSpec), (LPVOID)postdata, (DWORD)strlen(postdata));
					sprintf(tmp, "HttpSendRequestA %d %p\n", GetCurrentThreadId(), request->_request);
					OutputDebugStringA(tmp);
				}
				else
				{
					result = HttpSendRequestA(request->_request, NULL, 0, NULL, 0);
					sprintf(tmp, "HttpSendRequestA %d %p\n", GetCurrentThreadId(), request->_request);
					OutputDebugStringA(tmp);
				}

				if(result || GetLastError()==ERROR_IO_PENDING) return true;
				InternetCloseHandle(request->_request);
			}
			InternetCloseHandle(request->_connect);
		}
		InternetSetStatusCallback(request->_session, NULL);
		InternetCloseHandle(request->_session);
	}
	return false;
}

#endif
