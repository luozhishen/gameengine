#include <ZionBase.h>
#include <ZionCommon.h>
#include <JsonRPC.h>
#include "DataCache.h"
#include "DataCacheJsonRPC.h"
#include "DataCacheServer.h"
#include "SimpleDataCacheServer.h"

namespace Zion
{
	namespace DataCache
	{

		void JsonRPC_GetAvatar(const JSONRPC_RESPONSE& res, const char* args)
		{
			Json::Reader reader;
			Json::Value _args;
			Json::Value _avatar_id;
			for(;;)
			{
				if(!reader.parse(String(args), _args)) break;
				if(!_args.isArray() || _args.size()!=1) break;
				_avatar_id = _args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_avatar_id.isUInt()) break;
				if(CONFIG_SIMPLE_MODE)
				{
					RPCSIMPLE_GetAvatar(res, (_U32)_avatar_id.asUInt());
				}
				else
				{
					RPCIMPL_GetAvatar(res, (_U32)_avatar_id.asUInt());
				}
				return;
			}
			JsonRPC_Send(res, -1);
		}

		void JsonRPC_KeepAlive(const JSONRPC_RESPONSE& res, const char* args)
		{
			Json::Reader reader;
			Json::Value _args;
			Json::Value _avatar_id;
			for(;;)
			{
				if(!reader.parse(String(args), _args)) break;
				if(!_args.isArray() || _args.size()!=1) break;
				_avatar_id = _args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_avatar_id.isUInt()) break;
				if(CONFIG_SIMPLE_MODE)
				{
					RPCSIMPLE_KeepAlive(res, (_U32)_avatar_id.asUInt());
				}
				else
				{
					RPCIMPL_KeepAlive(res, (_U32)_avatar_id.asUInt());
				}
				return;
			}
			JsonRPC_Send(res, -1);
		}

		void JsonRPC_SaveToDB(const JSONRPC_RESPONSE& res, const char* args)
		{
			Json::Reader reader;
			Json::Value _args;
			Json::Value _avatar_id;
			for(;;)
			{
				if(!reader.parse(String(args), _args)) break;
				if(!_args.isArray() || _args.size()!=1) break;
				_avatar_id = _args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_avatar_id.isUInt()) break;
				if(CONFIG_SIMPLE_MODE)
				{
					RPCSIMPLE_SaveToDB(res, (_U32)_avatar_id.asUInt());
				}
				else
				{
					RPCIMPL_SaveToDB(res, (_U32)_avatar_id.asUInt());
				}
				return;
			}
			JsonRPC_Send(res, -1);
		}

		void JsonRPC_ExecuteDDL(const JSONRPC_RESPONSE& res, const char* args)
		{
			Json::Reader reader;
			Json::Value _args;
			Json::Value _avatar_id;
			Json::Value _method_name;
			Json::Value _data;
			for(;;)
			{
				if(!reader.parse(String(args), _args)) break;
				if(!_args.isArray() || _args.size()!=3) break;
				_avatar_id = _args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_avatar_id.isUInt()) break;
				_method_name = _args.get((Json::Value::UInt)1, Json::nullValue);
				if(!_method_name.isString()) break;
				_data = _args.get((Json::Value::UInt)2, Json::nullValue);
				if(!_data.isString()) break;
				_U32 len = (_U32)_data.asString().size();
				if((len&0x1)!=0) break;
				_U8* data = (_U8*)ZION_ALLOC(len/2);
				if(!data || !ToBin(_data.asCString(), data, len/2)) break;
				if(CONFIG_SIMPLE_MODE)
				{
					RPCSIMPLE_ExecuteDDL(res, (_U32)_avatar_id.asUInt(), _method_name.asCString(), data, len/2);
				}
				else
				{
					RPCIMPL_ExecuteDDL(res, (_U32)_avatar_id.asUInt(), _method_name.asCString(), data, len/2);
				}
				ZION_FREE(data);
				return;
			}
			JsonRPC_Send(res, -1);
		}

		void JsonRPC_CreateObject(const JSONRPC_RESPONSE& res, const char* args)
		{
			Json::Reader reader;
			Json::Value _args;
			Json::Value _avatar_id;
			Json::Value _suuid;
			Json::Value _type;
			Json::Value _data;
			for(;;)
			{
				if(!reader.parse(String(args), _args)) break;
				if(!_args.isArray() || _args.size()!=4) break;
				_avatar_id = _args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_avatar_id.isUInt()) break;
				_suuid = _args.get((Json::Value::UInt)1, Json::nullValue);
				if(!_suuid.isString()) break;
				_type = _args.get((Json::Value::UInt)2, Json::nullValue);
				if(!_type.isString()) break;
				_data = _args.get((Json::Value::UInt)3, Json::nullValue);
				if(!_data.isObject()) break;

				A_UUID _uuid;
				if(!AUuidFromString(_suuid.asCString(), _uuid)) break;
				Json::FastWriter writer;
				if(CONFIG_SIMPLE_MODE)
				{
					RPCSIMPLE_CreateObject(res, (_U32)_avatar_id.asUInt(), _uuid, _type.asCString(), writer.write(_data).c_str());
				}
				else
				{
					RPCIMPL_CreateObject(res, (_U32)_avatar_id.asUInt(), _uuid, _type.asCString(), writer.write(_data).c_str());
				}
				return;
			}
			JsonRPC_Send(res, -1);
		}

		void JsonRPC_UpdateObject(const JSONRPC_RESPONSE& res, const char* args)
		{
			Json::Reader reader;
			Json::Value _args;
			Json::Value _avatar_id;
			Json::Value _suuid;
			Json::Value _data;
			for(;;)
			{
				if(!reader.parse(String(args), _args)) break;
				if(!_args.isArray() || _args.size()!=4) break;
				_avatar_id = _args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_avatar_id.isUInt()) break;
				_suuid = _args.get((Json::Value::UInt)1, Json::nullValue);
				if(!_suuid.isString()) break;
				_data = _args.get((Json::Value::UInt)2, Json::nullValue);
				if(!_data.isObject()) break;

				A_UUID _uuid;
				if(!AUuidFromString(_suuid.asCString(), _uuid)) break;
				Json::FastWriter writer;
				if(CONFIG_SIMPLE_MODE)
				{
					RPCSIMPLE_UpdateObject(res, (_U32)_avatar_id.asUInt(), _uuid, writer.write(_data).c_str());
				}
				else
				{
					RPCIMPL_UpdateObject(res, (_U32)_avatar_id.asUInt(), _uuid, writer.write(_data).c_str());
				}
				return;
			}
			JsonRPC_Send(res, -1);
		}

		void JsonRPC_DeleteObject(const JSONRPC_RESPONSE& res, const char* args)
		{
			Json::Reader reader;
			Json::Value _args;
			Json::Value _avatar_id;
			Json::Value _suuid;
			for(;;)
			{
				if(!reader.parse(String(args), _args)) break;
				if(!_args.isArray() || _args.size()!=4) break;
				_avatar_id = _args.get((Json::Value::UInt)0, Json::nullValue);
				if(!_avatar_id.isUInt()) break;
				_suuid = _args.get((Json::Value::UInt)1, Json::nullValue);
				if(!_suuid.isArray()) break;
				Array<A_UUID> _uuids;
				_uuids.resize(_suuid.size());
				_U32 i;
				for(i=0; i<(_U32)_uuids.size(); i++)
				{
					Json::Value item = _suuid.get((Json::UInt)i, Json::nullValue);
					if(!item.isString()) break;
					if(!AUuidFromString(_suuid.asCString(), _uuids[i])) break;
				}
				if(i!=(_U32)_uuids.size()) break;
				Json::FastWriter writer;
				if(CONFIG_SIMPLE_MODE)
				{
					RPCSIMPLE_DeleteObject(res, (_U32)_avatar_id.asUInt(), &_uuids[0], (_U32)_uuids.size());
				}
				else
				{
					RPCIMPL_DeleteObject(res, (_U32)_avatar_id.asUInt(), &_uuids[0], (_U32)_uuids.size());
				}
				return;
			}
			JsonRPC_Send(res, -1);
		}

	}
}
