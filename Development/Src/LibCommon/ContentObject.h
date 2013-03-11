#ifndef __ATLAS_CONTENT_DB_API__
#define __ATLAS_CONTENT_DB_API__

struct A_CONTENT_OBJECT;

namespace Atlas
{
	namespace ContentObject
	{

		bool Register(const DDLReflect::STRUCT_INFO* info, bool bExactMatch=true, const char* keys=NULL);
		void GetTypeList(Atlas::Vector<const DDLReflect::STRUCT_INFO*>& list);
		_U16 GetTypeId(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(_U16 id);

		A_CONTENT_OBJECT* Create(const DDLReflect::STRUCT_INFO* info, A_UUID& uuid);
		A_CONTENT_OBJECT* Alloc(const DDLReflect::STRUCT_INFO* info, const A_UUID& uuid);
		void Delete(const A_UUID& uuid);

		const DDLReflect::STRUCT_INFO* GetType(const A_UUID& uuid);
		A_CONTENT_OBJECT* Modify(const A_UUID& uuid, const DDLReflect::STRUCT_INFO* info=NULL);
		const A_CONTENT_OBJECT* QueryByUUID(const A_UUID& uuid, const DDLReflect::STRUCT_INFO* info=NULL);
		const A_CONTENT_OBJECT* QueryByName(const char* name, const DDLReflect::STRUCT_INFO* info=NULL);
		const A_CONTENT_OBJECT* QueryByKey(const DDLReflect::STRUCT_INFO* info, const char* value1, const char* value2=NULL, const char* value3=NULL, const char* value4=NULL);

		bool BuildIndex(const DDLReflect::STRUCT_INFO* info=NULL);
		const Atlas::String& BuildIndexGetErrorMsg();

		// will remove this function.
		bool GetList(const DDLReflect::STRUCT_INFO* info, Atlas::Vector<A_UUID>& list, bool bExactMatch);

		const A_CONTENT_OBJECT* FindFirst(const DDLReflect::STRUCT_INFO* info, bool bExactMatch);
		const A_CONTENT_OBJECT* FindNext(const DDLReflect::STRUCT_INFO* info, bool bExactMatch, const A_CONTENT_OBJECT* object);

		void CreateContentGroup(const DDLReflect::STRUCT_INFO* info, const char* name, const char* file);
		const char* QueryContentGroupName(const DDLReflect::STRUCT_INFO* info);
		const char* QueryContentGroupFile(const DDLReflect::STRUCT_INFO* info);
		void GetContentFileList(Atlas::Vector<Atlas::String>& list);

		bool LoadContent(const char* path=NULL);
		bool LoadContentFromFile(const char* filename);
		bool SaveContent(const char* file=NULL, bool force=false);
		void ClearContent();
		bool IsContentDirty();

	}
}

#endif
