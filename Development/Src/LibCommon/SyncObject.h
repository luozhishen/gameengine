#ifndef __ATLAS_SYNC_OBJECT__
#define __ATLAS_SYNC_OBJECT__

#include <list>

namespace Atlas
{
	namespace Sync
	{

		class CSyncProxy;
		class CSyncMonitor;

		class CSyncObject
		{
			friend class CSyncProxy;
			friend class CSyncMonitor;
		public:
			CSyncObject(const DDLReflect::STRUCT_INFO* info, const void* data);
			~CSyncObject();
			
			_U16 GetTypeId() { return m_nTypeId; }
			const DDLReflect::STRUCT_INFO* GetInfo() { return m_pInfo; }
			const void* GetData() { return m_pData; }

			bool IsDirty();
			void SetDirty(_U32 start, _U32 len);
			void Clear();

		protected:
			void SetMonitor(CSyncMonitor* pMonitor);
			void AddProxy(CSyncProxy* pProxy);
			void DelProxy(CSyncProxy* pProxy);

		private:
			_U16 m_nTypeId;
			const DDLReflect::STRUCT_INFO* m_pInfo;
			const _U8* m_pData;
			CSyncMonitor* m_pMonitor;
			std::map<CSyncProxy*, bool> m_mapProxys;

			std::list<std::pair<_U32, _U32>> m_DirtyArea;
			bool m_bFullUpdate;
		};

/*

			bool IsDirty();
			void SetDirty(_U32 start, _U32 len);
			void FullUpdate();
			void DirtyReset();

			bool WriteDirty(DDL::BufferWriter& w);
			bool WriteFULL(DDL::BufferWriter& w);
			bool Read(DDL::BufferReader& r);
*/

	}
}

#endif
