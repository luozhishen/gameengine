#ifndef __CARDGAME_CLIENT_APP__
#define __CARDGAME_CLIENT_APP__

namespace Zion
{

	class CCardClientApp : public Zion::CClientApp
	{
	public:
		CCardClientApp(bool bThread);
		virtual ~CCardClientApp();

		virtual Zion::CClient* NewClient();
	};

}

#endif
