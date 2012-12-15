#ifndef __HTTP_CLIENT_CONNECTION__
#define __HTTP_CLIENT_CONNECTION__

#include <mosdk.h>

namespace Atlas
{

	class CHttpClientConnection : public CClientConnectionBase
	{
	public:
		CHttpClientConnection(CClient* pClient);
		virtual ~CHttpClientConnection();

		virtual void Tick();

		virtual bool Login(const SOCK_ADDR& sa, _U32 nUID, const char* pToken);
		virtual void Logout();
		virtual void SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data);

	private:
		MOREQUEST* m_pLoginRequest;
		MOREQUEST* m_pNotifyRequest;
		std::string m_SessionKey;
		std::list<std::string> m_SendQueue;
	};

}

#endif
