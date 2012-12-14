////////////////////////////////////////////////
// THIS FILE IS AUTOGENERATED. DO NOT MODIFY! //
////////////////////////////////////////////////

#ifndef __SGGAMEDDL_DDL_CODEGEN__
#define __SGGAMEDDL_DDL_CODEGEN__

#include <AtlasDefines.h>
#include <AtlasUUID.h>
#include <DDL.h>
#include <DDLProxy.h>
#include <DDLStub.h>
#include <DDLReflect.h>

#pragma pack(push)
#pragma pack(4)

#include "../../LibCommon/CommonDDL.h"

const _U32 SG_PLAYERNAME_LENMAX = 100;

const _U32 SG_ENEMYNAME_LENMAX = 100;

const _U32 SG_BAGITEM_MAX = 20;

const _U32 SG_GENERAL_MAX = 10;

const _U32 SG_SOLDIER_MAX = 10;

const _U32 ARCHETYPE_URL_LENGTH_MAX = 128;

const _U32 SG_DESCRIPTION_MAX = 512;

struct SG_ATTR_MOD_CONFIG
{
	_F32 MOD_HPAddition;
	_F32 MOD_HPPercent;
	_F32 MOD_ExtraHP;
	_F32 MOD_ATKAddition;
	_F32 MOD_ATKPercent;
	_F32 MOD_ExtraATK;
	_F32 MOD_DEFAddition;
	_F32 MOD_DEFPercent;
	_F32 MOD_ExtraDEF;
	_F32 MOD_HITAddition;
	_F32 MOD_EvasionAddition;
	_F32 MOD_CRITAddition;
	_F32 MOD_SPAddition;
	_F32 MOD_WSAddition;
	_F32 MOD_WSPercent;
	_F32 MOD_ExtraWS;
	_F32 MOD_STRAddition;
	_F32 MOD_STRPercent;
	_F32 MOD_ExtraSTR;
	_F32 MOD_INTAddition;
	_F32 MOD_INTPercent;
	_F32 MOD_ExtraINT;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_ATTR_MOD_CONFIG>(SG_ATTR_MOD_CONFIG& Value);
	template<>
	bool BufferWriter::Write<SG_ATTR_MOD_CONFIG>(const SG_ATTR_MOD_CONFIG& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_ATTR_MOD_CONFIG>();
	extern STRUCT_INFO _rfl_struct_SG_ATTR_MOD_CONFIG_info;
}

struct SG_ATTR_BASE_CONFIG
{
	_F32 Base_HP;
	_F32 Base_ATK;
	_F32 Base_DEF;
	_F32 Base_HIT;
	_F32 Base_Evasion;
	_F32 Base_CRIT;
	_F32 Base_SP;
	_F32 Base_WS;
	_F32 Base_STR;
	_F32 Base_INT;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_ATTR_BASE_CONFIG>(SG_ATTR_BASE_CONFIG& Value);
	template<>
	bool BufferWriter::Write<SG_ATTR_BASE_CONFIG>(const SG_ATTR_BASE_CONFIG& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_ATTR_BASE_CONFIG>();
	extern STRUCT_INFO _rfl_struct_SG_ATTR_BASE_CONFIG_info;
}

struct SG_PAWN_CONFIG
{
	SG_ATTR_BASE_CONFIG BaseConfig;
	SG_ATTR_MOD_CONFIG SigmaConfig;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_PAWN_CONFIG>(SG_PAWN_CONFIG& Value);
	template<>
	bool BufferWriter::Write<SG_PAWN_CONFIG>(const SG_PAWN_CONFIG& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_PAWN_CONFIG>();
	extern STRUCT_INFO _rfl_struct_SG_PAWN_CONFIG_info;
}

struct SG_ITEM_CONFIG : A_CONTENT_OBJECT
{
	_U32 item_id;
	_U32 stack_max;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_ITEM_CONFIG>(SG_ITEM_CONFIG& Value);
	template<>
	bool BufferWriter::Write<SG_ITEM_CONFIG>(const SG_ITEM_CONFIG& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_ITEM_CONFIG>();
	extern STRUCT_INFO _rfl_struct_SG_ITEM_CONFIG_info;
}

struct SG_EQUIPT_ITEM_CONFIG : SG_ITEM_CONFIG
{
	SG_ATTR_MOD_CONFIG mod_config;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_EQUIPT_ITEM_CONFIG>(SG_EQUIPT_ITEM_CONFIG& Value);
	template<>
	bool BufferWriter::Write<SG_EQUIPT_ITEM_CONFIG>(const SG_EQUIPT_ITEM_CONFIG& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_EQUIPT_ITEM_CONFIG>();
	extern STRUCT_INFO _rfl_struct_SG_EQUIPT_ITEM_CONFIG_info;
}

struct SG_USABLE_ITEM_CONFIG : SG_ITEM_CONFIG
{
	DDL::String<100> functional;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_USABLE_ITEM_CONFIG>(SG_USABLE_ITEM_CONFIG& Value);
	template<>
	bool BufferWriter::Write<SG_USABLE_ITEM_CONFIG>(const SG_USABLE_ITEM_CONFIG& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_USABLE_ITEM_CONFIG>();
	extern STRUCT_INFO _rfl_struct_SG_USABLE_ITEM_CONFIG_info;
}

struct SG_GEM_ITEM_CONFIG : SG_ITEM_CONFIG
{
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_GEM_ITEM_CONFIG>(SG_GEM_ITEM_CONFIG& Value);
	template<>
	bool BufferWriter::Write<SG_GEM_ITEM_CONFIG>(const SG_GEM_ITEM_CONFIG& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_GEM_ITEM_CONFIG>();
	extern STRUCT_INFO _rfl_struct_SG_GEM_ITEM_CONFIG_info;
}

struct SG_ENEMY_CONFIG : A_CONTENT_OBJECT
{
	SG_PAWN_CONFIG PawnConfig;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_ENEMY_CONFIG>(SG_ENEMY_CONFIG& Value);
	template<>
	bool BufferWriter::Write<SG_ENEMY_CONFIG>(const SG_ENEMY_CONFIG& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_ENEMY_CONFIG>();
	extern STRUCT_INFO _rfl_struct_SG_ENEMY_CONFIG_info;
}

struct SG_SOLDIER_LEVEL_CONFIG : A_CONTENT_OBJECT
{
	_U32 attr_id;
	_U32 level;
	SG_PAWN_CONFIG PawnConfig;
	_U32 money;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_SOLDIER_LEVEL_CONFIG>(SG_SOLDIER_LEVEL_CONFIG& Value);
	template<>
	bool BufferWriter::Write<SG_SOLDIER_LEVEL_CONFIG>(const SG_SOLDIER_LEVEL_CONFIG& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_SOLDIER_LEVEL_CONFIG>();
	extern STRUCT_INFO _rfl_struct_SG_SOLDIER_LEVEL_CONFIG_info;
}

struct SG_GENERAL_LEVEL_CONFIG : A_CONTENT_OBJECT
{
	_U32 attr_id;
	_U32 level;
	SG_PAWN_CONFIG PawnConfig;
	_U32 exp;
	_S32 levelup_money;
	_U32 levelup_XP;
	_U16 mod_scale;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_GENERAL_LEVEL_CONFIG>(SG_GENERAL_LEVEL_CONFIG& Value);
	template<>
	bool BufferWriter::Write<SG_GENERAL_LEVEL_CONFIG>(const SG_GENERAL_LEVEL_CONFIG& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_GENERAL_LEVEL_CONFIG>();
	extern STRUCT_INFO _rfl_struct_SG_GENERAL_LEVEL_CONFIG_info;
}

struct SG_SOLDIER_CONFIG : A_CONTENT_OBJECT
{
	_U32 soldier_id;
	_U32 attr_id;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_SOLDIER_CONFIG>(SG_SOLDIER_CONFIG& Value);
	template<>
	bool BufferWriter::Write<SG_SOLDIER_CONFIG>(const SG_SOLDIER_CONFIG& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_SOLDIER_CONFIG>();
	extern STRUCT_INFO _rfl_struct_SG_SOLDIER_CONFIG_info;
}

struct SG_GENERAL_CONFIG : A_CONTENT_OBJECT
{
	_U32 general_id;
	_U32 attr_id;
	_U8 type;
	_S8 classes;
	_U16 atk_type;
	_U16 def_type;
	_S32 gift;
	_S32 com_atk_skill;
	_S32 sp_skill;
	_U32 atk_CD;
	DDL::String<SG_DESCRIPTION_MAX> description;
	_U32 space;
	_S32 req_title;
	_S32 req_money;
	_S32 unlock_level;
	_S32 pre_general_id;
	_S32 pre_level;
	_S32 product_consume;
	_S32 product_time;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_GENERAL_CONFIG>(SG_GENERAL_CONFIG& Value);
	template<>
	bool BufferWriter::Write<SG_GENERAL_CONFIG>(const SG_GENERAL_CONFIG& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_GENERAL_CONFIG>();
	extern STRUCT_INFO _rfl_struct_SG_GENERAL_CONFIG_info;
}

struct SG_EQUIPPED_UNIT_PVE
{
	DDL::String<ARCHETYPE_URL_LENGTH_MAX> ArchetypeURL;
	SG_PAWN_CONFIG PawnConfig;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_EQUIPPED_UNIT_PVE>(SG_EQUIPPED_UNIT_PVE& Value);
	template<>
	bool BufferWriter::Write<SG_EQUIPPED_UNIT_PVE>(const SG_EQUIPPED_UNIT_PVE& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_EQUIPPED_UNIT_PVE>();
	extern STRUCT_INFO _rfl_struct_SG_EQUIPPED_UNIT_PVE_info;
}

struct SG_PLAYER_PVE
{
	DDL::String<ARCHETYPE_URL_LENGTH_MAX> ArchetypeURL;
	SG_PAWN_CONFIG PawnConfig;
	DDL::Array<SG_EQUIPPED_UNIT_PVE, 2> EquippedGenerals;
	DDL::Array<SG_EQUIPPED_UNIT_PVE, 3> EquippedSoldiers;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_PLAYER_PVE>(SG_PLAYER_PVE& Value);
	template<>
	bool BufferWriter::Write<SG_PLAYER_PVE>(const SG_PLAYER_PVE& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_PLAYER_PVE>();
	extern STRUCT_INFO _rfl_struct_SG_PLAYER_PVE_info;
}

struct SG_EQUIP_SLOTS
{
	A_UUID head;
	A_UUID hand;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_EQUIP_SLOTS>(SG_EQUIP_SLOTS& Value);
	template<>
	bool BufferWriter::Write<SG_EQUIP_SLOTS>(const SG_EQUIP_SLOTS& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_EQUIP_SLOTS>();
	extern STRUCT_INFO _rfl_struct_SG_EQUIP_SLOTS_info;
}

struct SG_GENERAL : A_LIVE_OBJECT
{
	_U32 type;
	_U16 level;
	_U32 exp;
	SG_EQUIP_SLOTS equip_slots;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_GENERAL>(SG_GENERAL& Value);
	template<>
	bool BufferWriter::Write<SG_GENERAL>(const SG_GENERAL& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_GENERAL>();
	extern STRUCT_INFO _rfl_struct_SG_GENERAL_info;
}

struct SG_SOLDIER : A_LIVE_OBJECT
{
	_U32 type;
	_U16 level;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_SOLDIER>(SG_SOLDIER& Value);
	template<>
	bool BufferWriter::Write<SG_SOLDIER>(const SG_SOLDIER& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_SOLDIER>();
	extern STRUCT_INFO _rfl_struct_SG_SOLDIER_info;
}

struct SG_PLAYER : A_LIVE_OBJECT
{
	DDL::String<SG_PLAYERNAME_LENMAX> nick;
	_U32 gold;
	_U32 rmb;
	_U8 sex;
	DDL::Array<_U16, 2> equip_generals;
	DDL::Array<_U16, 3> equip_soldiers;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_PLAYER>(SG_PLAYER& Value);
	template<>
	bool BufferWriter::Write<SG_PLAYER>(const SG_PLAYER& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_PLAYER>();
	extern STRUCT_INFO _rfl_struct_SG_PLAYER_info;
}

struct SG_ITEM : A_LIVE_OBJECT
{
	_U32 item_id;
	_U32 count;
	_U32 expired_time;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_ITEM>(SG_ITEM& Value);
	template<>
	bool BufferWriter::Write<SG_ITEM>(const SG_ITEM& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_ITEM>();
	extern STRUCT_INFO _rfl_struct_SG_ITEM_info;
}

struct SG_EQUIPT_ITEM : SG_ITEM
{
	SG_ATTR_MOD_CONFIG mod_config;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_EQUIPT_ITEM>(SG_EQUIPT_ITEM& Value);
	template<>
	bool BufferWriter::Write<SG_EQUIPT_ITEM>(const SG_EQUIPT_ITEM& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_EQUIPT_ITEM>();
	extern STRUCT_INFO _rfl_struct_SG_EQUIPT_ITEM_info;
}

struct SG_USABLE_ITEM : SG_ITEM
{
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_USABLE_ITEM>(SG_USABLE_ITEM& Value);
	template<>
	bool BufferWriter::Write<SG_USABLE_ITEM>(const SG_USABLE_ITEM& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_USABLE_ITEM>();
	extern STRUCT_INFO _rfl_struct_SG_USABLE_ITEM_info;
}

struct SG_GEM_ITEM : SG_ITEM
{
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_GEM_ITEM>(SG_GEM_ITEM& Value);
	template<>
	bool BufferWriter::Write<SG_GEM_ITEM>(const SG_GEM_ITEM& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_GEM_ITEM>();
	extern STRUCT_INFO _rfl_struct_SG_GEM_ITEM_info;
}

class SGGAME_C2S;

class SGGAME_S2C;

namespace DDLStub
{

