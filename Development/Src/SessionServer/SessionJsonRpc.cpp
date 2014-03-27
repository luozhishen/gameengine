#include <ZionBase.h>
#include <ZionCommon.h>
#include <JsonRPC.h>
#include "SessionJsonRPC.h"
#include "SessionRpcImpl.h"

namespace Zion
{
	namespace Session
	{

		void JsonRPC_Echo(const JsonValue& args)
		{
			JsonRPC_Send("[0]");
		}

		void JsonRpc_LoginUser(const JsonValue& args)
		// void RPCIMPL_LoginUser(_U32 user_id);
		{
			for(;;)
			{
				if(args.GetSize()!=1) break;
				const JsonValue& user_id = args.Get((_U32)0);
				if(!user_id.IsU32()) break;
				RPCIMPL_LoginUser(user_id.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_LogoutUser(const JsonValue& args)
		// void RPCIMPL_LogoutUser(_U32 user_id, _U32 user_seq);
		{
			for(;;)
			{
				if(args.GetSize()!=2) break;
				const JsonValue& user_id = args.Get((_U32)0);
				if(!user_id.IsU32()) break;
				const JsonValue& user_seq = args.Get((_U32)1);
				if(!user_seq.IsU32()) break;
				RPCIMPL_LogoutUser((_U32)user_id.AsU32(), (_U32)user_seq.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_LockUser(const JsonValue& args)
		// void RPCIMPL_LockUser(_U32 user_id, _U32 user_seq, _U32 req_seq)
		{
			for(;;)
			{
				if(args.GetSize()!=3) break;
				const JsonValue& user_id = args.Get((_U32)0);
				if(!user_id.IsU32()) break;
				const JsonValue& user_seq = args.Get((_U32)1);
				if(!user_seq.IsU32()) break;
				const JsonValue& req_seq = args.Get((_U32)2);
				if(!req_seq.IsU32()) break;
				RPCIMPL_LockUser((_U32)user_id.AsU32(), (_U32)user_seq.AsU32(), (_U32)req_seq.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_UnlockUser(const JsonValue& args)
		// void RPCIMPL_UnlockUser(_U32 user_id, _U32 user_seq, const char* last_response, const char* session_data)
		{
			for(;;)
			{
				if(args.GetSize()!=4) break;
				const JsonValue& user_id = args.Get((_U32)0);
				if(!user_id.IsU32()) break;
				const JsonValue& user_seq = args.Get((_U32)1);
				if(!user_seq.IsU32()) break;
				const JsonValue& last_response = args.Get((_U32)2);
				if(!last_response.IsSTR()) break;
				const JsonValue& session_data = args.Get((_U32)3);
				if(!user_seq.IsSTR()) break;
				RPCIMPL_UnlockUser((_U32)user_id.AsU32(), (_U32)user_seq.AsU32(), last_response.AsCSTR(),user_seq.AsCSTR());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_BindAvatar(const JsonValue& args)
		// void RPCIMPL_BindAvatar(_U32 user_id, _U32 user_seq, _U32 avatar_id, const char* avatar_name);
		{
			for(;;)
			{
				if(args.GetSize()!=5) break;
				const JsonValue& user_id = args.Get((_U32)0);
				if(!user_id.IsU32()) break;
				const JsonValue& user_seq = args.Get((_U32)1);
				if(!user_seq.IsU32()) break;
				const JsonValue& server_id = args.Get((_U32)2);
				if(!server_id.IsU32()) break;
				const JsonValue& avatar_id = args.Get((_U32)3);
				if(!avatar_id.IsU32()) break;
				const JsonValue& avatar_name = args.Get((_U32)4);
				if(!avatar_name.IsSTR()) break;
				RPCIMPL_BindAvatar((_U32)user_id.AsU32(), (_U32)user_seq.AsU32(), (_U32)server_id.AsU32(), (_U32)avatar_id.AsU32(), avatar_name.AsCSTR());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_UnbindAvatar(const JsonValue& args)
		// void RPCIMPL_UnbindAvatar(_U32 user_id, _U32 user_seq);
		{
			for(;;)
			{
				if(args.GetSize()!=2) break;
				const JsonValue& user_id = args.Get((_U32)0);
				if(!user_id.IsU32()) break;
				const JsonValue& user_seq = args.Get((_U32)1);
				if(!user_seq.IsU32()) break;
				RPCIMPL_UnbindAvatar((_U32)user_id.AsU32(), (_U32)user_seq.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_SendToUserID(const JsonValue& args)
		// void RPCIMPL_SendToUserID(_U32 user_id, const char* msg);
		{
			for(;;)
			{
				if(args.GetSize()!=2) break;
				const JsonValue& user_id = args.Get((_U32)0);
				if(!user_id.IsU32()) break;
				const JsonValue& msg = args.Get((_U32)1);
				if(!msg.IsSTR()) break;
				RPCIMPL_SendToUserID((_U32)user_id.AsU32(), msg.AsCSTR());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_SendToAvatarID(const JsonValue& args)
		// void RPCIMPL_SendToAvatarID(_U32 avatar_id, const char* msg);
		{
			for(;;)
			{
				if(args.GetSize()!=3) break;
				const JsonValue& server_id = args.Get((_U32)0);
				if(!server_id.IsU32()) break;
				const JsonValue& avatar_id = args.Get((_U32)1);
				if(!avatar_id.IsU32()) break;
				const JsonValue& msg = args.Get((_U32)2);
				if(!msg.IsSTR()) break;
				RPCIMPL_SendToAvatarID((_U32)server_id.AsU32(), (_U32)avatar_id.AsU32(), msg.AsCSTR());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_SendToAvatarName(const JsonValue& args)
		// void RPCIMPL_SendToAvatarName(const char* avatar_name, const char* msg);
		{
			for(;;)
			{
				if(args.GetSize()!=3) break;
				const JsonValue& server_id = args.Get((_U32)0);
				if(!server_id.IsU32()) break;
				const JsonValue& avatar_name = args.Get((_U32)1);
				if(!avatar_name.IsSTR()) break;
				const JsonValue& msg = args.Get((_U32)2);
				if(!msg.IsSTR()) break;
				RPCIMPL_SendToAvatarName((_U32)server_id.AsU32(), avatar_name.AsCSTR(), msg.AsCSTR());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_JoinDomain(const JsonValue& args)
		// void RPCIMPL_JoinDomain(_U32 user_id, _U32 user_seq, _U32 domain_id);
		{
			for(;;)
			{
				if(args.GetSize()!=3) break;
				const JsonValue& user_id = args.Get((_U32)0);
				if(!user_id.IsU32()) break;
				const JsonValue& user_seq = args.Get((_U32)1);
				if(!user_seq.IsU32()) break;
				const JsonValue& domain_id = args.Get((_U32)2);
				if(!domain_id.IsU32()) break;
				RPCIMPL_JoinDomain((_U32)user_id.AsU32(), (_U32)user_seq.AsU32(), (_U32)domain_id.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_LeaveDomain(const JsonValue& args)
		// void RPCIMPL_LeaveDomain(_U32 user_id, _U32 user_seq, _U32 domain_id);
		{
			for(;;)
			{
				if(args.GetSize()!=3) break;
				const JsonValue& user_id = args.Get((_U32)0);
				if(!user_id.IsU32()) break;
				const JsonValue& user_seq = args.Get((_U32)1);
				if(!user_seq.IsU32()) break;
				const JsonValue& domain_id = args.Get((_U32)2);
				if(!domain_id.IsU32()) break;
				RPCIMPL_LeaveDomain((_U32)user_id.AsU32(), (_U32)user_seq.AsU32(), (_U32)domain_id.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_SendToDomain(const JsonValue& args)
		// void RPCIMPL_SendToDomain(_U32 domain_id, const char* msg);
		{
			for(;;)
			{
				if(args.GetSize()!=2) break;
				const JsonValue& domain_id = args.Get((_U32)0);
				if(!domain_id.IsU32()) break;
				const JsonValue& msg = args.Get((_U32)1);
				if(!msg.IsSTR()) break;
				RPCIMPL_SendToDomain((_U32)domain_id.AsU32(), msg.AsCSTR());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_WaitForMessage(const JsonValue& args)
		// void RPCIMPL_WaitForMessage(_U32 user_id, _U32 user_seq, _U32 msg_seq);
		{
			for(;;)
			{
				if(args.GetSize()!=3) break;
				const JsonValue& user_id = args.Get((_U32)0);
				if(!user_id.IsU32()) break;
				const JsonValue& user_seq = args.Get((_U32)1);
				if(!user_seq.IsU32()) break;
				const JsonValue& msg_seq = args.Get((_U32)1);
				if(!msg_seq.IsU32()) break;
				RPCIMPL_WaitForMessage((_U32)user_id.AsU32(), (_U32)user_seq.AsU32(), (_U32)msg_seq.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_GetMessage(const JsonValue& args)
		// void RPCIMPL_GetMessage(_U32 user_id, _U32 user_seq, _U32 msg_seq);
		{
			for(;;)
			{
				if(args.GetSize()!=3) break;
				const JsonValue& user_id = args.Get((_U32)0);
				if(!user_id.IsU32()) break;
				const JsonValue& user_seq = args.Get((_U32)1);
				if(!user_seq.IsU32()) break;
				const JsonValue& msg_seq = args.Get((_U32)1);
				if(!msg_seq.IsU32()) break;
				RPCIMPL_GetMessage((_U32)user_id.AsU32(), (_U32)user_seq.AsU32(), (_U32)msg_seq.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

	}
}
