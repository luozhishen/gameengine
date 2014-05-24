////////////////////////////////////////////////
// THIS FILE IS AUTOGENERATED. DO NOT MODIFY! //
////////////////////////////////////////////////

#include <ZionBase.h>
#include "CardGameDDL.h"

namespace DDL
{
	template<>
	bool BufferReader::Read<CG_AVATAR_DESC>(CG_AVATAR_DESC& Value)
	{
		if(!Read<_U32>(Value.avatar_id)) return false;
		if(!ReadString<100>(Value.avatar_name)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<CG_AVATAR_DESC>(const CG_AVATAR_DESC& Value)
	{
		if(!Write<_U32>(Value.avatar_id)) return false;
		if(!WriteString<100>(Value.avatar_name)) return false;
		return true;
	}
}

namespace DDLReflect
{

	static bool _struct_CG_AVATAR_DESC_readproc(DDL::BufferReader& buf, void* data)
	{
		return buf.Read<CG_AVATAR_DESC>(*((CG_AVATAR_DESC*)data));
	}

	static bool _struct_CG_AVATAR_DESC_writeproc(DDL::BufferWriter& buf, const void* data)
	{
		return buf.Write<CG_AVATAR_DESC>(*((const CG_AVATAR_DESC*)data));
	}

	static FIELD_INFO _struct_CG_AVATAR_DESC_fieldinfo[] =
	{
		{TYPE_U32, "avatar_id", 0, (_U16)ZION_OFFSETOF(CG_AVATAR_DESC, avatar_id), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U32), NULL},
		{TYPE_STRING, "avatar_name", 0, (_U16)ZION_OFFSETOF(CG_AVATAR_DESC, avatar_name), NULL, (_U16)100, (_U16)-1, (_U16)0, (_U16)sizeof(DDL::String<100>), NULL},
	};
	STRUCT_INFO _rfl_struct_CG_AVATAR_DESC_info = { NULL, "CG_AVATAR_DESC", sizeof(CG_AVATAR_DESC), 2, _struct_CG_AVATAR_DESC_fieldinfo, _struct_CG_AVATAR_DESC_readproc, _struct_CG_AVATAR_DESC_writeproc };
	template<>
	const STRUCT_INFO* GetStruct<CG_AVATAR_DESC>()
	{
		return &_rfl_struct_CG_AVATAR_DESC_info;
	}
}

namespace DDL
{
	template<>
	bool BufferReader::Read<CG_AVATAR>(CG_AVATAR& Value)
	{
		if(!BufferReader::Read<A_LIVE_OBJECT>(Value)) return false;
		if(!ReadString<100>(Value.avatar_name)) return false;
		if(!Read<A_CONFUSED_U32>(Value.money)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<CG_AVATAR>(const CG_AVATAR& Value)
	{
		if(!BufferWriter::Write<A_LIVE_OBJECT>(Value)) return false;
		if(!WriteString<100>(Value.avatar_name)) return false;
		if(!Write<A_CONFUSED_U32>(Value.money)) return false;
		return true;
	}
}

namespace DDLReflect
{

	static bool _struct_CG_AVATAR_readproc(DDL::BufferReader& buf, void* data)
	{
		return buf.Read<CG_AVATAR>(*((CG_AVATAR*)data));
	}

	static bool _struct_CG_AVATAR_writeproc(DDL::BufferWriter& buf, const void* data)
	{
		return buf.Write<CG_AVATAR>(*((const CG_AVATAR*)data));
	}

	static FIELD_INFO _struct_CG_AVATAR_fieldinfo[] =
	{
		{TYPE_STRING, "avatar_name", 0, (_U16)ZION_OFFSETOF(CG_AVATAR, avatar_name), NULL, (_U16)100, (_U16)-1, (_U16)0, (_U16)sizeof(DDL::String<100>), NULL},
		{TYPE_STRUCT, "money", 0, (_U16)ZION_OFFSETOF(CG_AVATAR, money), &_rfl_struct_A_CONFUSED_U32_info, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(A_CONFUSED_U32), NULL},
	};
	STRUCT_INFO _rfl_struct_CG_AVATAR_info = { &_rfl_struct_A_LIVE_OBJECT_info, "CG_AVATAR", sizeof(CG_AVATAR), 2, _struct_CG_AVATAR_fieldinfo, _struct_CG_AVATAR_readproc, _struct_CG_AVATAR_writeproc };
	template<>
	const STRUCT_INFO* GetStruct<CG_AVATAR>()
	{
		return &_rfl_struct_CG_AVATAR_info;
	}
}

namespace DDL
{
	template<>
	bool BufferReader::Read<CG_CARD>(CG_CARD& Value)
	{
		if(!BufferReader::Read<A_LIVE_OBJECT>(Value)) return false;
		if(!Read<A_UUID>(Value.content_uuid)) return false;
		if(!Read<_U32>(Value.win_count)) return false;
		if(!Read<_U32>(Value.lost_count)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<CG_CARD>(const CG_CARD& Value)
	{
		if(!BufferWriter::Write<A_LIVE_OBJECT>(Value)) return false;
		if(!Write<A_UUID>(Value.content_uuid)) return false;
		if(!Write<_U32>(Value.win_count)) return false;
		if(!Write<_U32>(Value.lost_count)) return false;
		return true;
	}
}

namespace DDLReflect
{

	static bool _struct_CG_CARD_readproc(DDL::BufferReader& buf, void* data)
	{
		return buf.Read<CG_CARD>(*((CG_CARD*)data));
	}

	static bool _struct_CG_CARD_writeproc(DDL::BufferWriter& buf, const void* data)
	{
		return buf.Write<CG_CARD>(*((const CG_CARD*)data));
	}

	static FIELD_INFO _struct_CG_CARD_fieldinfo[] =
	{
		{TYPE_UUID, "content_uuid", 0, (_U16)ZION_OFFSETOF(CG_CARD, content_uuid), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(A_UUID), NULL},
		{TYPE_U32, "win_count", 0, (_U16)ZION_OFFSETOF(CG_CARD, win_count), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U32), NULL},
		{TYPE_U32, "lost_count", 0, (_U16)ZION_OFFSETOF(CG_CARD, lost_count), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U32), NULL},
	};
	STRUCT_INFO _rfl_struct_CG_CARD_info = { &_rfl_struct_A_LIVE_OBJECT_info, "CG_CARD", sizeof(CG_CARD), 3, _struct_CG_CARD_fieldinfo, _struct_CG_CARD_readproc, _struct_CG_CARD_writeproc };
	template<>
	const STRUCT_INFO* GetStruct<CG_CARD>()
	{
		return &_rfl_struct_CG_CARD_info;
	}
}

namespace DDL
{
	template<>
	bool BufferReader::Read<CG_SHOPITEM>(CG_SHOPITEM& Value)
	{
		if(!BufferReader::Read<A_CONTENT_OBJECT>(Value)) return false;
		if(!ReadString<100>(Value.shopitem_id)) return false;
		if(!Read<_U32>(Value.price)) return false;
		if(!Read<A_UUID>(Value.content_uuid)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<CG_SHOPITEM>(const CG_SHOPITEM& Value)
	{
		if(!BufferWriter::Write<A_CONTENT_OBJECT>(Value)) return false;
		if(!WriteString<100>(Value.shopitem_id)) return false;
		if(!Write<_U32>(Value.price)) return false;
		if(!Write<A_UUID>(Value.content_uuid)) return false;
		return true;
	}
}

namespace DDLReflect
{

	static bool _struct_CG_SHOPITEM_readproc(DDL::BufferReader& buf, void* data)
	{
		return buf.Read<CG_SHOPITEM>(*((CG_SHOPITEM*)data));
	}

	static bool _struct_CG_SHOPITEM_writeproc(DDL::BufferWriter& buf, const void* data)
	{
		return buf.Write<CG_SHOPITEM>(*((const CG_SHOPITEM*)data));
	}

	static FIELD_INFO _struct_CG_SHOPITEM_fieldinfo[] =
	{
		{TYPE_STRING, "shopitem_id", 0, (_U16)ZION_OFFSETOF(CG_SHOPITEM, shopitem_id), NULL, (_U16)100, (_U16)-1, (_U16)0, (_U16)sizeof(DDL::String<100>), NULL},
		{TYPE_U32, "price", 0, (_U16)ZION_OFFSETOF(CG_SHOPITEM, price), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U32), NULL},
		{TYPE_UUID, "content_uuid", 0, (_U16)ZION_OFFSETOF(CG_SHOPITEM, content_uuid), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(A_UUID), NULL},
	};
	STRUCT_INFO _rfl_struct_CG_SHOPITEM_info = { &_rfl_struct_A_CONTENT_OBJECT_info, "CG_SHOPITEM", sizeof(CG_SHOPITEM), 3, _struct_CG_SHOPITEM_fieldinfo, _struct_CG_SHOPITEM_readproc, _struct_CG_SHOPITEM_writeproc };
	template<>
	const STRUCT_INFO* GetStruct<CG_SHOPITEM>()
	{
		return &_rfl_struct_CG_SHOPITEM_info;
	}
}

namespace DDL
{
	template<>
	bool BufferReader::Read<CG_CARD_CONFIG>(CG_CARD_CONFIG& Value)
	{
		if(!BufferReader::Read<A_CONTENT_OBJECT>(Value)) return false;
		if(!ReadString<100>(Value.disp_name)) return false;
		if(!Read<_U32>(Value.point)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<CG_CARD_CONFIG>(const CG_CARD_CONFIG& Value)
	{
		if(!BufferWriter::Write<A_CONTENT_OBJECT>(Value)) return false;
		if(!WriteString<100>(Value.disp_name)) return false;
		if(!Write<_U32>(Value.point)) return false;
		return true;
	}
}

namespace DDLReflect
{

	static bool _struct_CG_CARD_CONFIG_readproc(DDL::BufferReader& buf, void* data)
	{
		return buf.Read<CG_CARD_CONFIG>(*((CG_CARD_CONFIG*)data));
	}

	static bool _struct_CG_CARD_CONFIG_writeproc(DDL::BufferWriter& buf, const void* data)
	{
		return buf.Write<CG_CARD_CONFIG>(*((const CG_CARD_CONFIG*)data));
	}

	static FIELD_INFO _struct_CG_CARD_CONFIG_fieldinfo[] =
	{
		{TYPE_STRING, "disp_name", 0, (_U16)ZION_OFFSETOF(CG_CARD_CONFIG, disp_name), NULL, (_U16)100, (_U16)-1, (_U16)0, (_U16)sizeof(DDL::String<100>), NULL},
		{TYPE_U32, "point", 0, (_U16)ZION_OFFSETOF(CG_CARD_CONFIG, point), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U32), NULL},
	};
	STRUCT_INFO _rfl_struct_CG_CARD_CONFIG_info = { &_rfl_struct_A_CONTENT_OBJECT_info, "CG_CARD_CONFIG", sizeof(CG_CARD_CONFIG), 2, _struct_CG_CARD_CONFIG_fieldinfo, _struct_CG_CARD_CONFIG_readproc, _struct_CG_CARD_CONFIG_writeproc };
	template<>
	const STRUCT_INFO* GetStruct<CG_CARD_CONFIG>()
	{
		return &_rfl_struct_CG_CARD_CONFIG_info;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _class_CGSERVER_BASE_fieldinfos[] = 
	{
		// 0 Ping
		// 1 SelectServer
		{TYPE_U32, "server_id", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 2 GetAvatarList
		// 3 CreateAvatar
		{TYPE_STRING, "avatar_name", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 4 EnterGame
		{TYPE_U32, "avatar_id", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 5 LeaveGame
	};
	static FUNCTION_INFO _class_CGSERVER_BASE_funcinfos[] = 
	{
		{"Ping", 0, _class_CGSERVER_BASE_fieldinfos+0},
		{"SelectServer", 1, _class_CGSERVER_BASE_fieldinfos+0},
		{"GetAvatarList", 0, _class_CGSERVER_BASE_fieldinfos+1},
		{"CreateAvatar", 1, _class_CGSERVER_BASE_fieldinfos+1},
		{"EnterGame", 1, _class_CGSERVER_BASE_fieldinfos+2},
		{"LeaveGame", 0, _class_CGSERVER_BASE_fieldinfos+3},
	};
	static CLASS_INFO _class_CGSERVER_BASE_info = { 0, "CGSERVER_BASE", 6, _class_CGSERVER_BASE_funcinfos };
	template<>
	const CLASS_INFO* GetClass<CGSERVER_BASE>()
	{
		return &_class_CGSERVER_BASE_info;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _class_CGCALLBACK_BASE_fieldinfos[] = 
	{
		// 0 Pong
		// 1 GetAvatarListCallback
		{TYPE_STRUCT|TYPE_ARRAY, "arr", 0, 0, &_rfl_struct_CG_AVATAR_DESC_info, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_U32, "count", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 2 CreateAvatarCallback
		{TYPE_U32, "errcode", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 3 EnterGameCallback
		{TYPE_U32, "errcode", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 4 LeaveGameCallback
		{TYPE_U32, "errcode", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
	};
	static FUNCTION_INFO _class_CGCALLBACK_BASE_funcinfos[] = 
	{
		{"Pong", 0, _class_CGCALLBACK_BASE_fieldinfos+0},
		{"GetAvatarListCallback", 2, _class_CGCALLBACK_BASE_fieldinfos+0},
		{"CreateAvatarCallback", 1, _class_CGCALLBACK_BASE_fieldinfos+2},
		{"EnterGameCallback", 1, _class_CGCALLBACK_BASE_fieldinfos+3},
		{"LeaveGameCallback", 1, _class_CGCALLBACK_BASE_fieldinfos+4},
	};
	static CLASS_INFO _class_CGCALLBACK_BASE_info = { 0, "CGCALLBACK_BASE", 5, _class_CGCALLBACK_BASE_funcinfos };
	template<>
	const CLASS_INFO* GetClass<CGCALLBACK_BASE>()
	{
		return &_class_CGCALLBACK_BASE_info;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _class_CGSERVER_GAME_fieldinfos[] = 
	{
		// 0 Buy
		{TYPE_STRING, "shopitem_id", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 1 Discard
		{TYPE_UUID, "uuid", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 2 Beg
		{TYPE_U32, "money", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 3 Gamble
		{TYPE_UUID, "card", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		{TYPE_S32, "mode", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
	};
	static FUNCTION_INFO _class_CGSERVER_GAME_funcinfos[] = 
	{
		{"Buy", 1, _class_CGSERVER_GAME_fieldinfos+0},
		{"Discard", 1, _class_CGSERVER_GAME_fieldinfos+1},
		{"Beg", 1, _class_CGSERVER_GAME_fieldinfos+2},
		{"Gamble", 2, _class_CGSERVER_GAME_fieldinfos+3},
	};
	static CLASS_INFO _class_CGSERVER_GAME_info = { 0, "CGSERVER_GAME", 4, _class_CGSERVER_GAME_funcinfos };
	template<>
	const CLASS_INFO* GetClass<CGSERVER_GAME>()
	{
		return &_class_CGSERVER_GAME_info;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _class_CGCALLBACK_GAME_fieldinfos[] = 
	{
		// 0 Ready
		// 1 BegResult
		{TYPE_U32, "money", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 2 GambleResult
		{TYPE_U32, "point", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
	};
	static FUNCTION_INFO _class_CGCALLBACK_GAME_funcinfos[] = 
	{
		{"Ready", 0, _class_CGCALLBACK_GAME_fieldinfos+0},
		{"BegResult", 1, _class_CGCALLBACK_GAME_fieldinfos+0},
		{"GambleResult", 1, _class_CGCALLBACK_GAME_fieldinfos+1},
	};
	static CLASS_INFO _class_CGCALLBACK_GAME_info = { 0, "CGCALLBACK_GAME", 3, _class_CGCALLBACK_GAME_funcinfos };
	template<>
	const CLASS_INFO* GetClass<CGCALLBACK_GAME>()
	{
		return &_class_CGCALLBACK_GAME_info;
	}
}

