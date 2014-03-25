#include <ZionBase.h>
#include <JsonRPC.h>
#include "DataCacheRpcImpl.h"
#include "DataCacheDBApi.h"
#include <uv.h>

namespace Zion
{
	namespace DataCache
	{

		class CAvatarData;
		class CAvatarManager
		{
		public:
			CAvatarManager();
			~CAvatarManager();
			CAvatarData* GetAvatar(_U32 avatar_id);
			bool SetAvatar(_U32 avatar_id, CAvatarData* pAvatar);
			Map<_U32, CAvatarData*> g_AvatarMap;
			uv_rwlock_t g_AvatarLocker;
		};

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
			const OBJECT_DATA* GetObject(const A_UUID& _uuid);

			bool IsDirty();
			_U32 GetVersion();
			_U32 GetAvatarID() { return m_AvatarID; }

			bool Load();
			bool Save();
			void Send();

			void Lock();
			void Unlock();
			void Delete();
			bool IsDeleted();

		private:
			A_MUTEX m_Mutex;
			bool m_bDeleted;

			_U32 m_AvatarID;
			_U32 m_Version;
			Map<A_UUID, OBJECT_DATA> m_Objects;
			Set<A_UUID> m_DelList;
		};

		static CAvatarManager g_Manager;

		CAvatarManager::CAvatarManager()
		{
			uv_rwlock_init(&g_AvatarLocker);
		}

		CAvatarManager::~CAvatarManager()
		{
			uv_rwlock_destroy(&g_AvatarLocker);
		}

		CAvatarData* CAvatarManager::GetAvatar(_U32 avatar_id)
		{
			CAvatarData* pAvatar = NULL;
			Map<_U32, CAvatarData*>::iterator i;
			uv_rwlock_rdlock(&g_AvatarLocker);
			i = g_AvatarMap.find(avatar_id);
			if(i!=g_AvatarMap.end() && !i->second->IsDeleted())
			{
				pAvatar = i->second;
				pAvatar->Lock();
				if(pAvatar->IsDeleted())
				{
					pAvatar->Unlock();
					pAvatar = NULL;
				}
			}
			uv_rwlock_rdunlock(&g_AvatarLocker);
			return pAvatar;
		}

		bool CAvatarManager::SetAvatar(_U32 avatar_id, CAvatarData* pAvatar)
		{
			Map<_U32, CAvatarData*>::iterator i;
			bool result = false;
			uv_rwlock_wrlock(&g_AvatarLocker);
			i = g_AvatarMap.find(avatar_id);
			if(i==g_AvatarMap.end())
			{
				g_AvatarMap[avatar_id] = pAvatar;
				result = true;
			}
			else
			{
				if(i->second->IsDeleted())
				{
					i->second = pAvatar;
					result = true;
				}
			}
			uv_rwlock_wrunlock(&g_AvatarLocker);
			return result;
		}

		CAvatarData::CAvatarData(_U32 avatar_id)
		{
			m_AvatarID = avatar_id;
			A_MUTEX_INIT(&m_Mutex);
			m_bDeleted = false;
			m_Version = 0;
		}

		CAvatarData::~CAvatarData()
		{
			A_MUTEX_DESTROY(&m_Mutex);
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
			m_Version += 1;
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
			m_Version += 1;
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
			m_Version += 1;
			return true;
		}

		bool CAvatarData::ExistObject(const A_UUID& _uuid)
		{
			return m_Objects.find(_uuid)!=m_Objects.end() || m_DelList.find(_uuid)!=m_DelList.end();
		}

		const CAvatarData::OBJECT_DATA* CAvatarData::GetObject(const A_UUID& _uuid)
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

		_U32 CAvatarData::GetVersion()
		{
			return m_Version;
		}

		static bool db_load_callback(void* userptr, const A_UUID& _uuid, const char* type, const char* data)
		{
			((CAvatarData*)userptr)->CreateObject(_uuid, type, data, false, false);
			return true;
		}

		bool CAvatarData::Load()
		{
			IDBApi* db = AllocDataBase();
			if(db)
			{
				if(db->LoadAvatar(m_AvatarID, db_load_callback, this))
				{
					Map<A_UUID, OBJECT_DATA>::iterator i;
					for(i=m_Objects.begin(); i!=m_Objects.end(); i++)
					{
						i->second._dirty = false;
						i->second._is_new = false;
					}
					FreeDatabase(db);
					return true;
				}
				FreeDatabase(db);
			}
			return false;
		}

		bool CAvatarData::Save()
		{
			IDBApi* db = AllocDataBase();
			while(db)
			{
				Set<A_UUID>::iterator i1;
				while(!m_DelList.empty())
				{
					if(!db->DeleteAvatarObject(m_AvatarID, &(*m_DelList.begin()), 1))
					{
						break;
					}
					m_DelList.erase(m_DelList.begin());
				}

				Map<A_UUID, OBJECT_DATA>::iterator i;
				for(i=m_Objects.begin(); i!=m_Objects.end(); i++)
				{
					if(i->second._is_new)
					{
						if(!db->InsertAvatarObject(m_AvatarID, i->second._uuid, i->second._type.c_str(), i->second._data.c_str()))
						{
							break;
						}
						i->second._is_new = false;
						i->second._dirty = false;
						continue;
					}

					if(i->second._dirty)
					{
						if(!db->UpdateAvatarObject(m_AvatarID, i->second._uuid, i->second._data.c_str()))
						{
							break;
						}
						i->second._dirty = false;
						continue;
					}
				}
				
				FreeDatabase(db);
				return true;
			}
			if(db)
			{
				FreeDatabase(db);
			}
			return false;
		}