	template<typename CALLER, typename CLASS>
	class SGGAME_C2S : public DDLStub<CALLER, CLASS>
	{
	public:
		SGGAME_C2S(CLASS* Class) : DDLStub<CALLER, CLASS>(Class)
		{
		}

		virtual bool Dispatcher(CALLER* Caller, _U16 fid, DDL::BufferReader& Buf)
		{
			if(fid==0)
			{
				_U16 __length;
				_S8* _prefix_name;

				// <_S8> <name> <> <SG_PLAYERNAME_LENMAX>;
				if(!Buf.Read(__length)) return false;
				_prefix_name = (_S8*)alloca(sizeof(_prefix_name[0])*__length);
				if(!_prefix_name) return false;
				if(!Buf.ReadPointer(_prefix_name, __length)) return false;

				// call implement
				DDLStub<CALLER, CLASS>::GetClass()->CreateAvatar(Caller, _prefix_name);
				return true;
			}
			if(fid==1)
			{


				// call implement
				DDLStub<CALLER, CLASS>::GetClass()->QueryAvatar(Caller);
				return true;
			}
			if(fid==2)
			{


				// call implement
				DDLStub<CALLER, CLASS>::GetClass()->Ping(Caller);
				return true;
			}
			return false;
		}
	};

}

namespace DDLProxy
{

