#include "ZionBase.h"
#include "ZionCommon.h"
#include "ZionClient.h"
#include "DataSyncClient.h"

namespace Zion
{

	CDataSyncClient::CDataSyncClient(CClient* pClient) : CClientComponent(pClient), c2s(pClient)
	{
		pClient->RegisterStub(new DDLStub::DATASYNC_S2C<CDataSyncClient>(this));
	}


	void CDataSyncClient::SyncStart()
	{
	}

	void CDataSyncClient::SyncReady()
	{
	}

	void CDataSyncClient::SyncDisable()
	{
	}

	void CDataSyncClient::CreateObjectDone(const A_UUID& _uuid)
	{
	}

	void CDataSyncClient::CreateObject(const A_UUID& _uuid, const char* type, const char* data)
	{
	}

	void CDataSyncClient::RemoveObject(const A_UUID& _uuid)
	{
	}

	void CDataSyncClient::UpdateObject(const A_UUID& _uuid, const char* data)
	{
	}

}
