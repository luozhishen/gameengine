#ifndef __ATLAS_SYNC_DOMAIN__
#define __ATLAS_SYNC_DOMAIN__

namespace Atlas
{
	namespace Sync
	{

		class CSyncDomainManager;

		class CSyncDomain
		{
			friend class CSyncDomainManager;
		protected:
			CSyncDomain(_U16 domain);
			~CSyncDomain();
		
			void* CreateObject(_U32 id, _U16 index, const DDLReflect::STRUCT_INFO* info);
			bool DestoryObject(_U32 id, _U16 index);
			bool UpdateObject(_U32 id, _U16 index, _U32 len, const void* data);
			bool FullUpdateObject(_U32 id, _U16 index, _U32 len, const _U8* data);

			const void* GetObjectData(_U32 id, _U16 index, const DDLReflect::STRUCT_INFO* info);
			const DDLReflect::STRUCT_INFO* GetObjectType(_U32 id, _U16 index);

			void GetList(std::vector<CSyncObjectId>& list);

		private:
			std::map<CSyncObjectId, std::pair<const DDLReflect::STRUCT_INFO*, void*>> m_mapObjects;
			_U16 m_nDomainId;
		};

		class CSyncDomainManager
		{
		public:
			CSyncDomainManager();
			~CSyncDomainManager();

			bool IsDomainExist(_U16);
			void GetDomainList(std::vector<_U16>& list);

			bool IsObjectExist(_U16 domain, _U32 id, _U16 index);
			const DDLReflect::STRUCT_INFO* GetObjectType(_U16 domain, _U32 id, _U16 index);
			const void* GetObjectData(_U16 domain, _U32 id, _U16 index, const DDLReflect::STRUCT_INFO* info);

			bool CreateDomain(_U16 domain);
			bool DeleteDomain(_U16 domain);
			bool CreateObject(_U16 domain, _U32 id, _U16 index, _U16 type, _U32 len, const _U8* data);
			bool UpdateObject(_U16 domain, _U32 id, _U16 index, _U32 len, const _U8* data);
			bool FullUpdateObject(_U16 domain, _U32 id, _U16 index, _U32 len, const _U8* data);
			bool DeleteObject(_U32 domain, _U32 id, _U16 index);

		private:
			std::map<_U16, CSyncDomain*> m_mapDomain;
		};

	}
}

#endif
