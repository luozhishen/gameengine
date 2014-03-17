#include <ZionBase.h>
#include <ZionCommon.h>
#include <JsonRPC.h>
#include "DataCacheServer.h"
#include "DataCacheJsonRPC.h"
#include "DataCacheRpcImpl.h"

namespace Zion
{
	namespace DataCache
	{

		void JsonRPC_LoginUser(const JSONRPC_RESPONSE& res, const Json::Value& args)
		{
			Json::Value _token;
			for(;;)
			{
				if(args.size()!=1) break;

				_token = args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_token.isString()) break;

				RPCIMPL_LoginUser(res, _token.asCString());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRPC_CreateAvatar(const JSONRPC_RESPONSE& res, const Json::Value& args)
		{
			Json::Value _user_id;
			Json::Value _server_id;
			Json::Value _avatar_name;
			Json::Value _avatar_desc;
			Json::Value _avatar_objs;
			Array<A_UUID> uuids;
			Array<String> types;
			Array<String> datas;
			for(;;)
			{
				if(args.size()!=5) break;

				_user_id = args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_user_id.isUInt()) break;
				_server_id = args.get((Json::Value::UInt)1, Json::nullValue);
				if(!_server_id.isUInt()) break;
				_avatar_name = args.get((Json::Value::UInt)2, Json::nullValue);
				if(!_avatar_name.isString()) break;
				_avatar_desc = args.get((Json::Value::UInt)3, Json::nullValue);
				if(!_avatar_desc.isString()) break;
				_avatar_objs = args.get((Json::Value::UInt)4, Json::nullValue);
				if(!_avatar_objs.isObject()) break;

				RPCIMPL_CreateAvatar(res,
					(_U32)_user_id.asUInt(),
					(_U32)_server_id.asUInt(),
					_avatar_name.asCString(),
					_avatar_desc.asCString(),
					uuids, types, datas
					);
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRPC_DeleteAvatar(const JSONRPC_RESPONSE& res, const Json::Value& args)
		{
			Json::Value _avatar_id;
			for(;;)
			{
				if(args.size()!=1) break;
				_avatar_id = args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_avatar_id.isUInt()) break;
				RPCIMPL_DeleteAvatar(res, (_U32)_avatar_id.asUInt());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRPC_GetAvatarList(const JSONRPC_RESPONSE& res, const Json::Value& args)
		{
			Json::Value _user_id;
			Json::Value _server_id;
			for(;;)
			{
				if(args.size()!=2) break;
				_user_id = args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_user_id.isUInt()) break;
				_server_id = args.get((Json::Value::UInt)1, Json::nullValue);
				if(!_server_id.isUInt()) break;
				RPCIMPL_GetAvatarList(res, (_U32)_user_id.asUInt(), (_U32)_server_id.asUInt());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRPC_GetAvatar(const JSONRPC_RESPONSE& res, const Json::Value& args)
		{
			Json::Value _avatar_id;
			for(;;)
			{
				if(args.size()!=1) break;
				_avatar_id = args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_avatar_id.isUInt()) break;
				RPCIMPL_GetAvatar(res, (_U32)_avatar_id.asUInt());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRPC_SaveAvatar(const JSONRPC_RESPONSE& res, const Json::Value& args)
		{
			Json::Value _avatar_id;
			for(;;)
			{
				if(args.size()!=1) break;
				_avatar_id = args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_avatar_id.isUInt()) break;
				RPCIMPL_SaveAvatar(res, (_U32)_avatar_id.asUInt());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRPC_ClearAvatar(const JSONRPC_RESPONSE& res, const Json::Value& args)
		{
			Json::Value _avatar_id;
			for(;;)
			{
				if(args.size()!=1) break;
				_avatar_id = args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_avatar_id.isUInt()) break;
				RPCIMPL_ClearAvatar(res, (_U32)_avatar_id.asUInt());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRPC_KeepAlive(const JSONRPC_RESPONSE& res, const Json::Value& args)
		{
			Json::Value _avatar_id;
			for(;;)
			{
				if(args.size()!=1) break;
				_avatar_id = args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_avatar_id.isUInt()) break;
				RPCIMPL_KeepAlive(res, (_U32)_avatar_id.asUInt());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRPC_CreateObject(const JSONRPC_RESPONSE& res, const Json::Value& args)
		{
			Json::Value _avatar_id;
			Json::Value _suuid;
			Json::Value _type;
			Json::Value _data;
			for(;;)
			{
				if(args.size()!=4) break;
				_avatar_id = args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_avatar_id.isUInt()) break;
				_suuid = args.get((Json::Value::UInt)1, Json::nullValue);
				if(!_suuid.isString()) break;
				_type = args.get((Json::Value::UInt)2, Json::nullValue);
				if(!_type.isString()) break;
				_data = args.get((Json::Value::UInt)3, Json::nullValue);
				if(!_data.isString()) break;

				A_UUID _uuid;
				if(!AUuidFromString(_suuid.asCString(), _uuid)) break;
				RPCIMPL_CreateObject(res, (_U32)_avatar_id.asUInt(), _uuid, _type.asCString(), _data.asCString());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRPC_UpdateObject(const JSONRPC_RESPONSE& res, const Json::Value& args)
		{
			Json::Value _avatar_id;
			Json::Value _suuid;
			Json::Value _data;
			for(;;)
			{
				if(args.size()!=3) break;
				_avatar_id = args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_avatar_id.isUInt()) break;
				_suuid = args.get((Json::Value::UInt)1, Json::nullValue);
				if(!_suuid.isString()) break;
				_data = args.get((Json::Value::UInt)2, Json::nullValue);
				if(!_data.isString()) break;

				A_UUID _uuid;
				if(!AUuidFromString(_suuid.asCString(), _uuid)) break;
				RPCIMPL_UpdateObject(res, (_U32)_avatar_id.asUInt(), _uuid, _data.asCString());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRPC_DeleteObject(const JSONRPC_RESPONSE& res, const Json::Value& args)
		{
			Json::Value _avatar_id;
			Json::Value _suuid;
			for(;;)
			{
				if(args.size()!=2) break;
				_avatar_id = args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_avatar_id.isUInt()) break;
				_suuid = args.get((Json::Value::UInt)1, Json::nullValue);
				if(!_suuid.isArray()) break;
				Array<A_UUID> _uuids;
				_uuids.resize(_suuid.size());
				_U32 i;
				for(i=0; i<(_U32)_uuids.size(); i++)
				{
					Json::Value item = _suuid.get((Json::UInt)i, Json::nullValue);
					if(!item.isString()) break;
					if(!AUuidFromString(item.asCString(), _uuids[i])) break;
				}
				if(i!=(_U32)_uuids.size()) break;
				Json::FastWriter writer;
				RPCIMPL_DeleteObject(res, (_U32)_avatar_id.asUInt(), &_uuids[0], (_U32)_uuids.size());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRPC_loadObjectFromDB(const JSONRPC_RESPONSE& res, const Json::Value& args)
		{
			Json::Value _avatar_id;
			Json::Value juuid;
			for(;;)
			{
				if(args.size()!=2) break;
				_avatar_id = args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_avatar_id.isUInt()) break;
				juuid = args.get((Json::Value::UInt)1, Json::nullValue);
				if(!juuid.isString()) break;
				A_UUID _uuid;
				if(!AUuidFromString(juuid.asCString(), _uuid)) break;
				RPCIMPL_LoadObjectFromDB(res, (_U32)_avatar_id.asUInt(), _uuid);
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

	}
}
