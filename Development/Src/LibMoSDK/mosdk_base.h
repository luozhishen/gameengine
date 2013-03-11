#ifndef __MOSDK_BASE_INCLUDE__
#define __MOSDK_BASE_INCLUDE__

void MOInit(const char* appname);
void MOFini();

const char* MOGetDeviceUDID();
const char* MOGetAppName();
const char* MOGetOSName();
const char* MoGetResourcePath();

struct MOREQUEST;
typedef struct MOREQUEST MOREQUEST;

typedef enum {
	MOREQUESTSTATE_PENDING,
	MOREQUESTSTATE_FAILED,
	MOREQUESTSTATE_DONE,
} MOREQUESTSTATE;

MOREQUEST* MORequestString(const char* url, const Atlas::Map<Atlas::String, Atlas::String>& params);
MOREQUEST* MODownloadFile(const char* url, const Atlas::Map<Atlas::String, Atlas::String>& params, const char* path);
MOREQUEST* MOUploadFiles(const char* url, const Atlas::Map<Atlas::String, Atlas::String>& params, const Atlas::Map<Atlas::String, Atlas::String>& files);
MOREQUEST* MORequestString(const char* url, const char* postdata);
MOREQUEST* MODownloadFile(const char* url, const char* postdata, const char* path);
MOREQUEST* MOUploadFiles(const char* url, const char* postdata, const Atlas::Map<Atlas::String, Atlas::String>& files);
void MORequestDestory(MOREQUEST* request);

MOREQUESTSTATE MORequestStatus(MOREQUEST* request);
const char* MORequestGetResult(MOREQUEST* request);
int MORequestGetResultLength(MOREQUEST* request);

#endif // __MOSDK_BASE_INCLUDE__
