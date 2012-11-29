#ifndef __ATLAS_WORKER_SERVER__
#define __ATLAS_WORKER_SERVER__

namespace Atlas
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
