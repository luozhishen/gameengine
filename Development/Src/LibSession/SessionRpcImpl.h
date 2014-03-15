#ifndef __ZION_SESSION_RPCIMPL__
#define __ZION_SESSION_RPCIMPL__

namespace Zion
{
	namespace Session
	{

		void RPCIMPL_LoginUser(const JSONRPC_RESPONSE& res, _U32 user_id);
		// return errcode, _U32 user_seq
		void RPCIMPL_LogoutUser(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq);
		// return errcode

		void RPCIMPL_LockUser(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq);
		// return errcode, session_data
		void RPCIMPL_UnlockUser(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq, const char* session_data);
		// return errcode

		void RPCIMPL_BindAvatar(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq, _U32 avatar_id, const char* avatar_name);
		// return errcode
		void RPCIMPL_UnbindAvatar(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq);
		// return errcode

		void RPCIMPL_SendToUserID(const JSONRPC_RESPONSE& res, _U32 user_id, const char* msg);
		// return errcode
		void RPCIMPL_SendToAvatarID(const JSONRPC_RESPONSE& res, _U32 avatar_id, const char* msg);
		// return errcode
		void RPCIMPL_SendToAvatarName(const JSONRPC_RESPONSE& res, const char* avatar_name, const char* msg);
		// return errcode

		void RPCIMPL_JoinDomain(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq, _U32 domain_id);
		// return errcode
		void RPCIMPL_LeaveDomain(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq, _U32 domain_id);
		// return errcode
		void RPCIMPL_SendToDomain(const JSONRPC_RESPONSE& res, _U32 domain_id, const char* msg);
		// return errcode

		void RPCIMPL_WaitForMessage(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq, _U32 msg_seq);
		// return errcode, msg_seq, msg[]
		void RPCIMPL_GetMessage(const JSONRPC_RESPONSE& res, _U32 user_id, _U32 user_seq, _U32 msg_seq);
		// return errcode, msg_seq, msg[]

	}
}

#endif
