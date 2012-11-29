#include <AtlasBase.h>

#include "SyncCommon.h"
#include "SyncDomain.h"

namespace Atlas
{
	namespace Sync
	{

		CSyncDomain::CSyncDomain(_U16 domain)
		{
			m_nDomainId = domain;
		}

		CSyncDomain::~CSyncDomain()
		{
			std::map<CSyncObjectId, std::pair<const DDLReflect::STRUCT_INFO*, void*>>::iterator i;
			for(i=m_mapObjects.begin(); i!=m_mapObjects.end(); i++)
			{
				DDLReflect::DestoryObject(i->second.second);
			}
			m_mapObjects.clear();
		}
		
		void* CSyncDomain::CreateObject(_U32 id, _U16 index, const DDLReflect::STRUCT_INFO* info)
		{
			CSyncObjectId oid(m_nDomainId, id, index);
			if(m_mapObjects.find(oid)!=m_mapObjects.end()) return NULL;
			void* obj = DDLReflect::CreateObject(info);
			if(!obj) return NULL;
			m_mapObjects[oid] = std::pair<const DDLReflect::STRUCT_INFO*, void*>(info, obj);
			return obj;
		}

		bool CSyncDomain::DestoryObject(_U32 id, _U16 index)
		{
			std::map<CSyncObjectId, std::pair<const DDLReflect::STRUCT_INFO*, void*>>::iterator i;
			i = m_mapObjects.find(CSyncObjectId(m_nDomainId, id, index));
			if(i==m_mapObjects.end()) return false;
			DDLReflect::DestoryObject(i->second.second);
			m_mapObjects.erase(i);
			return true;
		}

		bool CSyncDomain::UpdateObject(_U32 id, _U16 index, _U32 len, const void* data)
		{
			std::map<CSyncObjectId, std::pair<const DDLReflect::STRUCT_INFO*, void*>>::iterator i;
			i = m_mapObjects.find(CSyncObjectId(m_nDomainId, id, index));
			if(i==m_mapObjects.end()) return false;
			return true;
		}

		bool CSyncDomain::FullUpdateObject(_U32 id, _U16 index, _U32 len, const _U8* data)
		{
			std::map<CSyncObjectId, std::pair<const DDLReflect::STRUCT_INFO*, void*>>::iterator i;
			i = m_mapObjects.find(CSyncObjectId(m_nDomainId, id, index));
			if(i==m_mapObjects.end()) return false;
			return true;
		}

		const void* CSyncDomain::GetObjectData(_U32 id, _U16 index, const DDLReflect::STRUCT_INFO* info)
		{
			std::map<CSyncObjectId, std::pair<const DDLReflect::STRUCT_INFO*, void*>>::iterator i;
			i = m_mapObjects.find(CSyncObjectId(m_nDomainId, id, index));
			if(i==m_mapObjects.end()) return NULL;
			return i->second.second;
		}

		const DDLReflect::STRUCT_INFO* CSyncDomain::GetObjectType(_U32 id, _U16 index)
		{
			std::map<CSyncObjectId, std::pair<const DDLReflect::STRUCT_INFO*, void*>>::iterator i;
			i = m_mapObjects.find(CSyncObjectId(m_nDomainId, id, index));
			if(i==m_mapObjects.end()) return NULL;
			return i->second.first;
		}

		void CSyncDomain::GetList(std::vector<CSyncObjectId>& list)
		{
			list.clear();
			std::map<CSyncObjectId, std::pair<const DDLReflect::STRUCT_INFO*, void*>>::iterator i;
			for(i=m_mapObjects.begin(); i!=m_mapObjects.end(); i++)
			{
				list.push_back(i->first);
			}
		}

		CSyncDomainManager::CSyncDomainManager()
		{
		}

		CSyncDomainManager::~CSyncDomainManager()
		{
			std::map<_U16, CSyncDomain*>::iterator i;
			for(i=m_mapDomain.begin(); i!=m_mapDomain.end(); i++)
			{
				delete i->second;
			}
			m_mapDomain.clear();
		}

		bool CSyncDomainManager::IsDomainExist(_U16 domain)
		{
			return m_mapDomain.find(domain)!=m_mapDomain.end();
		}

		void CSyncDomainManager::GetDomainList(std::vector<_U16>& list)
		{
			list.clear();

			std::map<_U16, CSyncDomain*>::iterator i;
			for(i=m_mapDomain.begin(); i!=m_mapDomain.end(); i++)
			{
				list.push_back(i->first);
			}
		}

		bool CSyncDomainManager::IsObjectExist(_U16 domain, _U32 id, _U16 index)
		{
			std::map<_U16, CSyncDomain*>::iterator i;
			if(i==m_mapDomain.end()) return false;
			return i->second->GetObjectData(id, index, NULL)!=NULL;
		}

		const DDLReflect::STRUCT_INFO* CSyncDomainManager::GetObjectType(_U16 domain, _U32 id, _U16 index)
		{
			std::map<_U16, CSyncDomain*>::iterator i;
			if(i==m_mapDomain.end()) return NULL;
			return i->second->GetObjectType(id, index);
		}

		const void* CSyncDomainManager::GetObjectData(_U16 domain, _U32 id, _U16 index, const DDLReflect::STRUCT_INFO* info)
		{
			std::map<_U16, CSyncDomain*>::iterator i;
			if(i==m_mapDomain.end()) return NULL;
			return i->second->GetObjectData(id, index, info);
		}

		bool CSyncDomainManager::CreateDomain(_U16 domain)
		{
			if(m_mapDomain.find(domain)!=m_mapDomain.end()) return false;
			CSyncDomain* pDomain = ATLAS_NEW CSyncDomain(domain);
			if(!pDomain) return false;
			m_mapDomain[domain] = pDomain;
			return true;
		}

		bool CSyncDomainManager::DeleteDomain(_U16 domain)
		{
			std::map<_U16, CSyncDomain*>::iterator i;
			if(i==m_mapDomain.end()) return false;
			delete i->second;
			m_mapDomain.erase(i);
			return true;
		}

		bool CSyncDomainManager::CreateObject(_U16 domain, _U32 id, _U16 index, _U16 type, _U32 len, const _U8* data)
		{
			std::map<_U16, CSyncDomain*>::iterator i;
			if(i==m_mapDomain.end()) return false;
			const DDLReflect::STRUCT_INFO* info = GetType(type);
			if(!info) return false;
			void* obj = i->second->CreateObject(id, index, info);
			if(!obj) return false;
			return true;
		}

		bool CSyncDomainManager::UpdateObject(_U16 domain, _U32 id, _U16 index, _U32 len, const _U8* data)
		{
			std::map<_U16, CSyncDomain*>::iterator i;
			if(i==m_mapDomain.end()) return false;
			return i->second->UpdateObject(id, index, len, data);
		}

		bool CSyncDomainManager::FullUpdateObject(_U16 domain, _U32 id, _U16 index, _U32 len, const _U8* data)
		{
			std::map<_U16, CSyncDomain*>::iterator i;
			if(i==m_mapDomain.end()) return false;
			return i->second->FullUpdateObject(id, index, len, data);
		}

		bool CSyncDomainManager::DeleteObject(_U32 domain, _U32 id, _U16 index)
		{
			std::map<_U16, CSyncDomain*>::iterator i;
			if(i==m_mapDomain.end()) return false;
			return i->second->DestoryObject(id, index);
		}

	}
}
