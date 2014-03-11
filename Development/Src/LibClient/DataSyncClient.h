#ifndef __ZION_CLIENT_DATASYNC__
#define __ZION_CLIENT_DATASYNC__

#include <DataSyncDDL.h>

namespace Zion
{

	class CDataSyncClient : public CClientComponent
	{
	public:
		CDataSyncClient(CClient* pClient);
		~CDataSyncClient();

		A_LIVE_OBJECT* CreateObject(const DDLReflect::STRUCT_INFO* pInfo);
		bool RemoveObject(const A_UUID& _uuid);
		LiveData::CObject* GetObject(const A_UUID& _uuid);

		template<typename T>
		T* CreateObject()
		{
			return (T*)CreateObject(DDLReflect::GetStruct<T>());
		}

		void Sync();
		bool InProcess();

		// DATASYNC_S2C
		void DS_SetMode(_U32 mode);
		void DS_SyncOpen(_U32 flag);
		void DS_SyncReady();
		void DS_SyncClose();
		void DS_CreateObjectDone(const A_UUID& _uuid);
		void DS_CreateObject(const char* type, const char* data);
		void DS_CreateObject(_U16 type, const _U8* data, _U32 len);
		void DS_UpdateObject(const A_UUID& _uuid, const char* data);
		void DS_UpdateObject(const A_UUID& _uuid, const _U8* data, _U32 len);
		void DS_DeleteObject(const A_UUID* _uuids, _U32 count);

	private:
		struct OBJECT_ADDITEM
		{
			A_LIVE_OBJECT*					obj;
			const DDLReflect::STRUCT_INFO*	info;
		};

		void Clear();

		LiveData::CAccesser m_Accesser;
		_U32 m_Flag;
		bool m_bReady;
		List<OBJECT_ADDITEM> m_NewQ;
		List<OBJECT_ADDITEM> m_WatQ;
		Array<A_UUID> m_DelList;
		DDLProxy::DATASYNC_BINARY_C2S<1024> m_Binary;
		DDLProxy::DATASYNC_JSON_C2S<1024> m_Json;
		LiveData::CAccesser* m_pAccesser;
	};

}

#endif
