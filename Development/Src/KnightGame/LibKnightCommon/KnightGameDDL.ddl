include "../../LibCommon/CommonDDL.ddl"

// AtlasDataTest
const _U32 DATATEST_RESOURCE_URL_MAX = 128;
const _U32 DATATEST_ARRAY_ELEMENT_MAX = 16;

struct DATATEST_INNER_OBJECT_CONFIG
{
	_U32									IntValue;	
	_F32									FloatValue;	
	string<DATATEST_RESOURCE_URL_MAX>		StrValue;	
};
task[GEN_STRUCT_SERIALIZE(DATATEST_INNER_OBJECT_CONFIG)];
task[GEN_STRUCT_REFLECT(DATATEST_INNER_OBJECT_CONFIG)];

struct DATATEST_INNER_STRUCT_CONFIG
{
	_U32									StructIntValue;
	array<_F32,DATATEST_ARRAY_ELEMENT_MAX>	StructArrFloatValue;
};
task[GEN_STRUCT_SERIALIZE(DATATEST_INNER_STRUCT_CONFIG)];
task[GEN_STRUCT_REFLECT(DATATEST_INNER_STRUCT_CONFIG)];

struct DATATEST_BASE_CONFIG : A_CONTENT_OBJECT
{
	string<DATATEST_RESOURCE_URL_MAX>		ResourceURL;	
	
	_U32									IntValue;	
	_F32									FloatValue;	
	_U8										EnumValue;	
	string<DATATEST_RESOURCE_URL_MAX>		StrValue;	
	DATATEST_INNER_OBJECT_CONFIG			ObjectInnerValue;	
	DATATEST_INNER_STRUCT_CONFIG			StructValue;	
	array<_U32,DATATEST_ARRAY_ELEMENT_MAX>	ArrIntValue;	
	array<DATATEST_INNER_STRUCT_CONFIG,DATATEST_ARRAY_ELEMENT_MAX>	ArrStructValue;	
	//array<DATATEST_INNER_OBJECT_CONFIG,1>	ArrObjectValue;	
};
task[GEN_STRUCT_SERIALIZE(DATATEST_BASE_CONFIG)];
task[GEN_STRUCT_REFLECT(DATATEST_BASE_CONFIG)];

//************************Lua common validate interface**************************//

const _U32	LUA_COMMON_PARAM_MAX = 16;
const _U8	LUA_COMMON_PARAM_TYPE_BOOL	= 0;
const _U8	LUA_COMMON_PARAM_TYPE_NUMBER = 1;
const _U8	LUA_COMMON_PARAM_TYPE_STRING = 2;
const _U8	LUA_COMMON_PARAM_TYPE_TABLE	= 3;

struct LUA_COMMON_VALIDATE_INTERFACE : A_CONTENT_OBJECT
{
	array<_U8,LUA_COMMON_PARAM_MAX>	ArrParams;
};
task[GEN_STRUCT_SERIALIZE(LUA_COMMON_VALIDATE_INTERFACE)];
task[GEN_STRUCT_REFLECT(LUA_COMMON_VALIDATE_INTERFACE)];

//************************KNIGHT**************************//
// Basic
const _U32 KNIGHT_DESCRIPTION_MAX = 256;
const _U32 KNIGHT_RESOURCE_URL_MAX = 128;
const _U32 KNIGHT_ARRAY_ELEMENT_MAX = 16;
const _U32 KNIGHT_ID_MAX = 16;

//**********************COMMON STRUCTS********************//
struct KNIGHT_ATTRIBUTE_MODIFY_SETTING
{
	_U8														AttributeType;
	_F32													Value;
};
task[GEN_STRUCT_SERIALIZE(KNIGHT_ATTRIBUTE_MODIFY_SETTING)];
task[GEN_STRUCT_REFLECT(KNIGHT_ATTRIBUTE_MODIFY_SETTING)];

//************************CALC**************************//   
struct KNIGHT_CALC_CONFIG : A_CONTENT_OBJECT
{
	_F32													Influence;
};
task[GEN_STRUCT_SERIALIZE(KNIGHT_CALC_CONFIG)];
task[GEN_STRUCT_REFLECT(KNIGHT_CALC_CONFIG)];

