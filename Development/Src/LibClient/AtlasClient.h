#ifndef __ATLAS_CLIENT__
#define __ATLAS_CLIENT__

namespace Atlas
{

	class CClientComponent;
	class CClientDataSyncComponent;
	class CClient;
	class CClientApp;
	class CStressClient;
	class CStressCase;

	class CClientLogOutput
	{
	public:
		enum TYPE
		{
			TYPE_INFO,
			TYPE_ERROR,
			TYPE_WANNING,
			TYPE_DEBUG,
		};

		void I(const char* fmt, ...);
		void E(const char* fmt, ...);
		void W(const char* fmt, ...);
		void D(const char* fmt, ...);

	protected:
		virtual void Write(int type, const char* pLine);
	};

	// 1. 表示一个客户端
	class CClient : public CNoCopy
	{
		friend class CClientApp;
		friend class CStressClient;
		friend class CStressCase;
	public:
		typedef enum {
			CLIENT_NA,
			CLIENT_LOGINING,
			CLIENT_LOGINED,
			CLIENT_AUTH_FAILED,
		} CLIENT_STATE;

		CClient(CClientApp* pClientApp, _U32 recvsize=6*1024);
		virtual ~CClient();

		CClientApp* GetClientApp() { return m_pClientApp; }
		CLIENT_STATE GetClientState() { return m_nClientState; }
		CClientLogOutput& GetLogOutput();

		bool Login(const SOCKADDR& sa, _U32 nUID, const char* pToken="");
		bool LoginForStress(_U32 id);
		void Logout();

		void AddComponent(CClientComponent* pComponent);
		virtual void InitializeComponents();
		CClientDataSyncComponent* GetDataSyncComponent();

		virtual void OnConnect();
		virtual void OnDisconnect();
		virtual void OnData(_U16 id, _U32 len, const _U8* data);
		virtual void OnConnectFailed();
		virtual void OnLoginDone();
		virtual void SendData(_U16 id, _U32 len, const _U8* data);

		bool Send(_U16 iid, _U16 fid, DDL::MemoryWriter& Buf)
		{
			ATLAS_ASSERT(iid<256 && fid<256);
			SendData(iid|(fid<<8), Buf.GetSize(), Buf.GetBuf());
			return true;
		}

		sigslot::signal0<>							_OnConnect;
		sigslot::signal0<>							_OnDisconnect;
		sigslot::signal3<_U16, _U32, const _U8*>	_OnData;
		sigslot::signal0<>							_OnConnectFailed;
		sigslot::signal0<>							_OnLoginDone;

		sigslot::signal1<int>						_OnSyncDomainCreate;
		sigslot::signal1<int>						_OnSyncDomainDestory;
		sigslot::signal2<int, int>					_OnSyncObjectCreate;
		sigslot::signal2<int, int>					_OnSyncObjectDestory;
		sigslot::signal2<int, int>					_OnSyncObjectUpdate;

		bool ExistSyncDomain(_U16 domain);
		bool ExistSyncObject(_U16 domain, _U32 id, _U16 index);
		void GetSyncDomainList(std::vector<_U16>& list);
		void GetSyncObjectList(_U16 domain, std::vector<std::pair<_U32, _U16>>& list);
		const DDLReflect::STRUCT_INFO* GetSyncObjectType(_U16 domain, _U32 id, _U16 index);
		void* GetSyncObjectType(_U16 domain, _U32 id, _U16 index, const DDLReflect::STRUCT_INFO* info);

	protected:
		A_MUTEX m_mtxClient;
		_U8 m_LoginData[1024];
		_U16 m_nLoginDataSize;

		void OnRawConnect(HCONNECT hConn);
		void OnRawData(_U32 len, const _U8* data);
		void OnRawDisconnect();
		void OnRawConnectFailed();

	private:
		CClientApp* m_pClientApp;
		HCONNECT m_hConnect;

		std::list<CClientComponent*> m_Components;
		CClientDataSyncComponent* m_pDataSyncComponent;

		CLIENT_STATE m_nClientState;
		_U8* m_pRecvBuff;
		_U32 m_nRecvBuffLen;
		_U32 m_nRecvBuffSize;
		CClientLogOutput* m_pLogOutput;
		std::string m_strSessionAddr;
		bool m_bNeedRedirect;
	};

	// 1. 扩展CClient
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

	class CClientDataSyncComponent : CClientComponent
	{
	public:
		CClientDataSyncComponent(Atlas::CClient* pClient);
		virtual ~CClientDataSyncComponent();

		bool AppendObject(const DDLReflect::STRUCT_INFO* info, const void* data);
		bool RemoveObject(const DDLReflect::STRUCT_INFO* info, const void* data);
		void RemoveAllObjects();

		void GetObjects(std::vector<std::pair<const void*, const DDLReflect::STRUCT_INFO*>>& objects);

	private:
		std::map<const void*, const DDLReflect::STRUCT_INFO*> m_Objects;
	};

}

#endif
