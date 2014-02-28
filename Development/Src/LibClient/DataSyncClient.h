#ifndef __ZION_CLIENT_DATASYNC__
#define __ZION_CLIENT_DATASYNC__

namespace Zion
{

	class CDataSyncClient : public CClientComponent
	{
	public:

		CDataSyncClient(CClient* pClient);

		//
		bool InSync();
		void SyncStart();

		// api
		void SyncStart(_U16 mode);
		void SyncReady();
		void SyncDisable();
		void CreateObjectDone(const A_UUID& _uuid);
		void CreateObject(const A_UUID& _uuid, const char* type, const char* data);
		void RemoveObject(const A_UUID& _uuid);
		void UpdateObject(const A_UUID& _uuid, const char* data);

	private:
		DDLProxy::DATASYNC_C2S<CClient, DDL::TMemoryWriter<1024>> c2s;
	};

}

#endif
