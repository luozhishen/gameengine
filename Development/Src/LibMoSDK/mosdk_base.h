#ifndef __MOSDK_BASE_INCLUDE__
#define __MOSDK_BASE_INCLUDE__

void MOInit(const char* appname);
void MOFini();

const char* MOGetDeviceUDID();
const char* MOGetAppName();
const char* MOGetOSName();

struct MOREQUEST;
typedef struct MOREQUEST MOREQUEST;

typedef enum {
	MOREQUESTSTATE_PENDING,
	MOREQUESTSTATE_FAILED,
	MOREQUESTSTATE_DONE,
} MOREQUESTSTATE;

MOREQUEST* MORequestString(const char* url, const std::map<std::string, std::string>& params);
MOREQUEST* MODownloadFile(const char* url, const std::map<std::string, std::string>& params, const char* path);
MOREQUEST* MOUploadFiles(const char* url, const std::map<std::string, std::string>& params, const std::map<std::string, std::string>& files);
MOREQUEST* MORequestString(const char* url, const char* postdata);
MOREQUEST* MODownloadFile(const char* url, const char* postdata, const char* path);
MOREQUEST* MOUploadFiles(const char* url, const char* postdata, const std::map<std::string, std::string>& files);
void MORequestDestory(MOREQUEST* request);

MOREQUESTSTATE MORequestStatus(MOREQUEST* request);
const char* MORequestGetResult(MOREQUEST* request);
int MORequestGetResultLength(MOREQUEST* request);

#endif // __MOSDK_BASE_INCLUDE__
