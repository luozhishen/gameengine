#ifndef __ATLAS_CONTENT_DB_API__
#define __ATLAS_CONTENT_DB_API__

struct A_CONTENT_OBJECT;

namespace Atlas
{
	namespace ContentObject
	{

		class IContentGroup
		{
		public:
			virtual ~IContentGroup() {}
			virtual IContentGroup* Register(const DDLReflect::STRUCT_INFO* info, bool bExactMatch=true, const char* keys=NULL) = 0;
			virtual bool GetCookFlag() = 0;
			virtual void SetCookFlag(bool cook) = 0;
		};
		IContentGroup* CreateContentGroup(const char* name, const char* file, bool cook);
		IContentGroup* GetContentGroup(const char* name);

		void GetTypeList(Atlas::Vector<const DDLReflect::STRUCT_INFO*>& list);
		_U16 GetTypeId(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(_U16 id);
		bool GetTypePrimaryKey(const char* name, Atlas::Set<Atlas::String>& keys);

		A_CONTENT_OBJECT* Create(const DDLReflect::STRUCT_INFO* info, A_UUID& uuid);
		A_CONTENT_OBJECT* Alloc(const DDLReflect::STRUCT_INFO* info, const A_UUID& uuid);
		void Delete(const A_UUID& uuid);

		const DDLReflect::STRUCT_INFO* GetObjectType(const A_UUID& uuid);
		A_CONTENT_OBJECT* Modify(const A_UUID& uuid, const DDLReflect::STRUCT_INFO* info=NULL);
		const A_CONTENT_OBJECT* QueryByUUID(const A_UUID& uuid, const DDLReflect::STRUCT_INFO* info=NULL);
		const A_CONTENT_OBJECT* QueryByName(const char* name, const DDLReflect::STRUCT_INFO* info=NULL);
		const A_CONTENT_OBJECT* QueryByKey(const DDLReflect::STRUCT_INFO* info, const char* value1, const char* value2=NULL, const char* value3=NULL, const char* value4=NULL);
		const A_CONTENT_OBJECT* QueryByUniqueId(const DDLReflect::STRUCT_INFO* info, const char* value1);
		bool GenContentObjectUniqueId(_U16 id, const A_CONTENT_OBJECT* obj, Atlas::String& uid);

		bool BuildIndex(const DDLReflect::STRUCT_INFO* info=NULL);
		const Atlas::String& BuildIndexGetErrorMsg();

		// will remove this function.
		bool GetList(const DDLReflect::STRUCT_INFO* info, Atlas::Vector<A_UUID>& list, bool bExactMatch);

		const A_CONTENT_OBJECT* FindFirst(const DDLReflect::STRUCT_INFO* info, bool bExactMatch);
		const A_CONTENT_OBJECT* FindNext(const DDLReflect::STRUCT_INFO* info, bool bExactMatch, const A_CONTENT_OBJECT* object);

		bool LoadContent(const char* path=NULL, bool ignore=true);
		bool LoadContentFromBinaryFile(const char* filename, const char* deskey);
		bool SaveContent(const char* path=NULL, bool force=false);
		bool SaveContentToBinaryFile(const char* file, const char* deskey);
		void ClearContents();
		bool IsContentDirty();

	}
}

#endif
