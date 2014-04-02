#include <ZionBase.h>
#include <JsonRPC.h>
#include <uv.h>
#include <ObjectManager.h>
#include "UserSession.h"

namespace Zion
{
	namespace Session
	{

		CMessage::CMessage(const CMessage& val)
		{
			m_pData = val.m_pData;
			m_pData->count += 1;
		}

		CMessage::CMessage(const String& msg)
		{
			m_pData = (MESSAGE_DATA*)ZION_ALLOC(sizeof(MESSAGE_DATA)+(_U32)msg.size());
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
			// if(empty()) delete it;
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
			SetInvalidResponseID(m_PendingID);

			m_nReqSeq = (_U32)-1;
			//m_LastResponse;
			m_bLocked = false;
			//m_SessionData;
		}

		CUserSession::~CUserSession()
		{
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
			m_LastResponse = "[]";
			m_nMsgSeq = 0;
			m_LastMsg = "[]";
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

		const String& CUserSession::GetLastResponse()
		{
			return m_LastResponse;
		}
		
		const String& CUserSession::GetSessionData()
		{
			return m_SessionData;
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
		
		bool CUserSession::Unlock(const char* last_response, const String& session_data)
		{
			if(!m_bLocked) return false;
			m_nReqSeq += 1;
			m_LastResponse = last_response;
			m_SessionData = session_data;
			return true;
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
					m_nServerID = server_id;
					m_nAvatarID = avatar_id;
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
				String name = StringFormat("%u:%s", m_nServerID, m_AvatarName.c_str());
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
			m_Msgs.push_back(OutMsg);
			return true;
		}

		_U32 CUserSession::GetMsgSeq()
		{
			return m_nMsgSeq;
		}

		_U32 CUserSession::GetMsg(_U32 nMsgSeq, String& out)
		{
			if(m_nMsgSeq==nMsgSeq)
			{
				out = m_LastMsg;
				return m_nMsgSeq + 1;
			}
			if(m_nMsgSeq+1==nMsgSeq)
			{
				if(m_Msgs.empty())
				{
					out = "[]";
					return nMsgSeq;
				}
				m_nMsgSeq = nMsgSeq;
				return nMsgSeq + 1;
			}
			return 0;
		}

		bool CUserSession::WaitMsg(const JSONRPC_RESPONSE_ID& res)
		{
			return true;
		}

	}
}
