#include <ZionBase.h>
#include <JsonRPC.h>
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

		CMessage::CMessage(const char* pMsg)
		{
			_U32 len = (_U32)strlen(pMsg);
			m_pData = (MESSAGE_DATA*)ZION_ALLOC(sizeof(MESSAGE_DATA)+len);
			m_pData->count = 1;
			memcpy(m_pData->msg, pMsg, (size_t)len+1);
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

		CDomain::CDomain(CManager* pManager, _U32 nDomainID)
		{
			m_nDomainID = nDomainID;
			m_pManager = pManager;
		}

		CDomain::~CDomain()
		{
		}

		_U32 CDomain::GetDomainID()
		{
			return m_nDomainID;
		}

		bool CDomain::SendMsg(const char* msg)
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

		CUserSession::CUserSession(CManager* pManager, _U32 nUserID)
		{
			m_nUserID = nUserID;
			m_pManager = pManager;
			m_nUserSeq = (_U32)-1;
			m_bLocked = false;
			m_nServerID = (_U32)-1;
			m_nAvatarID = (_U32)-1;
			m_nReqSeq = 0;
			m_LastResponse = "[]";
			m_nMsgSeq = 0;
			m_LastMsg = "[]";
		}

		CUserSession::~CUserSession()
		{
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

		CDomain* CUserSession::GetDomain(_U32 nDomainID)
		{
			Map<_U32, CDomain*>::iterator i;
			i = m_Domains.find(nDomainID);
			if(i==m_Domains.end()) return NULL;
			return i->second;
		}

		bool CUserSession::Logout()
		{
			if(m_nAvatarID!=(_U32)-1)
			{
				UnbindAvatar();
			}
			while(!m_Domains.empty())
			{
				LeaveDomain(m_Domains.begin()->first);
			}
			m_Msgs.clear();
			m_nReqSeq = 0;
			m_LastResponse = "[]";
			m_nMsgSeq = 0;
			m_LastMsg = "[]";
			return true;
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
		
		bool CUserSession::Unlock(const char* last_response, const char* session_data)
		{
			if(!m_bLocked) return false;
			m_nReqSeq += 1;
			m_LastResponse = last_response;
			m_SessionData = session_data;
			return true;
		}

		bool CUserSession::BindAvatar(_U32 server_id, _U32 avatar_id, const char* avatar_name)
		{
			if(m_nAvatarID!=(_U32)-1) return false;

			_U64 id = (((_U64)server_id)<<32) | ((_U64)avatar_id);
			String name = StringFormat("%u:%s", server_id, avatar_name);

			Map<String, CUserSession*>::iterator n;
			n = m_pManager->m_AvatarNames.find(name);
			if(n!=m_pManager->m_AvatarNames.end()) return false;

			Map<_U64, CUserSession*>::iterator i;
			i = m_pManager->m_AvatarIDs.find(id);
			if(i!=m_pManager->m_AvatarIDs.end()) return false;

			m_pManager->m_AvatarIDs[id] = this;
			m_pManager->m_AvatarNames[name] = this;
			m_nServerID = server_id;
			m_nAvatarID = avatar_id;
			m_AvatarName = avatar_name;
			return true;
		}

		bool CUserSession::UnbindAvatar()
		{
			if(m_nAvatarID==(_U32)-1) return false;

			_U64 id = (((_U64)m_nServerID)<<32) | ((_U64)m_nAvatarID);
			String name = StringFormat("%u:%s", m_nServerID, m_AvatarName.c_str());

			Map<String, CUserSession*>::iterator n;
			n = m_pManager->m_AvatarNames.find(name);
			if(n==m_pManager->m_AvatarNames.end()) return false;

			Map<_U64, CUserSession*>::iterator i;
			i = m_pManager->m_AvatarIDs.find(id);
			if(i==m_pManager->m_AvatarIDs.end()) return false;

			m_pManager->m_AvatarIDs.erase(i);
			m_pManager->m_AvatarNames.erase(n);
			m_nServerID = (_U32)-1;
			m_nAvatarID = (_U32)-1;
			m_AvatarName = "";
			return true;
		}

		bool CUserSession::JoinDomain(_U32 domain_id)
		{
			Map<_U32, CDomain*>::iterator i;
			i = m_Domains.find(domain_id);
			if(i!=m_Domains.end()) return false;
			CDomain* domain;
			i = m_pManager->m_Domains.find(domain_id);
			if(i!=m_Domains.end())
			{
				domain = i->second;
			}
			else
			{
				domain = ZION_NEW CDomain(m_pManager, domain_id);
				m_pManager->m_Domains[domain_id] = domain;
			}
			domain->m_Users[m_nUserID] = this;
			m_Domains[domain_id] = domain;
			return true;
		}

		bool CUserSession::LeaveDomain(_U32 domain_id)
		{
			Map<_U32, CDomain*>::iterator i;
			i = m_Domains.find(domain_id);
			if(i==m_Domains.end()) return false;

			CDomain* domain = i->second;
			m_Domains.erase(i);

			domain->m_Users.erase(m_nUserID);
			if(domain->m_Users.empty())
			{
				m_pManager->m_Domains.erase(domain_id);
				ZION_DELETE domain;
			}
			return true;
		}

		bool CUserSession::SendMsg(const char* msg)
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

		/*
		bool CUserSession::WaitMsg(const JSONRPC_RESPONSE* res)
		{
			return true;
		}
		*/

		CManager::CManager()
		{
		}

		CManager::~CManager()
		{
		}

		CDomain* CManager::GetDomain(_U32 nDomainID)
		{
			Map<_U32, CDomain*>::iterator i;
			i = m_Domains.find(nDomainID);
			if(i==m_Domains.end()) return NULL;
			return i->second;
		}

		CUserSession* CManager::GetUser(_U32 nUserID)
		{
			Map<_U32, CUserSession*>::iterator i;
			i = m_Users.find(nUserID);
			if(i==m_Users.end()) return NULL;
			return i->second;
		}

		CUserSession* CManager::GetUser(_U32 nUserID, _U32 nUserSeq)
		{
			Map<_U32, CUserSession*>::iterator i;
			i = m_Users.find(nUserID);
			if(i==m_Users.end()) return NULL;
			if(i->second->GetUserSeq()!=nUserSeq) return NULL;
			return i->second;
		}

		CUserSession* CManager::GetAvatar(_U32 server_id, _U32 avatar_id)
		{
			_U64 id = (((_U64)server_id)<<32) | ((_U64)avatar_id);
			Map<_U64, CUserSession*>::iterator i;
			i = m_AvatarIDs.find(id);
			if(i==m_AvatarIDs.end()) return NULL;
			return i->second;

		}

		CUserSession* CManager::GetAvatar(_U32 server_id, const char* avatar_name)
		{
			String name = StringFormat("%u:%s", server_id, avatar_name);
			Map<String, CUserSession*>::iterator i;
			i = m_AvatarNames.find(name);
			if(i==m_AvatarNames.end()) return NULL;
			return i->second;
		}

		CUserSession* CManager::Login(_U32 nUserID)
		{
			Map<_U32, CUserSession*>::iterator i;
			i = m_Users.find(nUserID);
			CUserSession* session;
			if(i==m_Users.end())
			{
				session = ZION_NEW CUserSession(this, nUserID);
				m_Users[nUserID] = session;
			}
			else
			{
				session = i->second;
				if(session->IsLocked() || !session->Logout())
				{
					return NULL;
				}
			}
			return session;
		}

		bool CManager::Logout(_U32 nUserID, _U32 nUserSeq)
		{
			Map<_U32, CUserSession*>::iterator i;
			i = m_Users.find(nUserID);
			if(i==m_Users.end()) return false;
			CUserSession* session = i->second;
			if(session->GetUserSeq()!=nUserSeq) return false;
			if(!session->Logout()) return false;
			m_Users.erase(i);
			ZION_DELETE session;
			return true;

		}

	}
}
