#ifndef __ATLAS_SYNC_COMMON__
#define __ATLAS_SYNC_COMMON__

namespace Atlas
{
	namespace Sync
	{

		class CSyncObjectId
		{
		public:
			CSyncObjectId();
			CSyncObjectId(const CSyncObjectId& id);
			CSyncObjectId(_U16 domain, _U32 id, _U16 index);

			_U16 GetDomain() const;
			_U32 GetId() const;
			_U16 GetIndex() const;
			bool operator<(const CSyncObjectId& id) const;

			_U64 _oid;
		};

		bool Register(const DDLReflect::STRUCT_INFO* info);
		void GetTypeList(std::vector<const DDLReflect::STRUCT_INFO*>& list);
		_U16 GetTypeId(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(_U16 id);

	}
}

#endif
