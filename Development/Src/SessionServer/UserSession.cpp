#include <ZionBase.h>
#include <JsonRPC.h>
#include <uv.h>
#include <ObjectManager.h>
#include "UserSession.h"

namespace Zion
{
	namespace Session
	{
		
		class CServerManager
		{
		public:
			CServerManager()
			{
				A_MUTEX_INIT(&m_Locker);
			}
			~CServerManager()
			{
				A_MUTEX_DESTROY(&m_Locker);
			}

			void Change(_U32 New, _U32 Old)
			{
				Map<_U32, _U32>::iterator i;
				A_MUTEX_LOCK(&m_Locker);
				if(New!=(_U32)-1)
				{
					i = m_Servers.find(New);
					if(i==m_Servers.end())
					{
						m_Servers[New] = 1;
					}
					else
					{
						i->second += 1;
					}
				}
				if(Old!=(_U32)-1)
				{
					i = m_Servers.find(New);
					if(i!=m_Servers.end())
					{
						ZION_ASSERT(i->second>0);
						if(i->second>0)
						{
							i->second -= 1;
						}
					}
				}
				A_MUTEX_UNLOCK(&m_Locker);
			}

			void Send()
			{

				OutputStringStream stream;
				Map<_U32, _U32>::iterator i;
				_U32 count = 0;
				A_MUTEX_LOCK(&m_Locker);
				for(i=m_Servers.begin(); i!=m_Servers.end(); i++)
				{
					if(i==m_Servers.begin())
					{
						stream << ",";
					}
					stream << "\"" << i->first << "\":" << i->second;
					count += i->second;
				}
				A_MUTEX_UNLOCK(&m_Locker);

				String out = StringFormat("[0, %u, [%s]", count, stream.str().c_str());
				JsonRPC_Send(out.c_str());
			}

		private:
			A_MUTEX m_Locker;
			Map<_U32, _U32> m_Servers;
		};
		static CServerManager g_ServerManager;

		CMessage::CMessage(const CMessage& val)
		{
			m_pData = val.m_pData;
			m_pData->count += 1;
		}

		CMessage::CMessage(const String& msg)
		{
			m_pData = (MESSAGE_DATA*)ZION_ALLOC(sizeof(MESSAGE_DATA)+(_U32)msg.size());
			m_pData->size = (_U32)msg.size();
			m_pData->count = 1;
			memcpy(m_pData->msg, msg.c_str(), msg.size()+1);
		}

		CMessage::~CMessage()
		{
			m_pData->count -= 1;
			if(m_pData==0)
			{
				ZION_FREE(m_pData);
				m_pData = NULL;
			}
		}

		_U32 CMessage::GetSize()
		{
			return m_pData->size;
		}

		const char* CMessage::GetMsg()
		{
			return m_pData->msg;
		}

		static TObjectManager<CDomain, 100*10000> g_domain_manager;
		static TObjectMap<_U32> g_domain_id_map;

		CDomain* CDomain::Lock(_U32 nDomainID, bool alloc)
		{
			CDomain* domain;

			for(;;)
			{
				_U32 index = g_domain_id_map.Get(nDomainID);
				if(index==(_U32)-1)
				{
					if(!alloc) return NULL;

					domain = ZION_NEW CDomain(nDomainID);
					index = g_domain_manager.Insert(domain);
					if(index==(_U32)-1)
					{
						ZION_DELETE domain;
						return NULL;
					}
					if(g_domain_id_map.Insert(nDomainID, index)) break;
					g_domain_manager.Remove(index, domain);
					ZION_DELETE domain;
					continue;
				}
				domain = g_domain_manager.Lock(index);
				if(!domain) continue;
				if(domain->GetDomainID()==nDomainID) break;
				g_domain_manager.Unlock(index);
			}

			return domain;
		}
		
		void CDomain::Unlock(CDomain* domain)
		{
			_U32 index = domain->GetIndex();
			if(domain->m_Users.empty())
			{
				g_domain_id_map.Remove(domain->m_nDomainID, index);
				ZION_DELETE domain;
			}
			g_domain_manager.Unlock(index);
		}

