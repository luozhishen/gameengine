#ifndef __ATLAS_CLIENT_APP__
#define __ATLAS_CLIENT_APP__

#include <list>

namespace Atlas
{

	class CClient;
	class CClientApp;

	class CClientTask
	{
	public:
		CClientTask();
		virtual ~CClientTask();
		virtual void DoTask(CClient* pClient) = 0;
	};

	struct CLIENTAPP_ITEM
	{
		// 0 Connected 1 Disconnect 2 OnData 3 ConnectFailed 4 Client Task
		_U32 nCode;
		CClient* pClient;
		HCONNECT hConn;
		_U32 len;
		_U8* data;
	};

	class CClientApp : public CNoCopy
	{
	public:
		CClientApp(bool bThread, _U32 nThreadCount=1);
		virtual ~CClientApp();

		void SetParam(const char* name, const char* value);
		const char* GetParam(const char* name, const char* default_value=NULL);
		std::map<std::string, std::string>& GetParams();
		bool LoadParams();
		bool SaveParams();

		static CClientApp* GetDefault();
		virtual void InitApp();
		virtual CClient* NewClient();

		bool IsThread() { return m_bThread; }
		HWORKERS GetHWorkers() { return m_hWorkers; }
		HIOPOOL GetIOPool() { return m_hPool; }

		void EnableTick(bool bEnable=true);
		bool Tick(_U32 nTime=0);
		bool IsEnableTick() { return m_bEnableTick; }

		bool QueueTask(CClientTask* pTask, CClient* pClient);

		void OnConnect(CClient* pClient, HCONNECT hConn);
		void OnDisconnect(CClient* pClient, HCONNECT hConn);
		void OnData(CClient* pClient, _U32 len, const _U8* data);
		void OnConnectFailed(CClient* pClient);

	private:
		A_MUTEX m_mtxQueue;
		bool m_bThread;
		HWORKERS m_hWorkers;
		HIOPOOL m_hPool;
		std::list<CLIENTAPP_ITEM> m_Queue;
		bool m_bEnableTick;
		std::map<std::string, std::string> m_Params;
	};

}

#endif
