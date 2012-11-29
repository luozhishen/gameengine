#ifndef __ATLAS_SYNC_PROXY__
#define __ATLAS_SYNC_PROXY__

namespace Atlas
{
	namespace Sync
	{

		enum SYNC_STATE
		{
			SYNC_STATE_CREATE,
			SYNC_STATE_RECREATE,
			SYNC_STATE_DESTORY,
			SYNC_STATE_NORMAL,
		};

		class CSyncObjectState
		{
		public:
			CSyncObjectState();
			CSyncObjectState(const CSyncObjectState& state);
			CSyncObjectState(const CSyncObjectId& oid, CSyncObject* obj);
			
			CSyncObjectId m_ObjectId;
			CSyncObject* m_pObject;
			SYNC_STATE m_SyncState;
		};

		class CSyncObject;

		class CSyncProxy
		{
		public:
			CSyncProxy();
			~CSyncProxy();

			bool CreateDomain(_U16 domain);
			bool DestoryDomain(_U16 domain);
			bool IsExistDomain(_U16 domain);

			bool Attach(_U16 domain, _U32 id, _U16 index, CSyncObject* pObject);
			bool Detach(_U16 domain, _U32 id, _U16 index);
			void DetachAll();
			void ClearDetach();

		private:
			std::map<CSyncObjectId, CSyncObjectState> m_mapObjects;
			std::map<_U16, SYNC_STATE> m_mapDomains;
		};

	}
}

#endif
