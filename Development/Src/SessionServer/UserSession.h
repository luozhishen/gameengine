#ifndef __ZION_USER_SESSION__
#define __ZION_USER_SESSION__

namespace Zion
{
	namespace Session
	{

		struct MESSAGE_DATA
		{
			_U32 count;
			_U32 size;
			char msg[1];
		};

		class CMessage
		{
		public:
			CMessage(const CMessage& val);
			CMessage(const String& msg);
			~CMessage();

			_U32 GetSize();
			const char* GetMsg();

		private:
			MESSAGE_DATA* m_pData;
		};

		class CDomain;
		class CUserSession;

		class CDomain
		{
			friend class CUserSession;
		public:
			static CDomain* Lock(_U32 domain_id, bool alloc);
			static void Unlock(CDomain* domain);

			CDomain(_U32 nDomainID);
			~CDomain();

			void SetIndex(_U32 index);
			_U32 GetIndex();

			_U32 GetDomainID();
			bool SendMsg(const String& msg);

		private:
			_U32 m_nIndex;
			_U32 m_nDomainID;
			Map<_U32, CUserSession*> m_Users;
		};

		class CUserSession
		{
		public:
			static CUserSession* LockByUser(_U32 nUserID);
			static CUserSession* LockByUser(_U32 nUserID, _U32 nUserSeq);
			static CUserSession* LockByAvatar(_U32 server_id, _U32 avatar_id);
			static CUserSession* LockByAvatar(_U32 server_id, const String& avatar_name);
			static void Unlock(CUserSession* session);
			static CUserSession* Login(_U32 nUserID);
			static bool Logout(_U32 nUserID, _U32 nUserSeq);

			CUserSession(_U32 nUserID);
			~CUserSession();

			void SetIndex(_U32 index);
			_U32 GetIndex();
			void Reset();

			_U32 GetUserID();
			_U32 GetUserSeq();
			_U32 GetReqSeq();
			const String& GetLastResponse();
			const String& GetSessionData();
			_U32 GetServerID();
			_U32 GetAvatarID();
			const String& GetAvatarName();

			bool IsLocked();
			bool Lock();
			bool Unlock(const char* last_response, const String& session_data);
			void SetServer(_U32 server_id);
			bool BindAvatar(_U32 server_id, _U32 avatar_id, const String& avatar_name);
			bool UnbindAvatar();
			bool JoinDomain(_U32 domain_id);
			bool LeaveDomain(_U32 domain_id);

			bool SendMsg(const String& msg);
			bool SendMsg(CMessage& OutMsg);
			_U32 GetMsgSeq();
			bool GetMsg(_U32 nMsgSeq);
			void WaitMsg(const JSONRPC_RESPONSE_ID& res);
			const String& GetLastMsg();
			void CombineMsg();

		private:
			_U32 m_nIndex;
			_U32 m_nUserID;
			_U32 m_nUserSeq;
			_U32 m_nServerID;
			_U32 m_nAvatarID;
			_U32 m_nAvatarServerID;
			String m_AvatarName;
			Set<_U32> m_Domains;

			_U32 m_nMsgSeq;
			A_MUTEX m_MsgLocker;
			List<CMessage> m_Msgs;
			String m_LastMsg;
			JSONRPC_RESPONSE_ID m_PendingID;

			_U32 m_nReqSeq;
			String m_LastResponse;
			bool m_bLocked;
			String m_SessionData;
		};

	}
}

#endif
