
include "../../LibCommon/CommonDDL.ddl"

const _U32 SG_PLAYERNAME_LENMAX	= 100;
const _U32 ARCHETYPE_URL_LENGTH_MAX = 128;
const _U32 SG_DESCRIPTION_MAX = 512;
const _U32 SG_PLAYER_NAME_MAX = 10;
const _U32 SG_EQUIPT_NAME_MAX = 20;
const _U32 SG_EQUIPT_TEX_MAX = 256;
const _U32 SG_SOLDIER_NAME_MAX = 12;
const _U32 SG_LEVEL_SOLDIERS_INFO_MAX = 6;
const _U32 SG_SKILL_NAME_MAX = 10;
const _U32 SG_SKILL_DESC_MAX = 256;
const _U32 SG_GEM_NAME_MAX = 32;
const _U32 SG_GEM_DESC_MAX = 256;
const _U32 SG_QUEST_DIALOG_MAX = 512;
const _U32 SG_QUEST_NAME_MAX = 32;
const _U32 SG_QUEST_URL_MAX = 128;
const _U32 SG_QUEST_COND_MAX = 128;
const _U32 SG_QUEST_GOAL_MAX = 128;
const _U32 SG_EUQIPT_GEM_SLOTS_MAX = 8;

//������Ϣ����
struct SG_QUEST_INFO_CONFIG				: A_CONTENT_OBJECT
{
	_U32								quest_id;				//����id
	string<SG_QUEST_NAME_MAX>			quest_name;				//��������
	string<SG_QUEST_GOAL_MAX>			quest_goal_desc;		//����Ŀ������

	_U8									trigger_type;			//��������
	string<SG_QUEST_URL_MAX>			giver_url;				//giver_url
	_U8									submitee_type;			//��������
	string<SG_QUEST_URL_MAX>			submitee_url;			//submitee_url
	_U32								pre_quest_id;			//ǰ������
	_U32								req_level;				//�ȼ�Ҫ��

	_U8									condition1_type;		//����һ����
	string<SG_QUEST_COND_MAX>			condition1_param1;		//����һ����һ
	string<SG_QUEST_COND_MAX>			condition1_param2;		//����һ������
	_U8									condition2_type;		//����������
	string<SG_QUEST_COND_MAX>			condition2_param1;		//����������һ
	string<SG_QUEST_COND_MAX>			condition2_param2;		//������������

	_U8									quest_goal_type;		//����Ŀ������
	string<SG_QUEST_GOAL_MAX>			goal_string_param;		//Ŀ�����(string)
	_U32								goal_int_param;			//Ŀ�����(int)
	string<SG_QUEST_GOAL_MAX>			goal_string_param2;		//Ŀ�����(string)
	_U32								goal_int_param2;		//Ŀ�����(int)

	string<SG_QUEST_URL_MAX>			goal_pathfinding_url;	//Ŀ��Ѱ·url
	_U32								goal_pathfinding_prority;//Ŀ��Ѱ·���ȼ�
	_U8									req_pack_slot;			//����������(�����ύʱ)
	_U32								reward_xp;				//���齱��
	_U32								reward_gold;			//��Ϸ�ҽ���
	_U32								reward_rmb;				//rmb
	_U32								reward_reputaion;		//����
	_U32								reward_energy;			//����(ƣ��ֵ)

	_U32								reward_item1;			//��������1
	_U32								reward_item1_num;		//��������1����
	_U32								reward_item2;			//��������2
	_U32								reward_item2_num;		//��������2����
	_U32								reward_item3;			//��������3
	_U32								reward_item3_num;		//��������3����
	_U32								reward_item4;			//��������4
	_U32								reward_item4_num;		//��������4����

	string<SG_QUEST_DIALOG_MAX>			dialog_giving_npc1;		//npc�԰�1
	string<SG_QUEST_DIALOG_MAX>			dialog_giving_answer1;	//�ش�1
	string<SG_QUEST_DIALOG_MAX>			dialog_giving_npc2;		//npc�԰�2
	string<SG_QUEST_DIALOG_MAX>			dialog_giving_answer2;	//�ش�2
	string<SG_QUEST_DIALOG_MAX>			dialog_giving_npc3;		//npc�԰�3
	string<SG_QUEST_DIALOG_MAX>			dialog_giving_answer3;	//�ش�3
	string<SG_QUEST_DIALOG_MAX>			dialog_giving_npc4;		//npc�԰�4
	string<SG_QUEST_DIALOG_MAX>			dialog_giving_answer4;	//�ش�4

