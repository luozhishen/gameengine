////////////////////////////////////////////////
// THIS FILE IS AUTOGENERATED. DO NOT MODIFY! //
////////////////////////////////////////////////

#ifndef __SGSTRESSCASECONFIG_DDL_CODEGEN__
#define __SGSTRESSCASECONFIG_DDL_CODEGEN__

#include <ZionDefines.h>
#include <ZionSTL.h>
#include <ZionUUID.h>
#include <DDL.h>
#include <DDLProxy.h>
#include <DDLStub.h>
#include <DDLReflect.h>

#pragma pack(push)
#pragma pack(4)

struct STRESSCASE_ECHO_CONFIG
{
	_U32 reset_time;
	_U32 retry_time;
	_U32 disconnect_time;
};

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<STRESSCASE_ECHO_CONFIG>();
	extern STRUCT_INFO _rfl_struct_STRESSCASE_ECHO_CONFIG_info;
}

struct STRESSCASE_ECHO_STATUS
{
	_U32 failed_times;
	_U32 sucess_time;
};

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<STRESSCASE_ECHO_STATUS>();
	extern STRUCT_INFO _rfl_struct_STRESSCASE_ECHO_STATUS_info;
}

struct STRESSCASE_ENTERSERVER_CONFIG
{
	_U32 i;
};

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<STRESSCASE_ENTERSERVER_CONFIG>();
	extern STRUCT_INFO _rfl_struct_STRESSCASE_ENTERSERVER_CONFIG_info;
}

struct STRESSCASE_ENTERSERVER_STATUS
{
	_U32 total_times;
	_U32 success_times;
};

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<STRESSCASE_ENTERSERVER_STATUS>();
	extern STRUCT_INFO _rfl_struct_STRESSCASE_ENTERSERVER_STATUS_info;
}


#pragma pack(pop)

#endif
