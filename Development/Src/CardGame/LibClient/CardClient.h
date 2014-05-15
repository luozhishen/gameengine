#ifndef __CARDGAME_CLIENT__
#define __CARDGAME_CLIENT__

#include <CardGameDDL.h>

namespace Zion
{

	class CCardClient : public CClient
	{
	public:
		CCardClient(CClientApp* pClientApp, _U32 recvsize=6*1024);
		virtual ~CCardClient();

		DDLProxy::CGSERVER_BASE<1024> c2s_base;
		DDLProxy::CGSERVER_GAME<1024> c2s_game;
		DDLSigSlot::CGCALLBACK_BASE s2c_base;
		DDLSigSlot::CGCALLBACK_GAME s2c_game;
	};

}

#endif
