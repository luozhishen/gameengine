#ifndef __ZION_LIVE_OBJECT__
#define __ZION_LIVE_OBJECT__

namespace Zion
{
	namespace LiveObject
	{
		enum SYNCMODE
		{
			MASTER_CLIENT, // client => server
			MASTER_SERVER, // server => client
		};


		class CLiveManager
		{
		};

		class CLiveObject
		{
		public:
			CLiveObject(CLiveManager* pManager, const DDLReflect::STRUCT_INFO* info, const A_UUID& _uuid);

			int Get(const char* name) const;
			void Set(const char* name, void* value);

			bool IsDirty();
			void SetDirty();
		};



		bool Register(const DDLReflect::STRUCT_INFO* info);
		void GetTypeList(Zion::Array<const DDLReflect::STRUCT_INFO*>& list);
		_U16 GetTypeId(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(_U16 id);
	}
}

#endif
