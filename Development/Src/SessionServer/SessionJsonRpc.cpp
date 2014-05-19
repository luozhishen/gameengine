#include <ZionBase.h>
#include <ZionCommon.h>
#include <JsonRPC.h>
#include "SessionJsonRpc.h"
#include "SessionRpcImpl.h"

namespace Zion
{
	namespace Session
	{

		bool make_domain_id(_U32 domain_level, _U32 domain_id, _U32& real_id)
		{
			if(domain_level<30)
			{
				_U32 max = (1 << (32 - domain_level -1));
				if(domain_id<max)
				{
					real_id = (((_U32)-1) << (32 - domain_level)) | domain_id;
					return true;
				}
			}
			return false;
		}

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
				RPCIMPL_LogoutUser(user_id.AsU32(), user_seq.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_KickUser(const JsonValue& args)
		// void RPCIMPL_KickUser(_U32 user_id);
		{
			for(;;)
			{
				if(args.GetSize()!=1) break;
				const JsonValue& user_id = args.Get((_U32)0);
				if(!user_id.IsU32()) break;
				RPCIMPL_KickUser(user_id.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_LockSession(const JsonValue& args)
		// void RPCIMPL_LockSession(_U32 user_id, _U32 user_seq, _U32 req_seq)
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
				RPCIMPL_LockSession(user_id.AsU32(), user_seq.AsU32(), req_seq.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_UnlockSession(const JsonValue& args)
		// void RPCIMPL_UnlockSession(_U32 user_id, _U32 user_seq, _U32 request_seq)
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
				RPCIMPL_UnlockSession(user_id.AsU32(), user_seq.AsU32(), req_seq.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_GetSessionInfo(const JsonValue& args)
		// void RPCIMPL_GetSessionInfo(_U32 user_id)
		{
			for(;;)
			{
				if(args.GetSize()!=4) break;
				const JsonValue& user_id = args.Get((_U32)0);
				if(!user_id.IsU32()) break;
				RPCIMPL_GetSessionInfo(user_id.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_BindAvatar(const JsonValue& args)
		// void RPCIMPL_BindAvatar(_U32 user_id, _U32 user_seq, _U32 avatar_id, const char* avatar_name);
		{
			for(;;)
			{
				if(args.GetSize()!=4) break;
				const JsonValue& user_id = args.Get((_U32)0);
				if(!user_id.IsU32()) break;
				const JsonValue& server_id = args.Get((_U32)1);
				if(!server_id.IsU32()) break;
				const JsonValue& avatar_id = args.Get((_U32)2);
				if(!avatar_id.IsU32()) break;
				const JsonValue& avatar_name = args.Get((_U32)3);
				if(!avatar_name.IsSTR()) break;
				RPCIMPL_BindAvatar(user_id.AsU32(), server_id.AsU32(), avatar_id.AsU32(), avatar_name.AsSTR());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_UnbindAvatar(const JsonValue& args)
		// void RPCIMPL_UnbindAvatar(_U32 user_id, _U32 user_seq);
		{
			for(;;)
			{
				if(args.GetSize()!=1) break;
				const JsonValue& user_id = args.Get((_U32)0);
				if(!user_id.IsU32()) break;
				RPCIMPL_UnbindAvatar(user_id.AsU32());
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
				RPCIMPL_SendToUserID(user_id.AsU32(), msg.AsSTR());
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
				RPCIMPL_SendToAvatarID(server_id.AsU32(), avatar_id.AsU32(), msg.AsSTR());
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
				RPCIMPL_SendToAvatarName(server_id.AsU32(), avatar_name.AsSTR(), msg.AsSTR());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_JoinDomain(const JsonValue& args)
		// void RPCIMPL_JoinDomain(_U32 user_id, _U32 user_seq, _U32 domain_id);
		{
			for(;;)
			{
				if(args.GetSize()!=4) break;
				const JsonValue& user_id = args.Get((_U32)0);
				if(!user_id.IsU32()) break;
				const JsonValue& domain_lv = args.Get((_U32)1);
				if(!domain_lv.IsU32()) break;
				const JsonValue& domain_id = args.Get((_U32)2);
				if(!domain_id.IsU32()) break;
				_U32 real_id;
				if(!make_domain_id(domain_lv.AsU32(), domain_id.AsU32(), real_id)) break;
				RPCIMPL_JoinDomain(user_id.AsU32(), real_id);
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
				const JsonValue& domain_lv = args.Get((_U32)1);
				if(!domain_lv.IsU32()) break;
				const JsonValue& domain_id = args.Get((_U32)2);
				if(!domain_id.IsU32()) break;
				_U32 real_id;
				if(!make_domain_id(domain_lv.AsU32(), domain_id.AsU32(), real_id)) break;
				RPCIMPL_LeaveDomain(user_id.AsU32(), real_id);
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_SendToDomain(const JsonValue& args)
		// void RPCIMPL_SendToDomain(_U32 domain_id, const char* msg);
		{
			for(;;)
			{
				if(args.GetSize()!=3) break;
				const JsonValue& domain_lv = args.Get((_U32)0);
				if(!domain_lv.IsU32()) break;
				const JsonValue& domain_id = args.Get((_U32)1);
				if(!domain_id.IsU32()) break;
				const JsonValue& msg = args.Get((_U32)2);
				if(!msg.IsSTR()) break;
				_U32 real_id;
				if(!make_domain_id(domain_lv.AsU32(), domain_id.AsU32(), real_id)) break;
				RPCIMPL_SendToDomain(real_id, msg.AsSTR());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_GetDomainMemberList(const JsonValue& args)
		// void RPCIMPL_GetDomainMemberList(_U32 domain_level, _U32 domain_id);
		{
			for(;;)
			{
				if(args.GetSize()!=2) break;
				const JsonValue& domain_lv = args.Get((_U32)0);
				if(!domain_lv.IsU32()) break;
				const JsonValue& domain_id = args.Get((_U32)1);
				if(!domain_id.IsU32()) break;
				_U32 real_id;
				if(!make_domain_id(domain_lv.AsU32(), domain_id.AsU32(), real_id)) break;
				RPCIMPL_GetDomainMemberList(real_id);
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void JsonRpc_GetDomainMemberCount(const JsonValue& args)
		// void RPCIMPL_GetDomainMemberCount(_U32 domain_level, _U32 domain_id);
		{
			for(;;)
			{
				if(args.GetSize()!=2) break;
				const JsonValue& domain_lv = args.Get((_U32)0);
				if(!domain_lv.IsU32()) break;
				const JsonValue& domain_id = args.Get((_U32)1);
				if(!domain_id.IsU32()) break;
				_U32 real_id;
				if(!make_domain_id(domain_lv.AsU32(), domain_id.AsU32(), real_id)) break;
				RPCIMPL_GetDomainMemberCount(real_id);
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
				RPCIMPL_WaitForMessage(user_id.AsU32(), user_seq.AsU32(), msg_seq.AsU32());
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
				RPCIMPL_GetMessage(user_id.AsU32(), user_seq.AsU32(), msg_seq.AsU32());
				return;
			}
			JsonRPC_Send("[-1]");
		}

	}
}
