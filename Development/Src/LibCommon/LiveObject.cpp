#include <ZionBase.h>
#include "CommonDDL.h"
#include "LiveObject.h"

namespace Zion
{
	namespace LiveObject
	{

		CLiveMonitor::CLiveMonitor(const DDLReflect::STRUCT_INFO* info)
		{
			m_pInfo = info;
			m_bDirty = false;
		}

		bool CLiveMonitor::IsDirty()
		{
			return m_bDirty;
		}

		void CLiveMonitor::SetDirty()
		{
			m_bDirty = true;
		}

		void CLiveMonitor::SetDirty(const char* name, _U32 offset, _U32 size)
		{
			m_bDirty = true;
		}

		void CLiveMonitor::Clean()
		{
			m_bDirty = false;
		}

		CLiveObject::CLiveObject(CLiveManager* pManager, const DDLReflect::STRUCT_INFO* pInfo, A_LIVE_OBJECT* pData) : m_Monitor(pInfo), DDLDataObject::CObject(pInfo, pData, &m_Monitor, "", 0)
		{
		}

		bool CLiveObject::IsDirty()
		{
			return m_Monitor.IsDirty();
		}

		void CLiveObject::Clean()
		{
			return m_Monitor.Clean();
		}

		CLiveManager::CLiveManager()
		{
		}

		CLiveObject* CLiveManager::Append(const DDLReflect::STRUCT_INFO* pInfo)
		{
			A_UUID _uuid;
			AUuidGenerate(_uuid);
			if(m_ObjMap.find(_uuid)==m_ObjMap.end())
			{
				ZION_ASSERT(!"live object generate duplicate uuid");
				return NULL;
			}

			A_LIVE_OBJECT* data = (A_LIVE_OBJECT*)DDLReflect::CreateObject(pInfo);
			data->_uuid = _uuid;
			CLiveObject* obj = ZION_NEW CLiveObject(this, pInfo, data);
			m_ObjMap[data->_uuid] = obj;
			m_NewList.insert(data->_uuid);
			return NULL;
		}

		CLiveObject* CLiveManager::Append(const DDLReflect::STRUCT_INFO* pInfo, const _U8* buf, _U32 len)
		{
			A_LIVE_OBJECT* data = (A_LIVE_OBJECT*)DDLReflect::CreateObject(pInfo);
			DDL::MemoryReader reader(buf, len);
			if(!pInfo->read_proc(reader, data))
			{
				ZION_ASSERT(!"live object serialize binary error");
				DDLReflect::DestoryObject(pInfo, data);
				return NULL;
			}

			if(m_ObjMap.find(data->_uuid)==m_ObjMap.end())
			{
				ZION_ASSERT(!"live object alread existed");
				DDLReflect::DestoryObject(pInfo, data);
				return NULL;
			}

			CLiveObject* obj = ZION_NEW CLiveObject(this, pInfo, data);
			obj->Clean();
			m_ObjMap[data->_uuid] = obj;
			m_NewList.insert(data->_uuid);
			return NULL;
		}

		CLiveObject* CLiveManager::Append(const DDLReflect::STRUCT_INFO* pInfo, const char* str)
		{
			A_LIVE_OBJECT* data = (A_LIVE_OBJECT*)DDLReflect::CreateObject(pInfo);
			String val(str);
			if(!DDLReflect::Struct2Json(pInfo, (_U8*)data, val))
			{
				ZION_ASSERT(!"live object serialize json error");
				DDLReflect::DestoryObject(pInfo, data);
				return NULL;
			}

			if(m_ObjMap.find(data->_uuid)==m_ObjMap.end())
			{
				ZION_ASSERT(!"live object alread existed");
				DDLReflect::DestoryObject(pInfo, data);
				return NULL;
			}

			CLiveObject* obj = ZION_NEW CLiveObject(this, pInfo, data);
			obj->Clean();
			m_ObjMap[data->_uuid] = obj;
			m_NewList.insert(data->_uuid);
			return NULL;
		}

		void CLiveManager::Remove(const A_UUID& _uuid)
		{
			Map<A_UUID, CLiveObject*>::iterator o = m_ObjMap.find(_uuid);
			if(o==m_ObjMap.end())
			{
				ZION_ASSERT(!"object not found");
				return;
			}
			m_ObjMap.erase(o);

			Set<A_UUID>::iterator n = m_NewList.find(_uuid);
			if(n!=m_NewList.end())
			{
				m_NewList.erase(n);
			}
			else
			{
				n = m_DelList.find(_uuid);
				if(n!=m_DelList.end())
				{
					m_NewList.erase(n);
				}
			}
		}

		CLiveObject* CLiveManager::Get(const A_UUID& _uuid)
		{
			Map<A_UUID, CLiveObject*>::iterator o = m_ObjMap.find(_uuid);
			if(o==m_ObjMap.end()) return NULL;
			return o->second;
		}

		CLiveObject* CLiveManager::FindFirst()
		{
			return NULL;
		}

		CLiveObject* CLiveManager::FindNext()
		{
			return NULL;
		}

		static Zion::Map<Zion::String, std::pair<int, const DDLReflect::STRUCT_INFO*> > g_type_map;
		static Zion::Map<_U16, const DDLReflect::STRUCT_INFO*> g_typeid_map;
		static _U16 g_typeid_max = 0x2000;

		bool Register(const DDLReflect::STRUCT_INFO* info)
		{
			if(!DDLReflect::IsParent(info, DDLReflect::GetStruct<A_LIVE_OBJECT>()))
			{
				ZION_ASSERT(0);
				return false;
			}

			ZION_ASSERT(g_type_map.find(info->name)==g_type_map.end());
			if(g_type_map.find(info->name)!=g_type_map.end()) return false;
			g_type_map[info->name] = std::pair<int, const DDLReflect::STRUCT_INFO*>(g_typeid_max, info);
			g_typeid_map[g_typeid_max++] = info;
			return true;
		}

		void GetTypeList(Zion::Array<const DDLReflect::STRUCT_INFO*>& list)
		{
			list.clear();
			Zion::Map<_U16, const DDLReflect::STRUCT_INFO*>::const_iterator i;
			for(i=g_typeid_map.begin(); i!=g_typeid_map.end(); i++)
			{
				list.push_back(i->second);
			}
		}

		_U16 GetTypeId(const char* name)
		{
			Zion::Map<Zion::String, std::pair<int, const DDLReflect::STRUCT_INFO*> >::const_iterator i;
			i = g_type_map.find(name);
			if(i==g_type_map.end()) return (_U16)-1;
			return (_U16)i->second.first;
		}

		const DDLReflect::STRUCT_INFO* GetType(const char* name)
		{
			Zion::Map<Zion::String, std::pair<int, const DDLReflect::STRUCT_INFO*> >::const_iterator i;
			i = g_type_map.find(name);
			if(i==g_type_map.end()) return NULL;
			return i->second.second;
		}

		const DDLReflect::STRUCT_INFO* GetType(_U16 id)
		{
			Zion::Map<_U16, const DDLReflect::STRUCT_INFO*>::const_iterator i;
			i = g_typeid_map.find(id);
			if(i==g_typeid_map.end()) return NULL;
			return i->second;
		}
	}
}
