////////////////////////////////////////////////
// THIS FILE IS AUTOGENERATED. DO NOT MODIFY! //
////////////////////////////////////////////////

#include <ZionBase.h>
#include "CardGameDDL.h"

namespace DDL
{
	template<>
	bool BufferReader::Read<CARD_AVATAR_OPERATOR>(CARD_AVATAR_OPERATOR& Value)
	{
		if(!BufferReader::Read<A_CONTENT_OBJECT>(Value)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<CARD_AVATAR_OPERATOR>(const CARD_AVATAR_OPERATOR& Value)
	{
		if(!BufferWriter::Write<A_CONTENT_OBJECT>(Value)) return false;
		return true;
	}
}

namespace DDLReflect
{

	static bool _struct_CARD_AVATAR_OPERATOR_readproc(DDL::BufferReader& buf, void* data)
	{
		return buf.Read<CARD_AVATAR_OPERATOR>(*((CARD_AVATAR_OPERATOR*)data));
	}

	static bool _struct_CARD_AVATAR_OPERATOR_writeproc(DDL::BufferWriter& buf, const void* data)
	{
		return buf.Write<CARD_AVATAR_OPERATOR>(*((const CARD_AVATAR_OPERATOR*)data));
	}

	STRUCT_INFO _rfl_struct_CARD_AVATAR_OPERATOR_info = { &_rfl_struct_A_CONTENT_OBJECT_info, "CARD_AVATAR_OPERATOR", sizeof(CARD_AVATAR_OPERATOR), 0, NULL, _struct_CARD_AVATAR_OPERATOR_readproc, _struct_CARD_AVATAR_OPERATOR_writeproc };
	template<>
	const STRUCT_INFO* GetStruct<CARD_AVATAR_OPERATOR>()
	{
		return &_rfl_struct_CARD_AVATAR_OPERATOR_info;
	}
}

namespace DDL
{
	template<>
	bool BufferReader::Read<CARD_AVATAR_DESC>(CARD_AVATAR_DESC& Value)
	{
		if(!BufferReader::Read<A_LIVE_OBJECT>(Value)) return false;
		if(!Read<_U32>(Value.avatar_id)) return false;
		if(!ReadString<100>(Value.avatar_name)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<CARD_AVATAR_DESC>(const CARD_AVATAR_DESC& Value)
	{
		if(!BufferWriter::Write<A_LIVE_OBJECT>(Value)) return false;
		if(!Write<_U32>(Value.avatar_id)) return false;
		if(!WriteString<100>(Value.avatar_name)) return false;
		return true;
	}
}

namespace DDLReflect
{

	static bool _struct_CARD_AVATAR_DESC_readproc(DDL::BufferReader& buf, void* data)
	{
		return buf.Read<CARD_AVATAR_DESC>(*((CARD_AVATAR_DESC*)data));
	}

	static bool _struct_CARD_AVATAR_DESC_writeproc(DDL::BufferWriter& buf, const void* data)
	{
		return buf.Write<CARD_AVATAR_DESC>(*((const CARD_AVATAR_DESC*)data));
	}

	static FIELD_INFO _struct_CARD_AVATAR_DESC_fieldinfo[] =
	{
		{TYPE_U32, "avatar_id", 0, (_U16)ZION_OFFSETOF(CARD_AVATAR_DESC, avatar_id), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U32), NULL},
		{TYPE_STRING, "avatar_name", 0, (_U16)ZION_OFFSETOF(CARD_AVATAR_DESC, avatar_name), NULL, (_U16)100, (_U16)-1, (_U16)0, (_U16)sizeof(DDL::String<100>), NULL},
	};
	STRUCT_INFO _rfl_struct_CARD_AVATAR_DESC_info = { &_rfl_struct_A_LIVE_OBJECT_info, "CARD_AVATAR_DESC", sizeof(CARD_AVATAR_DESC), 2, _struct_CARD_AVATAR_DESC_fieldinfo, _struct_CARD_AVATAR_DESC_readproc, _struct_CARD_AVATAR_DESC_writeproc };
	template<>
	const STRUCT_INFO* GetStruct<CARD_AVATAR_DESC>()
	{
		return &_rfl_struct_CARD_AVATAR_DESC_info;
	}
}

namespace DDL
{
	template<>
	bool BufferReader::Read<CARD_AVATAR>(CARD_AVATAR& Value)
	{
		if(!BufferReader::Read<A_LIVE_OBJECT>(Value)) return false;
		if(!ReadString<100>(Value.avatar_name)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<CARD_AVATAR>(const CARD_AVATAR& Value)
	{
		if(!BufferWriter::Write<A_LIVE_OBJECT>(Value)) return false;
		if(!WriteString<100>(Value.avatar_name)) return false;
		return true;
	}
}

namespace DDLReflect
{

	static bool _struct_CARD_AVATAR_readproc(DDL::BufferReader& buf, void* data)
	{
		return buf.Read<CARD_AVATAR>(*((CARD_AVATAR*)data));
	}

	static bool _struct_CARD_AVATAR_writeproc(DDL::BufferWriter& buf, const void* data)
	{
		return buf.Write<CARD_AVATAR>(*((const CARD_AVATAR*)data));
	}

	static FIELD_INFO _struct_CARD_AVATAR_fieldinfo[] =
	{
		{TYPE_STRING, "avatar_name", 0, (_U16)ZION_OFFSETOF(CARD_AVATAR, avatar_name), NULL, (_U16)100, (_U16)-1, (_U16)0, (_U16)sizeof(DDL::String<100>), NULL},
	};
	STRUCT_INFO _rfl_struct_CARD_AVATAR_info = { &_rfl_struct_A_LIVE_OBJECT_info, "CARD_AVATAR", sizeof(CARD_AVATAR), 1, _struct_CARD_AVATAR_fieldinfo, _struct_CARD_AVATAR_readproc, _struct_CARD_AVATAR_writeproc };
	template<>
	const STRUCT_INFO* GetStruct<CARD_AVATAR>()
	{
		return &_rfl_struct_CARD_AVATAR_info;
	}
}

namespace DDL
{
	template<>
	bool BufferReader::Read<CARD_AVATAR_OWNOBJ>(CARD_AVATAR_OWNOBJ& Value)
	{
		if(!BufferReader::Read<A_LIVE_OBJECT>(Value)) return false;
		if(!ReadString<100>(Value.obj_name)) return false;
		if(!Read<_S32>(Value.obj_value)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<CARD_AVATAR_OWNOBJ>(const CARD_AVATAR_OWNOBJ& Value)
	{
		if(!BufferWriter::Write<A_LIVE_OBJECT>(Value)) return false;
		if(!WriteString<100>(Value.obj_name)) return false;
		if(!Write<_S32>(Value.obj_value)) return false;
		return true;
	}
}

namespace DDLReflect
{

	static bool _struct_CARD_AVATAR_OWNOBJ_readproc(DDL::BufferReader& buf, void* data)
	{
		return buf.Read<CARD_AVATAR_OWNOBJ>(*((CARD_AVATAR_OWNOBJ*)data));
	}

	static bool _struct_CARD_AVATAR_OWNOBJ_writeproc(DDL::BufferWriter& buf, const void* data)
	{
		return buf.Write<CARD_AVATAR_OWNOBJ>(*((const CARD_AVATAR_OWNOBJ*)data));
	}

	static FIELD_INFO _struct_CARD_AVATAR_OWNOBJ_fieldinfo[] =
	{
		{TYPE_STRING, "obj_name", 0, (_U16)ZION_OFFSETOF(CARD_AVATAR_OWNOBJ, obj_name), NULL, (_U16)100, (_U16)-1, (_U16)0, (_U16)sizeof(DDL::String<100>), NULL},
		{TYPE_S32, "obj_value", 0, (_U16)ZION_OFFSETOF(CARD_AVATAR_OWNOBJ, obj_value), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_S32), NULL},
	};
	STRUCT_INFO _rfl_struct_CARD_AVATAR_OWNOBJ_info = { &_rfl_struct_A_LIVE_OBJECT_info, "CARD_AVATAR_OWNOBJ", sizeof(CARD_AVATAR_OWNOBJ), 2, _struct_CARD_AVATAR_OWNOBJ_fieldinfo, _struct_CARD_AVATAR_OWNOBJ_readproc, _struct_CARD_AVATAR_OWNOBJ_writeproc };
	template<>
	const STRUCT_INFO* GetStruct<CARD_AVATAR_OWNOBJ>()
	{
		return &_rfl_struct_CARD_AVATAR_OWNOBJ_info;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _class_CARDGAME_C2S_fieldinfos[] = 
	{
		// 0 Ping
		// 1 SelectServer
		{TYPE_U32, "server_id", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 2 GetAvatarList
		// 3 CreateAvatar
		{TYPE_STRING, "avatar_name", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_U32, "type", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 4 EnterGame
		{TYPE_U32, "avatar_id", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 5 Change
		{TYPE_UUID, "obj_uuid", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_S32, "value", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 6 LeaveGame
	};
	static FUNCTION_INFO _class_CARDGAME_C2S_funcinfos[] = 
	{
		{"Ping", 0, _class_CARDGAME_C2S_fieldinfos+0},
		{"SelectServer", 1, _class_CARDGAME_C2S_fieldinfos+0},
		{"GetAvatarList", 0, _class_CARDGAME_C2S_fieldinfos+1},
		{"CreateAvatar", 2, _class_CARDGAME_C2S_fieldinfos+1},
		{"EnterGame", 1, _class_CARDGAME_C2S_fieldinfos+3},
		{"Change", 2, _class_CARDGAME_C2S_fieldinfos+4},
		{"LeaveGame", 0, _class_CARDGAME_C2S_fieldinfos+6},
	};
	static CLASS_INFO _class_CARDGAME_C2S_info = { 0, "CARDGAME_C2S", 7, _class_CARDGAME_C2S_funcinfos };
	template<>
	const CLASS_INFO* GetClass<CARDGAME_C2S>()
	{
		return &_class_CARDGAME_C2S_info;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _class_CARDGAME_S2C_fieldinfos[] = 
	{
		// 0 Pong
		// 1 GetAvatarListCallback
		{TYPE_U32, "errcode", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_STRUCT|TYPE_ARRAY, "arr", 0, 0, &_rfl_struct_CARD_AVATAR_DESC_info, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_U32, "count", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 2 CreateAvatarCallback
		{TYPE_U32, "errcode", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 3 EnterGameCallback
		{TYPE_U32, "errcode", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 4 SyncAvatar
		{TYPE_STRUCT, "avatar", 0, 0, &_rfl_struct_CARD_AVATAR_info, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 5 SyncAvatarOwnObj
		{TYPE_STRUCT, "ownobj", 0, 0, &_rfl_struct_CARD_AVATAR_OWNOBJ_info, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 6 SyncCompleted
		// 7 LeaveGameCallback
		{TYPE_U32, "errcode", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
	};
	static FUNCTION_INFO _class_CARDGAME_S2C_funcinfos[] = 
	{
		{"Pong", 0, _class_CARDGAME_S2C_fieldinfos+0},
		{"GetAvatarListCallback", 3, _class_CARDGAME_S2C_fieldinfos+0},
		{"CreateAvatarCallback", 1, _class_CARDGAME_S2C_fieldinfos+3},
		{"EnterGameCallback", 1, _class_CARDGAME_S2C_fieldinfos+4},
		{"SyncAvatar", 1, _class_CARDGAME_S2C_fieldinfos+5},
		{"SyncAvatarOwnObj", 1, _class_CARDGAME_S2C_fieldinfos+6},
		{"SyncCompleted", 0, _class_CARDGAME_S2C_fieldinfos+7},
		{"LeaveGameCallback", 1, _class_CARDGAME_S2C_fieldinfos+7},
	};
	static CLASS_INFO _class_CARDGAME_S2C_info = { 0, "CARDGAME_S2C", 8, _class_CARDGAME_S2C_funcinfos };
	template<>
	const CLASS_INFO* GetClass<CARDGAME_S2C>()
	{
		return &_class_CARDGAME_S2C_info;
	}
}

