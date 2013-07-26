#include <ZionBase.h>
#include <ZionClient.h>
#include <ZionCommon.h>

#include <time.h>
#include "CardCommon.h"
#include "CardClient.h"

namespace Zion
{

	CCardClient::CCardClient(CClientApp* pClientApp, _U32 recvsize) : CClient(pClientApp, recvsize), c2s(this), s2c(this)
	{
	}	

	CCardClient::~CCardClient()
	{

	}

	void CCardClient::OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		if(iid==GetServerStubID<CARDGAME_C2S>())
		{
			DDL::MemoryReader buf(data, len);
			s2c.Dispatcher(this, fid, buf);
		}
	}

}
