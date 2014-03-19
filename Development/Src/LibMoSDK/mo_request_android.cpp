#ifdef ANDROID

#include <ZionDefines.h>
#include <ZionSTL.h>
#include "mosdk.h"

void MOEnableDebug(bool enable)
{
}

MOREQUEST* MORequestString(const char* url, const Zion::Map<Zion::String, Zion::String>& params)
{
	return NULL;
}

MOREQUEST* MODownloadFile(const char* url, const Zion::Map<Zion::String, Zion::String>& params, const char* path, bool append)
{
	return NULL;
}

MOREQUEST* MORequestString(const char* url, const char* postdata)
{
	return NULL;
}

MOREQUEST* MODownloadFile(const char* url, const char* postdata, const char* path, bool append)
{
	return NULL;
}

void MORequestDestory(MOREQUEST* request)
{
}

MOREQUESTSTATE MORequestStatus(MOREQUEST* request)
{
	return MOREQUESTSTATE_DONE;
}

const char* MORequestGetResult(MOREQUEST* request)
{
	return NULL;
}

int MORequestGetResultLength(MOREQUEST* request)
{
	return 0;
}

#endif
