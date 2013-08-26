#ifndef __HTTP_CLIENT_CONNECTION__
#define __HTTP_CLIENT_CONNECTION__

#include <mosdk.h>

namespace Zion
{

	class CHttpConnection : public CClientConnectionBase
	{
	public:
		enum STATE
		{
			STATE_PAUSE,
			STATE_RUNNING,
			STATE_RETRY,
		};

		typedef std::tr1::function<void (STATE)> STATE_CALLBACK;

		CHttpConnection(CClient* pClient);
		virtual ~CHttpConnection();

		virtual void Tick();

		virtual bool Login(const char* pUrl, const char* pToken);
		virtual void Logout();
		virtual void SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data);

		void Retry();
		void Cancel();
		void SetStateCallback(STATE_CALLBACK callback);

	protected:
		void ProcessLoginRequest();
		void ProcessLogoutRequest();
		void ProcessQueueRequest();
		void ProcessPullRequest();
		int ProcessRequest(MOREQUEST* request);
		void SendRequest();

	private:
		bool m_bPullRequest;
		Zion::String m_BaseUrl;
		MOREQUEST* m_pLoginRequest;
		MOREQUEST* m_pLogoutRequest;
		MOREQUEST* m_pPullRequest;
		MOREQUEST* m_pCurrentRequest;
		Zion::String m_SessionKey;
		Zion::List<Zion::String> m_SendQueue;
		STATE m_nHttpState;
		STATE_CALLBACK m_StateCallback;
		Zion::String m_LastRequestString;
		_U32 m_nRequestSeq, m_nPullSeq, m_nLogoutRetry;
		bool m_bInLogout;
	};

}

#endif
