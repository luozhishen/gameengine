#include <ZionBase.h>
#include <ZionCommon.h>
#include <JsonRPC.h>
#include "SessionJsonRPC.h"
#include "SessionRpcImpl.h"

namespace Zion
{
	namespace Session
	{

		void JsonRpc_LoginUser(const JSONRPC_RESPONSE& res, const Json::Value& args)
		// void RPCIMPL_LoginUser(const JSONRPC_RESPONSE& res, _U32 user_id);
		{
			for(;;)
			{
				if(args.size()!=1) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				RPCIMPL_LoginUser(res, (_U32)user_id.asUInt());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRpc_LogoutUser(const JSONRPC_RESPONSE& res, const Json::Value& args)
		// void RPCIMPL_LogoutUser(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq);
		{
			for(;;)
			{
				if(args.size()!=2) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				Json::Value user_seq = args.get((Json::UInt)1, Json::nullValue);
				if(!user_seq.isUInt()) break;
				RPCIMPL_LogoutUser(res, (_U32)user_id.asUInt(), (_U32)user_seq.asUInt());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRpc_LockUser(const JSONRPC_RESPONSE& res, const Json::Value& args)
		// void RPCIMPL_LockUser(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq);
		{
			for(;;)
			{
				if(args.size()!=2) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				Json::Value user_seq = args.get((Json::UInt)1, Json::nullValue);
				if(!user_seq.isUInt()) break;
				RPCIMPL_LockUser(res, (_U32)user_id.asUInt(), (_U32)user_seq.asUInt());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRpc_UnlockUser(const JSONRPC_RESPONSE& res, const Json::Value& args)
		// void RPCIMPL_UnlockUser(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq, const char* session_data);
		{
			for(;;)
			{
				if(args.size()!=3) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				Json::Value user_seq = args.get((Json::UInt)1, Json::nullValue);
				if(!user_seq.isUInt()) break;
				Json::Value session_data = args.get((Json::UInt)2, Json::nullValue);
				if(!user_seq.isString()) break;
				RPCIMPL_UnlockUser(res, (_U32)user_id.asUInt(), (_U32)user_seq.asUInt(), user_seq.asCString());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRpc_BindAvatar(const JSONRPC_RESPONSE& res, const Json::Value& args)
		// void RPCIMPL_BindAvatar(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq, _U32 avatar_id, const char* avatar_name);
		{
			for(;;)
			{
				if(args.size()!=4) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				Json::Value user_seq = args.get((Json::UInt)1, Json::nullValue);
				if(!user_seq.isUInt()) break;
				Json::Value avatar_id = args.get((Json::UInt)2, Json::nullValue);
				if(!avatar_id.isUInt()) break;
				Json::Value avatar_name = args.get((Json::UInt)3, Json::nullValue);
				if(!avatar_name.isString()) break;
				RPCIMPL_BindAvatar(res, (_U32)user_id.asUInt(), (_U32)user_seq.asUInt(), (_U32)avatar_id.asUInt(), avatar_name.asCString());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRpc_UnbindAvatar(const JSONRPC_RESPONSE& res, const Json::Value& args)
		// void RPCIMPL_UnbindAvatar(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq);
		{
			for(;;)
			{
				if(args.size()!=2) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				Json::Value user_seq = args.get((Json::UInt)1, Json::nullValue);
				if(!user_seq.isUInt()) break;
				RPCIMPL_UnbindAvatar(res, (_U32)user_id.asUInt(), (_U32)user_seq.asUInt());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRpc_SendToUserID(const JSONRPC_RESPONSE& res, const Json::Value& args)
		// void RPCIMPL_SendToUserID(const JSONRPC_RESPONSE& res, _U32 user_id, const char* msg);
		{
			for(;;)
			{
				if(args.size()!=2) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				Json::Value msg = args.get((Json::UInt)1, Json::nullValue);
				if(!msg.isString()) break;
				RPCIMPL_SendToUserID(res, (_U32)user_id.asUInt(), msg.asCString());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRpc_SendToAvatarID(const JSONRPC_RESPONSE& res, const Json::Value& args)
		// void RPCIMPL_SendToAvatarID(const JSONRPC_RESPONSE& res, _U32 avatar_id, const char* msg);
		{
			for(;;)
			{
				if(args.size()!=2) break;
				Json::Value avatar_id = args.get((Json::UInt)0, Json::nullValue);
				if(!avatar_id.isUInt()) break;
				Json::Value msg = args.get((Json::UInt)1, Json::nullValue);
				if(!msg.isString()) break;
				RPCIMPL_SendToAvatarID(res, (_U32)avatar_id.asUInt(), msg.asCString());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRpc_SendToAvatarName(const JSONRPC_RESPONSE& res, const Json::Value& args)
		// void RPCIMPL_SendToAvatarName(const JSONRPC_RESPONSE& res, const char* avatar_name, const char* msg);
		{
			for(;;)
			{
				if(args.size()!=2) break;
				Json::Value avatar_name = args.get((Json::UInt)0, Json::nullValue);
				if(!avatar_name.isString()) break;
				Json::Value msg = args.get((Json::UInt)1, Json::nullValue);
				if(!msg.isString()) break;
				RPCIMPL_SendToAvatarName(res, avatar_name.asCString(), msg.asCString());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRpc_JoinDomain(const JSONRPC_RESPONSE& res, const Json::Value& args)
		// void RPCIMPL_JoinDomain(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq, _U32 domain_id);
		{
			for(;;)
			{
				if(args.size()!=3) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				Json::Value user_seq = args.get((Json::UInt)1, Json::nullValue);
				if(!user_seq.isUInt()) break;
				Json::Value domain_id = args.get((Json::UInt)2, Json::nullValue);
				if(!domain_id.isUInt()) break;
				RPCIMPL_JoinDomain(res, (_U32)user_id.asUInt(), (_U32)user_seq.asUInt(), (_U32)domain_id.asUInt());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRpc_LeaveDomain(const JSONRPC_RESPONSE& res, const Json::Value& args)
		// void RPCIMPL_LeaveDomain(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq, _U32 domain_id);
		{
			for(;;)
			{
				if(args.size()!=3) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				Json::Value user_seq = args.get((Json::UInt)1, Json::nullValue);
				if(!user_seq.isUInt()) break;
				Json::Value domain_id = args.get((Json::UInt)2, Json::nullValue);
				if(!domain_id.isUInt()) break;
				RPCIMPL_LeaveDomain(res, (_U32)user_id.asUInt(), (_U32)user_seq.asUInt(), (_U32)domain_id.asUInt());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRpc_SendToDomain(const JSONRPC_RESPONSE& res, const Json::Value& args)
		// void RPCIMPL_SendToDomain(const JSONRPC_RESPONSE& res, _U32 domain_id, const char* msg);
		{
			for(;;)
			{
				if(args.size()!=2) break;
				Json::Value domain_id = args.get((Json::UInt)0, Json::nullValue);
				if(!domain_id.isUInt()) break;
				Json::Value msg = args.get((Json::UInt)1, Json::nullValue);
				if(!msg.isString()) break;
				RPCIMPL_SendToDomain(res, (_U32)domain_id.asUInt(), msg.asCString());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRpc_WaitForMessage(const JSONRPC_RESPONSE& res, const Json::Value& args)
		// void RPCIMPL_WaitForMessage(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq, _U32 msg_seq);
		{
			for(;;)
			{
				if(args.size()!=3) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				Json::Value user_seq = args.get((Json::UInt)1, Json::nullValue);
				if(!user_seq.isUInt()) break;
				Json::Value msg_seq = args.get((Json::UInt)1, Json::nullValue);
				if(!msg_seq.isUInt()) break;
				RPCIMPL_WaitForMessage(res, (_U32)user_id.asUInt(), (_U32)user_seq.asUInt(), (_U32)msg_seq.asUInt());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

		void JsonRpc_GetMessage(const JSONRPC_RESPONSE& res, const Json::Value& args)
		// void RPCIMPL_GetMessage(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq, _U32 msg_seq);
		{
			for(;;)
			{
				if(args.size()!=3) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				Json::Value user_seq = args.get((Json::UInt)1, Json::nullValue);
				if(!user_seq.isUInt()) break;
				Json::Value msg_seq = args.get((Json::UInt)1, Json::nullValue);
				if(!msg_seq.isUInt()) break;
				RPCIMPL_GetMessage(res, (_U32)user_id.asUInt(), (_U32)user_seq.asUInt(), (_U32)msg_seq.asUInt());
				return;
			}
			JsonRPC_Send(res, "[-1]");
		}

	}
}
