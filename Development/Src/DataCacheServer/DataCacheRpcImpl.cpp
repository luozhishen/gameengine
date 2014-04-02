#include <ZionBase.h>
#include <JsonRPC.h>
#include <uv.h>
#include "DataCacheRpcImpl.h"
#include "DataCacheDBApi.h"
#include "ObjectManager.h"

namespace Zion
{
	namespace DataCache
	{


		class CAvatarData;

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

			_U32 GetIndex();
			void SetIndex(_U32 index);

			bool IsDirty();
			_U32 GetVersion();
			_U32 GetAvatarID() { return m_AvatarID; }

			bool Load();
			bool Save();
			void Send();

		private:
			_U32 m_Index;
			_U32 m_AvatarID;
			_U32 m_Version;
			Map<A_UUID, OBJECT_DATA> m_Objects;
			Set<A_UUID> m_DelList;
		};

		static TObjectManager<CAvatarData, 100*10000> g_avatar_manager;
		static TObjectMap<_U32> g_avatar_id_map;

		static _U32 InsertAvatar(CAvatarData* data)
		{
			_U32 index = g_avatar_manager.Insert(data);
			if(index==(_U32)-1) return (_U32)-1;
			_U32 avatar_id = data->GetAvatarID();
			if(!g_avatar_id_map.Insert(avatar_id, index))
			{
				g_avatar_manager.Remove(index, data);
				return (_U32)-1;
			}
			else
			{
				data->SetIndex(index);
			}
			return index;
		}

		static bool RemoveAvatar(CAvatarData* data)
		{
			bool ret;
			_U32 avatar_id = data->GetAvatarID();
			ret = g_avatar_id_map.Remove(avatar_id, data->GetIndex());
			ZION_ASSERT(ret);
			if(ret)
			{
				ret = g_avatar_manager.Remove(data->GetIndex(), data);
				ZION_ASSERT(ret);
			}
			return ret;
		}

		static CAvatarData* LockAvatar(_U32 avatar_id)
		{
			_U32 index = g_avatar_id_map.Get(avatar_id);
			if(index==(_U32)-1) return NULL;
			CAvatarData* data = g_avatar_manager.Lock(index);
			if(data->GetAvatarID()!=avatar_id)
			{
				g_avatar_manager.Unlock(index);
				return NULL;
			}
			return data;
		}

		void UnlockAvatar(CAvatarData* data)
		{
			g_avatar_manager.Unlock(data->GetIndex());
		}