//************************SKILL**************************//                                                          
// Skill Base
struct KNIGHT_SKILL_CONFIG : A_CONTENT_OBJECT
{
	string<KNIGHT_RESOURCE_URL_MAX>							ResourceURL;
	string<KNIGHT_DESCRIPTION_MAX>							RealName;
	string<KNIGHT_DESCRIPTION_MAX>							Desc;
	_U32													Level;
	_U8														Target;
	array<string<KNIGHT_ID_MAX>,KNIGHT_ARRAY_ELEMENT_MAX>	BuffIDs;
};
task[GEN_STRUCT_SERIALIZE(KNIGHT_SKILL_CONFIG)];
task[GEN_STRUCT_REFLECT(KNIGHT_SKILL_CONFIG)];

//Skill Active
struct KNIGHT_SKILL_ACTIVE_CONFIG : KNIGHT_SKILL_CONFIG
{
	_U8														Element;
	_U8														FormulaType;
	_S32													AveValue;
	_S32													Range;
	_F32													FormulaRatio;
	_F32													Probability;
}
task[GEN_STRUCT_SERIALIZE(KNIGHT_SKILL_ACTIVE_CONFIG)];
task[GEN_STRUCT_REFLECT(KNIGHT_SKILL_ACTIVE_CONFIG)];

//************************BUFF**************************//
//Buff
struct KNIGHT_BUFF_CONFIG : A_CONTENT_OBJECT
{
	string<KNIGHT_RESOURCE_URL_MAX>							ResourceURL;
	string<KNIGHT_DESCRIPTION_MAX>							RealName;
	string<KNIGHT_DESCRIPTION_MAX>							Desc;
	_U32													Duration;
	_U8														IconType;					
}
task[GEN_STRUCT_SERIALIZE(KNIGHT_BUFF_CONFIG)];
task[GEN_STRUCT_REFLECT(KNIGHT_BUFF_CONFIG)];

//************************BUFF_STATS**************************//
struct KNIGHT_BUFF_STATS : KNIGHT_BUFF_CONFIG
{
	array<KNIGHT_ATTRIBUTE_MODIFY_SETTING,KNIGHT_ARRAY_ELEMENT_MAX>		AttrModSetting;
}
task[GEN_STRUCT_SERIALIZE(KNIGHT_BUFF_STATS)];
task[GEN_STRUCT_REFLECT(KNIGHT_BUFF_STATS)];

//************************BUFF_DAMAGE_CORRECTION**************************//
struct KNIGHT_ELEM_DAMAGE_CORRECTION
{
	_U8														ElementType;
	_F32													Ratio;
};
task[GEN_STRUCT_SERIALIZE(KNIGHT_ELEM_DAMAGE_CORRECTION)];
task[GEN_STRUCT_REFLECT(KNIGHT_ELEM_DAMAGE_CORRECTION)];

struct KNIGHT_WEAPON_CORRECTION
{
	_U8														WeaponType;
	_F32													Ratio;
};
task[GEN_STRUCT_SERIALIZE(KNIGHT_WEAPON_CORRECTION)];
task[GEN_STRUCT_REFLECT(KNIGHT_WEAPON_CORRECTION)];

struct KNIGHT_BUFF_DAMAGE_CORRECTION : KNIGHT_BUFF_CONFIG
{
	_U8																	BuffAtkElemType;
	array<KNIGHT_WEAPON_CORRECTION,KNIGHT_ARRAY_ELEMENT_MAX>			BuffWeaponAtkCorrection;
	array<KNIGHT_WEAPON_CORRECTION,KNIGHT_ARRAY_ELEMENT_MAX>			BuffWeaponDefCorrection;
}
task[GEN_STRUCT_SERIALIZE(KNIGHT_BUFF_DAMAGE_CORRECTION)];
task[GEN_STRUCT_REFLECT(KNIGHT_BUFF_DAMAGE_CORRECTION)];

