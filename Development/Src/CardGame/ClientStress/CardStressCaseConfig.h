////////////////////////////////////////////////
// THIS FILE IS AUTOGENERATED. DO NOT MODIFY! //
////////////////////////////////////////////////

#ifndef __CARDSTRESSCASECONFIG_DDL_CODEGEN__
#define __CARDSTRESSCASECONFIG_DDL_CODEGEN__

#pragma pack(push)
#pragma pack(4)

struct STRESSCASE_ECHO_CONFIG
{
	_U32 reset_time;
	_U32 retry_time;
	_U32 disconnect_time;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<STRESSCASE_ECHO_CONFIG>(STRESSCASE_ECHO_CONFIG& Value);
	template<>
	bool BufferWriter::Write<STRESSCASE_ECHO_CONFIG>(const STRESSCASE_ECHO_CONFIG& Value);
}

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

namespace DDL
{
	template<>
	bool BufferReader::Read<STRESSCASE_ECHO_STATUS>(STRESSCASE_ECHO_STATUS& Value);
	template<>
	bool BufferWriter::Write<STRESSCASE_ECHO_STATUS>(const STRESSCASE_ECHO_STATUS& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<STRESSCASE_ECHO_STATUS>();
	extern STRUCT_INFO _rfl_struct_STRESSCASE_ECHO_STATUS_info;
}

struct STRESSCASE_ENTERSERVER_CONFIG
{
	_U32 reset_time;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<STRESSCASE_ENTERSERVER_CONFIG>(STRESSCASE_ENTERSERVER_CONFIG& Value);
	template<>
	bool BufferWriter::Write<STRESSCASE_ENTERSERVER_CONFIG>(const STRESSCASE_ENTERSERVER_CONFIG& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<STRESSCASE_ENTERSERVER_CONFIG>();
	extern STRUCT_INFO _rfl_struct_STRESSCASE_ENTERSERVER_CONFIG_info;
}

struct STRESSCASE_ENTERSERVER_STATUS
{
	_U32 failed_times;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<STRESSCASE_ENTERSERVER_STATUS>(STRESSCASE_ENTERSERVER_STATUS& Value);
	template<>
	bool BufferWriter::Write<STRESSCASE_ENTERSERVER_STATUS>(const STRESSCASE_ENTERSERVER_STATUS& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<STRESSCASE_ENTERSERVER_STATUS>();
	extern STRUCT_INFO _rfl_struct_STRESSCASE_ENTERSERVER_STATUS_info;
}


#pragma pack(pop)

#endif
