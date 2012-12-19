#ifndef __ATLAS_CONTENT_DB_API__
#define __ATLAS_CONTENT_DB_API__

struct A_CONTENT_OBJECT;

namespace Atlas
{
	namespace ContentObject
	{

		bool Register(const DDLReflect::STRUCT_INFO* info, const char* keys=NULL);
		void GetTypeList(std::vector<const DDLReflect::STRUCT_INFO*>& list);
		_U16 GetTypeId(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(_U16 id);
		const char* GetContentTypeKeys(const char* name);

		A_CONTENT_OBJECT* Create(const DDLReflect::STRUCT_INFO* info, A_UUID& uuid);
		A_CONTENT_OBJECT* Alloc(const DDLReflect::STRUCT_INFO* info, const A_UUID& uuid);
		void Delete(const A_UUID& uuid);

		const DDLReflect::STRUCT_INFO* GetType(const A_UUID& uuid);
		const A_CONTENT_OBJECT* Query(const A_UUID& uuid, const DDLReflect::STRUCT_INFO* info=NULL);
		const A_CONTENT_OBJECT* Query(const char* name, const DDLReflect::STRUCT_INFO* info=NULL);
		A_CONTENT_OBJECT* Modify(const A_UUID& uuid, const DDLReflect::STRUCT_INFO* info=NULL);

		bool GetList(const DDLReflect::STRUCT_INFO* info, std::vector<A_UUID>& list, bool bExactMatch);

		template<typename T>
		T* Create(A_UUID& uuid)
		{
			return (T*)Create(DDLReflect::GetStruct<T>(), uuid);
		}

		template<typename T>
		T* Alloc(const A_UUID& uuid)
		{
			return (T*)Alloc(DDLReflect::GetStruct<T>(), uuid);
		}

		template<typename T>
		const T* Query(const A_UUID& uuid)
		{
			return (const T*)Query(uuid, DDLReflect::GetStruct<T>());
		}

		template<typename T>
		const T* Query(const char* name)
		{
			return (const T*)Query(name, DDLReflect::GetStruct<T>());
		}

		template<typename T>
		T* Modify(const A_UUID& uuid)
		{
			return (T*)Modify(uuid, DDLReflect::GetStruct<T>());
		}

		void CreateContentGroup(const DDLReflect::STRUCT_INFO* info, const char* name, const char* file);
		const char* QueryContentGroupName(const DDLReflect::STRUCT_INFO* info);
		const char* QueryContentGroupFile(const DDLReflect::STRUCT_INFO* info);

		bool LoadContent();
		bool LoadContentFromFile(const char* filename);
		bool SaveContent(const char* file=NULL, bool force=false);
		bool IsContentDirty();

	}
}

#endif
