
include "../../LibCommon/CommonDDL.ddl"

const _U32 SG_PLAYERNAME_LENMAX	= 100;
const _U32 SG_ENEMYNAME_LENMAX = 100;
const _U32 SG_BAGITEM_MAX = 20;
const _U32 SG_GENERAL_MAX = 10;
const _U32 SG_SOLDIER_MAX = 10;
const _U32 ARCHETYPE_URL_LENGTH_MAX = 128;
const _U32 SG_DESCRIPTION_MAX = 512;

struct SG_ATTR_MOD_CONFIG
{
	_F32								MOD_HPAddition;
	_F32								MOD_HPPercent;
	_F32								MOD_ExtraHP;
	_F32								MOD_ATKAddition;
	_F32								MOD_ATKPercent;
	_F32								MOD_ExtraATK;
	_F32								MOD_DEFAddition;
	_F32								MOD_DEFPercent;
	_F32								MOD_ExtraDEF;
	_F32								MOD_HITAddition;
	_F32								MOD_EvasionAddition;
	_F32								MOD_CRITAddition;
	_F32								MOD_SPAddition;
	_F32								MOD_WSAddition;
	_F32								MOD_WSPercent;
	_F32								MOD_ExtraWS;
	_F32								MOD_STRAddition;
	_F32								MOD_STRPercent;
	_F32								MOD_ExtraSTR;
	_F32								MOD_INTAddition;
	_F32								MOD_INTPercent;
	_F32								MOD_ExtraINT;
};
task[GEN_STRUCT_SERIALIZE(SG_ATTR_MOD_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_ATTR_MOD_CONFIG)];

struct SG_ATTR_BASE_CONFIG
{
	_F32								Base_HP;
	_F32								Base_ATK;
	_F32								Base_DEF;
	_F32								Base_HIT;
	_F32								Base_Evasion;
	_F32								Base_CRIT;			//����
	_F32								Base_SP;			//ʿ������
	_F32								Base_WS;			//�ƶ��ٶ�
	_F32								Base_STR;			//����			
	_F32								Base_INT;			//����
};
task[GEN_STRUCT_SERIALIZE(SG_ATTR_BASE_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_ATTR_BASE_CONFIG)];

struct SG_PAWN_CONFIG
{
	SG_ATTR_BASE_CONFIG					BaseConfig;
	SG_ATTR_MOD_CONFIG					SigmaConfig;		
};
task[GEN_STRUCT_SERIALIZE(SG_PAWN_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_PAWN_CONFIG)];

struct SG_ITEM_CONFIG : A_CONTENT_OBJECT
{
	_U32								item_id;			//��Ʒid
	_U32								stack_max;			//���������
};
task[GEN_STRUCT_SERIALIZE(SG_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_ITEM_CONFIG)];

struct SG_EQUIPT_ITEM_CONFIG : SG_ITEM_CONFIG				//װ��
{
	SG_ATTR_MOD_CONFIG					mod_config;
};
task[GEN_STRUCT_SERIALIZE(SG_EQUIPT_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_EQUIPT_ITEM_CONFIG)];

struct SG_USABLE_ITEM_CONFIG : SG_ITEM_CONFIG				//��ʹ��
{
	string<100>							functional;
};
task[GEN_STRUCT_SERIALIZE(SG_USABLE_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_USABLE_ITEM_CONFIG)];

struct SG_GEM_ITEM_CONFIG : SG_ITEM_CONFIG					//��ʯ
{
};
task[GEN_STRUCT_SERIALIZE(SG_GEM_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_GEM_ITEM_CONFIG)];

struct SG_ENEMY_CONFIG : A_CONTENT_OBJECT
{
	// string name;
	_U32								enemy_id;			//����id
	_U32								attr_id;			//����id
	SG_PAWN_CONFIG						PawnConfig;