	template<typename CLIENT, typename BUFFER>
	class SGGAME_C2S : public DDLProxy<CLIENT, BUFFER>
	{
	public:
		SGGAME_C2S(CLIENT* Client) : DDLProxy<CLIENT, BUFFER>(Client, DDLReflect::GetClassID<typename ::SGGAME_C2S>())
		{
		}

		static SGGAME_C2S<CLIENT, BUFFER> Get(CLIENT* Client)
		{
			SGGAME_C2S<CLIENT, BUFFER> Proxy(Client);
			return Proxy;
		}

		bool CreateAvatar(_S8* name)
		{
			BUFFER Buf;
			_U16 __length;
			// <_S8> <name> <> <SG_PLAYERNAME_LENMAX>
			__length = (_U16)(SG_PLAYERNAME_LENMAX);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WritePointer(name, __length)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 0, Buf);
		}

		bool QueryAvatar()
		{
			BUFFER Buf;

			// send
			return this->GetClient()->Send(this->GetClassID(), 1, Buf);
		}

		bool Ping()
		{
			BUFFER Buf;

			// send
			return this->GetClient()->Send(this->GetClassID(), 2, Buf);
		}
	};

}

namespace DDLReflect
{
	template<>
	const CLASS_INFO* GetClass<SGGAME_C2S>();
}

namespace DDLStub
{

