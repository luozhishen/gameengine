#ifndef __ATLAS_LIVE_OBJECT__
#define __ATLAS_LIVE_OBJECT__

namespace Zion
{
	namespace LiveObject
	{
		bool Register(const DDLReflect::STRUCT_INFO* info);
		void GetTypeList(Zion::Vector<const DDLReflect::STRUCT_INFO*>& list);
		_U16 GetTypeId(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(_U16 id);
	}
}

#endif
