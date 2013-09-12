#ifndef __NONBLOCK_CONNECTION__
#define __NONBLOCK_CONNECTION__

namespace Zion
{

	class CNonblockConnection : public CClientConnection
	{
	public:
		CNonblockConnection(CClient* pClient, _U32 recvsize);
		virtual ~CNonblockConnection();

		virtual void Tick();

		virtual bool Connect(const SOCK_ADDR& sa);
		virtual void Disconnect();
		virtual void SendData(_U32 len, const _U8* data, bool bPending);

		void Clear();

	private:
		bool m_bNeedDisconnect, m_bConnecting;
		SOCK_HANDLE m_hSocket;
		List<std::pair<_U8*, _U32> > m_SendQueue;
		_U32 m_nSendLen;
	};

}

#endif
