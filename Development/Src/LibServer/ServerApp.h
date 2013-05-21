#ifndef __ATLAS_SERVER_APP__
#define __ATLAS_SERVER_APP__

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

	// 1. 服务端程序的线程模式 the thread model of server programme
	// 2. 处理服务端配置文件 process config file for server
	// 3. 处理命令行选项 process command line option
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
		SOCK_ADDR& GetListenAddrPort();
		SOCK_ADDR& GetRPCAddrPort();

		void SetRPCAddrPort(_U32 ip, _U16 port);
		void SetRPCAddrPort(const _STR str);
		void SetClusterRpcEP(const _STR str);
		SOCK_ADDR& GetClusterRpcEP();

		virtual void InitApp();
		virtual void FiniApp();

	private:
		HWORKERS m_hIOWorkers;
		SOCK_ADDR m_saRPC;		//rpc binding addr and port of server self
		SOCK_ADDR m_saListen; //server listen addr and port
		SOCK_ADDR m_saCluster; //cluster port only for server who communicate with cluster
	};

}

#endif
