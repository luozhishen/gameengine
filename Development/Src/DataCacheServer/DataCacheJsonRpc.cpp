#include <ZionBase.h>
#include <ZionCommon.h>
#include <JsonRPC.h>
#include "DataCacheServer.h"
#include "DataCacheJsonRpc.h"
#include "DataCacheRpcImpl.h"

namespace Zion
{
	namespace DataCache
	{

		void JsonRPC_Echo(const JsonValue& args)
		{
			JsonRPC_Send("[0]");
		}

		void JsonRPC_CreateAvatar(const JsonValue& args)
		{
			if(CONFIG_ENABLE_RPC_REPLAYLOG)
				WriteRPCLog("CreateAvatar", args);

			Array<A_UUID> uuids;
			Array<String> types;
			Array<String> datas;
			for(;;)
			{
				if(args.GetSize()!=5) break;

				const JsonValue& _user_id = args.Get((_U32)0);
				if(!_user_id.IsU32()) break;
				const JsonValue& _avatar_scope = args.Get((_U32)1);
				if(!_avatar_scope.IsU32()) break;
				const JsonValue& _avatar_name = args.Get((_U32)2);
				if(!_avatar_name.IsSTR()) break;
				const JsonValue& _avatar_desc = args.Get((_U32)3);
				if(!_avatar_desc.IsSTR()) break;
				const JsonValue& _avatar_objs = args.Get((_U32)4);
				if(!_avatar_objs.IsArray()) break;

				_U32 i = 0;
				for(i=0; i<_avatar_objs.GetSize(); i++)
				{
					const JsonValue& obj = _avatar_objs.Get(i);
					if(!obj.IsObject()) break;
					const JsonValue& uuid = obj.Get("uuid");
					if(!uuid.IsSTR()) break;
					const JsonValue& type = obj.Get("type");
					if(!type.IsSTR()) break;
					const JsonValue& data = obj.Get("data");
					if(!data.IsSTR()) break;

					A_UUID _uuid;
					if(!AUuidFromString(uuid.AsCSTR(), _uuid)) break;

					uuids.push_back(_uuid);
					types.push_back(type.AsSTR());
					datas.push_back(data.AsSTR());
				}
				if(i<_avatar_objs.GetSize()) break;

				RPCIMPL_CreateAvatar(
					(_U32)_user_id.AsU32(),
					(_U32)_avatar_scope.AsU32(),
					_avatar_name.AsCSTR(),
					_avatar_desc.AsCSTR(),
					uuids, types, datas
					);
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRPC_DeleteAvatar(const JsonValue& args)
		{
			if(CONFIG_ENABLE_RPC_REPLAYLOG)
				WriteRPCLog("DeleteAvatar", args);

			for(;;)
			{
				if(args.GetSize()!=3) break;
				const JsonValue& _user_id = args.Get((_U32)0);
				if(!_user_id.IsU32()) break;
				const JsonValue& _avatar_scope = args.Get((_U32)1);
				if(!_avatar_scope.IsU32()) break;
				const JsonValue& _avatar_id = args.Get((_U32)2);
				if(!_avatar_id.IsU32()) break;
				RPCIMPL_DeleteAvatar(_user_id.AsU32(), _avatar_scope.AsU32(), _avatar_id.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRPC_GetAvatar(const JsonValue& args)
		{
			if(CONFIG_ENABLE_RPC_REPLAYLOG)
				WriteRPCLog("GetAvatar", args);

			for(;;)
			{
				if(args.GetSize()!=1) break;
				const JsonValue& _avatar_id = args.Get((_U32)0);
				if(!_avatar_id.IsU32()) break;
				RPCIMPL_GetAvatar((_U32)_avatar_id.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRPC_SaveToDB(const JsonValue& args)
		{
			if(CONFIG_ENABLE_RPC_REPLAYLOG)
				WriteRPCLog("SaveAvatar", args);

			for(;;)
			{
				if(args.GetSize()!=1) break;
				const JsonValue& _avatar_id = args.Get((_U32)0);
				if(!_avatar_id.IsU32()) break;
				RPCIMPL_SaveToDB((_U32)_avatar_id.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRPC_KeepAlive(const JsonValue& args)
		{
			if(CONFIG_ENABLE_RPC_REPLAYLOG)
				WriteRPCLog("KeepAlive", args);

			for(;;)
			{
				if(args.GetSize()!=1) break;
				const JsonValue& _avatar_id = args.Get((_U32)0);
				if(!_avatar_id.IsU32()) break;
				RPCIMPL_KeepAlive((_U32)_avatar_id.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRPC_CreateObject(const JsonValue& args)
		{
			if(CONFIG_ENABLE_RPC_REPLAYLOG)
				WriteRPCLog("CreateObject", args);

			for(;;)
			{
				if(args.GetSize()!=5) break;
				const JsonValue& _avatar_id = args.Get((_U32)0);
				if(!_avatar_id.IsU32()) break;
				const JsonValue& _version = args.Get((_U32)1);
				if(!_version.IsU32()) break;
				const JsonValue& _suuid = args.Get((_U32)2);
				if(!_suuid.IsSTR()) break;
				const JsonValue& _type = args.Get((_U32)3);
				if(!_type.IsSTR()) break;
				const JsonValue& _data = args.Get((_U32)4);
				if(!_data.IsSTR()) break;

				A_UUID _uuid;
				if(!AUuidFromString(_suuid.AsCSTR(), _uuid)) break;
				RPCIMPL_CreateObject((_U32)_avatar_id.AsU32(), (_U32)_version.AsU32(), _uuid, _type.AsCSTR(), _data.AsCSTR());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRPC_UpdateObject(const JsonValue& args)
		{
			if(CONFIG_ENABLE_RPC_REPLAYLOG)
				WriteRPCLog("UpdateObject", args);

			for(;;)
			{
				if(args.GetSize()!=4) break;
				const JsonValue& _avatar_id = args.Get((_U32)0);
				if(!_avatar_id.IsU32()) break;
				const JsonValue& _version = args.Get((_U32)1);
				if(!_version.IsU32()) break;
				const JsonValue& _suuid = args.Get((_U32)2);
				if(!_suuid.IsSTR()) break;
				const JsonValue& _data = args.Get((_U32)3);
				if(!_data.IsSTR()) break;

				A_UUID _uuid;
				if(!AUuidFromString(_suuid.AsCSTR(), _uuid)) break;
				RPCIMPL_UpdateObject((_U32)_avatar_id.AsU32(), (_U32)_version.AsU32(), _uuid, _data.AsCSTR());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRPC_DeleteObject(const JsonValue& args)
		{
			if(CONFIG_ENABLE_RPC_REPLAYLOG)
				WriteRPCLog("DeleteObject", args);

			for(;;)
			{
				if(args.GetSize()!=3) break;
				const JsonValue& _avatar_id = args.Get((_U32)0);
				if(!_avatar_id.IsU32()) break;
				const JsonValue& _version = args.Get((_U32)1);
				if(!_version.IsU32()) break;
				const JsonValue& _suuid = args.Get((_U32)2);
				if(!_suuid.IsArray()) break;
				Array<A_UUID> _uuids;
				_uuids.resize(_suuid.GetSize());
				_U32 i;
				for(i=0; i<(_U32)_uuids.size(); i++)
				{
					const JsonValue& item = _suuid.Get(i);
					if(!item.IsSTR()) break;
					if(!AUuidFromString(item.AsCSTR(), _uuids[i])) break;
				}
				if(i!=(_U32)_uuids.size()) break;
				Json::FastWriter writer;
				RPCIMPL_DeleteObject((_U32)_avatar_id.AsU32(), (_U32)_version.AsU32(), &_uuids[0], (_U32)_uuids.size());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRPC_LoadObject(const JsonValue& args)
		{
			if(CONFIG_ENABLE_RPC_REPLAYLOG)
				WriteRPCLog("LoadObject", args);

			for(;;)
			{
				if(args.GetSize()!=3) break;
				const JsonValue& _avatar_id = args.Get((_U32)0);
				if(!_avatar_id.IsU32()) break;
				const JsonValue& _version = args.Get((_U32)0);
				if(!_version.IsU32()) break;
				const JsonValue& juuid = args.Get((_U32)2);
				if(!juuid.IsSTR()) break;
				A_UUID _uuid;
				if(!AUuidFromString(juuid.AsCSTR(), _uuid)) break;
				RPCIMPL_LoadObject(_avatar_id.AsU32(), _version.AsU32(), _uuid);
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRPC_ExecuteBatch(const JsonValue& args)
		{
			if(CONFIG_ENABLE_RPC_REPLAYLOG)
				WriteRPCLog("ExecuteBatch", args);

			for(;;)
			{
				if(args.GetSize()!=4) break;
				const JsonValue& _avatar_id = args.Get((_U32)0);
				if(!_avatar_id.IsU32()) break;
				const JsonValue& _version = args.Get((_U32)1);
				if(!_version.IsU32()) break;
				const JsonValue& _tasks = args.Get((_U32)2);
				if(!_tasks.IsArray()) break;
				const JsonValue& _save = args.Get((_U32)3);
				if(!_save.IsBool()) break;

				Array<TASK> _array;
				Set<A_UUID> _uuids;
				_U32 i;
				for(i=0; i<_tasks.GetSize(); i++)
				{
					const JsonValue& node = _tasks.Get(i);
					if(!node.IsObject()) break;
					const JsonValue& _tasktype = node.Get("task_type");
					if(!_tasktype.IsSTR()) break;
					TASK task;
					if(_tasktype.AsSTR()=="createObject")
					{
						task._task_type = TASK_CREATE_OBJECT;
						const JsonValue& _obj_uuid = node.Get("obj_uuid");
						if(!_obj_uuid.IsSTR()) break;
						if(!AUuidFromString(_obj_uuid.AsCSTR(), task._obj_uuid)) break;
						const JsonValue& _obj_type = node.Get("obj_type");
						if(!_obj_type.IsSTR()) break;
						task._obj_type = _obj_type.AsSTR();
						const JsonValue& _obj_data = node.Get("obj_data");
						if(!_obj_data.IsSTR()) break;
						task._obj_data = _obj_data.AsSTR();
						if(_uuids.find(task._obj_uuid)!=_uuids.end()) break;
						_uuids.insert(task._obj_uuid);
					}
					else if(_tasktype.AsSTR()=="updateObject")
					{
						task._task_type = TASK_UPDATE_OBJECT;
						const JsonValue& _obj_uuid = node.Get("obj_uuid");
						if(!_obj_uuid.IsSTR()) break;
						if(!AUuidFromString(_obj_uuid.AsCSTR(), task._obj_uuid)) break;
						const JsonValue& _obj_data = node.Get("obj_data");
						if(!_obj_data.IsSTR()) break;
						task._obj_data = _obj_data.AsSTR();
						if(_uuids.find(task._obj_uuid)!=_uuids.end()) break;
						_uuids.insert(task._obj_uuid);
					} 
					else if(_tasktype.AsSTR()=="deleteObject")
					{
						task._task_type = TASK_DELETE_OBJECT;
						const JsonValue& _obj_uuid = node.Get("obj_uuid");
						if(!_obj_uuid.IsSTR()) break;
						if(!AUuidFromString(_obj_uuid.AsCSTR(), task._obj_uuid)) break;
						if(_uuids.find(task._obj_uuid)!=_uuids.end()) break;
						_uuids.insert(task._obj_uuid);
					}
					else if(_tasktype.AsSTR()=="deleteTask")
					{
						if(!_save.AsBool()) break;

						task._task_type = TASK_DELETE_TASK;
						const JsonValue& _task_id = node.Get("task_id");
						if(!_task_id.IsU32()) break;
						task._task_id = _task_id.AsU32();
					}
					else
					{
						break;
					}
				}
				if(i==_tasks.GetSize())
				{
					RPCIMPL_ExecuteBatch(_avatar_id.AsU32(), _version.AsU32(), _array, _save.AsBool());
					return;
				}
			}
			JsonRPC_Send("[-1]");
		}

	}
}