		CAvatarData::CAvatarData(_U32 avatar_id)
		{
			m_AvatarID = avatar_id;
			m_Version = 0;
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

		_U32 CAvatarData::GetIndex()
		{
			return m_Index;
		}

		void CAvatarData::SetIndex(_U32 index)
		{
			m_Index = index;
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

		_U32 _GetAvatar_Count = 0;
		_U32 _GetAvatar_Error = 0;
		void RPCIMPL_GetAvatar(_U32 avatar_id)
		{
			CAvatarData* pAvatar = LockAvatar(avatar_id);
			if(!pAvatar)
			{
				pAvatar = ZION_NEW CAvatarData(avatar_id);
				if(!pAvatar->Load() || !InsertAvatar(pAvatar))
				{
					ZION_DELETE pAvatar;
					JsonRPC_Send("[-1]");
					_GetAvatar_Error += 1;
					return;
				}
			}
			pAvatar->Send();
			UnlockAvatar(pAvatar);
			_GetAvatar_Count += 1;
		}
		
		void RPCIMPL_SaveAvatar(_U32 avatar_id)
		{
			CAvatarData* pAvatar = LockAvatar(avatar_id);
			if(pAvatar)
			{
				JsonRPC_Send("[-1]");
				return;
			}

			if(pAvatar->Save())
			{
				JsonRPC_Send("[0]");
			}
			else
			{
				JsonRPC_Send("[-1]");
			}
			UnlockAvatar(pAvatar);
		}

		void RPCIMPL_KeepAlive(_U32 avatar_id)
		{
			CAvatarData* pAvatar = LockAvatar(avatar_id);
			if(!pAvatar)
			{
				JsonRPC_Send("[-1]");
				return;
			}
			JsonRPC_Send(StringFormat("[0,%u]", pAvatar->GetVersion()).c_str());;
			UnlockAvatar(pAvatar);
		}

		void RPCIMPL_CreateObject(_U32 avatar_id, _U32 version, const A_UUID& _uuid, const char* type, const char* data)
		{
			CAvatarData* pAvatar = LockAvatar(avatar_id);
			if(!pAvatar)
			{
				JsonRPC_Send("[-1]");
				return;
			}
			if(pAvatar->GetVersion()==version)
			{
				if(pAvatar->CreateObject(_uuid, type, data, false, true))
				{
					UnlockAvatar(pAvatar);
					JsonRPC_Send(StringFormat("[0,%u]", pAvatar->GetVersion()).c_str());
					return;
				}
			}
			UnlockAvatar(pAvatar);
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_UpdateObject(_U32 avatar_id, _U32 version, const A_UUID& _uuid, const char* data)
		{
			CAvatarData* pAvatar = LockAvatar(avatar_id);
			if(!pAvatar)
			{
				JsonRPC_Send("[-1]");
				return;
			}
			if(pAvatar->GetVersion()==version)
			{
				if(pAvatar->UpdateObject(_uuid, data))
				{
					UnlockAvatar(pAvatar);
					JsonRPC_Send(StringFormat("[0,%u]", pAvatar->GetVersion()).c_str());
					return;
				}
			}
			UnlockAvatar(pAvatar);
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_DeleteObject(_U32 avatar_id, _U32 version, const A_UUID* _uuids, _U32 count)
		{
			CAvatarData* pAvatar = LockAvatar(avatar_id);
			if(!pAvatar)
			{
				JsonRPC_Send("[-1]");
				return;
			}
			if(pAvatar->GetVersion()==version)
			{
				for(_U32 l=0; l<count; l++)
				{
					pAvatar->DeleteObject(_uuids[l]);
				}
				UnlockAvatar(pAvatar);
				JsonRPC_Send(StringFormat("[0,%u]", pAvatar->GetVersion()).c_str());
				return;
			}
			UnlockAvatar(pAvatar);
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_LoadObjectFromDB(_U32 avatar_id, const A_UUID& _uuid)
		{
			CAvatarData* pAvatar = LockAvatar(avatar_id);
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
							UnlockAvatar(pAvatar);
							FreeDatabase(db);
							return;
						}
					}
					FreeDatabase(db);
				}
			}
			UnlockAvatar(pAvatar);
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_ExecuteBatch(_U32 avatar_id, _U32 version, const Array<TASK>& tasks)
		{
			CAvatarData* pAvatar = LockAvatar(avatar_id);
			if(!pAvatar)
			{
				JsonRPC_Send("[-1]");
				return;
			}

			if(pAvatar->GetVersion()!=version)
			{
				UnlockAvatar(pAvatar);
				JsonRPC_Send("[-1]");
				return;
			}

			IDBApi* db = AllocDataBase();
			if(!db)
			{
				UnlockAvatar(pAvatar);
				JsonRPC_Send("[-1]");
				return;
			}

			if(!db->BeginTranscation())
			{
				UnlockAvatar(pAvatar);
				FreeDatabase(db);
				JsonRPC_Send("[-1]");
				return;
			}

			for(;;)
			{
				_U32 i;
				for(i=0; i<(_U32)tasks.size(); i++)
				{
					bool is_error = true;
					switch(tasks[i]._task_type)
					{
					case TASK_CREATE_OBJECT:
						if(pAvatar->ExistObject(tasks[i]._obj_uuid)) is_error = false;
						break;
					case TASK_DELETE_OBJECT:
					case TASK_UPDATE_OBJECT:
						if(!pAvatar->ExistObject(tasks[i]._obj_uuid)) is_error = false;
						break;
					case TASK_DELETE_TASK:
						if(db->LockTask(avatar_id, tasks[i]._task_id)) is_error = false;
						break;
					}
					if(is_error) break;
				}
				if(i!=(_U32)tasks.size()) break;

				for(i=0; i<(_U32)tasks.size(); i++)
				{
					switch(tasks[i]._task_type)
					{
					case TASK_CREATE_OBJECT:
						pAvatar->CreateObject(tasks[i]._obj_uuid, tasks[i]._obj_type.c_str(), tasks[i]._obj_data.c_str(), true, true);
						break;
					case TASK_DELETE_OBJECT:
						pAvatar->DeleteObject(tasks[i]._obj_uuid);
						break;
					case TASK_UPDATE_OBJECT:
						pAvatar->UpdateObject(tasks[i]._obj_uuid, tasks[i]._obj_data.c_str());
						break;
					case TASK_DELETE_TASK:
						// do something
						db->DeleteTask(avatar_id, tasks[i]._task_id);
						break;
					}
				}

				pAvatar->Save();
				if(db->CommitTransaction())
				{
					// clear dirty
				}
				else
				{
					// ???
				}
				version = pAvatar->GetVersion();
				UnlockAvatar(pAvatar);
				FreeDatabase(db);
				JsonRPC_Send(StringFormat("[0,%u]", version).c_str());
				return;
			}
			UnlockAvatar(pAvatar);
			db->RollbackTransaction();
			FreeDatabase(db);
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_Init()
		{
			g_avatar_manager.GetCount();
			_U32 avatar_id = (_U32)-1;
			g_avatar_id_map.Get(avatar_id);
		}

		void RPCIMPL_FlushAllData()
		{
			for(_U32 i=0; i<g_avatar_manager.GetCount(); i++)
			{
				CAvatarData* data = g_avatar_manager.Lock(i);
				if(!data) continue;

				RemoveAvatar(data);
				ZION_DELETE data;
			}
		}

	}
}
