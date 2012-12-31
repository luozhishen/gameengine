#ifndef __HTTP_CLIENT_CONNECTION__
#define __HTTP_CLIENT_CONNECTION__

#include <mosdk.h>

namespace Atlas
{

	class CHttpClientConnection : public CClientConnectionBase
	{
	public:
		enum STATE
		{
			STATE_PAUSE,
			STATE_RUNNING,
			STATE_RETRY,
		};

		typedef std::tr1::function<void (STATE)> STATE_CALLBACK;

		CHttpClientConnection(CClient* pClient);
		virtual ~CHttpClientConnection();

		virtual void Tick();

		virtual bool Login(const char* pUrl, const char* pToken);
		virtual void Logout();
		virtual void SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data);

		void Retry();
		void Cancel();
		void SetStateCallback(STATE_CALLBACK callback);

	protected:
		void ProcessLoginRequest();
		void ProcessQueueRequest();
		void SendRequest();
		void DoDisconnect();

	private:
		std::string m_BaseUrl;
		MOREQUEST* m_pLoginRequest;
		MOREQUEST* m_pNotifyRequest;
		MOREQUEST* m_pCurrentRequest;
		std::string m_SessionKey;
		std::list<std::string> m_SendQueue;
		STATE m_nHttpState;
		STATE_CALLBACK m_StateCallback;
	};

}

#endif