		CDomain::CDomain(_U32 nDomainID)
		{
			m_nDomainID = nDomainID;
			m_nIndex = (_U32)-1;
		}

		CDomain::~CDomain()
		{
		}

		void CDomain::SetIndex(_U32 index)
		{
			m_nIndex = index;
		}

		_U32 CDomain::GetIndex()
		{
			return m_nIndex;
		}

		_U32 CDomain::GetDomainID()
		{
			return m_nDomainID;
		}

		bool CDomain::SendMsg(const String& msg)
		{
			if(m_Users.size()>0)
			{
				CMessage OutMsg(msg);
				Map<_U32, CUserSession*>::iterator i;
				for(i=m_Users.begin(); i!=m_Users.end(); i++)
				{
					i->second->SendMsg(OutMsg);
				}
			}
			return true;
		}

		static TObjectManager<CUserSession, 100*10000> g_session_manager;
		static TObjectMap<_U32> g_session_user_id_map;
		static TObjectMap<_U64> g_session_avatar_id_map;
		static TObjectMap<String> g_session_avatar_name_map;

		CUserSession* CUserSession::LockByUser(_U32 nUserID)
		{
			_U32 index = g_session_user_id_map.Get(nUserID);
			if(index==(_U32)-1) return NULL;
			CUserSession* session = g_session_manager.Lock(index);
			if(!session) return NULL;
			if(session->GetUserID()==nUserID) return session;
			if(session->GetUserID()==nUserID && session->GetUserSeq()) return session;
			g_session_manager.Unlock(index);
			return NULL;
		}

		CUserSession* CUserSession::LockByUser(_U32 nUserID, _U32 nUserSeq)
		{
			_U32 index = g_session_user_id_map.Get(nUserID);
			if(index==(_U32)-1) return NULL;
			CUserSession* session = g_session_manager.Lock(index);
			if(!session) return NULL;
			if(session->GetUserID()==nUserID && session->GetUserSeq()==nUserSeq) return session;
			g_session_manager.Unlock(index);
			return NULL;
		}

		CUserSession* CUserSession::LockByAvatar(_U32 server_id, _U32 avatar_id)
		{
			_U64 id = (((_U64)server_id)<<32) | ((_U64)avatar_id);
			_U32 index = g_session_avatar_id_map.Get(id);
			if(index==(_U32)-1) return NULL;
			CUserSession* session = g_session_manager.Lock(index);
			if(!session) return NULL;
			if(session->m_nServerID==server_id && session->m_nAvatarID==avatar_id) return session;
			g_session_manager.Unlock(index);
			return NULL;
		}
		CUserSession* CUserSession::LockByAvatar(_U32 server_id, const String& avatar_name)
		{
			String name = StringFormat("%u:%s", server_id, avatar_name.c_str());
			_U32 index = g_session_avatar_name_map.Get(name);
			if(index==(_U32)-1) return NULL;
			CUserSession* session = g_session_manager.Lock(index);
			if(!session) return NULL;
			if(session->m_nServerID==server_id && session->m_AvatarName==avatar_name) return session;
			g_session_manager.Unlock(index);
			return NULL;
		}

		void CUserSession::Unlock(CUserSession* session)
		{
			g_session_manager.Unlock(session->m_nIndex);
		}

		CUserSession* CUserSession::Login(_U32 nUserID)
		{
			CUserSession* session;
			for(;;)
			{
				_U32 index = g_session_user_id_map.Get(nUserID);
				if(index==(_U32)-1)
				{
					session = ZION_NEW CUserSession(nUserID);
					index = g_session_manager.Insert(session);
					if(index==(_U32)-1)
					{
						ZION_DELETE session;
						return NULL;
					}
					if(g_session_user_id_map.Insert(nUserID, index))
					{
						session->SetIndex(index);
						break;
					}
					g_session_manager.Remove(index, session);
					ZION_DELETE session;
					continue;
				}
				session = g_session_manager.Lock(index);
				if(!session) continue;
				if(session->GetUserID()==nUserID) break;
				g_session_manager.Unlock(index);
			}

			// if(inprocess) return NULL;
			session->Reset();
			return session;
		}

