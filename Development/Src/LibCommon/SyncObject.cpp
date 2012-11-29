#include <AtlasBase.h>

#include "SyncCommon.h"
#include "SyncObject.h"
#include "SyncMonitor.h"
#include "SyncProxy.h"

namespace Atlas
{
	namespace Sync
	{

		CSyncObject::CSyncObject(const DDLReflect::STRUCT_INFO* info, const void* data)
		{
			m_nTypeId = Atlas::Sync::GetTypeId(info->name);
			ATLAS_ASSERT(m_nTypeId!=(_U16)-1);
			m_pInfo = info;
			m_pData = (const _U8*)data;
			m_pMonitor = NULL;
		}

		CSyncObject::~CSyncObject()
		{
			ATLAS_ASSERT(m_pMonitor==NULL);
			ATLAS_ASSERT(m_mapProxys.empty());
		}

		bool CSyncObject::IsDirty()
		{
			return m_bFullUpdate || m_DirtyArea.size()>0;
		}
		
		void CSyncObject::SetDirty(_U32 start, _U32 len)
		{
			if(m_bFullUpdate) return;

			_U32 end = start + len;
			ATLAS_ASSERT(start<m_pInfo->size && end<=m_pInfo->size);
			ATLAS_ASSERT(len>0);
			if(len==0) return;

			std::list<std::pair<_U32, _U32>>::iterator i;
			for(i = m_DirtyArea.begin(); i!=m_DirtyArea.end(); i++)
			{
				if(start<=i->second)
				{
					break;
				}
			}

			if(i==m_DirtyArea.end() || end<i->first)
			{
				m_DirtyArea.insert(i, std::pair<_U32, _U32>(start, end));
				return;
			}
			if(start<i->first) i->first = start;
			if(end<=i->second) return;

			start = i->first;
			i->second = end;
			std::list<std::pair<_U32, _U32>>::iterator cur = (i++);
			while(i!=m_DirtyArea.end())
			{
				if(end<i->first) break;
				if(end<=i->second)
				{
					cur->second = i->second;
					m_DirtyArea.erase(i);
					break;
				}
				else
				{
					m_DirtyArea.erase(i++);
				}
			}
		}

		void CSyncObject::Clear()
		{
			m_DirtyArea.clear();
			m_bFullUpdate = false;
		}

		void CSyncObject::SetMonitor(CSyncMonitor* pMonitor)
		{
			m_pMonitor = pMonitor;
		}

		void CSyncObject::AddProxy(CSyncProxy* pProxy)
		{
			ATLAS_ASSERT(m_mapProxys.find(pProxy)==m_mapProxys.end());
			m_mapProxys[pProxy] = true;
		}

		void CSyncObject::DelProxy(CSyncProxy* pProxy)
		{
			ATLAS_ASSERT(m_mapProxys.find(pProxy)!=m_mapProxys.end());
			m_mapProxys.erase(pProxy);
		}

/*
		bool CSyncObject::IsDirty()
		{
			return m_bIsFullUpdate || m_DirtyArea.size()>0;
		}

		void CSyncObject::SetDirty(_U32 start, _U32 len)
		{
		}

		void CSyncObject::FullUpdate()
		{
			m_DirtyArea.clear();
			m_bIsFullUpdate = true;
		}

		void CSyncObject::DirtyReset()
		{
			m_bIsFullUpdate = false;
			m_DirtyArea.clear();
		}

		bool CSyncObject::WriteDirty(DDL::BufferWriter& w)
		{
			if(m_bIsFullUpdate)
			{
				return WriteFULL(w);
			}
			else
			{
				ATLAS_ASSERT(m_DirtyArea.size()>0);
				if(!w.Write<_U32>((_U32)m_DirtyArea.size())) return false;
				std::list<std::pair<_U32, _U32>>::iterator i;
				for(i=m_DirtyArea.begin(); i!=m_DirtyArea.end(); i++)
				{
					ATLAS_ASSERT(i->first<m_Len && i->second<=m_Len-i->first);
					ATLAS_ASSERT(i->first<i->second);
					if(i->first<m_Len && i->second<=m_Len-i->first && i->first<i->second)
					{
						if(!w.Write<_U32>(i->first)) return false;
						if(!w.Write<_U32>(i->second-i->first)) return false;
						if(!w.WriteData(m_Data+i->first, i->second-i->first)) return false;
					}
					else
					{
						if(!w.Write<_U32>(0)) return false;
						if(!w.Write<_U32>(0)) return false;
					}
				}
				return true;
			}
		}

		bool CSyncObject::WriteFULL(DDL::BufferWriter& w)
		{
			if(!w.Write<_U32>(1)) return false;
			if(!w.Write<_U32>(0)) return false;
			if(!w.Write<_U32>(m_Len)) return false;
			if(!w.WriteData(m_Data, m_Len)) return false;
			return true;
		}

		bool CSyncObject::Read(DDL::BufferReader& r)
		{
			_U32 count, start, len;
			if(!r.Read<_U32>(count)) return false;
			for(_U32 i=0; i<count; i++)
			{
				if(!r.Read<_U32>(start)) return false;
				if(!r.Read<_U32>(len)) return false;
				ATLAS_ASSERT(len>0);

				ATLAS_ASSERT(start<m_Len && start+len<=m_Len);
				if(start<m_Len && start+len<=m_Len)
				{
					if(!r.ReadBuffer(m_Data+start, len)) return false;
				}
				else
				{
					if(!r.ReadBuffer(len)) return false;
				}
			}
			return true;
		}
*/
	}
}
