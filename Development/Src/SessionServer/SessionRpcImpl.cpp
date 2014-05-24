#include <ZionBase.h>
#include <JsonRPC.h>
#include "SessionRpcImpl.h"
#include "UserSession.h"

namespace Zion
{
	namespace Session
	{

		void RPCIMPL_LoginUser(_U32 user_id)
		// return errcode, session_key
		{
			CUserSession* session = CUserSession::Login(user_id);
			if(session)
			{
				JsonRPC_Send(StringFormat("[0,\"%s\"]", session->GetSessionKey().c_str()).c_str());
				session->Lock();
				CUserSession::Unlock(session);
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_LogoutUser(const String& session_key)
		// return errcode
		{
			if(CUserSession::Logout(session_key))
			{
				JsonRPC_Send("[0]");
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_KickUser(_U32 user_id)
		// return errcode
		{
			CUserSession* session = CUserSession::LockByUser(user_id);
			if(session)
			{
				CUserSession::Unlock(session);
			}
			JsonRPC_Send("[0]");
		}

		void RPCIMPL_LockSession(const String& session_key)
		// return errcode, user_id, scope_id, avatar_id, request_seq
		{
			CUserSession* session = CUserSession::LockByUser(session_key);
			if(session)
			{
				if(session->Lock())
				{
					JsonRPC_Send(StringFormat("[0,%u,%d,%d,%u]", session->GetUserID(), (_S32)session->GetAvatarScope(), (_S32)session->GetAvatarID(), session->GetReqSeq()).c_str());
				}
				else
				{
					JsonRPC_Send("[\"TRY_AGAIN\"]");
				}
				CUserSession::Unlock(session);
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_UnlockSession(const String& session_key, _U32 req_seq)
		// return errcode
		{
			CUserSession* session = CUserSession::LockByUser(session_key);
			if(session)
			{
				session->Unlock(req_seq!=0);
				CUserSession::Unlock(session);
				JsonRPC_Send("[0]");
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_GetSessionInfo(_U32 user_id)
		// return error, session_info {xxx=xxxx,xxx=xxxx}
		{
			CUserSession* session = CUserSession::LockByUser(user_id);
			if(session)
			{
				ZION_ASSERT(0);
				CUserSession::Unlock(session);
				JsonRPC_Send("[0]");
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_BindAvatar(_U32 user_id, _U32 server_id, _U32 avatar_id, const String& avatar_name)
		// return errcode
		{
			CUserSession* session = CUserSession::LockByUser(user_id);
			if(session)
			{
				if(session->BindAvatar(server_id, avatar_id, avatar_name))
				{
					CUserSession::Unlock(session);
					JsonRPC_Send("[0]");
					return;
				}
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_UnbindAvatar(_U32 user_id)
		// return errcode
		{
			CUserSession* session = CUserSession::LockByUser(user_id);
			if(session)
			{
				if(session->UnbindAvatar())
				{
					CUserSession::Unlock(session);
					JsonRPC_Send("[0]");
					return;
				}
				CUserSession::Unlock(session);
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_SendToUserID(_U32 user_id, const String& msg)
		// return errcode
		{
			CUserSession* session = CUserSession::LockByUser(user_id);
			if(session)
			{
				CMessage Msg(msg);
				if(session->SendMsg(Msg))
				{
					CUserSession::Unlock(session);
					JsonRPC_Send("[0]");
					return;
				}
				CUserSession::Unlock(session);
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_SendToAvatarID(_U32 server_id, _U32 avatar_id, const String& msg)
		// return errcode
		{
			CUserSession* session = CUserSession::LockByAvatar(server_id, avatar_id);
			if(session)
			{
				CMessage Msg(msg);
				if(session->SendMsg(Msg))
				{
					CUserSession::Unlock(session);
					JsonRPC_Send("[0]");
					return;
				}
				CUserSession::Unlock(session);
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_SendToAvatarName(_U32 server_id, const String& avatar_name, const String& msg)
		// return errcode
		{
			CUserSession* session = CUserSession::LockByAvatar(server_id, avatar_name);
			if(session)
			{
				CMessage Msg(msg);
				if(session->SendMsg(Msg))
				{
					CUserSession::Unlock(session);
					JsonRPC_Send("[0]");
					return;
				}
				CUserSession::Unlock(session);
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_JoinDomain(_U32 user_id, _U32 domain_id)
		// return errcode
		{
			CUserSession* session = CUserSession::LockByUser(user_id);
			if(session)
			{
				if(session->JoinDomain(domain_id))
				{
					CUserSession::Unlock(session);
					JsonRPC_Send("[0]");
					return;
				}
				CUserSession::Unlock(session);
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_LeaveDomain(_U32 user_id, _U32 domain_id)
		// return errcode
		{
			CUserSession* session = CUserSession::LockByUser(user_id);
			if(session)
			{
				if(session->LeaveDomain(domain_id))
				{
					CUserSession::Unlock(session);
					JsonRPC_Send("[0]");
					return;
				}
				CUserSession::Unlock(session);
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_SendToDomain(_U32 domain_id, const String& msg)
		// return errcode
		{
			CDomain* domain = CDomain::Lock(domain_id, false);
			if(domain)
			{
				if(domain->SendMsg(msg))
				{
					CDomain::Unlock(domain);
					JsonRPC_Send("[0]");
					return;
				}
				CDomain::Unlock(domain);
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_GetDomainMemberList(_U32 domain_id)
		// return errcode
		{
			CDomain* domain = CDomain::Lock(domain_id, false);
			if(domain)
			{
				domain->SendMemberList();
				CDomain::Unlock(domain);
				return;
			}
			JsonRPC_Send("[0, []]");
		}

		void RPCIMPL_GetDomainMemberCount(_U32 domain_id)
		// return errcode, count
		{
			CDomain* domain = CDomain::Lock(domain_id, false);
			if(domain)
			{
				_U32 count = domain->GetMemberCount();
				CDomain::Unlock(domain);
				JsonRPC_Send(StringFormat("[0,%u]", count).c_str());
				return;
			}
			JsonRPC_Send("[0, 0]");
		}

		void RPCIMPL_WaitForMessage(const String& session_key, _U32 msg_seq)
		// return errcode, msg[]
		{
			CUserSession* session = CUserSession::LockByUser(session_key);
			if(!session)
			{
				JsonRPC_Send("[-1]");
				return;
			}

			if(session->GetMsgSeq()==msg_seq)
			{
				JsonRPC_Send(session->GetLastMsg().c_str());
			}
			else if(session->GetMsgSeq()+1==msg_seq)
			{
				session->PullMsg();
				if(session->GetMsgSeq()==msg_seq)
				{
					JsonRPC_Send(session->GetLastMsg().c_str());
				}
				else
				{
					JSONRPC_RESPONSE_ID res;
					if(JsonRPC_SetPending(res))
					{
						session->WaitMsg(res);
					}
					else
					{
						JsonRPC_Send("[-1]");
					}
				}
			}
			else
			{
				JsonRPC_Send("[-1]");
			}

			CUserSession::Unlock(session);
		}

		void RPCIMPL_GetMessage(const String& session_key, _U32 msg_seq)
		// return errcode, msg[]
		{
			CUserSession* session = CUserSession::LockByUser(session_key);
			if(!session)
			{
				JsonRPC_Send("[-1]");
				return;
			}

			if(session->GetMsgSeq()==msg_seq)
			{
				JsonRPC_Send(session->GetLastMsg().c_str());
			}
			else if(session->GetMsgSeq()+1==msg_seq)
			{
				session->PullMsg();
				if(session->GetMsgSeq()==msg_seq)
				{
					JsonRPC_Send(session->GetLastMsg().c_str());
				}
				else
				{
					JsonRPC_Send("[-1]");
				}
			}
			else
			{
				JsonRPC_Send("[-1]");
			}

			CUserSession::Unlock(session);
		}

	}
}
