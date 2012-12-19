#ifndef __ATLAS_CLIENT__
#define __ATLAS_CLIENT__

namespace Atlas
{

	class CClientComponent;
	class CClientConnectionBase;
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
			STATE_PENDING,
			STATE_LOGINED,
			STATE_FAILED,
		} CLIENT_STATE;

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

		const std::string& GetClientConnectionType();
		CClientConnectionBase* GetClientConnection();

		bool Login(const char* pUrl, _U32 nUID, const char* pToken);
		bool LoginForStress(_U32 id);
		void Logout();

		void AddComponent(CClientComponent* pComponent);
		virtual void InitializeComponents();
		virtual void Tick();

		virtual void OnLoginDone();
		virtual void OnLoginFailed();
		virtual void OnDisconnected();
		virtual void OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data);
		void SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data);
		bool Send(_U16 iid, _U16 fid, DDL::MemoryWriter& Buf)
		{
			ATLAS_ASSERT(iid<256 && fid<256);
			SendData(iid, fid, Buf.GetSize(), Buf.GetBuf());
			return true;
		}

		sigslot::signal0<>								_OnLoginDone;
		sigslot::signal0<>								_OnLoginFailed;
		sigslot::signal0<>								_OnDisconnected;
		sigslot::signal4<_U16, _U16, _U32, const _U8*>	_OnData;

	protected:
		A_MUTEX m_mtxClient;

	private:
		CClientApp* m_pClientApp;
		std::string m_ClientConnectionType;
		CClientConnectionBase* m_pClientConnection;
		std::list<CClientComponent*> m_Components;
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

		virtual bool Login(const char* pUrl, _U32 nUID, const char* pToken) = 0;
		virtual void Logout() = 0;
		virtual void SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data) = 0;

	protected:
		void SetErrorCode(_U32 errcode);

		CClient::CLIENT_STATE m_nState;
		_U32 m_nErrCode;
	};

}

#endif
