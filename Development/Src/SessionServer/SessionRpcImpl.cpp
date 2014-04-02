#include <ZionBase.h>
#include <JsonRPC.h>
#include "SessionRpcImpl.h"
#include "UserSession.h"

namespace Zion
{
	namespace Session
	{

		void RPCIMPL_LoginUser(_U32 user_id)
		// return errcode, _U32 user_seq
		{
			CUserSession* session = CUserSession::Login(user_id);
			if(session)
			{
				JsonRPC_Send(StringFormat("[0,%u]", session->GetUserSeq()).c_str());
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_LogoutUser(_U32 user_id, _U32 user_seq)
		// return errcode
		{
			if(CUserSession::Logout(user_id, user_seq))
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

		void RPCIMPL_GetSession(_U32 user_id, _U32 user_seq, _U32 req_seq, bool with_lock)
		// return errcode, server_id, avatar_id, last_response, session_data
		{
			CUserSession* session = CUserSession::LockByUser(user_id, user_seq);
			if(session)
			{
				if(req_seq==session->GetReqSeq())
				{
					JsonRPC_Send(StringFormat("[0,%u,%u,\"%s\",\"\"]", session->GetServerID(), session->GetAvatarID(), session->GetLastResponse().c_str()).c_str());
					CUserSession::Unlock(session);
					return;
				}
				if(req_seq==session->GetReqSeq()+1 && session->Lock())
				{
					JsonRPC_Send(StringFormat("[0,%u,%u,\"\",\"%s\"]", session->GetServerID(), session->GetAvatarID(), session->GetSessionData().c_str()).c_str());
					CUserSession::Unlock(session);
					return;
				}
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_SetSession(_U32 user_id, _U32 user_seq, _U32 req_seq, const String& session_data, bool free_lock)
		// return errcode
		{
			CUserSession* session = CUserSession::LockByUser(user_id, user_seq);
			if(session)
			{
				/*
				if(session->Unlock(last_response, session_data))
				{
					JsonRPC_Send("[0]");
					return;
				}
				*/
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_EnterServer(_U32 user_id, _U32 user_seq, _U32 server_id)
		// return errcode
		{
			CUserSession* session = CUserSession::LockByUser(user_id, user_seq);
			if(session)
			{
				session->SetServer(server_id);
				CUserSession::Unlock(session);
				JsonRPC_Send("[0]");
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_LeaveServer(_U32 user_id, _U32 user_seq)
		// return errcode
		{
			CUserSession* session = CUserSession::LockByUser(user_id, user_seq);
			if(session)
			{
				session->SetServer((_U32)-1);
				CUserSession::Unlock(session);
				JsonRPC_Send("[0]");
				return;
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_BindAvatar(_U32 user_id, _U32 user_seq, _U32 server_id, _U32 avatar_id, const String& avatar_name)
		// return errcode
		{
			CUserSession* session = CUserSession::LockByUser(user_id, user_seq);
			if(session)
			{
				if(session->BindAvatar(server_id, avatar_id, avatar_name))
				{
					JsonRPC_Send("[0]");
					return;
				}
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_UnbindAvatar(_U32 user_id, _U32 user_seq, _U32 server_id)
		// return errcode
		{
			CUserSession* session = CUserSession::LockByUser(user_id, user_seq);
			if(!session)
			{
				if(session->UnbindAvatar())
				{
					session->SetServer(server_id);
					JsonRPC_Send("[0]");
					return;
				}
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
					JsonRPC_Send("[0]");
					return;
				}
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
					JsonRPC_Send("[0]");
					return;
				}
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
					JsonRPC_Send("[0]");
					return;
				}
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_JoinDomain(_U32 user_id, _U32 user_seq, _U32 domain_id)
		// return errcode
		{
			CUserSession* session = CUserSession::LockByUser(user_id, user_seq);
			if(session)
			{
				if(session->JoinDomain(domain_id))
				{
					JsonRPC_Send("[0]");
					return;
				}
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_LeaveDomain(_U32 user_id, _U32 user_seq, _U32 domain_id)
		// return errcode
		{
			CUserSession* session = CUserSession::LockByUser(user_id, user_seq);
			if(session)
			{
				if(session->LeaveDomain(domain_id))
				{
					JsonRPC_Send("[0]");
					return;
				}
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

		void RPCIMPL_WaitForMessage(_U32 user_id, _U32 user_seq, _U32 msg_seq)
		// return errcode, msg[]
		{
			CUserSession* session = CUserSession::LockByUser(user_id, user_seq);
			if(session)
			{
				if(session->GetMsg(msg_seq))
				{
					JsonRPC_Send(session->GetLastMsg().c_str());
					CUserSession::Unlock(session);
					return;
				}

				if(session->GetMsgSeq()+1==msg_seq)
				{
					JSONRPC_RESPONSE_ID res;
					if(JsonRPC_SetPending(res))
					{
						session->WaitMsg(res);
						CUserSession::Unlock(session);
						return;
					}
				}
			}
			JsonRPC_Send("[-1]");
		}

		void RPCIMPL_GetMessage(_U32 user_id, _U32 user_seq, _U32 msg_seq)
		// return errcode, msg[]
		{
			CUserSession* session = CUserSession::LockByUser(user_id, user_seq);
			if(session)
			{
				if(session->GetMsg(msg_seq))
				{
					JsonRPC_Send(session->GetLastMsg().c_str());
					CUserSession::Unlock(session);
					return;
				}
			}
			JsonRPC_Send("[-1]");
		}

	}
}
