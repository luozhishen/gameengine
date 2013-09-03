#ifdef _WIN32

#pragma warning(disable:4100)
#pragma warning(disable:4996)
#pragma warning(disable:4121)

#include <ZionSTL.h>
#include <crtdbg.h>
#include "mosdk.h"
#include "mo_common.h"

#include <windows.h>
#include <wininet.h>

static bool http_request(MOREQUEST* request, const char* url, const char* header, const char* postdata);
static char g_TypeSpec[] = "Content-Type: application/x-www-form-urlencoded\r\n";

struct MOREQUEST
{
	HINTERNET _connect;
	HINTERNET _request;
	Zion::String _result;
	FILE* _file;
	int _length;
	MOREQUESTSTATE _state;
	Zion::String _postdata;
	Zion::String _postheader;
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
		_RPT1(_CRT_WARN, "callback return error = %d\n", res->dwError);
		return;
	}

	DWORD len = 0;
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
				_RPT1(_CRT_WARN, "InternetReadFileEx return error = %d\n", GetLastError());
			}
			return;
		}
		len = ib.dwBufferLength;
		if(len==0) break;

		if(request->_file)
		{
			fwrite(request->buf, 1, len, request->_file);
			request->_length += len;
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
		InternetCloseHandle(_session);
	}
	HINTERNET _session;
};
static MO_REQUEST_WIN32_INIT g_MO_REQUEST_WIN32_INIT;
static bool g_MO_EnableDebug = false;

void MOEnableDebug(bool enable)
{
	g_MO_EnableDebug = enable;
}

MOREQUEST* MORequestString(const char* url, const Zion::Map<Zion::String, Zion::String>& params)
{
	Zion::String postdata;
	build_http_param(postdata, params);
	return MORequestString(url, postdata.c_str());
}

MOREQUEST* MODownloadFile(const char* url, const Zion::Map<Zion::String, Zion::String>& params, const char* path, bool append)
{
	Zion::String postdata;
	build_http_param(postdata, params);
	return MODownloadFile(url, postdata.c_str(), path, append);
}

MOREQUEST* MORequestString(const char* url, const char* postdata)
{
	MOREQUEST* request = new MOREQUEST;
	request->_file = NULL;
	if(http_request(request, url, postdata?g_TypeSpec:NULL, postdata))
	{
		return request;
	}
	else
	{
		delete request;
		return NULL;
	}
}

MOREQUEST* MODownloadFile(const char* url, const char* postdata, const char* path, bool append)
{
	FILE* file = fopen(path, append?"ab":"wb");
	if(file==NULL) return NULL;
	fseek(file, 0, SEEK_END);
	int loc = ftell(file);
	if(loc<=1000) loc = 0; else loc -= 1000;
	fseek(file, loc, SEEK_SET);

	MOREQUEST* request = new MOREQUEST;
	request->_file = file;
	request->_length = loc;
	if(postdata) request->_postheader += g_TypeSpec;
	char seekstr[100];
	sprintf(seekstr, "Range:bytes=%d-\r\n", loc);
	request->_postheader += seekstr;

	if(http_request(request, url, request->_postheader.c_str(), postdata))
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

void MORequestDestory(MOREQUEST* request)
{
	while(request->_state==MOREQUESTSTATE_PENDING) SwitchToThread();
	InternetCloseHandle(request->_request);
	InternetCloseHandle(request->_connect);
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
	return request->_file?request->_length:(int)(request->_result.size());
}

bool http_request(MOREQUEST* request, const char* url, const char* header, const char* postdata)
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
		if(g_MO_EnableDebug)
		{
			DWORD dwTimeout = 10*60*1000, dwLength = sizeof(dwTimeout);
			InternetQueryOption(request->_connect, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimeout, &dwLength);
			InternetQueryOption(request->_connect, INTERNET_OPTION_SEND_TIMEOUT, &dwTimeout, &dwLength);
			InternetQueryOption(request->_connect, INTERNET_OPTION_RECEIVE_TIMEOUT, &dwTimeout, &dwLength);
			InternetQueryOption(request->_connect, INTERNET_OPTION_DATA_SEND_TIMEOUT, &dwTimeout, &dwLength);
			InternetQueryOption(request->_connect, INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, &dwTimeout, &dwLength);
			dwTimeout = 10*60*1000;
			InternetSetOption(request->_connect, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimeout, sizeof(dwTimeout));
			InternetSetOption(request->_connect, INTERNET_OPTION_SEND_TIMEOUT, &dwTimeout, sizeof(dwTimeout));
			InternetSetOption(request->_connect, INTERNET_OPTION_RECEIVE_TIMEOUT, &dwTimeout, sizeof(dwTimeout));
			InternetSetOption(request->_connect, INTERNET_OPTION_DATA_SEND_TIMEOUT, &dwTimeout, sizeof(dwTimeout));
			InternetSetOption(request->_connect, INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, &dwTimeout, sizeof(dwTimeout));
		}

		request->_state = MOREQUESTSTATE_PENDING;
		request->_result = "";
		request->_postdata = postdata;
		DWORD flag = INTERNET_FLAG_NO_UI|INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_RELOAD;
		if(!g_MO_EnableDebug)
		{
			flag |= INTERNET_FLAG_NO_COOKIES;
		}

		if(urlcomps.nScheme==INTERNET_SCHEME_HTTPS) flag |= INTERNET_FLAG_SECURE;
		Zion::String urlpath;
		urlpath = urlcomps.lpszUrlPath;
		urlpath += urlcomps.lpszExtraInfo;
		request->_request = HttpOpenRequestA(request->_connect, request->_postdata.empty()?"GET":"POST", urlpath.c_str(), NULL, NULL, NULL, flag, (DWORD_PTR)request);
		if(request->_request)
		{
			BOOL result;
			if(request->_postdata.empty())
			{
				result = HttpSendRequestA(request->_request, header, header?(DWORD)strlen(header):0, NULL, 0);
			}
			else
			{
				result = HttpSendRequestA(request->_request, header, header?(DWORD)strlen(header):0, (LPVOID)request->_postdata.c_str(), (DWORD)request->_postdata.size());
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
