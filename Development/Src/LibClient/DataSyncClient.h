#ifndef __ZION_CLIENT_DATASYNC__
#define __ZION_CLIENT_DATASYNC__

#include <DataSyncDDL.h>

namespace Zion
{

	class CDataSyncClient : public CClientComponent
	{
	public:
		CDataSyncClient(CClient* pClient);

		A_LIVE_OBJECT* CreateObject(const DDLReflect::STRUCT_INFO* pInfo);
		void RemoveObject(const A_UUID& _uuid);
		LiveObject::CObject* GetObject(const A_UUID& _uuid);

		template<typename T>
		T* CreateObject()
		{
			return (T*)CreateObject(DDLReflect::GetStruct<T>());
		}

		// DATASYNC_S2C
		void DS_SyncOpen(_U32 flag);
		void DS_SyncReady();
		void DS_SyncClose();
		void DS_CreateObjectDone(const A_UUID& _uuid);
		void DS_CreateObject(const A_UUID& _uuid, const char* type, const char* data);
		void DS_CreateObject(const A_UUID& _uuid, const char* type, const _U8* data, _U32 len);
		void DS_RemoveObject(const A_UUID& _uuid);
		void DS_UpdateObject(const A_UUID& _uuid, const char* data);
		void DS_UpdateObject(const A_UUID& _uuid, const _U8* data, _U32 len);

	private:
		DDLProxy::DATASYNC_C2S<CClient, DDL::TMemoryWriter<1024>> c2s;
	};

}

#endif
