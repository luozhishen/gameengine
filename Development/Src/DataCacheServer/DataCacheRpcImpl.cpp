#include <ZionBase.h>
#include <JsonRPC.h>
#include "DataCacheRpcImpl.h"
#include "DataCacheDBApi.h"

namespace Zion
{
	namespace DataCache
	{

		class CAvatarData
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

			CAvatarData(_U32 avatar_id);
			~CAvatarData();

			bool CreateObject(const A_UUID& _uuid, const char* type, const char* data, bool is_new, bool is_dirty);
			bool UpdateObject(const A_UUID& _uuid, const char* data);
			bool DeleteObject(const A_UUID& _uuid);
			bool ExistObject(const A_UUID& _uuid);
			OBJECT_DATA* GetObject(const A_UUID& _uuid);

			bool IsDirty();
			_U32 GetAvatarID() { return m_AvatarID; }

			bool Load();
			bool Save();
			void Send(const JSONRPC_RESPONSE* res);

		private:
			_U32 m_AvatarID;
			Map<A_UUID, OBJECT_DATA> m_Objects;
			Set<A_UUID> m_DelList;
		};

		CAvatarData::CAvatarData(_U32 avatar_id)
		{
			m_AvatarID = avatar_id;
		}

		CAvatarData::~CAvatarData()
		{
		}

		bool CAvatarData::CreateObject(const A_UUID& _uuid, const char* type, const char* data, bool is_new, bool is_dirty)
		{
			if(m_Objects.find(_uuid)!=m_Objects.end())
			{
				ZION_ASSERT(0);
				return false;
			}

			OBJECT_DATA od = { _uuid, type, data, is_dirty, is_new };
			m_Objects[_uuid] = od;
			return true;
		}

		bool CAvatarData::UpdateObject(const A_UUID& _uuid, const char* data)
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

		bool CAvatarData::DeleteObject(const A_UUID& _uuid)
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

		bool CAvatarData::ExistObject(const A_UUID& _uuid)
		{
			return m_Objects.find(_uuid)!=m_Objects.end() || m_DelList.find(_uuid)!=m_DelList.end();
		}

		CAvatarData::OBJECT_DATA* CAvatarData::GetObject(const A_UUID& _uuid)
		{
			Map<A_UUID, OBJECT_DATA>::iterator i = m_Objects.find(_uuid);
			if(i==m_Objects.end()) return NULL;
			return &i->second;
		}

		bool CAvatarData::IsDirty()
		{
			if(!m_DelList.empty()) return true;

			Map<A_UUID, OBJECT_DATA>::iterator i;
			for(i=m_Objects.begin(); i!=m_Objects.end(); i++)
			{
				if(i->second._dirty || i->second._is_new) return true;
			}
			return false;
		}

		static bool db_load_callback(void* userptr, const A_UUID& _uuid, const char* type, const char* data)
		{
			((CAvatarData*)userptr)->CreateObject(_uuid, type, data, false, false);
			return true;
		}

		bool CAvatarData::Load()
		{
			if(!LoadAvatar(m_AvatarID, db_load_callback, this))
			{
				return false;
			}

			Map<A_UUID, OBJECT_DATA>::iterator i;
			for(i=m_Objects.begin(); i!=m_Objects.end(); i++)
			{
				i->second._dirty = false;
				i->second._is_new = false;
			}
			return true;
		}

		bool CAvatarData::Save()
		{
			Set<A_UUID>::iterator i1;
			while(!m_DelList.empty())
			{
				if(!DeleteAvatarObject(m_AvatarID, &(*m_DelList.begin()), 1))
				{
					return false;
				}

				m_DelList.erase(m_DelList.begin());
			}

			Map<A_UUID, OBJECT_DATA>::iterator i;
			for(i=m_Objects.begin(); i!=m_Objects.end(); i++)
			{
				if(i->second._is_new)
				{
					if(!InsertAvatarObject(m_AvatarID, i->second._uuid, i->second._type.c_str(), i->second._data.c_str()))
					{
						return false;
					}
					i->second._is_new = false;
					i->second._dirty = false;
					continue;
				}

				if(i->second._dirty)
				{
					if(!UpdateAvatarObject(m_AvatarID, i->second._uuid, i->second._data.c_str()))
					{
						return false;
					}
					i->second._dirty = false;
					continue;
				}
			}

			return true;
		}

