#ifndef __ZION_LIVE_OBJECT__
#define __ZION_LIVE_OBJECT__

namespace Zion
{
	namespace LiveObject
	{

		class CLiveObject;
		class CLiveManager;

		class CLiveMonitor : public DDLDataObject::IMonitor
		{
		public:
			CLiveMonitor(const DDLReflect::STRUCT_INFO* info);

			bool IsDirty();
			void SetDirty();
			virtual void SetDirty(const char* name, _U32 offset, _U32 size);
			void Clean();

		private:
			const DDLReflect::STRUCT_INFO* m_pInfo;
			bool m_bDirty;
		};

		class CLiveObject : public DDLDataObject::CObject
		{
		public:
			CLiveObject(CLiveManager* pManager, const DDLReflect::STRUCT_INFO* pInfo, A_LIVE_OBJECT* pData);

			bool IsDirty();
			void Clean();

		private:
			CLiveManager* m_pManager;
			CLiveMonitor m_Monitor;
		};

		class CLiveManager
		{
		public:
			CLiveManager();

			CLiveObject* Get(const A_UUID& _uuid);

		protected:
			Map<A_UUID, CLiveObject*> m_ObjMap;
			List<A_UUID> m_DelList;
		};

		bool Register(const DDLReflect::STRUCT_INFO* info);
		void GetTypeList(Zion::Array<const DDLReflect::STRUCT_INFO*>& list);
		_U16 GetTypeId(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(_U16 id);

	}
}

#endif
