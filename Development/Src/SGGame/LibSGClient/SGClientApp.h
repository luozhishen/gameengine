#ifndef __SGGAME_CLIENT_APP__
#define __SGGAME_CLIENT_APP__

namespace Atlas
{

	class CSGClientApp : public Atlas::CClientApp
	{
	public:
		CSGClientApp(bool bThread);
		virtual ~CSGClientApp();

		virtual Atlas::CClient* NewClient();
	};

}

#endif