		bool CUserSession::Logout(_U32 nUserID, _U32 nUserSeq)
		{
			CUserSession* session = LockByUser(nUserID, nUserSeq);
			if(!session) return false;
			session->Reset();
			g_session_manager.Remove(nUserID, session);
			g_session_user_id_map.Remove(session->m_nUserID, session->m_nIndex);
			ZION_DELETE session;
			return true;
		}

		CUserSession::CUserSession(_U32 nUserID)
		{
			m_nUserID = nUserID;
			m_nUserSeq = (_U32)-1;
			m_nServerID = (_U32)-1;
			m_nAvatarID = (_U32)-1;
			//m_AvatarName;
			//m_Domains;

			m_nMsgSeq = (_U32)-1;
			m_LastMsg = "[]";
			//m_Msgs;
			A_MUTEX_INIT(&m_MsgLocker);
			SetInvalidResponseID(m_PendingID);

			m_nReqSeq = (_U32)-1;
			//m_LastResponse;
			m_bLocked = false;
			//m_SessionData;
		}

		CUserSession::~CUserSession()
		{
			A_MUTEX_DESTROY(&m_MsgLocker);
		}

		void CUserSession::SetIndex(_U32 index)
		{
			m_nIndex = index;
		}

		_U32 CUserSession::GetIndex()
		{
			return m_nIndex;
		}

		void CUserSession::Reset()
		{
			if(m_nAvatarID!=(_U32)-1)
			{
				UnbindAvatar();
			}
			while(!m_Domains.empty())
			{
				LeaveDomain(*m_Domains.begin());
			}
			m_Msgs.clear();
			m_nReqSeq = 0;
			m_nMsgSeq = 0;
			m_LastMsg = "[0,[]]";
		}

		_U32 CUserSession::GetUserID()
		{
			return m_nUserID;
		}

		_U32 CUserSession::GetUserSeq()
		{
			return m_nUserSeq;
		}

		_U32 CUserSession::GetReqSeq()
		{
			return m_nReqSeq;
		}

		_U32 CUserSession::GetServerID()
		{
			return m_nServerID;
		}

		_U32 CUserSession::GetAvatarID()
		{
			return m_nAvatarID;
		}

		const String& CUserSession::GetAvatarName()
		{
			return m_AvatarName;
		}

		bool CUserSession::IsLocked()
		{
			return m_bLocked;
		}

		bool CUserSession::Lock()
		{
			if(m_bLocked) return false;
			m_bLocked = true;
			return true;
		}
		
		bool CUserSession::Unlock()
		{
			if(!m_bLocked) return false;
			m_nReqSeq += 1;
			return true;
		}

		void CUserSession::SetServer(_U32 server_id)
		{
			if(m_nServerID!=server_id)
			{
				g_ServerManager.Change(server_id, m_nServerID);
				m_nServerID = server_id;
			}
		}

		bool CUserSession::BindAvatar(_U32 server_id, _U32 avatar_id, const String& avatar_name)
		{
			if(m_nAvatarID!=(_U32)-1) return false;

			_U64 id = (((_U64)server_id)<<32) | ((_U64)avatar_id);
			String name = StringFormat("%u:%s", server_id, avatar_name.c_str());

			if(g_session_avatar_name_map.Insert(name, m_nIndex))
			{
				if(g_session_avatar_id_map.Insert(id, m_nIndex))
				{
					SetServer(server_id);
					m_nAvatarID = avatar_id;
					m_nAvatarServerID = m_nServerID;
					m_AvatarName = avatar_name;
					return true;
				}
				g_session_avatar_name_map.Remove(name, m_nIndex);
			}

			return false;
		}