		void CAvatarData::Send(const JSONRPC_RESPONSE* res)
		{
			String ret;
			bool first = true;
			Map<A_UUID, OBJECT_DATA>::iterator i;
			for(i=m_Objects.begin(); i!=m_Objects.end(); i++)
			{
				if(!ret.empty()) ret += ",";

				char suuid[100];
				AUuidToString(i->second._uuid, suuid);
				ret += StringFormat("\"%s:%s\":", suuid, i->second._type.c_str());
				ret += i->second._data;
			}
			JsonRPC_Send(res, ("[0, {" + ret + "}]").c_str());
		}

		Map<_U32, CAvatarData*> g_AvatarMap;

		void RPCIMPL_LoginUser(const JSONRPC_RESPONSE* res, const char* token)
		{
			_U32 user_id = LoginUser(token);
			if(user_id!=(_U32)-1)
			{
				JsonRPC_Send(res, StringFormat("[0,%u]", user_id).c_str());
			}
			else
			{
				JsonRPC_Send(res, "[-1]");
			}
			return;
		}

		void RPCIMPL_CreateAvatar(
				const JSONRPC_RESPONSE* res,
				_U32 user_id,
				_U32 server_id,
				const char* avatar_name,
				const char* avatar_desc,
				const Array<A_UUID>& uuids,
				const Array<String>& types,
				const Array<String>& datas)
		{
			if(uuids.size()==types.size() && uuids.size()==datas.size())
			{
				_U32 avatar_id = CreateAvatar(user_id, server_id, avatar_name, avatar_desc);
				if(avatar_id!=(_U32)-1)
				{
					size_t i;
					for(i=0; i<uuids.size(); i++)
					{
						if(!InsertAvatarObject(avatar_id, uuids[i], types[i].c_str(), datas[i].c_str()))
						{
							break;
						}
					}
					if(i==uuids.size())
					{
						JsonRPC_Send(res, StringFormat("[0,%u]", avatar_id).c_str());
						return;
					}
				}
			}
			JsonRPC_Send(res, "[-1]");
			return;
		}

		void RPCIMPL_DeleteAvatar(const JSONRPC_RESPONSE* res, _U32 avatar_id)
		{
			if(DeleteAvatar(avatar_id))
			{
				JsonRPC_Send(res, "[0]");
				return;
			}
			JsonRPC_Send(res, "[-1]");
			return;
		}

		static bool GetAvatarListCallback(void* userptr, _U32 avatar_id, _U32 state, const char* avatar_name, const char* avatar_desc)
		{
			String& val = *((String*)userptr);
			if(!val.empty()) val += ",";
			val += '{';
			val += StringFormat("\"%s\":%u", "avatar_id", avatar_id);
			val += ",";
			val += StringFormat("\"%s\":%u", "state", state);
			val += ",";
			val += StringFormat("\"%s\":\"%s\"", "avatar_name", avatar_name);
			val += ",";
			val += StringFormat("\"%s\":\"%s\"", "avatar_desc", avatar_desc);
			val += '}';
			return true;
		}

		void RPCIMPL_GetAvatarList(const JSONRPC_RESPONSE* res, _U32 user_id, _U32 server_id)
		{
			String val;
			if(!GetAvatarList(user_id, server_id, GetAvatarListCallback, &val))
			{
				JsonRPC_Send(res, "[-1]");
			}
			else
			{
				JsonRPC_Send(res, StringFormat("[0,[%s]]", val.c_str()).c_str());
			}
			return;
		}

		void RPCIMPL_GetAvatar(const JSONRPC_RESPONSE* res, _U32 avatar_id)
		{
			CAvatarData* pAvatar = NULL;
			Map<_U32, CAvatarData*>::iterator i = g_AvatarMap.find(avatar_id);
			if(i==g_AvatarMap.end())
			{
				pAvatar = ZION_NEW CAvatarData(avatar_id);
				if(!pAvatar->Load())
				{
					ZION_DELETE pAvatar;
					JsonRPC_Send(res, "[-1]");
					return;
				}
				g_AvatarMap[avatar_id] = pAvatar;
			}
			else
			{
				pAvatar = i->second;
			}
			pAvatar->Send(res);
		}
		
