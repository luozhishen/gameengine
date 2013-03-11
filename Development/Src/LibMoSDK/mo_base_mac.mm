#ifndef _WIN32

#include <AtlasSTL.h>
#include "mosdk.h"

#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/socket.h>
#include <net/if_dl.h>
#include <net/if.h>
#import <Foundation/NSString.h>
#import <CommonCrypto/CommonDigest.h>
#import <UIKit/UIDevice.h>

static char g_AppName[1000] = "UNKNOWN_APP";
static char g_UDID[1000] = "UNKNOWN_UDID";
static char g_OSName[1000] = "APPLE";
static char g_ResourcePath[1000] = "";
static const char* get_wifi_mac(char* addr);
static const char* get_ios_udid(char* addr);
static const char* get_random_deviceid(char* addr);
static bool save_to_local(const char* addr);
static bool load_from_local(char* addr, int len);

void MOInit(const char* appname)
{
	strcpy(g_AppName, appname);

	if(!load_from_local(g_UDID, sizeof(g_UDID)))
	{
        if(get_ios_udid(g_UDID) || get_wifi_mac(g_UDID) || get_random_deviceid(g_UDID))
		{
			save_to_local(g_UDID);
		}
	}

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    char type[100], version[100];
    strcpy(type, [[UIDevice currentDevice].model UTF8String]);
    strcpy(version, [[UIDevice currentDevice].systemVersion UTF8String]);
    if(strstr(type, "iPhone"))
    {
        sprintf(g_OSName, "IOS_iPhone_%s", version);
    }
    else if(strstr(type, "iPod")) {
        sprintf(g_OSName, "IOS_iPod_%s", version);
    }
    else if(strstr(type, "iPad")) {
        sprintf(g_OSName, "IOS_iPad_%s", version);
    }
    else {
        sprintf(g_OSName, "IOS_unknown_%s", version);
    }
#else
	sprintf(g_OSName, "OSX_%s", [[UIDevice currentDevice].systemVersion UTF8String]);
#endif

	NSString *respath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@""];
	strcat(g_ResourcePath, [respath UTF8String]);
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

const char* MoGetResourcePath()
{
	return g_ResourcePath;
}

const char* get_wifi_mac(char* addr)
{
	int						mib[6];
	size_t					len;
	char*					buf;
	unsigned char*			ptr;
	struct if_msghdr*		ifm;
	struct sockaddr_dl*		sdl;
	
	mib[0] = CTL_NET;
	mib[1] = AF_ROUTE;
	mib[2] = 0;
	mib[3] = AF_LINK;
	mib[4] = NET_RT_IFLIST;
	
	if((mib[5] = if_nametoindex("en0")) == 0) {
		return NULL;
	}
	
	if(sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
		return NULL;
	}
	
	if((buf = (char*)malloc(len)) == NULL) {
		return NULL;
	}
	
	if(sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
		return NULL;
	}
	
	ifm = (struct if_msghdr *)buf;
	sdl = (struct sockaddr_dl *)(ifm + 1);
	ptr = (unsigned char *)LLADDR(sdl);
	sprintf(addr, "mac%02x%02x%02x%02x%02x%02x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
	free(buf);
	return addr;
}

const char* get_ios_udid(char* addr)
{
	NSString* udid;
	if(![UIDevice instancesRespondToSelector:@selector(uniqueIdentifier)]) return NULL;
	udid = [[UIDevice currentDevice] uniqueIdentifier];
	if(!udid) return NULL;
	strcpy(addr, "udid");
	strcat(addr, [udid UTF8String]);
	return addr;
}

const char* get_random_deviceid(char* addr)
{
    unsigned char result[16];
    const char *cStr = [[[NSProcessInfo processInfo] globallyUniqueString] UTF8String];
    CC_MD5(cStr, strlen(cStr), result);
	sprintf(addr, "rand%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
            result[0], result[1], result[2], result[3], 
            result[4], result[5], result[6], result[7],
            result[8], result[9], result[10], result[11],
            result[12], result[13], result[14], result[15]
            );
	return addr;
}

bool save_to_local(const char* addr)
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    char path[1000];
    sprintf(path, "%s/mo_udid.txt", [documentsDirectory UTF8String]);
    FILE* fp = fopen(path, "wt");
    if(fp==NULL) return false;
    fputs(addr, fp);
    fclose(fp);
	return true;
}

bool load_from_local(char* addr, int len)
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    char path[1000];
    sprintf(path, "%s/mo_udid.txt", [documentsDirectory UTF8String]);
    FILE* fp = fopen(path, "rt");
    if(fp==NULL) return false;
    addr = fgets(addr, len, fp);
    fclose(fp);
	return addr!=NULL;
}

#endif