	template<typename CALLER, typename CLASS>
	class SGGAME_S2C : public DDLStub<CALLER, CLASS>
	{
	public:
		SGGAME_S2C(CLASS* Class) : DDLStub<CALLER, CLASS>(Class)
		{
		}

		virtual bool Dispatcher(CALLER* Caller, _U16 fid, DDL::BufferReader& Buf)
		{
			if(fid==0)
			{
				_U32 _prefix_code;

				// <_U32> <code> <> <>;
				if(!Buf.Read(_prefix_code)) return false;

				// call implement
				DDLStub<CALLER, CLASS>::GetClass()->CreatAvatarResult(Caller, _prefix_code);
				return true;
			}
			if(fid==1)
			{
				_U32 _prefix_code;

				// <_U32> <code> <> <>;
				if(!Buf.Read(_prefix_code)) return false;

				// call implement
				DDLStub<CALLER, CLASS>::GetClass()->QueryAvatarResult(Caller, _prefix_code);
				return true;
			}
			if(fid==2)
			{


				// call implement
				DDLStub<CALLER, CLASS>::GetClass()->Pong(Caller);
				return true;
			}
			return false;
		}
	};

}

namespace DDLProxy
{

	template<typename CLIENT, typename BUFFER>
	class SGGAME_S2C : public DDLProxy<CLIENT, BUFFER>
	{
	public:
		SGGAME_S2C(CLIENT* Client) : DDLProxy<CLIENT, BUFFER>(Client, DDLReflect::GetClassID<typename ::SGGAME_S2C>())
		{
		}

		static SGGAME_S2C<CLIENT, BUFFER> Get(CLIENT* Client)
		{
			SGGAME_S2C<CLIENT, BUFFER> Proxy(Client);
			return Proxy;
		}

		bool CreatAvatarResult(_U32 code)
		{
			BUFFER Buf;
			// <_U32> <code> <> <>
			if(!Buf.Write(code)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 0, Buf);
		}

		bool QueryAvatarResult(_U32 code)
		{
			BUFFER Buf;
			// <_U32> <code> <> <>
			if(!Buf.Write(code)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 1, Buf);
		}

		bool Pong()
		{
			BUFFER Buf;

			// send
			return this->GetClient()->Send(this->GetClassID(), 2, Buf);
		}
	};

}

namespace DDLReflect
{
	template<>
	const CLASS_INFO* GetClass<SGGAME_S2C>();
}


#pragma pack(pop)

#endif