		void CAvatarData::Send()
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
			JsonRPC_Send((StringFormat("[0, %u, %u, {", m_AvatarID, m_Version) + ret + "}]").c_str());
		}

		void CAvatarData::Lock()
		{
			A_MUTEX_LOCK(&m_Mutex);
		}

		void CAvatarData::Unlock()
		{
			A_MUTEX_UNLOCK(&m_Mutex);
		}
		
		void CAvatarData::Delete()
		{
			_U32 avatar_id = m_AvatarID;
			CAvatarData* pAvatar = this;
			ZION_ASSERT(!m_bDeleted);
			m_bDeleted = true;
			A_MUTEX_UNLOCK(&m_Mutex);

			Map<_U32, CAvatarData*>::iterator i;
			uv_rwlock_wrlock(&g_Manager.g_AvatarLocker);
			i = g_Manager.g_AvatarMap.find(avatar_id);
			if(i!=g_Manager.g_AvatarMap.end() && i->second==pAvatar)
			{
				g_Manager.g_AvatarMap.erase(i);
			}
			uv_rwlock_wrunlock(&g_Manager.g_AvatarLocker);

			delete pAvatar;
		}

		bool CAvatarData::IsDeleted()
		{
			return m_bDeleted;
		}

		void RPCIMPL_LoginUser(const char* token)
		{
			IDBApi* db = AllocDataBase();
			if(db)
			{
				_U32 user_id = db->LoginUser(token, "ip", "dv_type", "os_type", "dv_id");
				if(user_id!=(_U32)-1)
				{
					FreeDatabase(db);
					JsonRPC_Send(StringFormat("[0,%u]", user_id).c_str());
					return;
				}
				FreeDatabase(db);
			}
			JsonRPC_Send("[-1]");
			return;
		}

		void RPCIMPL_CreateAvatar(
				_U32 user_id,
				_U32 server_id,
				const char* avatar_name,
				const char* avatar_desc,
				const Array<A_UUID>& uuids,
				const Array<String>& types,
				const Array<String>& datas)
		{
			IDBApi* db = AllocDataBase();
			if(db)
			{
				if(uuids.size()==types.size() && uuids.size()==datas.size())
				{
					_U32 avatar_id = db->CreateAvatar(user_id, server_id, avatar_name, avatar_desc);
					if(avatar_id!=(_U32)-1)
					{
						size_t i;
						for(i=0; i<uuids.size(); i++)
						{
							if(!db->InsertAvatarObject(avatar_id, uuids[i], types[i].c_str(), datas[i].c_str()))
							{
								break;
							}
						}
						if(i==uuids.size())
						{
							JsonRPC_Send(StringFormat("[0,%u]", avatar_id).c_str());
							FreeDatabase(db);
							return;
						}
					}
				}
				FreeDatabase(db);
			}
			JsonRPC_Send("[-1]");
			return;
		}

		void RPCIMPL_DeleteAvatar(_U32 avatar_id)
		{
			IDBApi* db = AllocDataBase();
			if(db)
			{
				if(db->DeleteAvatar(avatar_id))
				{
					JsonRPC_Send("[0]");
					FreeDatabase(db);
					return;
				}
				FreeDatabase(db);
			}
			JsonRPC_Send("[-1]");
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

		void RPCIMPL_GetAvatarList(_U32 user_id, _U32 server_id)
		{
			IDBApi* db = AllocDataBase();
			if(db)
			{
				String val;
				if(db->GetAvatarList(user_id, server_id, GetAvatarListCallback, &val))
				{
					JsonRPC_Send(StringFormat("[0,[%s]]", val.c_str()).c_str());
					FreeDatabase(db);
					return;
				}
				FreeDatabase(db);
			}
			JsonRPC_Send("[-1]");
			return;
		}

		void RPCIMPL_GetAvatar(_U32 avatar_id)
		{
			CAvatarData* pAvatar = g_Manager.GetAvatar(avatar_id);
			if(!pAvatar)
			{
				pAvatar = ZION_NEW CAvatarData(avatar_id);
				pAvatar->Lock();
				if(!pAvatar->Load() || !g_Manager.SetAvatar(avatar_id, pAvatar))
				{
					pAvatar->Unlock();
					ZION_DELETE pAvatar;
					JsonRPC_Send("[-1]");
					return;
				}
			}
			pAvatar->Send();
			pAvatar->Unlock();
		}
		
		void RPCIMPL_SaveAvatar(_U32 avatar_id)
		{
			CAvatarData* pAvatar = g_Manager.GetAvatar(avatar_id);
			if(pAvatar)
			{
				JsonRPC_Send("[-1]");
				return;
			}

			if(pAvatar->Save())
			{
				pAvatar->Unlock();
				JsonRPC_Send("[0]");
			}
			else
			{
				pAvatar->Unlock();
				JsonRPC_Send("[-1]");
			}
		}

		void RPCIMPL_ClearAvatar(_U32 avatar_id)
		{
			CAvatarData* pAvatar = g_Manager.GetAvatar(avatar_id);
			if(!pAvatar)
			{
				JsonRPC_Send("[-1]");
				return;
			}

			if(pAvatar->Save())
			{
				pAvatar->Unlock();
				JsonRPC_Send("[0]");
			}
			else
			{
				pAvatar->Unlock();
				JsonRPC_Send("[-1]");
			}
		}

		void RPCIMPL_KeepAlive(_U32 avatar_id)
		{
			CAvatarData* pAvatar = g_Manager.GetAvatar(avatar_id);
			if(!pAvatar)
			{
				JsonRPC_Send("[-1]");
				return;
			}
			JsonRPC_Send(StringFormat("[0,%u]", pAvatar->GetVersion()).c_str());;
		}

		void RPCIMPL_CreateObject(_U32 avatar_id, _U32 version, const A_UUID& _uuid, const char* type, const char* data)
		{
			CAvatarData* pAvatar = g_Manager.GetAvatar(avatar_id);
			if(!pAvatar)
			{
				JsonRPC_Send("[-1]");
				return;
			}
			if(pAvatar->GetVersion()!=version && version!=(_U32)-1)
			{
				JsonRPC_Send("[-1]");
				return;
			}

			if(pAvatar->CreateObject(_uuid, type, data, false, true))
			{
				pAvatar->Unlock();
				JsonRPC_Send(StringFormat("[0,%u]", pAvatar->GetVersion()).c_str());
			}
			else
			{
				pAvatar->Unlock();
				JsonRPC_Send("[-1]");
			}
		}

		void RPCIMPL_UpdateObject(_U32 avatar_id, _U32 version, const A_UUID& _uuid, const char* data)
		{
			CAvatarData* pAvatar = g_Manager.GetAvatar(avatar_id);
			if(!pAvatar)
			{
				JsonRPC_Send("[-1]");
				return;
			}
			if(pAvatar->GetVersion()!=version && version!=(_U32)-1)
			{
				JsonRPC_Send("[-1]");
				return;
			}

			if(pAvatar->UpdateObject(_uuid, data))
			{
				pAvatar->Unlock();
				JsonRPC_Send(StringFormat("[0,%u]", pAvatar->GetVersion()).c_str());
			}
			else
			{
				pAvatar->Unlock();
				JsonRPC_Send("[-1]");
			}
		}

		void RPCIMPL_DeleteObject(_U32 avatar_id, _U32 version, const A_UUID* _uuids, _U32 count)
		{
			CAvatarData* pAvatar = g_Manager.GetAvatar(avatar_id);
			if(!pAvatar)
			{
				JsonRPC_Send("[-1]");
				return;
			}
			if(pAvatar->GetVersion()!=version && version!=(_U32)-1)
			{
				JsonRPC_Send("[-1]");
				return;
			}

			for(_U32 l=0; l<count; l++)
			{
				pAvatar->DeleteObject(_uuids[l]);
			}

			pAvatar->Unlock();
			JsonRPC_Send(StringFormat("[0,%u]", pAvatar->GetVersion()).c_str());
		}

		void RPCIMPL_LoadObjectFromDB(_U32 avatar_id, const A_UUID& _uuid)
		{
			CAvatarData* pAvatar = g_Manager.GetAvatar(avatar_id);
			if(!pAvatar)
			{
				JsonRPC_Send("[-1]");
				return;
			}

			if(!pAvatar->ExistObject(_uuid))
			{
				IDBApi* db = AllocDataBase();
				if(db)
				{
					if(db->QueryAvatarObject(avatar_id, _uuid, db_load_callback, pAvatar))
					{
						const CAvatarData::OBJECT_DATA* pObject = pAvatar->GetObject(_uuid);
						if(pObject)
						{
							char suuid[100];
							AUuidToString(_uuid, suuid);
							JsonRPC_Send(StringFormat("[0, %u, \"%s\", \"%s\", \"%s\"]", pAvatar->GetVersion(), suuid, pObject->_type.c_str(), pObject->_data.c_str()).c_str());
							pAvatar->Unlock();
							FreeDatabase(db);
							return;
						}
					}
					FreeDatabase(db);
				}
			}
			pAvatar->Unlock();
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_FlushAllData()
		{
			Map<_U32, CAvatarData*>::iterator i;
			for(i=g_Manager.g_AvatarMap.begin(); i!=g_Manager.g_AvatarMap.end(); i++)
			{
				if(!i->second->Save())
				{
					printf("save avatar(%u) failed", i->second->GetAvatarID());
				}
			}
		}

	}
}
