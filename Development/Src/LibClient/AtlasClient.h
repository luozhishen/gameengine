#ifndef __ATLAS_CLIENT__
#define __ATLAS_CLIENT__

namespace Atlas
{

	class CClientComponent;
	class CClientConnectionComponent;
	class CClient;
	class CClientApp;
	class CStressClient;
	class CStressCase;

	class CClient : public CNoCopy
	{
		friend class CClientApp;
		friend class CStressClient;
		friend class CStressCase;
	public:

		typedef enum
		{
			STATE_NA,
			STATE_LOGINING,
			STATE_LOGIN_FAILED,
			STATE_LOGINED,
			STATE_DISCONNECTED,
		} CLIENT_STATE;

		typedef enum
		{
			ERRCODE_UNKNOWN					= 1,
			ERRCODE_SERVICE_UNAVALIABLE		= 2,
			ERRCODE_AUTH_FAIL				= 3,
		} CLIENT_ERRCODE;

		CClient(CClientApp* pClientApp, _U32 recvsize=6*1024);
		virtual ~CClient();

		CClientApp* GetClientApp() { return m_pClientApp; }

		bool Login(const char* pURL, const char* pUsername, const char* pPassword);
		bool Login(const char* pURL, const char* pDevice);
		virtual bool LoginForStress(const char* pURL, _U32 id);
		bool Logout();
		void SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data);
		CLIENT_STATE GetClientState();
		_U32 GetLoginErrorCode();

		void AddComponent(CClientComponent* pComponent);
		virtual void InitializeComponents();
		void SetConnectionComponent(CClientConnectionComponent* pConnectionComponent);

		virtual void OnConnected();
		virtual void OnLoginDone();
		virtual void OnDisconnect();
		virtual void OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data);
		virtual void OnConnectFailed();

		bool Send(_U16 iid, _U16 fid, DDL::MemoryWriter& Buf)
		{
			ATLAS_ASSERT(iid<256 && fid<256);
			SendData(iid, fid, Buf.GetSize(), Buf.GetBuf());
			return true;
		}

		sigslot::signal0<>								_OnConnected;
		sigslot::signal0<>								_OnDisconnect;
		sigslot::signal4<_U16, _U16, _U32, const _U8*>	_OnData;
		sigslot::signal0<>								_OnConnectFailed;
		sigslot::signal0<>								_OnLoginDone;

	protected:
		A_MUTEX m_mtxClient;

	private:
		CClientApp* m_pClientApp;
		std::list<CClientComponent*> m_Components;
		CClientConnectionComponent* m_pConnectionComponent;
	};

	class CClientComponent : public CNoCopy
	{
	public:
		CClientComponent(CClient* pClient);
		virtual ~CClientComponent();

		CClient* GetClient() { return m_pClient; }

		virtual void Tick() { }
		virtual void Reset() { }

	private:
		CClient* m_pClient;
	};

	class CClientConnectionComponent
	{
	public:
		CClientConnectionComponent(CClient* pClient);
		virtual ~CClientConnectionComponent();

		virtual bool Login(const char* pURL, const char* pUsername, const char* pPassword) = 0;
		virtual bool Login(const char* pURL, const char* pDevice) = 0;
		virtual bool Logout() = 0;
		virtual void SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data) = 0;
		virtual CClient::CLIENT_STATE GetState() = 0;
		virtual _U32 GetErrorCode() = 0;
	};

}

#endif
