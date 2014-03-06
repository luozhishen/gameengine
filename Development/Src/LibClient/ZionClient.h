#ifndef __ZION_CLIENT__
#define __ZION_CLIENT__

#define CLIENT_LOG(client, fmt, ...)	\
{	\
	if(client->GetLogCallback())	\
	{	\
		Zion::String str = Zion::StringFormat(fmt, ##__VA_ARGS__);	\
		(client->GetLogCallback())(str.c_str());	\
	}	\
}

namespace Zion
{

	class CClientComponent;
	class CClientConnectionBase;
	class CClient;
	class CClientApp;
	class CStressClient;
	class CStressCase;
	class CClientLoginMethod;
	class CDataSyncClient;

	class CClient : public CNoCopy, private CStubDispatcher, public DDLProxy::IClient
	{
		friend class CClientApp;
		friend class CStressClient;
		friend class CStressCase;
	public:
		typedef enum {
			STATE_NA,
			STATE_LOGINING,
			STATE_PENDING,
			STATE_LOGINED,
			STATE_FAILED,
		} CLIENT_STATE;

#ifdef ZION_DISABLE_STL_FUNCTION
		typedef void (*LOG_CALLBACK)(const char*)>;
#else
		typedef std::tr1::function<void (const char*)> LOG_CALLBACK;
#endif

		enum {
			ERRCODE_SUCCESSED = 0,
			ERRCODE_UNKOWN,
			ERRCODE_AUTH_FAILED,
			ERRCODE_NETWORK,
		};

		CClient(CClientApp* pClientApp, _U32 recvsize=6*1024);
		virtual ~CClient();

		CClientApp* GetClientApp() { return m_pClientApp; }
		CLIENT_STATE GetState();
		_U32 GetErrorCode();

		const Zion::String& GetClientConnectionType();
		CClientConnectionBase* GetClientConnection();

		CDataSyncClient* GetDataSync();

		void SetLogCallback(LOG_CALLBACK logproc);
		LOG_CALLBACK GetLogCallback();

		bool Login(const char* pUrl, const CClientLoginMethod* pMethod);
		bool LoginByDevice(const char* pUrl);
		bool LoginForStress(_U32 id);
		void Logout();

		void AddComponent(CClientComponent* pComponent);
		virtual void InitializeComponents();
		virtual void Tick();

		void RegisterStub(DDLStub::IStub* pStub);

		/*virtual*/ void OnLoginDone();
		/*virtual*/ void OnLoginFailed();
		/*virtual*/ void OnDisconnected();
		/*virtual*/ void OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data);
		virtual bool SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data);

		sigslot::signal0<>								_OnLoginDone;
		sigslot::signal0<>								_OnLoginFailed;
		sigslot::signal0<>								_OnDisconnected;
		sigslot::signal4<_U16, _U16, _U32, const _U8*>	_OnData;

	protected:
		A_MUTEX m_mtxClient;

	private:
		CClientApp* m_pClientApp;
		Zion::String m_ClientConnectionType;
		CClientConnectionBase* m_pClientConnection;
		CDataSyncClient* m_pDataSync;
		Zion::List<CClientComponent*> m_Components;
		Zion::Map<_U16, DDLStub::IStub*> m_DDLStubs;
		LOG_CALLBACK m_LogCallback;
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

	class CClientConnectionBase : public CClientComponent
	{
	public:
		CClientConnectionBase(CClient* pClient);
		virtual ~CClientConnectionBase();

		CClient::CLIENT_STATE GetState();
		_U32 GetErrorCode();

		virtual bool Login(const char* pUrl, const CClientLoginMethod* pMethod) = 0;
		virtual void Logout() = 0;
		virtual void SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data) = 0;

	protected:
		void SetErrorCode(_U32 errcode);

		CClient::CLIENT_STATE m_nState;
		_U32 m_nErrCode;
	};

}

#endif
