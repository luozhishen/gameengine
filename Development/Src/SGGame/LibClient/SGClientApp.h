#ifndef __SGGAME_CLIENT_APP__
#define __SGGAME_CLIENT_APP__

namespace Zion
{

	class CSGClientApp : public Zion::CClientApp
	{
	public:
		CSGClientApp(bool bThread);
		virtual ~CSGClientApp();

		virtual Zion::CClient* NewClient();
	};

}

#endif
