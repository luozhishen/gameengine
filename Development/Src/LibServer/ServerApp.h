#ifndef __ZION_SERVER_APP__
#define __ZION_SERVER_APP__

namespace Zion
{
	class CServerTask;
	class CServerApp;
	class CServerBase;

	class CServerTaskManager
	{
	protected:
		CServerTaskManager();
		~CServerTaskManager();

	public:
		static CServerTaskManager& GetSingileton();

		HWORKERS GetHWorkers() { return m_hWorkers; }

		void SetWorkerCount(_U32 nCount);

	private:
		HWORKERS m_hWorkers;
	};

	// 1. ����˳�����߳�ģʽ the thread model of server programme
	// 2. �������������ļ� process config file for server
	// 3. ����������ѡ�� process command line option
	class CServerApp : public CNoCopy
	{
	public:
		CServerApp(_U32 nIOThreadCount=20);
		virtual ~CServerApp();
	
		HWORKERS GetIOWorkers() { return m_hIOWorkers; }

		_U64 Queue(CServerTask* pTask);
		_U64 Queue(HSERVER hServer, CServerTask* pTask);
		bool Cancel(_U64 nTaskID);

		void Run();
		void SetListenAddrPort(const _STR str);
		ASOCK_ADDR& GetListenAddrPort();
		ASOCK_ADDR& GetRPCAddrPort();

		void SetRPCAddrPort(_U32 ip, _U16 port);
		void SetRPCAddrPort(const _STR str);
		void SetClusterRpcEP(const _STR str);
		ASOCK_ADDR& GetClusterRpcEP();

		virtual void InitApp();
		virtual void FiniApp();

	private:
		HWORKERS m_hIOWorkers;
		ASOCK_ADDR m_saRPC;		//rpc binding addr and port of server self
		ASOCK_ADDR m_saListen; //server listen addr and port
		ASOCK_ADDR m_saCluster; //cluster port only for server who communicate with cluster
	};

}

#endif
