#ifndef __NONBLOCK_CONNECTION__
#define __NONBLOCK_CONNECTION__

namespace Atlas
{

	class CNonblockConnection : CClientConnection
	{
	public:
		CNonblockConnection(CClient* pClient);
		virtual ~CNonblockConnection();

		virtual bool Connect(const SOCKADDR& sa);
		virtual void Disconnect();
		virtual void SendData(_U32 len, const _U8* data, bool bPending);
	};

}

#endif
