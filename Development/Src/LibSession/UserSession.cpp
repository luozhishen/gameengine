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
			m_nAvatarID = (_U32)-1;
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
			return true;
		}

		bool CUserSession::Lock()
		{
			return true;
		}
		
		bool CUserSession::Unlock(const char* last_response, const char* session_data)
		{
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

		bool CUserSession::WaitMsg(const JSONRPC_RESPONSE& res)
		{
			return true;
		}

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

		CUserSession* CManager::Login(_U32 nUserID)
		{
			return NULL;
		}

	}
}