	string<SG_QUEST_DIALOG_MAX>			dialog_submit_npc1;		//npc��ɶԻ�1
	string<SG_QUEST_DIALOG_MAX>			dialog_submit_answer1;	//��ɻش�1
	string<SG_QUEST_DIALOG_MAX>			dialog_submit_npc2;		//npc��ɶԻ�2
	string<SG_QUEST_DIALOG_MAX>			dialog_submit_answer2;	//��ɻش�2

	string<SG_QUEST_DIALOG_MAX>			dialog_wip_npc;			//npcδ��ɶԻ�
	string<SG_QUEST_DIALOG_MAX>			dialog_wip_answer;		//δ��ɻش�
};
task[GEN_STRUCT_SERIALIZE(SG_QUEST_INFO_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_QUEST_INFO_CONFIG)];

//�ؿ���Ϣ����
struct SG_LEVEL_INFO_CONFIG				: A_CONTENT_OBJECT
{
	_U32								level_id;				//�ؿ�����id
	string<SG_DESCRIPTION_MAX>			description;			//�ؿ�����
	
	string<SG_SOLDIER_NAME_MAX>			soldier_name1;			//��������
	string<SG_SOLDIER_NAME_MAX>			soldier_name2;			//��������
	string<SG_SOLDIER_NAME_MAX>			soldier_name3;			//��������
	string<SG_SOLDIER_NAME_MAX>			soldier_name4;			//��������
	string<SG_SOLDIER_NAME_MAX>			soldier_name5;			//��������
	string<SG_SOLDIER_NAME_MAX>			soldier_name6;			//��������

	_U8									atk_type1;				//��������
	_U8									atk_type2;				//��������
	_U8									atk_type3;				//��������
	_U8									atk_type4;				//��������
	_U8									atk_type5;				//��������
	_U8									atk_type6;				//��������

	_U8									wpt_type1;				//����
	_U8									wpt_type2;				//����
	_U8									wpt_type3;				//����
	_U8									wpt_type4;				//����
	_U8									wpt_type5;				//����
	_U8									wpt_type6;				//����
};
task[GEN_STRUCT_SERIALIZE(SG_LEVEL_INFO_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_LEVEL_INFO_CONFIG)];

struct SG_DIRTY_WORD_CONFIG				: A_CONTENT_OBJECT
{
	string<SG_PLAYER_NAME_MAX>			dirty_word;
};
task[GEN_STRUCT_SERIALIZE(SG_DIRTY_WORD_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_DIRTY_WORD_CONFIG)];

struct SG_PLAYER_RANDOM_NAME_CONFIG : A_CONTENT_OBJECT
{
	string<SG_PLAYER_NAME_MAX>			family_name;
	string<SG_PLAYER_NAME_MAX>			male_name;
	string<SG_PLAYER_NAME_MAX>			female_name;
};
task[GEN_STRUCT_SERIALIZE(SG_PLAYER_RANDOM_NAME_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_PLAYER_RANDOM_NAME_CONFIG)];

struct SG_LEVEL_DROP_CONFIG : A_CONTENT_OBJECT
{
	string<100>							level_name;
	_U32								exp;
	_U32								gold;
	_U32								group1_id;
	_F32								group1_rate;
	_U32								group2_id;
	_F32								group2_rate;
	_U32								group3_id;
	_F32								group3_rate;
	_U32								group4_id;
	_F32								group4_rate;
};
task[GEN_STRUCT_SERIALIZE(SG_LEVEL_DROP_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_LEVEL_DROP_CONFIG)];

struct SG_DROP_ITEM_BASE : A_CONTENT_OBJECT
{
	_U32								item_id;
	_U32								count;
};
task[GEN_STRUCT_SERIALIZE(SG_DROP_ITEM_BASE)];
task[GEN_STRUCT_REFLECT(SG_DROP_ITEM_BASE)];

struct SG_DROP_ITEM_CONFIG : SG_DROP_ITEM_BASE
{
	_U32								group_id;
	_F32								rate;
};
task[GEN_STRUCT_SERIALIZE(SG_DROP_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_DROP_ITEM_CONFIG)];

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
	
	string<SG_EQUIPT_TEX_MAX>			tex;				//����
	_U32								U;					
	_U32								V;
	_U32								UL;
	_U32								VL;
	_U32								price;				//���ۼ۸�
};
task[GEN_STRUCT_SERIALIZE(SG_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_ITEM_CONFIG)];

