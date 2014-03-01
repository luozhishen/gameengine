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

	A_LIVE_OBJECT* CDataSyncClient::CreateObject(const DDLReflect::STRUCT_INFO* pInfo)
	{
		if(!DDLReflect::IsChild(DDLReflect::GetStruct<A_LIVE_OBJECT>(), pInfo)) return NULL;
		return NULL;
	}

	void CDataSyncClient::RemoveObject(const A_UUID& _uuid)
	{
	}

	LiveObject::CObject* CDataSyncClient::GetObject(const A_UUID& _uuid)
	{
		return NULL;
	}

	void CDataSyncClient::DS_SyncStart()
	{
	}

	void CDataSyncClient::DS_SyncReady()
	{
	}

	void CDataSyncClient::DS_SyncDisable()
	{
	}

	void CDataSyncClient::DS_CreateObjectDone(const A_UUID& _uuid)
	{
	}

	void CDataSyncClient::DS_CreateObject(const A_UUID& _uuid, const char* type, const char* data)
	{
	}

	void CDataSyncClient::DS_RemoveObject(const A_UUID& _uuid)
	{
	}

	void CDataSyncClient::DS_UpdateObject(const A_UUID& _uuid, const char* data)
	{
	}

}
