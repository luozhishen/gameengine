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
		typedef enum {
			STATE_NA,
			STATE_LOGINING,
			STATE_LOGINED,
			STATE_FAILED,
		} CLIENT_STATE;

		enum CLIENT_ERRCODE
		{
			ERRCODE_SUCCESSED = 0,
			ERRCODE_SERVICE_UNAVALIABLE = 1,
			ERRCODE_AUTH_FAILED = 2,
		};

		CClient(CClientApp* pClientApp, _U32 recvsize=6*1024);
		virtual ~CClient();

		CClientApp* GetClientApp() { return m_pClientApp; }

		bool Login(const char* pUsername, const char* pPassword, const char* pUrl=NULL);
		bool Login(const char* pDevice, const char* pUrl=NULL);
		bool LoginForStress(_U32 id, const char* pUrl=NULL);
		bool Logout();
		CLIENT_STATE GetState();
		_U32 GetErrCode();
		void SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data);

		void AddComponent(CClientComponent* pComponent);
		virtual void InitializeComponents();

		virtual void OnConnected();
		virtual void OnDisconnected();
		virtual void OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data);
		virtual void OnConnectFailed();
		virtual void OnLoginDone();

		bool Send(_U16 iid, _U16 fid, DDL::MemoryWriter& Buf)
		{
			SendData(iid, fid, Buf.GetSize(), Buf.GetBuf());
			return true;
		}

		sigslot::signal0<>								_OnConnect;
		sigslot::signal0<>								_OnDisconnected;
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

	class CClientConnectionComponent : public CClientComponent
	{
	public:
		CClientConnectionComponent(CClient* pClient);
		virtual ~CClientConnectionComponent();

		virtual bool Login(const char* pUsername, const char* pPassword, const char* pUrl) = 0;
		virtual bool Login(const char* pDevice, const char* pUrl) = 0;
		virtual bool LoginForStress(_U32 id, const char* pUrl) = 0;
		virtual bool Logout() = 0;
		virtual void SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data) = 0;

		CClient::CLIENT_STATE GetState();
		_U32 GetErrCode();

	protected:
		CClient::CLIENT_STATE m_nState;
		_U32 m_nErrCode;
	};

}

#endif