struct SG_EQUIPT_ITEM_CONFIG : SG_ITEM_CONFIG				//װ��
{
	SG_ATTR_MOD_CONFIG					mod_config;
	string<SG_EQUIPT_NAME_MAX>			item_name;			//װ������
	_U8									item_type;			//װ������
	_U8									quanlity;			//Ʒ��
	_F32								quanlity_rate;		//Ʒ��ϵ��
	_U32								req_level;			//����ȼ�
	_U8									major_attr;			//����������
	_U32								major_attr_num;		//����������
	_F32								turbo_rate;			//ǿ��������
	_U8									minor_attr_num;		//����������
	_U32								HP_MAX;				//��������
	_U32								POW_MAX;			//��������
	_U32								ITEM_INT_MAX;		//��������
	_F32								HIT_MAX;			//��������
	_F32								CRIT_MAX;			//��������
	_F32								MISS_MAX;			//��������
	_U32								SLOT_NUM;			//�������
	_U8									HAS_SKILL;			//Я������
	_U32								COM_MATERIAL;		//ͨ�ò���ID
	_U32								COM_REQ_NUM;		//ͨ�ò�������
	_U32								KEY_MATERIAL;		//�ؼ�����ID
	_U32								KEY_REQ_NUM;		//�ؼ���������
	_U32								COMBINE_COST;		//�ϳɷ���
	_U32								TURBO_BASE_COST;	//ǿ�������۸�
	_U32								RESET_COST;			//ϴ���۸�
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
	string<SG_GEM_NAME_MAX>		gem_name;					//��ʯ����
	string<SG_GEM_DESC_MAX>		desc;						//��ʯ����
	_U8							quality;					//��ʯƷ��
	_U8							level;						//��ʯ�ȼ�
	_U8							attri_type;					//��ʯ����
	_U32						attri;						//��ʯ����ֵ
	_U32						compose_next;				//�ϳɵ���һ���ȼ�
	_U32						compose_num;				//�ϳ���Ҫ������
};
task[GEN_STRUCT_SERIALIZE(SG_GEM_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_GEM_ITEM_CONFIG)];

struct SG_ENEMY_CONFIG : A_CONTENT_OBJECT
{
	_U32								enemy_id;			//����id
	SG_PAWN_CONFIG						PawnConfig;
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
	_S32								levelup_gold;		//���������Ǯ
	_S32								levelup_rmb;		//���������Ǯ
	_S32								levelup_XP;			//�������辭��
};

task[GEN_STRUCT_SERIALIZE(SG_GENERAL_LEVEL_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_GENERAL_LEVEL_CONFIG)];

struct SG_SOLDIER_CONFIG : A_CONTENT_OBJECT
{
	_U32								soldier_id;
	_U32								attr_id;
	
	_U8									type;				//С������
	_U8									atk_type;			//��������
	_U8									def_type;			//��������
	string<SG_DESCRIPTION_MAX>			description;		//����
	_S32								req_gold;			//��Ҫ�Ľ��
	_S32								req_rmb;			//��Ҫ�������
	_S32								unlock_level;		//�����ȼ�
	_S32								pre_general_id;		//ǰ�ñ���
	_S32								pre_level;			//ǰ�õȼ�
	string<ARCHETYPE_URL_LENGTH_MAX>    archetype;			//archetype
};
task[GEN_STRUCT_SERIALIZE(SG_SOLDIER_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_SOLDIER_CONFIG)];

struct SG_GENERAL_CONFIG : A_CONTENT_OBJECT
{
	_U32								general_id;
	_U32								attr_id;

	string<SG_DESCRIPTION_MAX>			description;		//����
	_S32								req_title;			//Ҫ��Ĺ�ְ
	_S32								req_gold;			//��Ҫ�Ľ��
	_S32								req_rmb;			//��Ҫ�������
	_S32								unlock_level;		//�����ȼ�
	_S32								pre_general_id;		//ǰ�ñ���
	_S32								pre_level;			//ǰ�õȼ�
	string<ARCHETYPE_URL_LENGTH_MAX>    archetype;			//archetype
	_U32								gift_id;			//�츳ID
	string<SG_SKILL_DESC_MAX>			skill_name;			//��������
	string<SG_DESCRIPTION_MAX>			skill_desc;			//���ܱ���
	_S32								rank;				//�׼�
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
	string<SG_PLAYERNAME_LENMAX>		Nick;
};
task[GEN_STRUCT_SERIALIZE(SG_PLAYER_PVE)];
task[GEN_STRUCT_REFLECT(SG_PLAYER_PVE)];

