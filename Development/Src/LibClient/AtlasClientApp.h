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
		// 0 LoginDone 1 LoginFailed 2 Disconnect 3 OnData
		_U32 nCode;
		CClient* pClient;
		_U16 iid;
		_U16 fid;
		_U32 len;
		_U8* data;
	};

	class CClientApp : public CNoCopy
	{
	public:
		CClientApp(bool bThread);
		virtual ~CClientApp();

		void SetPacketSize(_U32 sendsize, _U32 recvsize);
		_U32 GetUpPacketSize();
		_U32 GetDownPacketSize();

		void SetParam(const char* name, const char* value);
		const char* GetParam(const char* name, const char* default_value=NULL);
		const std::map<std::string, std::string>& GetParams();
		bool LoadParams(const char* filename);
		bool SaveParams(const char* filename);

		static CClientApp* GetDefault();
		virtual void InitApp();
		virtual void FiniApp();
		virtual CClient* NewClient();

		bool IsThread() { return m_bThread; }

		void EnableTick(bool bEnable=true);
		bool Tick(_U32 nTime=0);
		bool IsEnableTick() { return m_bEnableTick; }

		void QueueLoginDone(CClient* pClient);
		void QueueLoginFailed(CClient* pClient);
		void QueueDisconnected(CClient* pClient);
		void QueueData(CClient* pClient, _U16 iid, _U16 fid, _U32 len, const _U8* data);
		void QueueTask(CClient* pClient, CClientTask* pTask);

	private:
		std::map<std::string, std::string> m_Params;
		A_MUTEX m_mtxQueue;
		std::list<CLIENTAPP_ITEM> m_Queue;
		bool m_bThread;
		bool m_bEnableTick;
		_U32 m_nRecvSize, m_nSendSize;
	};

}

#endif
