////////////////////////////////////////////////
// THIS FILE IS AUTOGENERATED. DO NOT MODIFY! //
////////////////////////////////////////////////

#ifndef __KNIGHTGAMEDDL_DDL_CODEGEN__
#define __KNIGHTGAMEDDL_DDL_CODEGEN__

#include <AtlasDefines.h>
#include <AtlasSTL.h>
#include <AtlasUUID.h>
#include <DDL.h>
#include <DDLProxy.h>
#include <DDLStub.h>
#include <DDLReflect.h>

#pragma pack(push)
#pragma pack(4)

#include "../../LibCommon/CommonDDL.h"

const _U32 KNIGHT_DESCRIPTION_MAX = 256;

const _U32 KNIGHT_RESOURCE_URL_MAX = 128;

const _U32 KNIGHT_SKILL_NAME_MAX = 16;

const _U32 KNIGHT_SKILL_BUFF_MAX = 8;

struct KNIGHT_SKILL_CONFIG : A_CONTENT_OBJECT
{
	DDL::String<KNIGHT_SKILL_NAME_MAX> SkillName;
	DDL::String<KNIGHT_DESCRIPTION_MAX> SkillDesc;
	DDL::String<KNIGHT_RESOURCE_URL_MAX> ResourceURL;
	_U32 Element;
	_U32 FormulaType;
	_U32 Target;
	_U32 Range;
	_U32 AveValue;
	_F32 Probability;
	DDL::Array<DDL::String<KNIGHT_RESOURCE_URL_MAX>,KNIGHT_SKILL_BUFF_MAX> BuffPaths;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<KNIGHT_SKILL_CONFIG>(KNIGHT_SKILL_CONFIG& Value);
	template<>
	bool BufferWriter::Write<KNIGHT_SKILL_CONFIG>(const KNIGHT_SKILL_CONFIG& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<KNIGHT_SKILL_CONFIG>();
	extern STRUCT_INFO _rfl_struct_KNIGHT_SKILL_CONFIG_info;
}


#pragma pack(pop)

#endif
