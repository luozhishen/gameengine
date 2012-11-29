#include <AtlasBase.h>

#include "SyncObject.h"
#include "SyncMonitor.h"

namespace Atlas
{
	namespace Sync
	{
		
		CSyncMonitor::CSyncMonitor()
		{
		}

		CSyncMonitor::~CSyncMonitor()
		{
			while(m_mapObjects.size()>0)
			{
				Detach(m_mapObjects.begin()->first);
			}
		}

		bool CSyncMonitor::Attach(CSyncObject* pObject)
		{
			ATLAS_ASSERT(m_mapObjects.find(pObject)==m_mapObjects.end());
			pObject->SetMonitor(this);
			m_mapObjects[pObject] = true;
			return true;
		}

		bool CSyncMonitor::Detach(CSyncObject* pObject)
		{
			ATLAS_ASSERT(m_mapObjects.find(pObject)!=m_mapObjects.end());
			m_mapObjects.erase(pObject);
			return true;
		}

		bool CSyncMonitor::SetDirty(CSyncObject* pObject)
		{
			std::map<CSyncObject*, bool>::iterator i;
			i = m_mapObjects.find(pObject);
			ATLAS_ASSERT(i!=m_mapObjects.end());
			if(i==m_mapObjects.end()) return false;
			i->second = true;
			return true;
		}

	}
}
