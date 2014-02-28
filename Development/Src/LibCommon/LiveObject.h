#ifndef __ZION_LIVE_OBJECT__
#define __ZION_LIVE_OBJECT__

namespace Zion
{
	namespace LiveObject
	{
		enum SYNCMODE
		{
			SYNCMODE_DISABLE,
			SYNCMODE_CLIENT,
			SYNCMODE_SERVER,
		};

		bool Register(const DDLReflect::STRUCT_INFO* info);
		void GetTypeList(Zion::Array<const DDLReflect::STRUCT_INFO*>& list);
		_U16 GetTypeId(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(const char* name);
		const DDLReflect::STRUCT_INFO* GetType(_U16 id);
	}
}

#endif
