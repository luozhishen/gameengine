#ifndef __SGGAME_CLIENT_APP__
#define __SGGAME_CLIENT_APP__

namespace Atlas
{

	class CSGClientApp : public Atlas::CClientApp
	{
	public:
		CSGClientApp(bool bThread, _U32 nThreadCount=1);
		virtual ~CSGClientApp();

		virtual Atlas::CClient* NewClient();
	};

}

#endif