		bool CUserSession::UnbindAvatar()
		{
			if(m_nAvatarID==(_U32)-1) return false;

			if(m_nAvatarID!=(_U32)-1)
			{
				_U64 id = (((_U64)m_nServerID)<<32) | ((_U64)m_nAvatarID);
				g_session_avatar_id_map.Remove(id, m_nIndex);
				m_nAvatarID = (_U32)-1;
			}

			if(!m_AvatarName.empty())
			{
				String name = StringFormat("%u:%s", m_nAvatarServerID, m_AvatarName.c_str());
				g_session_avatar_name_map.Remove(name, m_nIndex);
				m_AvatarName.clear();
			}

			return true;
		}

		bool CUserSession::JoinDomain(_U32 domain_id)
		{
			CDomain* domain = CDomain::Lock(domain_id, true);
			if(!domain) return false;
			domain->m_Users[m_nUserID] = this;
			m_Domains.insert(domain_id);
			CDomain::Unlock(domain);
			return true;
		}

		bool CUserSession::LeaveDomain(_U32 domain_id)
		{
			CDomain* domain = CDomain::Lock(domain_id, false);
			if(!domain) return false;

			Map<_U32, CUserSession*>::iterator i;
			i = domain->m_Users.find(m_nUserID);
			if(i!=domain->m_Users.end())
			{
				domain->m_Users.erase(i);
			}

			CDomain::Unlock(domain);
			m_Domains.erase(domain_id);
			return true;
		}

		bool CUserSession::SendMsg(const String& msg)
		{
			CMessage OutMsg(msg);
			SendMsg(OutMsg);
			return true;
		}
		
		bool CUserSession::SendMsg(CMessage& OutMsg)
		{
			A_MUTEX_LOCK(&m_MsgLocker);
			m_Msgs.push_back(OutMsg);
			if(IsValidResponseID(m_PendingID))
			{
				CombineMsg();
				JsonRPC_Send(m_PendingID, m_LastMsg.c_str());
				SetInvalidResponseID(m_PendingID);
			}
			A_MUTEX_UNLOCK(&m_MsgLocker);
			return true;
		}

		_U32 CUserSession::GetMsgSeq()
		{
			return m_nMsgSeq;
		}

		bool CUserSession::GetMsg(_U32 nMsgSeq)
		{
			if(m_nMsgSeq==nMsgSeq)
			{
				return true;
			}
			if(m_nMsgSeq+1!=nMsgSeq)
			{
				return false;
			}

			A_MUTEX_LOCK(&m_MsgLocker);
			if(!m_Msgs.empty())
			{
				CombineMsg();
			}
			A_MUTEX_UNLOCK(&m_MsgLocker);

			return m_nMsgSeq==nMsgSeq;
		}

		void CUserSession::WaitMsg(const JSONRPC_RESPONSE_ID& res)
		{
			A_MUTEX_LOCK(&m_MsgLocker);
			if(IsValidResponseID(m_PendingID))
			{
				JsonRPC_Send(m_PendingID, "[-1]");
				SetInvalidResponseID(m_PendingID);
			}
			m_PendingID = res;
			A_MUTEX_UNLOCK(&m_MsgLocker);
		}

		const String& CUserSession::GetLastMsg()
		{
			return m_LastMsg;
		}

		void CUserSession::CombineMsg()
		{
			_U32 size;
			List<CMessage>::iterator i;
			size = 4;
			for(i=m_Msgs.begin(); i!=m_Msgs.end(); i++)
			{
				size += (*i).GetSize();
			}
			size += (_U32)m_Msgs.size() - 1 + 2;
			m_LastMsg.clear();
			m_LastMsg.resize(size+2);
			m_LastMsg.replace(0, 4, "[0,[");
			size = 4;
			for(i=m_Msgs.begin(); i!=m_Msgs.end(); i++)
			{
				if(i==m_Msgs.begin())
				{
					m_LastMsg.replace(size, 1, ",");
					size += 1;
				}
				m_LastMsg.replace(size, (*i).GetSize(), (*i).GetMsg());
				size += (*i).GetSize();
			}
			m_LastMsg.replace(size, 2, "]]");
			m_Msgs.clear();
			m_nMsgSeq += 1;
		}

		void RPCIMPL_GetServerInfo()
		// return errcode, count, server_list[]
		{
			g_ServerManager.Send();
		}


	}
}
