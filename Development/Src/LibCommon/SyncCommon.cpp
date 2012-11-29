#include <AtlasBase.h>

#include "CommonDDL.h"
#include "SyncCommon.h"
#include "ContentObject.h"
#include "LiveObject.h"

namespace Atlas
{
	namespace Sync
	{

		CSyncObjectId::CSyncObjectId()
		{
			_oid = (_U64)-1;
		}

		CSyncObjectId::CSyncObjectId(const CSyncObjectId& id)
		{
			_oid = id._oid;
		}

		CSyncObjectId::CSyncObjectId(_U16 domain, _U32 id, _U16 index)
		{
			_oid = (((_U64)domain)<<48) | (((_U64)index)<<32) | (_U64)id;
		}

		_U16 CSyncObjectId::GetDomain() const
		{
			return (_U16)(_oid>>48);
		}

		_U32 CSyncObjectId::GetId() const
		{
			return (_U32)((_oid>>32)&0xffffffff);
		}

		_U16 CSyncObjectId::GetIndex() const
		{
			return (_U16)((_oid>>32)&0xffff);
		}

		bool CSyncObjectId::operator<(const CSyncObjectId& id) const
		{
			return _oid<id._oid;
		}

		static std::map<std::string, std::pair<int, const DDLReflect::STRUCT_INFO*>> g_type_map;
		static std::map<_U16, const DDLReflect::STRUCT_INFO*> g_typeid_map;
		static _U16 g_typeid_max = 0x3000;

		bool Register(const DDLReflect::STRUCT_INFO* info)
		{
			if(!DDLReflect::IsParent(info, DDLReflect::GetStruct<A_SYNC_OBJECT>()))
			{
				ATLAS_ASSERT(0);
				return false;
			}

			ATLAS_ASSERT(g_type_map.find(info->name)==g_type_map.end());
			if(g_type_map.find(info->name)!=g_type_map.end()) return false;
			g_type_map[info->name] = std::pair<int, const DDLReflect::STRUCT_INFO*>(g_typeid_max, info);
			g_typeid_map[g_typeid_max++] = info;
			return true;
		}

		void GetTypeList(std::vector<const DDLReflect::STRUCT_INFO*>& list)
		{
			list.clear();
			std::map<_U16, const DDLReflect::STRUCT_INFO*>::const_iterator i;
			for(i=g_typeid_map.begin(); i!=g_typeid_map.end(); i++)
			{
				list.push_back(i->second);
			}
		}

		_U16 GetTypeId(const char* name)
		{
			std::map<std::string, std::pair<int, const DDLReflect::STRUCT_INFO*>>::const_iterator i;
			i = g_type_map.find(name);
			if(i!=g_type_map.end())
			{
				return (_U16)(i->second.first);
			}
			_U16 ret;
			ret = Atlas::ContentObject::GetTypeId(name);
			if(ret==(_U16)-1)
			{
				ret = Atlas::LiveObject::GetTypeId(name);
			}
			return ret;
		}

		const DDLReflect::STRUCT_INFO* GetType(const char* name)
		{
			std::map<std::string, std::pair<int, const DDLReflect::STRUCT_INFO*>>::const_iterator i;
			i = g_type_map.find(name);
			if(i==g_type_map.end()) return NULL;
			return i->second.second;
		}

		const DDLReflect::STRUCT_INFO* GetType(_U16 id)
		{
			std::map<_U16, const DDLReflect::STRUCT_INFO*>::const_iterator i;
			i = g_typeid_map.find(id);
			if(i==g_typeid_map.end()) return NULL;
			return i->second;
		}

	}
}
