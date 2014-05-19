#ifndef __ZION_SESSION_JSONRPC__
#define __ZION_SESSION_JSONRPC__

namespace Zion
{
	namespace Session
	{

		void JsonRPC_Echo(const JsonValue& args);
		void JsonRpc_LoginUser(const JsonValue& args);
		void JsonRpc_LogoutUser(const JsonValue& args);
		void JsonRpc_KickUser(const JsonValue& args);
		void JsonRpc_LockSession(const JsonValue& args);
		void JsonRpc_UnlockSession(const JsonValue& args);
		void JsonRpc_GetSessionInfo(const JsonValue& args);
		void JsonRpc_BindAvatar(const JsonValue& args);
		void JsonRpc_UnbindAvatar(const JsonValue& args);
		void JsonRpc_SendToUserID(const JsonValue& args);
		void JsonRpc_SendToAvatarID(const JsonValue& args);
		void JsonRpc_SendToAvatarName(const JsonValue& args);
		void JsonRpc_JoinDomain(const JsonValue& args);
		void JsonRpc_LeaveDomain(const JsonValue& args);
		void JsonRpc_SendToDomain(const JsonValue& args);
		void JsonRpc_GetDomainMemberList(const JsonValue& args);
		void JsonRpc_GetDomainMemberCount(const JsonValue& args);
		void JsonRpc_WaitForMessage(const JsonValue& args);
		void JsonRpc_GetMessage(const JsonValue& args);

	}
}

#endif
