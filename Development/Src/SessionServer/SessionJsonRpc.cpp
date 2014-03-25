#include <ZionBase.h>
#include <ZionCommon.h>
#include <JsonRPC.h>
#include "SessionJsonRPC.h"
#include "SessionRpcImpl.h"

namespace Zion
{
	namespace Session
	{

		void JsonRPC_Echo(const Json::Value& args)
		{
			JsonRPC_Send("[0]");
		}

		void JsonRpc_LoginUser(const Json::Value& args)
		// void RPCIMPL_LoginUser(_U32 user_id);
		{
			for(;;)
			{
				if(args.size()!=1) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				RPCIMPL_LoginUser((_U32)user_id.asUInt());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_LogoutUser(const Json::Value& args)
		// void RPCIMPL_LogoutUser(_U32 user_id, _U32 user_seq);
		{
			for(;;)
			{
				if(args.size()!=2) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				Json::Value user_seq = args.get((Json::UInt)1, Json::nullValue);
				if(!user_seq.isUInt()) break;
				RPCIMPL_LogoutUser((_U32)user_id.asUInt(), (_U32)user_seq.asUInt());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_LockUser(const Json::Value& args)
		// void RPCIMPL_LockUser(_U32 user_id, _U32 user_seq, _U32 req_seq)
		{
			for(;;)
			{
				if(args.size()!=3) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				Json::Value user_seq = args.get((Json::UInt)1, Json::nullValue);
				if(!user_seq.isUInt()) break;
				Json::Value req_seq = args.get((Json::UInt)2, Json::nullValue);
				if(!req_seq.isUInt()) break;
				RPCIMPL_LockUser((_U32)user_id.asUInt(), (_U32)user_seq.asUInt(), (_U32)req_seq.asUInt());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_UnlockUser(const Json::Value& args)
		// void RPCIMPL_UnlockUser(_U32 user_id, _U32 user_seq, const char* last_response, const char* session_data)
		{
			for(;;)
			{
				if(args.size()!=4) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				Json::Value user_seq = args.get((Json::UInt)1, Json::nullValue);
				if(!user_seq.isUInt()) break;
				Json::Value last_response = args.get((Json::UInt)2, Json::nullValue);
				if(!last_response.isString()) break;
				Json::Value session_data = args.get((Json::UInt)3, Json::nullValue);
				if(!user_seq.isString()) break;
				RPCIMPL_UnlockUser((_U32)user_id.asUInt(), (_U32)user_seq.asUInt(), last_response.asCString(),user_seq.asCString());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_BindAvatar(const Json::Value& args)
		// void RPCIMPL_BindAvatar(_U32 user_id, _U32 user_seq, _U32 avatar_id, const char* avatar_name);
		{
			for(;;)
			{
				if(args.size()!=5) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				Json::Value user_seq = args.get((Json::UInt)1, Json::nullValue);
				if(!user_seq.isUInt()) break;
				Json::Value server_id = args.get((Json::UInt)2, Json::nullValue);
				if(!server_id.isUInt()) break;
				Json::Value avatar_id = args.get((Json::UInt)3, Json::nullValue);
				if(!avatar_id.isUInt()) break;
				Json::Value avatar_name = args.get((Json::UInt)4, Json::nullValue);
				if(!avatar_name.isString()) break;
				RPCIMPL_BindAvatar((_U32)user_id.asUInt(), (_U32)user_seq.asUInt(), (_U32)server_id.asUInt(), (_U32)avatar_id.asUInt(), avatar_name.asCString());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_UnbindAvatar(const Json::Value& args)
		// void RPCIMPL_UnbindAvatar(_U32 user_id, _U32 user_seq);
		{
			for(;;)
			{
				if(args.size()!=2) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				Json::Value user_seq = args.get((Json::UInt)1, Json::nullValue);
				if(!user_seq.isUInt()) break;
				RPCIMPL_UnbindAvatar((_U32)user_id.asUInt(), (_U32)user_seq.asUInt());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_SendToUserID(const Json::Value& args)
		// void RPCIMPL_SendToUserID(_U32 user_id, const char* msg);
		{
			for(;;)
			{
				if(args.size()!=2) break;
				Json::Value user_id = args.get((Json::UInt)0, Json::nullValue);
				if(!user_id.isUInt()) break;
				Json::Value msg = args.get((Json::UInt)1, Json::nullValue);
				if(!msg.isString()) break;
				RPCIMPL_SendToUserID((_U32)user_id.asUInt(), msg.asCString());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_SendToAvatarID(const Json::Value& args)
		// void RPCIMPL_SendToAvatarID(_U32 avatar_id, const char* msg);
		{
			for(;;)
			{
				if(args.size()!=3) break;
				Json::Value server_id = args.get((Json::UInt)0, Json::nullValue);
				if(!server_id.isUInt()) break;
				Json::Value avatar_id = args.get((Json::UInt)1, Json::nullValue);
				if(!avatar_id.isUInt()) break;
				Json::Value msg = args.get((Json::UInt)2, Json::nullValue);
				if(!msg.isString()) break;
				RPCIMPL_SendToAvatarID((_U32)server_id.asUInt(), (_U32)avatar_id.asUInt(), msg.asCString());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_SendToAvatarName(const Json::Value& args)
		// void RPCIMPL_SendToAvatarName(const char* avatar_name, const char* msg);
		{
			for(;;)
			{
				if(args.size()!=3) break;
				Json::Value server_id = args.get((Json::UInt)0, Json::nullValue);
				if(!server_id.isUInt()) break;
				Json::Value avatar_name = args.get((Json::UInt)1, Json::nullValue);
				if(!avatar_name.isString()) break;
				Json::Value msg = args.get((Json::UInt)2, Json::nullValue);
				if(!msg.isString()) break;
				RPCIMPL_SendToAvatarName((_U32)server_id.asUInt(), avatar_name.asCString(), msg.asCString());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_JoinDomain(const Json::Value& args)
		// void RPCIMPL_JoinDomain(_U32 user_id, _U32 user_seq, _U32 domain_id);
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
				RPCIMPL_JoinDomain((_U32)user_id.asUInt(), (_U32)user_seq.asUInt(), (_U32)domain_id.asUInt());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_LeaveDomain(const Json::Value& args)
		// void RPCIMPL_LeaveDomain(_U32 user_id, _U32 user_seq, _U32 domain_id);
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
				RPCIMPL_LeaveDomain((_U32)user_id.asUInt(), (_U32)user_seq.asUInt(), (_U32)domain_id.asUInt());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_SendToDomain(const Json::Value& args)
		// void RPCIMPL_SendToDomain(_U32 domain_id, const char* msg);
		{
			for(;;)
			{
				if(args.size()!=2) break;
				Json::Value domain_id = args.get((Json::UInt)0, Json::nullValue);
				if(!domain_id.isUInt()) break;
				Json::Value msg = args.get((Json::UInt)1, Json::nullValue);
				if(!msg.isString()) break;
				RPCIMPL_SendToDomain((_U32)domain_id.asUInt(), msg.asCString());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_WaitForMessage(const Json::Value& args)
		// void RPCIMPL_WaitForMessage(_U32 user_id, _U32 user_seq, _U32 msg_seq);
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
				RPCIMPL_WaitForMessage((_U32)user_id.asUInt(), (_U32)user_seq.asUInt(), (_U32)msg_seq.asUInt());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_GetMessage(const Json::Value& args)
		// void RPCIMPL_GetMessage(_U32 user_id, _U32 user_seq, _U32 msg_seq);
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
				RPCIMPL_GetMessage((_U32)user_id.asUInt(), (_U32)user_seq.asUInt(), (_U32)msg_seq.asUInt());
				return;
			}
			JsonRPC_Send("[-1]");
		}

	}
}
