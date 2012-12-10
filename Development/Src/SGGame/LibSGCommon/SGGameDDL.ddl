
include "../../LibCommon/CommonDDL.ddl"

const _U32 SG_PLAYERNAME_LENMAX	= 100;
const _U32 SG_ENEMYNAME_LENMAX = 100;
const _U32 SG_BAGITEM_MAX = 20;
const _U32 SG_GENERAL_MAX = 10;
const _U32 SG_SOLDIER_MAX = 10;

struct SG_PAWN_CONFIG : A_CONTENT_OBJECT
{
	_F32 BaseHP;
	_F32 SigmaHPAddition;
	_F32 SigmaHPPercent;
	_F32 SigmaExtraHP;
	_F32 BaseATK;
	_F32 SigmaATKAddition;
	_F32 SigmaATKPercent;
	_F32 SigmaExtraATK;
	_F32 BaseDEF;
	_F32 SigmaDEFAddition;
	_F32 SigmaDEFPercent;
	_F32 SigmaExtraDEF;
	_F32 BaseHIT;
	_F32 SigmaHITAddition;
	_F32 BaseEvasion;
	_F32 SigmaEvasionAddition;
	_F32 BaseCRIT;
	_F32 SigmaCRITAddition;
	_F32 BaseSP;
	_F32 SigmaSPAddition;
	_F32 BaseWS;
	_F32 SigmaWSAddition;
	_F32 SigmaWSPercent;
	_F32 SigmaExtraWS;
	_F32 BaseSTR;
	_F32 SigmaSTRAddition;
	_F32 SigmaSTRPercent;
	_F32 SigmaExtraSTR;
	_F32 BaseINT;
	_F32 SigmaINTAddition;
	_F32 SigmaINTPercent;
	_F32 SigmaExtraINT;
};
task[GEN_STRUCT_SERIALIZE(SG_PAWN_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_PAWN_CONFIG)];

struct SG_ENEMY_CONFIG : SG_PAWN_CONFIG
{
};
task[GEN_STRUCT_SERIALIZE(SG_ENEMY_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_ENEMY_CONFIG)];

struct SG_SOLDIER_CONFIG : SG_PAWN_CONFIG
{
	_U16 type;
	_U16 level;
};
task[GEN_STRUCT_SERIALIZE(SG_SOLDIER_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_SOLDIER_CONFIG)];

struct SG_GENERAL_CONFIG : SG_PAWN_CONFIG
{
	_U32 type;
	_U16 level;
	_U32 exp;
};
task[GEN_STRUCT_SERIALIZE(SG_GENERAL_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_GENERAL_CONFIG)];



struct SG_EQUIP_SLOTS
{
	A_UUID							head;
	A_UUID							hand;
};
task[GEN_STRUCT_SERIALIZE(SG_EQUIP_SLOTS)];
task[GEN_STRUCT_REFLECT(SG_EQUIP_SLOTS)];

struct SG_GENERAL : A_LIVE_OBJECT
{
	_U32							type;
	_U16							level;
	_U32							exp;
	SG_EQUIP_SLOTS equip_slots;
};
task[GEN_STRUCT_SERIALIZE(SG_GENERAL)];
task[GEN_STRUCT_REFLECT(SG_GENERAL)];

struct SG_SOLDIER : A_LIVE_OBJECT
{
	_U32							type;
	_U16							level;
};
task[GEN_STRUCT_SERIALIZE(SG_SOLDIER)];
task[GEN_STRUCT_REFLECT(SG_SOLDIER)];

struct SG_PLAYER : A_LIVE_OBJECT
{
	string<SG_PLAYERNAME_LENMAX>	nick;
	_U32							gold;
	_U32							rmb;
	_U8								sex;
	array<_U16, 2>					equip_generals;
	array<_U16, 3>					equip_soldiers;
};
task[GEN_STRUCT_SERIALIZE(SG_PLAYER)];
task[GEN_STRUCT_REFLECT(SG_PLAYER)];







class SGGAME_C2S
{
	CreateAvatar(_S8 name[SG_PLAYERNAME_LENMAX]);
	QueryAvatar();

	Ping();
};

class SGGAME_S2C
{
	CreatAvatarResult(_U32 code);
	QueryAvatarResult(_U32 code);

	Pong();
};

task[GEN_CLASS_STUB(SGGAME_C2S)];
task[GEN_CLASS_PROXY(SGGAME_C2S)];
task[GEN_CLASS_REFLECT(SGGAME_C2S)];
task[GEN_CLASS_STUB(SGGAME_S2C)];
task[GEN_CLASS_PROXY(SGGAME_S2C)];
task[GEN_CLASS_REFLECT(SGGAME_S2C)];
