#ifndef __ZION_USER_SESSION__
#define __ZION_USER_SESSION__

namespace Zion
{
	namespace Session
	{

		struct MESSAGE_DATA
		{
			_U32 count;
			char msg[1];
		};

		class CMessage
		{
		public:
			CMessage(const CMessage& val);
			CMessage(const char* pMsg);
			~CMessage();

		private:
			MESSAGE_DATA* m_pData;
		};

		class CDomain;
		class CUserSession;
		class CManager;

		class CDomain
		{
			friend class CUserSession;
		public:
			CDomain(CManager* pManager, _U32 nDomainID);
			~CDomain();

			_U32 GetDomainID();

			bool SendMsg(const char* msg);

		private:
			CManager* m_pManager;
			_U32 m_nDomainID;
			Map<_U32, CUserSession*> m_Users;
		};

		class CUserSession
		{
		public:
			CUserSession(CManager* pManager, _U32 nUserID);
			~CUserSession();

			_U32 GetUserID();
			_U32 GetUserSeq();
			_U32 GetReqSeq();
			const String& GetLastResponse();
			const String& GetSessionData();
			_U32 GetServerID();
			_U32 GetAvatarID();
			const String& GetAvatarName();
			CDomain* GetDomain(_U32 nDomainID);

			bool Logout();
			bool IsLocked();
			bool Lock();
			bool Unlock(const char* last_response, const char* session_data);
			bool BindAvatar(_U32 server_id, _U32 avatar_id, const char* avatar_name);
			bool UnbindAvatar();
			bool JoinDomain(_U32 domain_id);
			bool LeaveDomain(_U32 domain_id);

			bool SendMsg(const char* msg);
			bool SendMsg(CMessage& OutMsg);
			_U32 GetMsgSeq();
			_U32 GetMsg(_U32 nMsgSeq, String& out);
			bool WaitMsg(const JSONRPC_RESPONSE& res);

		private:
			CManager* m_pManager;
			_U32 m_nUserID;
			_U32 m_nUserSeq;
			bool m_bLocked;
			_U32 m_nServerID;
			_U32 m_nAvatarID;
			String m_AvatarName;
			List<CMessage> m_Msgs;
			Map<_U32, CDomain*> m_Domains;
			_U32 m_nReqSeq;
			String m_LastResponse;
			String m_SessionData;
			_U32 m_nMsgSeq;
			String m_LastMsg;
		};

		class CManager
		{
			friend class CUserSession;
		public:
			CManager();
			~CManager();

			CDomain* GetDomain(_U32 nDomainID);
			CUserSession* GetUser(_U32 nUserID);
			CUserSession* GetUser(_U32 nUserID, _U32 nUserSeq);
			CUserSession* GetAvatar(_U32 server_id, _U32 avatar_id);
			CUserSession* GetAvatar(_U32 server_id, const char* avatar_name);
			CUserSession* Login(_U32 nUserID);
			bool Logout(_U32 nUserID, _U32 nUserSeq);

		protected:
			Map<_U32, CDomain*> m_Domains;
			Map<_U32, CUserSession*> m_Users;
			Map<_U64, CUserSession*> m_AvatarIDs;
			Map<String, CUserSession*> m_AvatarNames;
		};
	}
}

#endif
