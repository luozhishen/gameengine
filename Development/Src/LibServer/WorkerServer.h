#ifndef __ZION_WORKER_SERVER__
#define __ZION_WORKER_SERVER__

namespace Zion
{

	class CServerTask;

	class CWorkerServer : CServerBase
	{
	public:
		CWorkerServer(CServerApp* pServerApp);
		virtual ~CWorkerServer();
	};

}

#endif
