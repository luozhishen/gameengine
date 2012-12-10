#ifndef __ATLAS_CLIENT__
#define __ATLAS_CLIENT__

namespace Atlas
{

	class CClientComponent;
	class CClientConnection;
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

		typedef enum {
			ERRCODE_SUCCESSED = 0,
			ERRCODE_UNKOWN,
			ERRCODE_AUTH_FAILED,
			ERRCODE_NETWORK,
		};

		CClient(CClientApp* pClientApp, _U32 recvsize=6*1024);
		virtual ~CClient();

		CClientApp* GetClientApp() { return m_pClientApp; }
		CLIENT_STATE GetState() { return m_nState; }
		void SetErrorCode(_U32 errcode);

		bool Login(const SOCKADDR& sa, _U32 nUID, const char* pToken="");
		bool LoginForStress(_U32 id);
		void Logout();

		void AddComponent(CClientComponent* pComponent);
		virtual void InitializeComponents();

		virtual void OnLoginDone();
		virtual void OnLoginFailed();
		virtual void OnDisconnected();
		virtual void OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data);
		virtual void SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data);
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

		void OnRawConnected();
		void OnRawData(_U32 len, const _U8* data);
		void OnRawDisconnected();
		void OnRawConnectFailed();

		void ProcessPacket(_U32 len, const _U8* data);

	protected:
		A_MUTEX m_mtxClient;

	private:
		CClientApp* m_pClientApp;
		CClientConnection* m_pClientConnection;

		std::list<CClientComponent*> m_Components;

		CLIENT_STATE m_nState;
		_U32 m_nErrCode;

		_U8* m_pRecvBuff;
		_U32 m_nRecvBuffLen;
		_U32 m_nRecvBuffSize;
		bool m_bNeedRedirect;
		SOCKADDR m_saRedirectAddr;
		_U8 m_LoginData[1024];
		_U16 m_nLoginDataSize;
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

	class CClientConnection : public CClientComponent
	{
	public:
		CClientConnection(CClient* pClient);
		virtual ~CClientConnection();

		virtual bool Connect(const SOCKADDR& sa) = 0;
		virtual void Disconnect() = 0;
		virtual void SendData(_U32 len, const _U8* data, bool bPending=false) = 0;
	};

}

#endif
