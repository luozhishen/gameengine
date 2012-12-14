#ifndef __MOSDK_CLIENT_INCLUDE__
#define __MOSDK_CLIENT_INCLUDE__

struct MOCLIENT;
typedef struct MOCLIENT MOCLIENT;

typedef enum {
	MOCLIENTSTATE_NA,
	MOCLIENTSTATE_LOGINING,
	MOCLIENTSTATE_AUTH,
	MOCLIENTSTATE_FAILED,
	MOCLIENTSTATE_LOGOUTING,
} MOCLIENTSTATE;

MOCLIENT* MOCreateClient(const char* baseurl, const char* prefix=NULL);
void MODestoryClient(MOCLIENT* client);
bool MOLoginByDeviceID(MOCLIENT* client);
bool MOLoginByToken(MOCLIENT* client, const char* token);
bool MOLoginByUsername(MOCLIENT* client, const char* username, const char* password);
void MOLogout(MOCLIENT* client);
MOCLIENTSTATE MOGetClientState(MOCLIENT* client);
const char* MOGetSessionKey(MOCLIENT* client);
const char* MOGetClientBaseUrl(MOCLIENT* client);
MOREQUEST* MOClientRequestString(MOCLIENT* client, const char* method, const std::map<std::string, std::string>& params);
MOREQUEST* MOClientDownloadFile(MOCLIENT* client, const char* method, const std::map<std::string, std::string>& params, const char* path);
MOREQUEST* MOClientUploadFiles(MOCLIENT* client, const char* method, const std::map<std::string, std::string>& files);
MOREQUEST* MOClientUploadFiles(MOCLIENT* client, const char* method, const std::map<std::string, std::string>& params, const std::map<std::string, std::string>& files);
int MOClientGetResultCode(MOREQUEST* request);
const char* MOClientGetResultString(MOREQUEST* request);

#endif // __MOSDK_CLIENT_INCLUDE__
