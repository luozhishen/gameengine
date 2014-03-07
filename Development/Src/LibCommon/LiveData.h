#ifndef __ZION_LIVE_DATA__
#define __ZION_LIVE_DATA__

namespace Zion
{
	namespace LiveData
	{

		class CMonitor;
		class CObject;
		class CManagerBase;
		class CManager;

		class CMonitor : public DataObject::IMonitor
		{
		public:
			CMonitor(const DDLReflect::STRUCT_INFO* info);

			bool IsDirty();
			void SetDirty();
			virtual void SetDirty(const char* name, _U32 offset, _U32 size);
			void Clean();

		private:
			const DDLReflect::STRUCT_INFO* m_pInfo;
			bool m_bDirty;
		};

		class CObject : public DataObject::CObject
		{
		public:
			CObject(CManagerBase* pManager, const DDLReflect::STRUCT_INFO* pInfo, A_LIVE_OBJECT* pData);

			bool IsDirty();
			void Clean();

		private:
			CManagerBase* m_pManager;
			CMonitor m_Monitor;
		};

		class CManagerBase
		{
		public:
			CManagerBase();

			void EnableMonitor(bool bEnable);
			bool IsEnableMointor();

			CObject* Append(const DDLReflect::STRUCT_INFO* pInfo);
			CObject* Append(const DDLReflect::STRUCT_INFO* pInfo, A_LIVE_OBJECT* data);
			CObject* Append(const DDLReflect::STRUCT_INFO* pInfo, const _U8* data, _U32 len);
			CObject* Append(const DDLReflect::STRUCT_INFO* pInfo, const char* data);
			bool Remove(const A_UUID& _uuid);
			void Clear();

			CObject* Get(const A_UUID& _uuid);
			CObject* FindFirst();
			CObject* FindNext(CObject* obj);

		protected:
			bool m_bEnableMonitor;
			Map<A_UUID, CObject*> m_ObjMap;
		};

		class CRequestClient : public DDLProxy::IClient, public DDLStub::CDispatcher
		{
		public:
			CRequestClient(CManager* pManager);
			virtual ~CRequestClient();

			virtual bool SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data);

		private:
			CManager* m_pManager;
		};

		class CResponseClient : public DDLProxy::IClient
		{
		public:
			CResponseClient(CManager* pManager);
			virtual ~CResponseClient();

			virtual bool SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data);

		private:
			CManager* m_pManager;
		};

		class CManager : public CManagerBase
		{
		public:
			CManager();

			bool RegisterRequestStub(_U16 iid, DDLStub::IStub* pStub);
			bool SendRequestData(_U16 iid, _U16 fid, _U32 len, const _U8* data);
			bool SendResponseData(_U16 iid, _U16 fid, _U32 len, const _U8* data);

			bool HasRequestClient();
			bool HasResponseClient();
			bool HasResponseStub();
			void SetRequestClient(DDLProxy::IClient* pRquestClientImpl);
			void SetResponseClient(DDLProxy::IClient* pResponseClientImpl);
			void SetResponseStub(DDLStub::CDispatcher* m_pResponseStubImpl);
			DDLProxy::IClient* GetRequestClient();
			DDLProxy::IClient* GetResponseClient();
			DDLStub::CDispatcher* GetResponseStub();

		private:
			CRequestClient m_RequestClient;
			CResponseClient m_ResponseClient;
			DDLProxy::IClient* m_pRequestClientImpl;
			DDLProxy::IClient* m_pResponseClientImpl;
			DDLStub::CDispatcher* m_pResponseStubImpl;
		};

	}
}

#endif