//************************BUFF_MAGIC_CORRECTION**************************//
struct KNIGHT_BUFF_MAGIC_CORRECTION : KNIGHT_BUFF_CONFIG
{
	array<_U8,KNIGHT_ARRAY_ELEMENT_MAX>									BuffElemType;
	_U8																	BuffElemDefMode;
	_U8																	BuffElemSeal;
	_F32																BuffElemAtkEVT;
	_F32																BuffElemDefEVT;
	_F32																BuffElemMiss;
}
task[GEN_STRUCT_SERIALIZE(KNIGHT_BUFF_MAGIC_CORRECTION)];
task[GEN_STRUCT_REFLECT(KNIGHT_BUFF_MAGIC_CORRECTION)];

//************************BUFF_BUFF_CORRECTION**************************//
struct KNIGHT_BUFF_BUFF_CORRECTION : KNIGHT_BUFF_CONFIG
{
	array<string<KNIGHT_ID_MAX>,KNIGHT_ARRAY_ELEMENT_MAX>				IDs;
	_U8																	Type;
	_F32																MissRate;
}
task[GEN_STRUCT_SERIALIZE(KNIGHT_BUFF_BUFF_CORRECTION)];
task[GEN_STRUCT_REFLECT(KNIGHT_BUFF_BUFF_CORRECTION)];

//************************BUFF_DOT**************************//
struct KNIGHT_BUFF_DOT : KNIGHT_BUFF_CONFIG
{
	_U8																	Type;
	_S32																AveValue;
	_S32																Range;
	_F32																Ratio;
}
task[GEN_STRUCT_SERIALIZE(KNIGHT_BUFF_DOT)];
task[GEN_STRUCT_REFLECT(KNIGHT_BUFF_DOT)];

//************************BUFF_ATTACK_SLICES_CORRECTION******************//
struct KNIGHT_BUFF_ATTACK_SLICES_CORRECTION : KNIGHT_BUFF_CONFIG
{
	_U8														AttackType;
	_S32													HitScopeBias;
	_S32													HitTimingBias;
}
task[GEN_STRUCT_SERIALIZE(KNIGHT_BUFF_ATTACK_SLICES_CORRECTION)];
task[GEN_STRUCT_REFLECT(KNIGHT_BUFF_ATTACK_SLICES_CORRECTION)];

//************************MANTRA******************//
struct KNIGHT_MANTRA_CONFIG : A_CONTENT_OBJECT
{
	string<KNIGHT_DESCRIPTION_MAX>							RealName;
	string<KNIGHT_DESCRIPTION_MAX>							Condition;
	_S32													Cost;
}
task[GEN_STRUCT_SERIALIZE(KNIGHT_MANTRA_CONFIG)];
task[GEN_STRUCT_REFLECT(KNIGHT_MANTRA_CONFIG)];

