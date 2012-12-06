////////////////////////////////////////////////
// THIS FILE IS AUTOGENERATED. DO NOT MODIFY! //
////////////////////////////////////////////////

#include "SGGameDDL.h"

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_PAWN_CONFIG>(SG_PAWN_CONFIG& Value)
	{
		if(!BufferReader::Read<A_CONTENT_OBJECT>(Value)) return false;
		if(!Read<_F32>(Value.BaseHP)) return false;
		if(!Read<_F32>(Value.SigmaHPAddition)) return false;
		if(!Read<_F32>(Value.SigmaHPPercent)) return false;
		if(!Read<_F32>(Value.SigmaExtraHP)) return false;
		if(!Read<_F32>(Value.BaseATK)) return false;
		if(!Read<_F32>(Value.SigmaATKAddition)) return false;
		if(!Read<_F32>(Value.SigmaATKPercent)) return false;
		if(!Read<_F32>(Value.SigmaExtraATK)) return false;
		if(!Read<_F32>(Value.BaseDEF)) return false;
		if(!Read<_F32>(Value.SigmaDEFAddition)) return false;
		if(!Read<_F32>(Value.SigmaDEFPercent)) return false;
		if(!Read<_F32>(Value.SigmaExtraDEF)) return false;
		if(!Read<_F32>(Value.BaseHIT)) return false;
		if(!Read<_F32>(Value.SigmaHITAddition)) return false;
		if(!Read<_F32>(Value.BaseEvasion)) return false;
		if(!Read<_F32>(Value.SigmaEvasionAddition)) return false;
		if(!Read<_F32>(Value.BaseCRIT)) return false;
		if(!Read<_F32>(Value.SigmaCRITAddition)) return false;
		if(!Read<_F32>(Value.BaseSP)) return false;
		if(!Read<_F32>(Value.SigmaSPAddition)) return false;
		if(!Read<_F32>(Value.BaseWS)) return false;
		if(!Read<_F32>(Value.SigmaWSAddition)) return false;
		if(!Read<_F32>(Value.SigmaWSPercent)) return false;
		if(!Read<_F32>(Value.SigmaExtraWS)) return false;
		if(!Read<_F32>(Value.BaseSTR)) return false;
		if(!Read<_F32>(Value.SigmaSTRAddition)) return false;
		if(!Read<_F32>(Value.SigmaSTRPercent)) return false;
		if(!Read<_F32>(Value.SigmaExtraSTR)) return false;
		if(!Read<_F32>(Value.BaseINT)) return false;
		if(!Read<_F32>(Value.SigmaINTAddition)) return false;
		if(!Read<_F32>(Value.SigmaINTPercent)) return false;
		if(!Read<_F32>(Value.SigmaExtraINT)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<SG_PAWN_CONFIG>(const SG_PAWN_CONFIG& Value)
	{
		if(!BufferWriter::Write<A_CONTENT_OBJECT>(Value)) return false;
		if(!Write<_F32>(Value.BaseHP)) return false;
		if(!Write<_F32>(Value.SigmaHPAddition)) return false;
		if(!Write<_F32>(Value.SigmaHPPercent)) return false;
		if(!Write<_F32>(Value.SigmaExtraHP)) return false;
		if(!Write<_F32>(Value.BaseATK)) return false;
		if(!Write<_F32>(Value.SigmaATKAddition)) return false;
		if(!Write<_F32>(Value.SigmaATKPercent)) return false;
		if(!Write<_F32>(Value.SigmaExtraATK)) return false;
		if(!Write<_F32>(Value.BaseDEF)) return false;
		if(!Write<_F32>(Value.SigmaDEFAddition)) return false;
		if(!Write<_F32>(Value.SigmaDEFPercent)) return false;
		if(!Write<_F32>(Value.SigmaExtraDEF)) return false;
		if(!Write<_F32>(Value.BaseHIT)) return false;
		if(!Write<_F32>(Value.SigmaHITAddition)) return false;
		if(!Write<_F32>(Value.BaseEvasion)) return false;
		if(!Write<_F32>(Value.SigmaEvasionAddition)) return false;
		if(!Write<_F32>(Value.BaseCRIT)) return false;
		if(!Write<_F32>(Value.SigmaCRITAddition)) return false;
		if(!Write<_F32>(Value.BaseSP)) return false;
		if(!Write<_F32>(Value.SigmaSPAddition)) return false;
		if(!Write<_F32>(Value.BaseWS)) return false;
		if(!Write<_F32>(Value.SigmaWSAddition)) return false;
		if(!Write<_F32>(Value.SigmaWSPercent)) return false;
		if(!Write<_F32>(Value.SigmaExtraWS)) return false;
		if(!Write<_F32>(Value.BaseSTR)) return false;
		if(!Write<_F32>(Value.SigmaSTRAddition)) return false;
		if(!Write<_F32>(Value.SigmaSTRPercent)) return false;
		if(!Write<_F32>(Value.SigmaExtraSTR)) return false;
		if(!Write<_F32>(Value.BaseINT)) return false;
		if(!Write<_F32>(Value.SigmaINTAddition)) return false;
		if(!Write<_F32>(Value.SigmaINTPercent)) return false;
		if(!Write<_F32>(Value.SigmaExtraINT)) return false;
		return true;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _struct_SG_PAWN_CONFIG_fieldinfo[] =
	{
		{TYPE_F32, "BaseHP", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, BaseHP), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaHPAddition", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaHPAddition), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaHPPercent", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaHPPercent), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaExtraHP", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaExtraHP), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "BaseATK", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, BaseATK), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaATKAddition", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaATKAddition), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaATKPercent", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaATKPercent), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaExtraATK", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaExtraATK), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "BaseDEF", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, BaseDEF), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaDEFAddition", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaDEFAddition), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaDEFPercent", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaDEFPercent), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaExtraDEF", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaExtraDEF), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "BaseHIT", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, BaseHIT), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaHITAddition", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaHITAddition), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "BaseEvasion", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, BaseEvasion), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaEvasionAddition", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaEvasionAddition), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "BaseCRIT", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, BaseCRIT), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaCRITAddition", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaCRITAddition), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "BaseSP", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, BaseSP), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaSPAddition", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaSPAddition), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "BaseWS", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, BaseWS), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaWSAddition", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaWSAddition), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaWSPercent", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaWSPercent), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaExtraWS", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaExtraWS), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "BaseSTR", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, BaseSTR), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaSTRAddition", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaSTRAddition), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaSTRPercent", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaSTRPercent), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaExtraSTR", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaExtraSTR), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "BaseINT", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, BaseINT), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaINTAddition", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaINTAddition), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaINTPercent", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaINTPercent), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
		{TYPE_F32, "SigmaExtraINT", 0, ATLAS_OFFSETOF(SG_PAWN_CONFIG, SigmaExtraINT), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_F32), NULL},
	};
	STRUCT_INFO _rfl_struct_SG_PAWN_CONFIG_info = { &_rfl_struct_A_CONTENT_OBJECT_info, "SG_PAWN_CONFIG", sizeof(SG_PAWN_CONFIG), 32, _struct_SG_PAWN_CONFIG_fieldinfo };
	template<>
	const STRUCT_INFO* GetStruct<SG_PAWN_CONFIG>()
	{
		return &_rfl_struct_SG_PAWN_CONFIG_info;
	}
}

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_ENEMY_CONFIG>(SG_ENEMY_CONFIG& Value)
	{
		if(!BufferReader::Read<SG_PAWN_CONFIG>(Value)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<SG_ENEMY_CONFIG>(const SG_ENEMY_CONFIG& Value)
	{
		if(!BufferWriter::Write<SG_PAWN_CONFIG>(Value)) return false;
		return true;
	}
}

namespace DDLReflect
{
	STRUCT_INFO _rfl_struct_SG_ENEMY_CONFIG_info = { &_rfl_struct_SG_PAWN_CONFIG_info, "SG_ENEMY_CONFIG", sizeof(SG_ENEMY_CONFIG), 0, NULL };
	template<>
	const STRUCT_INFO* GetStruct<SG_ENEMY_CONFIG>()
	{
		return &_rfl_struct_SG_ENEMY_CONFIG_info;
	}
}

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_SOLDIER_CONFIG>(SG_SOLDIER_CONFIG& Value)
	{
		if(!BufferReader::Read<SG_PAWN_CONFIG>(Value)) return false;
		if(!Read<_U16>(Value.type)) return false;
		if(!Read<_U16>(Value.level)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<SG_SOLDIER_CONFIG>(const SG_SOLDIER_CONFIG& Value)
	{
		if(!BufferWriter::Write<SG_PAWN_CONFIG>(Value)) return false;
		if(!Write<_U16>(Value.type)) return false;
		if(!Write<_U16>(Value.level)) return false;
		return true;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _struct_SG_SOLDIER_CONFIG_fieldinfo[] =
	{
		{TYPE_U16, "type", 0, ATLAS_OFFSETOF(SG_SOLDIER_CONFIG, type), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U16), NULL},
		{TYPE_U16, "level", 0, ATLAS_OFFSETOF(SG_SOLDIER_CONFIG, level), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U16), NULL},
	};
	STRUCT_INFO _rfl_struct_SG_SOLDIER_CONFIG_info = { &_rfl_struct_SG_PAWN_CONFIG_info, "SG_SOLDIER_CONFIG", sizeof(SG_SOLDIER_CONFIG), 2, _struct_SG_SOLDIER_CONFIG_fieldinfo };
	template<>
	const STRUCT_INFO* GetStruct<SG_SOLDIER_CONFIG>()
	{
		return &_rfl_struct_SG_SOLDIER_CONFIG_info;
	}
}

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_GENERAL_CONFIG>(SG_GENERAL_CONFIG& Value)
	{
		if(!BufferReader::Read<SG_PAWN_CONFIG>(Value)) return false;
		if(!Read<_U32>(Value.type)) return false;
		if(!Read<_U16>(Value.level)) return false;
		if(!Read<_U32>(Value.exp)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<SG_GENERAL_CONFIG>(const SG_GENERAL_CONFIG& Value)
	{
		if(!BufferWriter::Write<SG_PAWN_CONFIG>(Value)) return false;
		if(!Write<_U32>(Value.type)) return false;
		if(!Write<_U16>(Value.level)) return false;
		if(!Write<_U32>(Value.exp)) return false;
		return true;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _struct_SG_GENERAL_CONFIG_fieldinfo[] =
	{
		{TYPE_U32, "type", 0, ATLAS_OFFSETOF(SG_GENERAL_CONFIG, type), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U32), NULL},
		{TYPE_U16, "level", 0, ATLAS_OFFSETOF(SG_GENERAL_CONFIG, level), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U16), NULL},
		{TYPE_U32, "exp", 0, ATLAS_OFFSETOF(SG_GENERAL_CONFIG, exp), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U32), NULL},
	};
	STRUCT_INFO _rfl_struct_SG_GENERAL_CONFIG_info = { &_rfl_struct_SG_PAWN_CONFIG_info, "SG_GENERAL_CONFIG", sizeof(SG_GENERAL_CONFIG), 3, _struct_SG_GENERAL_CONFIG_fieldinfo };
	template<>
	const STRUCT_INFO* GetStruct<SG_GENERAL_CONFIG>()
	{
		return &_rfl_struct_SG_GENERAL_CONFIG_info;
	}
}

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_EQUIP_SLOTS>(SG_EQUIP_SLOTS& Value)
	{
		if(!Read<A_UUID>(Value.head)) return false;
		if(!Read<A_UUID>(Value.hand)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<SG_EQUIP_SLOTS>(const SG_EQUIP_SLOTS& Value)
	{
		if(!Write<A_UUID>(Value.head)) return false;
		if(!Write<A_UUID>(Value.hand)) return false;
		return true;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _struct_SG_EQUIP_SLOTS_fieldinfo[] =
	{
		{TYPE_UUID, "head", 0, ATLAS_OFFSETOF(SG_EQUIP_SLOTS, head), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(A_UUID), NULL},
		{TYPE_UUID, "hand", 0, ATLAS_OFFSETOF(SG_EQUIP_SLOTS, hand), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(A_UUID), NULL},
	};
	STRUCT_INFO _rfl_struct_SG_EQUIP_SLOTS_info = { NULL, "SG_EQUIP_SLOTS", sizeof(SG_EQUIP_SLOTS), 2, _struct_SG_EQUIP_SLOTS_fieldinfo };
	template<>
	const STRUCT_INFO* GetStruct<SG_EQUIP_SLOTS>()
	{
		return &_rfl_struct_SG_EQUIP_SLOTS_info;
	}
}

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_GENERAL>(SG_GENERAL& Value)
	{
		if(!BufferReader::Read<A_LIVE_OBJECT>(Value)) return false;
		if(!Read<_U32>(Value.type)) return false;
		if(!Read<_U16>(Value.level)) return false;
		if(!Read<_U32>(Value.exp)) return false;
		if(!Read<SG_EQUIP_SLOTS>(Value.equip_slots)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<SG_GENERAL>(const SG_GENERAL& Value)
	{
		if(!BufferWriter::Write<A_LIVE_OBJECT>(Value)) return false;
		if(!Write<_U32>(Value.type)) return false;
		if(!Write<_U16>(Value.level)) return false;
		if(!Write<_U32>(Value.exp)) return false;
		if(!Write<SG_EQUIP_SLOTS>(Value.equip_slots)) return false;
		return true;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _struct_SG_GENERAL_fieldinfo[] =
	{
		{TYPE_U32, "type", 0, ATLAS_OFFSETOF(SG_GENERAL, type), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U32), NULL},
		{TYPE_U16, "level", 0, ATLAS_OFFSETOF(SG_GENERAL, level), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U16), NULL},
		{TYPE_U32, "exp", 0, ATLAS_OFFSETOF(SG_GENERAL, exp), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U32), NULL},
		{TYPE_STRUCT, "equip_slots", 0, ATLAS_OFFSETOF(SG_GENERAL, equip_slots), &_rfl_struct_SG_EQUIP_SLOTS_info, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(SG_EQUIP_SLOTS), NULL},
	};
	STRUCT_INFO _rfl_struct_SG_GENERAL_info = { &_rfl_struct_A_LIVE_OBJECT_info, "SG_GENERAL", sizeof(SG_GENERAL), 4, _struct_SG_GENERAL_fieldinfo };
	template<>
	const STRUCT_INFO* GetStruct<SG_GENERAL>()
	{
		return &_rfl_struct_SG_GENERAL_info;
	}
}

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_SOLDIER>(SG_SOLDIER& Value)
	{
		if(!BufferReader::Read<A_LIVE_OBJECT>(Value)) return false;
		if(!Read<_U32>(Value.type)) return false;
		if(!Read<_U16>(Value.level)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<SG_SOLDIER>(const SG_SOLDIER& Value)
	{
		if(!BufferWriter::Write<A_LIVE_OBJECT>(Value)) return false;
		if(!Write<_U32>(Value.type)) return false;
		if(!Write<_U16>(Value.level)) return false;
		return true;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _struct_SG_SOLDIER_fieldinfo[] =
	{
		{TYPE_U32, "type", 0, ATLAS_OFFSETOF(SG_SOLDIER, type), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U32), NULL},
		{TYPE_U16, "level", 0, ATLAS_OFFSETOF(SG_SOLDIER, level), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U16), NULL},
	};
	STRUCT_INFO _rfl_struct_SG_SOLDIER_info = { &_rfl_struct_A_LIVE_OBJECT_info, "SG_SOLDIER", sizeof(SG_SOLDIER), 2, _struct_SG_SOLDIER_fieldinfo };
	template<>
	const STRUCT_INFO* GetStruct<SG_SOLDIER>()
	{
		return &_rfl_struct_SG_SOLDIER_info;
	}
}

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_PLAYER>(SG_PLAYER& Value)
	{
		if(!BufferReader::Read<A_LIVE_OBJECT>(Value)) return false;
		if(!ReadString<SG_PLAYERNAME_LENMAX>(Value.nick)) return false;
		if(!Read<_U32>(Value.gold)) return false;
		if(!Read<_U32>(Value.rmb)) return false;
		if(!Read<_U8>(Value.sex)) return false;
		if(!ReadArray<_U16, 2>(Value.equip_generals)) return false;
		if(!ReadArray<_U16, 3>(Value.equip_soldiers)) return false;
		return true;
	}
	template<>
	bool BufferWriter::Write<SG_PLAYER>(const SG_PLAYER& Value)
	{
		if(!BufferWriter::Write<A_LIVE_OBJECT>(Value)) return false;
		if(!WriteString<SG_PLAYERNAME_LENMAX>(Value.nick)) return false;
		if(!Write<_U32>(Value.gold)) return false;
		if(!Write<_U32>(Value.rmb)) return false;
		if(!Write<_U8>(Value.sex)) return false;
		if(!WriteArray<_U16, 2>(Value.equip_generals)) return false;
		if(!WriteArray<_U16, 3>(Value.equip_soldiers)) return false;
		return true;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _struct_SG_PLAYER_fieldinfo[] =
	{
		{TYPE_STRING, "nick", 0, ATLAS_OFFSETOF(SG_PLAYER, nick), NULL, (_U16)SG_PLAYERNAME_LENMAX, (_U16)-1, (_U16)0, (_U16)sizeof(DDL::String<SG_PLAYERNAME_LENMAX>), NULL},
		{TYPE_U32, "gold", 0, ATLAS_OFFSETOF(SG_PLAYER, gold), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U32), NULL},
		{TYPE_U32, "rmb", 0, ATLAS_OFFSETOF(SG_PLAYER, rmb), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U32), NULL},
		{TYPE_U8, "sex", 0, ATLAS_OFFSETOF(SG_PLAYER, sex), NULL, (_U16)-1, (_U16)-1, (_U16)0, (_U16)sizeof(_U8), NULL},
		{TYPE_U16|TYPE_ARRAY, "equip_generals", 0, ATLAS_OFFSETOF(SG_PLAYER, equip_generals), NULL, (_U16)-1, (_U16)2, (_U16)((size_t)(&((DDL::Array<_U16, 2>*)NULL)->_Array)), (_U16)sizeof(_U16), NULL},
		{TYPE_U16|TYPE_ARRAY, "equip_soldiers", 0, ATLAS_OFFSETOF(SG_PLAYER, equip_soldiers), NULL, (_U16)-1, (_U16)3, (_U16)((size_t)(&((DDL::Array<_U16, 3>*)NULL)->_Array)), (_U16)sizeof(_U16), NULL},
	};
	STRUCT_INFO _rfl_struct_SG_PLAYER_info = { &_rfl_struct_A_LIVE_OBJECT_info, "SG_PLAYER", sizeof(SG_PLAYER), 6, _struct_SG_PLAYER_fieldinfo };
	template<>
	const STRUCT_INFO* GetStruct<SG_PLAYER>()
	{
		return &_rfl_struct_SG_PLAYER_info;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _class_SGGAME_C2S_fieldinfos[] = 
	{
		// 0 CreateAvatar
		{TYPE_S8|TYPE_ARRAY, "name", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 1 QueryAvatar
	};
	static FUNCTION_INFO _class_SGGAME_C2S_funcinfos[] = 
	{
		{"CreateAvatar", 1, _class_SGGAME_C2S_fieldinfos+0},
		{"QueryAvatar", 0, _class_SGGAME_C2S_fieldinfos+1},
	};
	static CLASS_INFO _class_SGGAME_C2S_info = { 0, "SGGAME_C2S", 2, _class_SGGAME_C2S_funcinfos };
	template<>
	const CLASS_INFO* GetClass<SGGAME_C2S>()
	{
		return &_class_SGGAME_C2S_info;
	}
}

namespace DDLReflect
{
	static FIELD_INFO _class_SGGAME_S2C_fieldinfos[] = 
	{
		// 0 CreatAvatarResult
		{TYPE_U32, "code", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
		// 1 QueryAvatarResult
		{TYPE_U32, "code", 0, 0, NULL, (_U16)-1,(_U16) -1, 0, 0, NULL},
	};
	static FUNCTION_INFO _class_SGGAME_S2C_funcinfos[] = 
	{
		{"CreatAvatarResult", 1, _class_SGGAME_S2C_fieldinfos+0},
		{"QueryAvatarResult", 1, _class_SGGAME_S2C_fieldinfos+1},
	};
	static CLASS_INFO _class_SGGAME_S2C_info = { 0, "SGGAME_S2C", 2, _class_SGGAME_S2C_funcinfos };
	template<>
	const CLASS_INFO* GetClass<SGGAME_S2C>()
	{
		return &_class_SGGAME_S2C_info;
	}
}

