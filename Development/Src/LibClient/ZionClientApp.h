#ifndef __ZION_CLIENT_APP__
#define __ZION_CLIENT_APP__

#include <list>

namespace Zion
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
		friend class CClient;
	public:
		CClientApp(const char* appname, bool bThread);
		virtual ~CClientApp();

		void SetPacketSize(_U32 sendsize, _U32 recvsize);
		_U32 GetUpPacketSize();
		_U32 GetDownPacketSize();

		void SetParam(const char* name, const char* value);
		const char* GetParam(const char* name, const char* default_value=NULL);
		const Zion::Map<Zion::String, Zion::String>& GetParams();
		bool LoadParams(const char* path = NULL);
		bool SaveParams();

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

	protected:
		void RegisterClient(CClient* pClient);
		void UnregisterClient(CClient* pClient);

	private:
		Zion::Map<Zion::String, Zion::String> m_Params;
		A_MUTEX m_mtxQueue;
		Zion::List<CLIENTAPP_ITEM> m_Queue;
		bool m_bThread;
		bool m_bEnableTick;
		_U32 m_nRecvSize, m_nSendSize;
		Zion::Set<CClient*> m_Clients;
	};

}

#endif
