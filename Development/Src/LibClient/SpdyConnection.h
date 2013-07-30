#ifndef __ZION_SPDY_CONNECTION__
#define __ZION_SPDY_CONNECTION__

namespace Zion
{

	class CSpdyConnection : public CClientConnectionBase
	{
	public:
		CSpdyConnection(CClient* pClient);
		virtual ~CSpdyConnection();
	};

}

#endif
