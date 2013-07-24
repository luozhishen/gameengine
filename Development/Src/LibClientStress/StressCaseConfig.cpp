////////////////////////////////////////////////
// THIS FILE IS AUTOGENERATED. DO NOT MODIFY! //
////////////////////////////////////////////////

#include "StressCaseConfig.h"

namespace DDLReflect
{

	static bool _struct_STRESSCASE_LOGIN_CONFIG_readproc(DDL::BufferReader& buf, void* data)
	{
		return buf.Read<STRESSCASE_LOGIN_CONFIG>(*((STRESSCASE_LOGIN_CONFIG*)data));
	}

	static bool _struct_STRESSCASE_LOGIN_CONFIG_writeproc(DDL::BufferWriter& buf, const void* data)
	{
		return buf.Write<STRESSCASE_LOGIN_CONFIG>(*((const STRESSCASE_LOGIN_CONFIG*)data));
	}

	static FIELD_INFO _struct_STRESSCASE_LOGIN_CONFIG_fieldinfo[] =
	{
		{TYPE_U32, "connection_reset_time", 0, (_U16)ZION_OFFSETOF(STRESSCASE_LOGIN_CONFIG, connection_reset_time), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U32), NULL},
	};
	STRUCT_INFO _rfl_struct_STRESSCASE_LOGIN_CONFIG_info = { NULL, "STRESSCASE_LOGIN_CONFIG", sizeof(STRESSCASE_LOGIN_CONFIG), 1, _struct_STRESSCASE_LOGIN_CONFIG_fieldinfo, _struct_STRESSCASE_LOGIN_CONFIG_readproc, _struct_STRESSCASE_LOGIN_CONFIG_writeproc };
	template<>
	const STRUCT_INFO* GetStruct<STRESSCASE_LOGIN_CONFIG>()
	{
		return &_rfl_struct_STRESSCASE_LOGIN_CONFIG_info;
	}
}

