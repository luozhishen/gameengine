#include <AtlasBase.h>

#include "SyncCommon.h"
#include "SyncObject.h"
#include "SyncProxy.h"

namespace Atlas
{
	namespace Sync
	{

		CSyncObjectState::CSyncObjectState() : m_ObjectId(), m_SyncState(SYNC_STATE_CREATE)
		{
		}

		CSyncObjectState::CSyncObjectState(const CSyncObjectState& state) : m_ObjectId(state.m_ObjectId), m_SyncState(state.m_SyncState)
		{
		}

		CSyncObjectState::CSyncObjectState(const CSyncObjectId& oid, CSyncObject* obj) : m_ObjectId(oid), m_pObject(obj), m_SyncState(SYNC_STATE_CREATE)
		{
		}

		CSyncProxy::CSyncProxy()
		{
		}

		CSyncProxy::~CSyncProxy()
		{
			DetachAll();
		}

		bool CSyncProxy::CreateDomain(_U16 domain)
		{
			std::map<_U16, SYNC_STATE>::iterator i;
			i = m_mapDomains.find(domain);
			if(i!=m_mapDomains.end())
			{
				if(i->second!=SYNC_STATE_DESTORY)
				{
					ATLAS_ASSERT(0);
					return false;
				}
				i->second = SYNC_STATE_RECREATE;
			}
			else
			{
				m_mapDomains[domain] = SYNC_STATE_CREATE;
			}
			return true;
		}

		bool CSyncProxy::DestoryDomain(_U16 domain)
		{
			std::map<_U16, SYNC_STATE>::iterator i;
			i = m_mapDomains.find(domain);
			if(i==m_mapDomains.end())
			{
				ATLAS_ASSERT(0);
				return false;
			}
			i->second = SYNC_STATE_DESTORY;
			return true;
		}

		bool CSyncProxy::IsExistDomain(_U16 domain)
		{
			return m_mapDomains.find(domain)!=m_mapDomains.end();
		}

		bool CSyncProxy::Attach(_U16 domain, _U32 id, _U16 index, CSyncObject* pObject)
		{
			if(!IsExistDomain(domain))
			{
				ATLAS_ASSERT(0);
				return false;
			}

			CSyncObjectId oid(domain, id, index);
			std::map<CSyncObjectId, CSyncObjectState>::iterator i;
			i = m_mapObjects.find(oid);
			if(i==m_mapObjects.end())
			{
				m_mapObjects[oid] = CSyncObjectState(oid, pObject);
			}
			else
			{
				ATLAS_ASSERT(i->second.m_SyncState==SYNC_STATE_DESTORY);
				if(i->second.m_SyncState!=SYNC_STATE_DESTORY) return false;
				i->second.m_SyncState = SYNC_STATE_RECREATE;
				i->second.m_pObject = pObject;
			}
			pObject->AddProxy(this);
			return true;
		}

		bool CSyncProxy::Detach(_U16 domain, _U32 id, _U16 index)
		{
			CSyncObjectId oid(domain, id, index);
			std::map<CSyncObjectId, CSyncObjectState>::iterator i;
			i = m_mapObjects.find(oid);
			if(i==m_mapObjects.end())
			{
				ATLAS_ASSERT(0);
				return false;
			}
			i->second.m_SyncState = SYNC_STATE_DESTORY;
			if(i->second.m_pObject)
			{
				i->second.m_pObject->DelProxy(this);
				i->second.m_pObject = NULL;
			}
			return true;
		}

		void CSyncProxy::DetachAll()
		{
			std::map<CSyncObjectId, CSyncObjectState>::iterator i;
			for(i=m_mapObjects.begin(); i!=m_mapObjects.end(); i++)
			{
				if(i->second.m_SyncState!=SYNC_STATE_DESTORY) continue;
				const CSyncObjectId& oid = m_mapObjects.begin()->first;
				Detach(oid.GetDomain(), oid.GetId(), oid.GetIndex());
			}
			ClearDetach();
			ATLAS_ASSERT(m_mapObjects.empty());
		}

		void CSyncProxy::ClearDetach()
		{
			std::map<CSyncObjectId, CSyncObjectState>::iterator i;
			for(i=m_mapObjects.end(); i!=m_mapObjects.begin(); i--)
			{

			}
		}

	}
}
