#ifndef __ZION_SESSION_RPCIMPL__
#define __ZION_SESSION_RPCIMPL__

namespace Zion
{
	namespace Session
	{

		void RPCIMPL_LoginUser(_U32 user_id);
		// return errcode, session_key
		void RPCIMPL_LogoutUser(const String& session_key);
		// return errcode
		void RPCIMPL_KickUser(_U32 user_id);
		// return errcode

		void RPCIMPL_LockSession(const String& session_key);
		// return errcode, user_id, server_id, avatar_id, request_seq
		void RPCIMPL_UnlockSession(const String& session_key, _U32 req_seq);
		// return errcode
		void RPCIMPL_GetSessionInfo(_U32 user_id);
		// return error, session_info {xxx=xxxx,xxx=xxxx}

		void RPCIMPL_BindAvatar(_U32 user_id, _U32 server_id, _U32 avatar_id, const String& avatar_name);
		// return errcode
		void RPCIMPL_UnbindAvatar(_U32 user_id);
		// return errcode

		void RPCIMPL_SendToUserID(_U32 user_id, const String& msg);
		// return errcode
		void RPCIMPL_SendToAvatarID(_U32 server_id, _U32 avatar_id, const String& msg);
		// return errcode
		void RPCIMPL_SendToAvatarName(_U32 server_id, const String& avatar_name, const String& msg);
		// return errcode

		void RPCIMPL_JoinDomain(_U32 user_id, _U32 domain_id);
		// return errcode
		void RPCIMPL_LeaveDomain(_U32 user_id, _U32 domain_id);
		// return errcode
		void RPCIMPL_SendToDomain(_U32 domain_id, const String& msg);
		// return errcode
		void RPCIMPL_GetDomainMemberList(_U32 domain_id);
		// return errcode
		void RPCIMPL_GetDomainMemberCount(_U32 domain_id);
		// return errcode, count

		void RPCIMPL_WaitForMessage(const String& session_key, _U32 msg_seq);
		// return errcode, msg[]
		void RPCIMPL_GetMessage(const String& session_key, _U32 msg_seq);
		// return errcode, msg[]

	}
}

#endif