//************************ITEM**************************//    
//Item
struct KNIGHT_ITEM_CONFIG : A_CONTENT_OBJECT
{
	string<KNIGHT_RESOURCE_URL_MAX>							ResourceURL;
	string<KNIGHT_RESOURCE_URL_MAX>							ResourceUIURL;
};
task[GEN_STRUCT_SERIALIZE(KNIGHT_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(KNIGHT_ITEM_CONFIG)];

//Equipment
struct KNIGHT_EQUIPMENT_CONFIG : KNIGHT_ITEM_CONFIG
{
	string<KNIGHT_DESCRIPTION_MAX>		RealName;
	string<KNIGHT_DESCRIPTION_MAX>		Desc;
	_U32								Level;
	_U32								Rank;
	_U32								MantraSize;
	_U8									Type;

	array<KNIGHT_ATTRIBUTE_MODIFY_SETTING,KNIGHT_ARRAY_ELEMENT_MAX>	InitialValue;
	array<KNIGHT_ATTRIBUTE_MODIFY_SETTING,KNIGHT_ARRAY_ELEMENT_MAX>	GrowthValue;

	array<string<KNIGHT_ID_MAX>,KNIGHT_ARRAY_ELEMENT_MAX>	SkillIDs;
	array<string<KNIGHT_ID_MAX>,KNIGHT_ARRAY_ELEMENT_MAX>	BuffIDs;
};
task[GEN_STRUCT_SERIALIZE(KNIGHT_EQUIPMENT_CONFIG)];
task[GEN_STRUCT_REFLECT(KNIGHT_EQUIPMENT_CONFIG)];

//Armor
struct KNIGHT_ARMOR_CONFIG : KNIGHT_EQUIPMENT_CONFIG
{
};
task[GEN_STRUCT_SERIALIZE(KNIGHT_ARMOR_CONFIG)];
task[GEN_STRUCT_REFLECT(KNIGHT_ARMOR_CONFIG)];

//Helmet
struct KNIGHT_HELMET_CONFIG : KNIGHT_EQUIPMENT_CONFIG
{
};
task[GEN_STRUCT_SERIALIZE(KNIGHT_HELMET_CONFIG)];
task[GEN_STRUCT_REFLECT(KNIGHT_HELMET_CONFIG)];

//Shield
struct KNIGHT_SHIELD_CONFIG : KNIGHT_EQUIPMENT_CONFIG
{
};
task[GEN_STRUCT_SERIALIZE(KNIGHT_SHIELD_CONFIG)];
task[GEN_STRUCT_REFLECT(KNIGHT_SHIELD_CONFIG)];

//Weapon
struct KNIGHT_WEAPON_CONFIG : KNIGHT_EQUIPMENT_CONFIG
{
};
task[GEN_STRUCT_SERIALIZE(KNIGHT_WEAPON_CONFIG)];
task[GEN_STRUCT_REFLECT(KNIGHT_WEAPON_CONFIG)];

//Mount
struct KNIGHT_MOUNT_CONFIG : KNIGHT_ITEM_CONFIG
{
	string<KNIGHT_DESCRIPTION_MAX>		RealName;
	string<KNIGHT_DESCRIPTION_MAX>		Desc;
};
task[GEN_STRUCT_SERIALIZE(KNIGHT_MOUNT_CONFIG)];
task[GEN_STRUCT_REFLECT(KNIGHT_MOUNT_CONFIG)];

//************************ATTACK**************************//    
//Attack
struct KNIGHT_ATTACK_PHASE
{
	_U8			Type;
	_U8			NumOfSlice;
};
task[GEN_STRUCT_SERIALIZE(KNIGHT_ATTACK_PHASE)];
task[GEN_STRUCT_REFLECT(KNIGHT_ATTACK_PHASE)];

struct KNIGHT_ATTACK_CONFIG : A_CONTENT_OBJECT
{
	string<KNIGHT_RESOURCE_URL_MAX>							ResourceURL;

	array<KNIGHT_ATTACK_PHASE,KNIGHT_ARRAY_ELEMENT_MAX>		AttackPhases;
	array<_U8,KNIGHT_ARRAY_ELEMENT_MAX>						CriticalStrikes;
	_F32													Efficacy;
	_U8														AppropriateWeapon;
	_U8														bNoAction;
};
task[GEN_STRUCT_SERIALIZE(KNIGHT_ATTACK_CONFIG)];
task[GEN_STRUCT_REFLECT(KNIGHT_ATTACK_CONFIG)];

//*********************************************************//    

class KNIGHT_C2S
{
	Ping();

	New(string value);
	Set(string value);
	Get();

	Boardcast(string msg);
};

class KNIGHT_S2C
{
	Pong();

	GetCallback(_U32 aid, string value);
	BoardcastCallback(_U32 uid, string msg);
};

task[GEN_CLASS_STUB(KNIGHT_C2S)];
task[GEN_CLASS_PROXY(KNIGHT_C2S)];
task[GEN_CLASS_REFLECT(KNIGHT_C2S)];
task[GEN_CLASS_STUB(KNIGHT_S2C)];
task[GEN_CLASS_PROXY(KNIGHT_S2C)];
task[GEN_CLASS_REFLECT(KNIGHT_S2C)];
