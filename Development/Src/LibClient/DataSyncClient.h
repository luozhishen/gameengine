#ifndef __ZION_CLIENT_DATASYNC__
#define __ZION_CLIENT_DATASYNC__

#include <DataSyncDDL.h>

#ifdef GetObject
#undef GetObject
#endif

namespace Zion
{

	class CDataSyncClient : public CClientComponent, public sigslot::has_slots<>, public IGameObjectContainer
	{
	public:
		CDataSyncClient(CClient* pClient);
		~CDataSyncClient();

		virtual bool GetGameObject(const A_UUID& _uuid, const DDLReflect::STRUCT_INFO*& pType, const A_LIVE_OBJECT*& pData);
		bool GetGameObject(CGameObject& Object, const A_UUID& _uuid);

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
		bool IsReady();
		_U32 GetSyncFlag();
		LiveData::CAccesser& GetAccesser();

		// DATASYNC_S2C
		void DS_SyncOpen(_U32 flag);
		void DS_SyncReady();
		void DS_SyncClose();
		void DS_CreateObjectDone(const A_UUID& _uuid);
		void DS_CreateObject(const char* type, const char* data);
		void DS_CreateObject(_U16 type, const _U8* data, _U32 len);
		void DS_UpdateObject(const A_UUID& _uuid, const char* data);
		void DS_UpdateObject(const A_UUID& _uuid, const _U8* data, _U32 len);
		void DS_DeleteObject(const A_UUID* _uuids, _U32 count);

		//
		void OnDisconnected();

		sigslot::signal0<>								_OnSyncOpen;
		sigslot::signal0<>								_OnSyncClose;
		sigslot::signal1<const A_UUID&>					_OnObjectCreate;
		sigslot::signal1<const A_UUID&>					_OnObjectUpdate;
		sigslot::signal1<const A_UUID&>					_OnObjectDelete;

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
