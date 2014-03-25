#ifndef __ZION_SESSION_JSONRPC__
#define __ZION_SESSION_JSONRPC__

namespace Zion
{
	namespace Session
	{

		void JsonRPC_Echo(const Json::Value& args);
		void JsonRpc_LoginUser(const Json::Value& args);
		void JsonRpc_LogoutUser(const Json::Value& args);
		void JsonRpc_LockUser(const Json::Value& args);
		void JsonRpc_UnlockUser(const Json::Value& args);
		void JsonRpc_BindAvatar(const Json::Value& args);
		void JsonRpc_UnbindAvatar(const Json::Value& args);
		void JsonRpc_SendToUserID(const Json::Value& args);
		void JsonRpc_SendToAvatarID(const Json::Value& args);
		void JsonRpc_SendToAvatarName(const Json::Value& args);
		void JsonRpc_JoinDomain(const Json::Value& args);
		void JsonRpc_LeaveDomain(const Json::Value& args);
		void JsonRpc_SendToDomain(const Json::Value& args);
		void JsonRpc_WaitForMessage(const Json::Value& args);
		void JsonRpc_GetMessage(const Json::Value& args);

	}
}

#endif
