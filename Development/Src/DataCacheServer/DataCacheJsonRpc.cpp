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

		void JsonRPC_LoginUser(const JsonValue& args)
		{
			for(;;)
			{
				if(args.GetSize()!=1) break;

				const JsonValue& _token = args.Get((_U32)0);
				if(!_token.IsSTR()) break;

				RPCIMPL_LoginUser(_token.AsCSTR());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRPC_CreateAvatar(const JsonValue& args)
		{
			Array<A_UUID> uuids;
			Array<String> types;
			Array<String> datas;
			for(;;)
			{
				if(args.GetSize()!=5) break;

				const JsonValue& _user_id = args.Get((_U32)0);
				if(!_user_id.IsU32()) break;
				const JsonValue& _server_id = args.Get((_U32)1);
				if(!_server_id.IsU32()) break;
				const JsonValue& _avatar_name = args.Get((_U32)2);
				if(!_avatar_name.IsSTR()) break;
				const JsonValue& _avatar_desc = args.Get((_U32)3);
				if(!_avatar_desc.IsSTR()) break;
				const JsonValue& _avatar_objs = args.Get((_U32)4);
				if(!_avatar_objs.IsObject()) break;

				RPCIMPL_CreateAvatar(
					(_U32)_user_id.AsU32(),
					(_U32)_server_id.AsU32(),
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
			for(;;)
			{
				if(args.GetSize()!=1) break;
				const JsonValue& _avatar_id = args.Get((_U32)0);
				if(!_avatar_id.IsU32()) break;
				RPCIMPL_DeleteAvatar((_U32)_avatar_id.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRPC_GetAvatarList(const JsonValue& args)
		{
			for(;;)
			{
				if(args.GetSize()!=2) break;
				const JsonValue& _user_id = args.Get((_U32)0);
				if(!_user_id.IsU32()) break;
				const JsonValue& _server_id = args.Get((_U32)1);
				if(!_server_id.IsU32()) break;
				RPCIMPL_GetAvatarList((_U32)_user_id.AsU32(), (_U32)_server_id.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRPC_GetAvatar(const JsonValue& args)
		{
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

		void JsonRPC_SaveAvatar(const JsonValue& args)
		{
			for(;;)
			{
				if(args.GetSize()!=1) break;
				const JsonValue& _avatar_id = args.Get((_U32)0);
				if(!_avatar_id.IsU32()) break;
				RPCIMPL_SaveAvatar((_U32)_avatar_id.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRPC_ClearAvatar(const JsonValue& args)
		{
			for(;;)
			{
				if(args.GetSize()!=1) break;
				const JsonValue& _avatar_id = args.Get((_U32)0);
				if(!_avatar_id.IsU32()) break;
				RPCIMPL_ClearAvatar((_U32)_avatar_id.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRPC_KeepAlive(const JsonValue& args)
		{
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

		void JsonRPC_loadObjectFromDB(const JsonValue& args)
		{
			for(;;)
			{
				if(args.GetSize()!=2) break;
				const JsonValue& _avatar_id = args.Get((_U32)0);
				if(!_avatar_id.IsU32()) break;
				const JsonValue& juuid = args.Get((_U32)1);
				if(!juuid.IsSTR()) break;
				A_UUID _uuid;
				if(!AUuidFromString(juuid.AsCSTR(), _uuid)) break;
				RPCIMPL_LoadObjectFromDB((_U32)_avatar_id.AsU32(), _uuid);
				return;
			}
			JsonRPC_Send("[-1]");
		}

	}
}
