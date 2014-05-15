#ifndef	__ZION_FASTCGI_SERVER__
#define	__ZION_FASTCGI_SERVER__

#include <functional>

namespace Zion
{

	bool FCGI_StartServer(const char* ep);
	void FCGI_StopServer();

	typedef std::function<void(const char*)>	FCGI_RESPONSE_PROC;
	bool FCGI_Bind(const char* url, FCGI_RESPONSE_PROC proc);

}

#endif

