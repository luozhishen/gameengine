#include <AtlasBase.h>
#include "AtlasClient.h"
#include "AtlasClientApp.h"
#include "NonblockConnection.h"

namespace Atlas
{

	CNonblockConnection::CNonblockConnection(CClient* pClient) : CClientConnection(pClient)
	{
	}

	CNonblockConnection::~CNonblockConnection()
	{
	}

	bool CNonblockConnection::Connect(const SOCKADDR& sa)
	{
		return false;
	}

	void CNonblockConnection::Disconnect()
	{
	}

	void CNonblockConnection::SendData(_U32 len, const _U8* data, bool bPending)
	{
	}

}