	_U8									type;				//Ӣ������
	_S8									classes;			//�׼�
	_U16								atk_type;			//��������
	_U16								def_type;			//��������
	_S32								gift;				//�츳
	_S32								com_atk_skill;		//��ͨ��������
	_S32								sp_skill;			//���⼼�� 
	_U32								atk_CD;				//�������ʱ��
	string<SG_DESCRIPTION_MAX>			description;		//����
	_S32								req_gold;			//��Ҫ�Ľ��
	_S32								req_rmb;			//��Ҫ�������
	_S32								unlock_level;		//�����ȼ�
	_S32								pre_general_id;		//ǰ�ñ���
	_S32								pre_level;			//ǰ�õȼ�
	_S32								product_consume;	//��������
	_S32								product_time;		//����CDʱ��
	string<ARCHETYPE_URL_LENGTH_MAX>    archetype;			//archetype
};
task[GEN_STRUCT_SERIALIZE(SG_ENEMY_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_ENEMY_CONFIG)];

struct SG_SOLDIER_LEVEL_CONFIG : A_CONTENT_OBJECT
{
	_U32								attr_id;
	_U32								level;
	SG_PAWN_CONFIG						PawnConfig;
	_U32								exp;
	_S32								levelup_gold;		//���������Ǯ
	_S32								levelup_rmb;		//���������Ǯ
	_S32								levelup_XP;			//�������辭��
	_U16								mod_scale;			//ģ������			
};
task[GEN_STRUCT_SERIALIZE(SG_SOLDIER_LEVEL_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_SOLDIER_LEVEL_CONFIG)];

struct SG_GENERAL_LEVEL_CONFIG : A_CONTENT_OBJECT
{
	_U32								attr_id;
	_U32								level;
	SG_PAWN_CONFIG						PawnConfig;
	_U32								exp;
	_S32								levelup_gold;		//���������Ǯ
	_S32								levelup_rmb;		//���������Ǯ
	_S32								levelup_XP;			//�������辭��
	_U16								mod_scale;			//ģ������
};

task[GEN_STRUCT_SERIALIZE(SG_GENERAL_LEVEL_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_GENERAL_LEVEL_CONFIG)];

struct SG_SOLDIER_CONFIG : A_CONTENT_OBJECT
{
	_U32								soldier_id;
	_U32								attr_id;

	_U8									type;				//Ӣ������
	_S8									classes;			//�׼�
	_U16								atk_type;			//��������
	_U16								def_type;			//��������
	_S32								gift;				//�츳
	_S32								com_atk_skill;		//��ͨ��������
	_S32								sp_skill;			//���⼼�� 
	_U32								atk_CD;				//�������ʱ��
	string<SG_DESCRIPTION_MAX>			description;		//����
	_S32								req_gold;			//��Ҫ�Ľ��
	_S32								req_rmb;			//��Ҫ�������
	_S32								unlock_level;		//�����ȼ�
	_S32								pre_general_id;		//ǰ�ñ���
	_S32								pre_level;			//ǰ�õȼ�
	_S32								product_consume;	//��������
	_S32								product_time;		//����CDʱ��
	string<ARCHETYPE_URL_LENGTH_MAX>    archetype;			//archetype
};
task[GEN_STRUCT_SERIALIZE(SG_SOLDIER_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_SOLDIER_CONFIG)];

struct SG_GENERAL_CONFIG : A_CONTENT_OBJECT
{
	_U32								general_id;
	_U32								attr_id;

	_U8									type;				//Ӣ������
	_S8									classes;			//�׼�
	_U16								atk_type;			//��������
	_U16								def_type;			//��������
	_S32								gift;				//�츳
	_S32								com_atk_skill;		//��ͨ��������
	_S32								sp_skill;			//���⼼�� 
	_U32								atk_CD;				//�������ʱ��
	string<SG_DESCRIPTION_MAX>			description;		//����
	_U32								space;				//ռ�ݸ���
	_S32								req_title;			//Ҫ��Ĺ�ְ
	_S32								req_gold;			//��Ҫ�Ľ��
	_S32								req_rmb;			//��Ҫ�������
	_S32								unlock_level;		//�����ȼ�
	_S32								pre_general_id;		//ǰ�ñ���
	_S32								pre_level;			//ǰ�õȼ�
	_S32								product_consume;	//��������
	_S32								product_time;		//����CDʱ��
	string<ARCHETYPE_URL_LENGTH_MAX>    archetype;			//archetype
};
task[GEN_STRUCT_SERIALIZE(SG_GENERAL_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_GENERAL_CONFIG)];