struct SG_EQUIP_SLOTS
{
	A_UUID								head;				//ͷ��
	A_UUID								weapon;				//����
	A_UUID								shoulder;			//�粿
	A_UUID								chest;				//�ز�
	A_UUID								leg;				//�Ȳ�
	A_UUID								decoration;			//reserve��Ʒ		
};
task[GEN_STRUCT_SERIALIZE(SG_EQUIP_SLOTS)];
task[GEN_STRUCT_REFLECT(SG_EQUIP_SLOTS)];

struct SG_GENERAL : A_LIVE_OBJECT
{
	_U32								general_id;
	_U16								level;
	_U32								exp;
	_U8									status;				//�佫״̬ 0-δ��ļ 1-�ھƹ�δ�ڶ� 2-�ھƹ��ڶ�
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
	array<_U32, 2>						equip_generals;
	array<_U32, 3>						equip_soldiers;
	_U8									increase_equipt_times;//��߹���ȴʱ�����
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
	_U8									turbo_level;				//��ǰǿ���ȼ�
	array<A_UUID, SG_EUQIPT_GEM_SLOTS_MAX>		gems_slots;			//��Ƕ�ı�ʯ
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

struct SG_SERVER_INFO
{
	_U32								server_id;
	string<100>							server_name;
	_U32								server_state;
	string<100>							avatar_nick;
	_U32								general_id;
	_U32								level;
};
task[GEN_STRUCT_SERIALIZE(SG_SERVER_INFO)];
task[GEN_STRUCT_REFLECT(SG_SERVER_INFO)];

class SGGAME_C2S
{
	Ping();

	GetServerList();
	EnterServer(_U32 server_id);
	QueryAvatar();
	CreateAvatar(string nick, _U32 general_id);
	DeleteAvatar();
	EnterGame();
	LeaveGame();

	QueryPlayer();
	QueryGenerals();
	QuerySoldiers();
	QueryBag();

	EquipItem(_U32 general_id, SG_EQUIP_SLOTS slots);
	EquipGenerals(_U32 generals[count], _U32 count);
	EquipSoldiers(_U32 soldiers[count], _U32 count);

	EnhanceSoldier(_U32 soldier_id);
	EnhanceEquipt(A_UUID uuid);
	ExtendEquipt(A_UUID uuid, A_UUID puuid);
	EnhanceCoolDown();
	EnhanceCoolDownClear();
	IncreaseEquipCoolDown();

	RefreshEquipNormal(A_UUID uuid);
	RefreshEquipProperty(A_UUID uuid);
	RefreshEquipAbility(A_UUID uuid);
	RefreshEquipDecideAccept(A_UUID uuid);

	BeginBattle(string name);
	EndBattle(string name, _U32 result);

	EquipGem(A_UUID item_uuid, A_UUID gem_uuid);
	UnequipGem(A_UUID item_uuid, A_UUID gem_uuid);
	GemCombine(A_UUID gems[count], _U32 count);

	QueryServerTime();
};

class SGGAME_S2C
{
	Pong();

	GetServerListResult(SG_SERVER_INFO infos[count], _U32 count);
	QueryAvatarFailed(_U32 code);
	QueryAvatarResult(SG_PLAYER player);
	CreateAvatarResult(_U32 code);

	QueryPlayerResult(SG_PLAYER player);
	QueryGeneralResult(SG_GENERAL generals[count], _U32 count);
	QuerySoldierResult(SG_SOLDIER soldiers[count], _U32 count);
	QueryBagBegin();
	QueryBagEquipt(SG_EQUIPT_ITEM items[count], _U32 count);
	QueryBagUsable(SG_USABLE_ITEM items[count], _U32 count);
	QueryBagGen(SG_GEM_ITEM items[count], _U32 count);
	QueryBagEnd();

	EnhanceCoolDownResult(_U32 time);
	RefreshEquipDone(SG_EQUIPT_ITEM euipt);

	BeginBattleResult(SG_PLAYER_PVE PlayerPVE);
	EndBattleResult(_U32 level, _U32 exp, _U32 gold, SG_DROP_ITEM_CONFIG drops[drop_count], _U32 drop_count);

	GemCombineResult(SG_GEM_ITEM gem);

	QueryServerTimeResult(_U32 time);
};

task[GEN_CLASS_STUB(SGGAME_C2S)];
task[GEN_CLASS_PROXY(SGGAME_C2S)];
task[GEN_CLASS_REFLECT(SGGAME_C2S)];
task[GEN_CLASS_STUB(SGGAME_S2C)];
task[GEN_CLASS_PROXY(SGGAME_S2C)];
task[GEN_CLASS_REFLECT(SGGAME_S2C)];

task[GEN_PHP_STUB(SGGAME_C2S)];
task[GEN_PHP_PROXY(SGGAME_S2C)];
