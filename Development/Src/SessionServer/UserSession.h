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
			_U32 GetMemberCount();

			bool SendMsg(const String& msg);
			void SendMemberList();

		private:
			_U32 m_nIndex;
			_U32 m_nDomainID;
			Map<_U32, CUserSession*> m_Users;
		};

		class CUserSession
		{
		public:
			static CUserSession* LockByUser(_U32 nUserID);
			static CUserSession* LockByUser(const String& session_key);
			static CUserSession* LockByAvatar(_U32 scope_id, _U32 avatar_id);
			static CUserSession* LockByAvatar(_U32 scope_id, const String& avatar_name);
			static void Unlock(CUserSession* session);
			static CUserSession* Login(_U32 nUserID);
			static bool Logout(const String& session_key);

			CUserSession(_U32 nUserID);
			~CUserSession();

			void SetIndex(_U32 index);
			_U32 GetIndex();
			void Reset();

			_U32 GetUserID();
			const String& GetSessionKey();
			_U32 GetReqSeq();
			_U32 GetAvatarScope();
			_U32 GetAvatarID();
			const String& GetAvatarName();

			bool IsLocked();
			bool Lock();
			bool Unlock();
			bool BindAvatar(_U32 scope_id, _U32 avatar_id, const String& avatar_name);
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
			String m_SessionKey;
			_U32 m_nAvatarID;
			_U32 m_nAvatarScopeID;
			String m_AvatarName;
			Set<_U32> m_Domains;

			_U32 m_nMsgSeq;
			A_MUTEX m_MsgLocker;
			List<CMessage> m_Msgs;
			String m_LastMsg;
			JSONRPC_RESPONSE_ID m_PendingID;

			_U32 m_nReqSeq;
			bool m_bLocked;
			time_t m_LockedTime;
		};

	}
}

#endif