struct SG_EQUIPPED_UNIT_PVE
{
	string<ARCHETYPE_URL_LENGTH_MAX>	ArchetypeURL;
	SG_PAWN_CONFIG						PawnConfig;
};
task[GEN_STRUCT_SERIALIZE(SG_EQUIPPED_UNIT_PVE)];
task[GEN_STRUCT_REFLECT(SG_EQUIPPED_UNIT_PVE)];

struct SG_PLAYER_PVE
{
	string<ARCHETYPE_URL_LENGTH_MAX>	ArchetypeURL;
	SG_PAWN_CONFIG						PawnConfig;

	array<SG_EQUIPPED_UNIT_PVE, 2>		EquippedGenerals;
	array<SG_EQUIPPED_UNIT_PVE, 3>		EquippedSoldiers;
};
task[GEN_STRUCT_SERIALIZE(SG_PLAYER_PVE)];
task[GEN_STRUCT_REFLECT(SG_PLAYER_PVE)];


struct SG_EQUIP_SLOTS
{
	A_UUID								head;
	A_UUID								hand;
};
task[GEN_STRUCT_SERIALIZE(SG_EQUIP_SLOTS)];
task[GEN_STRUCT_REFLECT(SG_EQUIP_SLOTS)];

struct SG_GENERAL : A_LIVE_OBJECT
{
	_U32								general_id;
	_U16								level;
	_U32								exp;
	SG_EQUIP_SLOTS						equip_slots;
};
task[GEN_STRUCT_SERIALIZE(SG_GENERAL)];
task[GEN_STRUCT_REFLECT(SG_GENERAL)];

struct SG_SOLDIER : A_LIVE_OBJECT
{
	_U32								soldier_id;
	_U16								level;
};
task[GEN_STRUCT_SERIALIZE(SG_SOLDIER)];
task[GEN_STRUCT_REFLECT(SG_SOLDIER)];

struct SG_PLAYER : SG_GENERAL
{
	string<SG_PLAYERNAME_LENMAX>		nick;
	_U32								gold;
	_U32								rmb;
	array<_U16, 2>						equip_generals;
	array<_U16, 3>						equip_soldiers;
};
task[GEN_STRUCT_SERIALIZE(SG_PLAYER)];
task[GEN_STRUCT_REFLECT(SG_PLAYER)];

struct SG_ITEM : A_LIVE_OBJECT
{
	_U32								item_id;
	_U32								count;
	_U32								expired_time;
};
task[GEN_STRUCT_SERIALIZE(SG_ITEM)];
task[GEN_STRUCT_REFLECT(SG_ITEM)];

struct SG_EQUIPT_ITEM : SG_ITEM
{
	SG_ATTR_MOD_CONFIG					mod_config;
};
task[GEN_STRUCT_SERIALIZE(SG_EQUIPT_ITEM)];
task[GEN_STRUCT_REFLECT(SG_EQUIPT_ITEM)];

struct SG_USABLE_ITEM : SG_ITEM
{
};
task[GEN_STRUCT_SERIALIZE(SG_USABLE_ITEM)];
task[GEN_STRUCT_REFLECT(SG_USABLE_ITEM)];

struct SG_GEM_ITEM : SG_ITEM
{
};
task[GEN_STRUCT_SERIALIZE(SG_GEM_ITEM)];
task[GEN_STRUCT_REFLECT(SG_GEM_ITEM)];

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
