////////////////////////////////////////////////
// THIS FILE IS AUTOGENERATED. DO NOT MODIFY! //
////////////////////////////////////////////////

#include <ZionBase.h>
#include "GameDDL.h"

namespace DDL
{
	template<>
	bool BufferReader::Read<A_GAME_OBJECT>(A_GAME_OBJECT& Value)
	{
		if(!BufferReader::Read<A_LIVE_OBJECT>(Value)) return false;
		if(!Read<A_UUID>(Value.content_uuid)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<A_GAME_OBJECT>(const A_GAME_OBJECT& Value)
	{
		if(!BufferWriter::Write<A_LIVE_OBJECT>(Value)) return false;
		if(!Write<A_UUID>(Value.content_uuid)) return false;
		return true;
	}
}

namespace DDLReflect
{

	static bool _struct_A_GAME_OBJECT_readproc(DDL::BufferReader& buf, void* data)
	{
		return buf.Read<A_GAME_OBJECT>(*((A_GAME_OBJECT*)data));
	}

	static bool _struct_A_GAME_OBJECT_writeproc(DDL::BufferWriter& buf, const void* data)
	{
		return buf.Write<A_GAME_OBJECT>(*((const A_GAME_OBJECT*)data));
	}

	static FIELD_INFO _struct_A_GAME_OBJECT_fieldinfo[] =
	{
		{TYPE_UUID, "content_uuid", 0, (_U16)ZION_OFFSETOF(A_GAME_OBJECT, content_uuid), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(A_UUID), NULL},
	};
	STRUCT_INFO _rfl_struct_A_GAME_OBJECT_info = { &_rfl_struct_A_LIVE_OBJECT_info, "A_GAME_OBJECT", sizeof(A_GAME_OBJECT), 1, _struct_A_GAME_OBJECT_fieldinfo, _struct_A_GAME_OBJECT_readproc, _struct_A_GAME_OBJECT_writeproc };
	template<>
	const STRUCT_INFO* GetStruct<A_GAME_OBJECT>()
	{
		return &_rfl_struct_A_GAME_OBJECT_info;
	}
}

