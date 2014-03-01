#ifndef __ZION_LIVE_OBJECT__
#define __ZION_LIVE_OBJECT__

namespace Zion
{
	namespace LiveObject
	{

		class CObject;
		class CManager;

		class CMonitor : public DDLDataObject::IMonitor
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

		class CObject : public DDLDataObject::CObject
		{
		public:
			CObject(CManager* pManager, const DDLReflect::STRUCT_INFO* pInfo, A_LIVE_OBJECT* pData);

			bool IsDirty();
			void Clean();

		private:
			CManager* m_pManager;
			CMonitor m_Monitor;
		};

		class CManager
		{
		public:
			CManager();

			CObject* Append(const DDLReflect::STRUCT_INFO* pInfo);
			CObject* Append(const DDLReflect::STRUCT_INFO* pInfo, const _U8* data, _U32 len);
			CObject* Append(const DDLReflect::STRUCT_INFO* pInfo, const char* data);
			void Remove(const A_UUID& _uuid);

			CObject* Get(const A_UUID& _uuid);
			CObject* FindFirst();
			CObject* FindNext();

		protected:
			Map<A_UUID, CObject*> m_ObjMap;
			Set<A_UUID> m_DelList;
			Set<A_UUID> m_NewList;
		};

		bool Register(const DDLReflect::STRUCT_INFO* info);
		void GetTypeList(Zion::Array<const DDLReflect::STRUCT_INFO*>& list);
		_U16 GetTypeId(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(_U16 id);

	}
}

#endif
