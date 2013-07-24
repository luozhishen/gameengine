#ifndef __SGGAME_CLIENT__
#define __SGGAME_CLIENT__

namespace Zion
{

	class CSGClient : public CClient
	{
	public:
		CSGClient(CClientApp* pClientApp, _U32 recvsize=6*1024);
		virtual ~CSGClient();
	};

}

#endif
