#ifndef __KNIGHTGAME_CLIENT_APP__
#define __KNIGHTGAME_CLIENT_APP__

namespace Atlas
{

	class CKnightClientApp : public Atlas::CClientApp
	{
	public:
		CKnightClientApp(bool bThread);
		virtual ~CKnightClientApp();

		virtual Atlas::CClient* NewClient();
	};

}

#endif //__KNIGHTGAME_CLIENT_APP__