		void RPCIMPL_SaveAvatar(const JSONRPC_RESPONSE* res, _U32 avatar_id)
		{
			Map<_U32, CAvatarData*>::iterator i = g_AvatarMap.find(avatar_id);
			if(i==g_AvatarMap.end())
			{
				JsonRPC_Send(res, "[-1]");
				return;
			}

			if(!i->second->Save())
			{
				JsonRPC_Send(res, "[-1]");
				return;
			}

			JsonRPC_Send(res, "[0]");
		}

		void RPCIMPL_ClearAvatar(const JSONRPC_RESPONSE* res, _U32 avatar_id)
		{
			Map<_U32, CAvatarData*>::iterator i = g_AvatarMap.find(avatar_id);
			if(i==g_AvatarMap.end())
			{
				JsonRPC_Send(res, "[-1]");
				return;
			}

			if(!i->second->Save())
			{
				JsonRPC_Send(res, "[-1]");
				return;
			}

			JsonRPC_Send(res, "[0]");
		}

		void RPCIMPL_KeepAlive(const JSONRPC_RESPONSE* res, _U32 avatar_id)
		{
			JsonRPC_Send(res, "[0]");
		}

		void RPCIMPL_CreateObject(const JSONRPC_RESPONSE* res, _U32 avatar_id, const A_UUID& _uuid, const char* type, const char* data)
		{
			Map<_U32, CAvatarData*>::iterator i = g_AvatarMap.find(avatar_id);
			if(i!=g_AvatarMap.end())
			{
				CAvatarData* pAvatar = i->second;
				if(pAvatar->CreateObject(_uuid, type, data, false, true))
				{
					JsonRPC_Send(res, "[0]");
					return;
				}
			}
			JsonRPC_Send(res, "[-1]");
		}

		void RPCIMPL_UpdateObject(const JSONRPC_RESPONSE* res, _U32 avatar_id, const A_UUID& _uuid, const char* data)
		{
			Map<_U32, CAvatarData*>::iterator i = g_AvatarMap.find(avatar_id);
			if(i!=g_AvatarMap.end())
			{
				CAvatarData* pAvatar = i->second;
				if(pAvatar->UpdateObject(_uuid, data))
				{
					JsonRPC_Send(res, "[0]");
					return;
				}
			}
			JsonRPC_Send(res, "[-1]");
		}

		void RPCIMPL_DeleteObject(const JSONRPC_RESPONSE* res, _U32 avatar_id, const A_UUID* _uuids, _U32 count)
		{
			Map<_U32, CAvatarData*>::iterator i = g_AvatarMap.find(avatar_id);
			if(i!=g_AvatarMap.end())
			{
				CAvatarData* pAvatar = i->second;
				for(_U32 l=0; l<count; l++)
				{
					pAvatar->DeleteObject(_uuids[l]);
				}
				JsonRPC_Send(res, "[0]");
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void RPCIMPL_LoadObjectFromDB(const JSONRPC_RESPONSE* res, _U32 avatar_id, const A_UUID& _uuid);
		void RPCIMPL_LoadObjectFromDB(const JSONRPC_RESPONSE* res, _U32 avatar_id, const A_UUID& _uuid)
		{
			Map<_U32, CAvatarData*>::iterator i = g_AvatarMap.find(avatar_id);
			if(i!=g_AvatarMap.end())
			{
				CAvatarData* pAvatar = i->second;
				if(!pAvatar->ExistObject(_uuid))
				{
					if(QueryAvatarObject(avatar_id, _uuid, db_load_callback, pAvatar))
					{
						CAvatarData::OBJECT_DATA* pObject = pAvatar->GetObject(_uuid);
						if(pObject)
						{
							char suuid[100];
							AUuidToString(_uuid, suuid);
							JsonRPC_Send(res, StringFormat("[0, \"%s\", \"%s\", \"%s\"]", suuid, pObject->_type.c_str(), pObject->_data.c_str()).c_str());
							return;
						}
					}
				}
			}
			JsonRPC_Send(res, "[-1]");
		}

		void RPCIMPL_FlushAllData()
		{
			Map<_U32, CAvatarData*>::iterator i;
			for(i=g_AvatarMap.begin(); i!=g_AvatarMap.end(); i++)
			{
				if(!i->second->Save())
				{
					printf("save avatar(%u) failed", i->second->GetAvatarID());
				}
			}
		}

	}
}
