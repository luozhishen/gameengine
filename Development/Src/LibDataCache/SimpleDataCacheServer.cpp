#include <ZionBase.h>
#include <JsonRPC.h>
#include "SimpleDataCacheServer.h"
#include "DataCacheDBApi.h"

namespace Zion
{
	namespace DataCache
	{

		class CSimpleAvatarData
		{
		public:
			struct OBJECT_DATA
			{
				A_UUID _uuid;
				String _type;
				String _data;
				bool _dirty;
				bool _is_new;
			};

			CSimpleAvatarData(_U32 avatar_id);
			~CSimpleAvatarData();

			bool CreateObject(const A_UUID& _uuid, const char* type, const char* data);
			bool UpdateObject(const A_UUID& _uuid, const char* data);
			bool DeleteObject(const A_UUID& _uuid);

			bool Load();
			bool Save();
			void Send(const JSONRPC_RESPONSE& res);

		private:
			_U32 m_AvatarID;
			Map<A_UUID, OBJECT_DATA> m_Objects;
			Set<A_UUID> m_DelList;
		};

		CSimpleAvatarData::CSimpleAvatarData(_U32 avatar_id)
		{
			m_AvatarID = avatar_id;
		}

		CSimpleAvatarData::~CSimpleAvatarData()
		{
		}

		bool CSimpleAvatarData::CreateObject(const A_UUID& _uuid, const char* type, const char* data)
		{
			if(m_Objects.find(_uuid)!=m_Objects.end())
			{
				ZION_ASSERT(0);
				return false;
			}

			OBJECT_DATA od = { _uuid, type, data, false, true };
			m_Objects[_uuid] = od;
			return true;
		}

		bool CSimpleAvatarData::UpdateObject(const A_UUID& _uuid, const char* data)
		{
			Map<A_UUID, OBJECT_DATA>::iterator i = m_Objects.find(_uuid);
			if(i==m_Objects.end())
			{
				ZION_ASSERT(0);
				return false;
			}

			i->second._data = data;
			i->second._dirty = true;
			return true;
		}

		bool CSimpleAvatarData::DeleteObject(const A_UUID& _uuid)
		{
			Map<A_UUID, OBJECT_DATA>::iterator i = m_Objects.find(_uuid);
			if(i==m_Objects.end())
			{
				ZION_ASSERT(0);
				return false;
			}
			if(!i->second._is_new) m_DelList.insert(_uuid);
			m_Objects.erase(i);
			return true;
		}
		
		static bool db_callback(void* userptr, const A_UUID& _uuid, const char* type, const char* data)
		{
			((CSimpleAvatarData*)userptr)->CreateObject(_uuid, type, data);
			return true;
		}

		bool CSimpleAvatarData::Load()
		{
			return LoadAvatar(m_AvatarID, db_callback, this);
		}

		bool CSimpleAvatarData::Save()
		{
			return true;
		}

		void CSimpleAvatarData::Send(const JSONRPC_RESPONSE& res)
		{
			String ret = "{";
			bool first = true;
			Map<A_UUID, OBJECT_DATA>::iterator i;
			for(i=m_Objects.begin(); i!=m_Objects.end(); i++)
			{
				if(!first)
				{
					ret += ",";
				}
				else
				{
					first = false;
				}
				char suuid[100];
				AUuidToString(i->second._uuid, suuid);
				ret += StringFormat("\"%s:%s\":", suuid, i->second._type);
				ret += i->second._data;
			}
			ret = "}";
			JsonRPC_Send(res, 0, ret.c_str());
		}

		Map<_U32, CSimpleAvatarData*> g_AvatarMap;

		void RPCSIMPLE_GetAvatar(const JSONRPC_RESPONSE& res, _U32 avatar_id)
		{
			CSimpleAvatarData* pAvatar = NULL;
			Map<_U32, CSimpleAvatarData*>::iterator i = g_AvatarMap.find(avatar_id);
			if(i==g_AvatarMap.end())
			{
				pAvatar = ZION_NEW CSimpleAvatarData(avatar_id);
				if(!pAvatar->Load())
				{
					ZION_DELETE pAvatar;
					JsonRPC_Send(res, -1);
					return;
				}
			}
			else
			{
				pAvatar = i->second;
			}
			pAvatar->Send(res);
		}

		void RPCSIMPLE_KeepAlive(const JSONRPC_RESPONSE& res, _U32 avatar_id)
		{
			JsonRPC_Send(res, 0);
		}

		void RPCSIMPLE_SaveToDB(const JSONRPC_RESPONSE& res, _U32 avatar_id)
		{
			Map<_U32, CSimpleAvatarData*>::iterator i = g_AvatarMap.find(avatar_id);
			if(i==g_AvatarMap.end())
			{
				JsonRPC_Send(res, -1);
				return;
			}

			if(!i->second->Save())
			{
				JsonRPC_Send(res, -1);
				return;
			}

			JsonRPC_Send(res, 0);
		}

		void RPCSIMPLE_ExecuteDDL(const JSONRPC_RESPONSE& res, _U32 avatar_id, const char* method_name, const _U8* buf, _U32 len)
		{
			JsonRPC_Send(res, 0);
		}

		void RPCSIMPLE_CreateObject(const JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data)
		{
			Map<_U32, CSimpleAvatarData*>::iterator i = g_AvatarMap.find(avatar_id);
			if(i!=g_AvatarMap.end())
			{
				CSimpleAvatarData* pAvatar = i->second;
				if(pAvatar->CreateObject(_uuid, type, data))
				{
					JsonRPC_Send(res, 0);
					return;
				}
			}
			JsonRPC_Send(res, -1);
		}

		void RPCSIMPLE_UpdateObject(const JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID& _uuid, const char* data)
		{
			Map<_U32, CSimpleAvatarData*>::iterator i = g_AvatarMap.find(avatar_id);
			if(i!=g_AvatarMap.end())
			{
				CSimpleAvatarData* pAvatar = i->second;
				if(pAvatar->UpdateObject(_uuid, data))
				{
					JsonRPC_Send(res, 0);
					return;
				}
			}
			JsonRPC_Send(res, -1);
		}

		void RPCSIMPLE_DeleteObject(const JSONRPC_RESPONSE& res, _U32 avatar_id, const A_UUID* _uuids, _U32 count)
		{
			Map<_U32, CSimpleAvatarData*>::iterator i = g_AvatarMap.find(avatar_id);
			if(i!=g_AvatarMap.end())
			{
				CSimpleAvatarData* pAvatar = i->second;
				for(_U32 l=0; l<count; l++)
				{
					pAvatar->DeleteObject(_uuids[l]);
				}
				JsonRPC_Send(res, 0);
			}
			JsonRPC_Send(res, -1);
		}

		void RPCSIMPLE_FlushAllData()
		{
		}

	}
}
