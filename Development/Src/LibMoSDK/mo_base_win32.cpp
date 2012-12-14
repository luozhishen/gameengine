#ifdef _WIN32

#pragma warning(disable:4100)
#pragma warning(disable:4996)

#include <map>
#include <string>
#include "mosdk.h"
#include <windows.h>

static char g_AppName[1000] = "UNKNOWN_APP";
static char g_UDID[1000] = "UNKNOWN_UDID";
static char g_OSName[1000] = "WINDOWS";

void MOInit(const char* appname)
{
	strcpy(g_AppName, appname);

	DWORD dwVersion = GetVersion();
	DWORD dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
	DWORD dwBuild = 0;
	if(dwVersion<0x80000000) dwBuild = (DWORD)(HIWORD(dwVersion));
	sprintf(g_OSName, "windows0%d0%d0%d", dwMajorVersion, dwMinorVersion, dwBuild);

}

void MOFini()
{

}

const char* MOGetDeviceUDID()
{
	return g_UDID;
}

const char* MOGetAppName()
{
	return g_AppName;
}

const char* MOGetOSName()
{
	return g_OSName;
}

#endif
