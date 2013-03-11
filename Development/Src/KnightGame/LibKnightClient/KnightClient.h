#ifndef __KNIGHTGAME_CLIENT__
#define __KNIGHTGAME_CLIENT__

namespace Atlas
{

	class CKnightClientCallback
	{
	public:
		virtual ~CKnightClientCallback();
	};

	class CKnightClient : public CClient
	{
	public:
		CKnightClient(CClientApp* pClientApp, _U32 recvsize=6*1024);
		virtual ~CKnightClient();
	};

}

#endif //__KNIGHTGAME_CLIENT__
