#ifndef __ZION_SESSION_JSONRPC__
#define __ZION_SESSION_JSONRPC__

namespace Zion
{
	namespace Session
	{

		void JsonRpc_LoginUser(const JSONRPC_RESPONSE& res, const Json::Value& args);
		void JsonRpc_LogoutUser(const JSONRPC_RESPONSE& res, const Json::Value& args);
		void JsonRpc_LockUser(const JSONRPC_RESPONSE& res, const Json::Value& args);
		void JsonRpc_UnlockUser(const JSONRPC_RESPONSE& res, const Json::Value& args);
		void JsonRpc_BindAvatar(const JSONRPC_RESPONSE& res, const Json::Value& args);
		void JsonRpc_UnbindAvatar(const JSONRPC_RESPONSE& res, const Json::Value& args);
		void JsonRpc_SendToUserID(const JSONRPC_RESPONSE& res, const Json::Value& args);
		void JsonRpc_SendToAvatarID(const JSONRPC_RESPONSE& res, const Json::Value& args);
		void JsonRpc_SendToAvatarName(const JSONRPC_RESPONSE& res, const Json::Value& args);
		void JsonRpc_JoinDomain(const JSONRPC_RESPONSE& res, const Json::Value& args);
		void JsonRpc_LeaveDomain(const JSONRPC_RESPONSE& res, const Json::Value& args);
		void JsonRpc_SendToDomain(const JSONRPC_RESPONSE& res, const Json::Value& args);
		void JsonRpc_WaitForMessage(const JSONRPC_RESPONSE& res, const Json::Value& args);
		void JsonRpc_GetMessage(const JSONRPC_RESPONSE& res, const Json::Value& args);

	}
}

#endif
