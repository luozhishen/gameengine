#ifndef __ZION_SESSION_RPCIMPL__
#define __ZION_SESSION_RPCIMPL__

namespace Zion
{
	namespace Session
	{

		void RPCIMPL_LoginUser(_U32 user_id);
		// return errcode, _U32 user_seq
		void RPCIMPL_LogoutUser(_U32 user_id, _U32 user_seq);
		// return errcode
		void RPCIMPL_KickUser(_U32 user_id);
		// return errcode

		void RPCIMPL_GetSession(_U32 user_id, _U32 user_seq, _U32 req_seq, bool with_lock);
		// return errcode, server_id, avatar_id, last_response, session_data
		void RPCIMPL_SetSession(_U32 user_id, _U32 user_seq, _U32 req_seq, const String& session_data, bool free_lock);
		// return errcode

		void RPCIMPL_BindAvatar(_U32 user_id, _U32 user_seq, _U32 server_id, _U32 avatar_id, const String& avatar_name);
		// return errcode
		void RPCIMPL_UnbindAvatar(_U32 user_id, _U32 user_seq);
		// return errcode

		void RPCIMPL_SendToUserID(_U32 user_id, const String& msg);
		// return errcode
		void RPCIMPL_SendToAvatarID(_U32 server_id, _U32 avatar_id, const String& msg);
		// return errcode
		void RPCIMPL_SendToAvatarName(_U32 server_id, const String& avatar_name, const String& msg);
		// return errcode

		void RPCIMPL_JoinDomain(_U32 user_id, _U32 user_seq, _U32 domain_id);
		// return errcode
		void RPCIMPL_LeaveDomain(_U32 user_id, _U32 user_seq, _U32 domain_id);
		// return errcode
		void RPCIMPL_SendToDomain(_U32 domain_id, const String& msg);
		// return errcode

		void RPCIMPL_WaitForMessage(_U32 user_id, _U32 user_seq, _U32 msg_seq);
		// return errcode, msg[]
		void RPCIMPL_GetMessage(_U32 user_id, _U32 user_seq, _U32 msg_seq);
		// return errcode, msg[]

	}
}

#endif
