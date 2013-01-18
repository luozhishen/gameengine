
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

//任务信息配置
struct SG_QUEST_INFO_CONFIG				: A_CONTENT_OBJECT
{
	_U32								quest_id;				//任务id
	string<SG_QUEST_NAME_MAX>			quest_name;				//任务名称
	string<SG_QUEST_GOAL_MAX>			quest_goal_desc;		//任务目标描述

	_U8									trigger_type;			//触发类型
	string<SG_QUEST_URL_MAX>			giver_url;				//giver_url
	_U8									submitee_type;			//交付类型
	string<SG_QUEST_URL_MAX>			submitee_url;			//submitee_url
	_U32								pre_quest_id;			//前置任务
	_U32								req_level;				//等级要求

	_U8									condition1_type;		//条件一类型
	string<SG_QUEST_COND_MAX>			condition1_param1;		//条件一参数一
	string<SG_QUEST_COND_MAX>			condition1_param2;		//条件一参数二
	_U8									condition2_type;		//条件二类型
	string<SG_QUEST_COND_MAX>			condition2_param1;		//条件二参数一
	string<SG_QUEST_COND_MAX>			condition2_param2;		//条件二参数二

	_U8									quest_goal_type;		//任务目标类型
	string<SG_QUEST_GOAL_MAX>			goal_string_param;		//目标参数(string)
	_U32								goal_int_param;			//目标参数(int)
	string<SG_QUEST_GOAL_MAX>			goal_string_param2;		//目标参数(string)
	_U32								goal_int_param2;		//目标参数(int)

	string<SG_QUEST_URL_MAX>			goal_pathfinding_url;	//目标寻路url
	_U32								goal_pathfinding_prority;//目标寻路优先级
	_U8									req_pack_slot;			//背包格子数(奖励提交时)
	_U32								reward_xp;				//经验奖励
	_U32								reward_gold;			//游戏币奖励
	_U32								reward_rmb;				//rmb
	_U32								reward_reputaion;		//军功
	_U32								reward_energy;			//体力(疲劳值)

	_U32								reward_item1;			//奖励道具1
	_U32								reward_item1_num;		//奖励道具1数量
	_U32								reward_item2;			//奖励道具2
	_U32								reward_item2_num;		//奖励道具2数量
	_U32								reward_item3;			//奖励道具3
	_U32								reward_item3_num;		//奖励道具3数量
	_U32								reward_item4;			//奖励道具4
	_U32								reward_item4_num;		//奖励道具4数量

	string<SG_QUEST_DIALOG_MAX>			dialog_giving_npc1;		//npc对白1
	string<SG_QUEST_DIALOG_MAX>			dialog_giving_answer1;	//回答1
	string<SG_QUEST_DIALOG_MAX>			dialog_giving_npc2;		//npc对白2
	string<SG_QUEST_DIALOG_MAX>			dialog_giving_answer2;	//回答2
	string<SG_QUEST_DIALOG_MAX>			dialog_giving_npc3;		//npc对白3
	string<SG_QUEST_DIALOG_MAX>			dialog_giving_answer3;	//回答3
	string<SG_QUEST_DIALOG_MAX>			dialog_giving_npc4;		//npc对白4
	string<SG_QUEST_DIALOG_MAX>			dialog_giving_answer4;	//回答4

	string<SG_QUEST_DIALOG_MAX>			dialog_submit_npc1;		//npc完成对话1
	string<SG_QUEST_DIALOG_MAX>			dialog_submit_answer1;	//完成回答1
	string<SG_QUEST_DIALOG_MAX>			dialog_submit_npc2;		//npc完成对话2
	string<SG_QUEST_DIALOG_MAX>			dialog_submit_answer2;	//完成回答2

	string<SG_QUEST_DIALOG_MAX>			dialog_wip_npc;			//npc未完成对话
	string<SG_QUEST_DIALOG_MAX>			dialog_wip_answer;		//未完成回答
};
task[GEN_STRUCT_SERIALIZE(SG_QUEST_INFO_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_QUEST_INFO_CONFIG)];

//关卡信息配置
struct SG_LEVEL_INFO_CONFIG				: A_CONTENT_OBJECT
{
	_U32								level_id;				//关卡配置id
	string<SG_DESCRIPTION_MAX>			description;			//关卡描述
	
	string<SG_SOLDIER_NAME_MAX>			soldier_name1;			//兵种名称
	string<SG_SOLDIER_NAME_MAX>			soldier_name2;			//兵种名称
	string<SG_SOLDIER_NAME_MAX>			soldier_name3;			//兵种名称
	string<SG_SOLDIER_NAME_MAX>			soldier_name4;			//兵种名称
	string<SG_SOLDIER_NAME_MAX>			soldier_name5;			//兵种名称
	string<SG_SOLDIER_NAME_MAX>			soldier_name6;			//兵种名称

	_U8									atk_type1;				//攻击类型
	_U8									atk_type2;				//攻击类型
	_U8									atk_type3;				//攻击类型
	_U8									atk_type4;				//攻击类型
	_U8									atk_type5;				//攻击类型
	_U8									atk_type6;				//攻击类型

	_U8									wpt_type1;				//弱点
	_U8									wpt_type2;				//弱点
	_U8									wpt_type3;				//弱点
	_U8									wpt_type4;				//弱点
	_U8									wpt_type5;				//弱点
	_U8									wpt_type6;				//弱点
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
	_F32								Base_CRIT;			//暴击
	_F32								Base_SP;			//士气增长
	_F32								Base_WS;			//移动速度
	_F32								Base_STR;			//力量			
	_F32								Base_INT;			//智力
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
	_U32								item_id;			//物品id
	_U32								stack_max;			//叠加最大数
	
	string<SG_EQUIPT_TEX_MAX>			tex;				//纹理
	_U32								U;					
	_U32								V;
	_U32								UL;
	_U32								VL;
	_U32								price;				//出售价格
};
task[GEN_STRUCT_SERIALIZE(SG_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_ITEM_CONFIG)];

struct SG_EQUIPT_ITEM_CONFIG : SG_ITEM_CONFIG				//装备
{
	SG_ATTR_MOD_CONFIG					mod_config;
	string<SG_EQUIPT_NAME_MAX>			item_name;			//装备名称
	_U8									item_type;			//装备类型
	_U8									quanlity;			//品质
	_F32								quanlity_rate;		//品质系数
	_U32								req_level;			//需求等级
	_U8									major_attr;			//主属性类型
	_U32								major_attr_num;		//主属性数量
	_F32								turbo_rate;			//强化增长率
	_U8									minor_attr_num;		//副属性数量
	_U32								HP_MAX;				//体质上限
	_U32								POW_MAX;			//力量上限
	_U32								ITEM_INT_MAX;		//智力上限
	_F32								HIT_MAX;			//命中上限
	_F32								CRIT_MAX;			//暴击上限
	_F32								MISS_MAX;			//闪避上限
	_U32								SLOT_NUM;			//插槽数量
	_U8									HAS_SKILL;			//携带技能
	_U32								COM_MATERIAL;		//通用材料ID
	_U32								COM_REQ_NUM;		//通用材料数量
	_U32								KEY_MATERIAL;		//关键材料ID
	_U32								KEY_REQ_NUM;		//关键材料数量
	_U32								COMBINE_COST;		//合成费用
	_U32								TURBO_BASE_COST;	//强化基础价格
	_U32								RESET_COST;			//洗练价格
};
task[GEN_STRUCT_SERIALIZE(SG_EQUIPT_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_EQUIPT_ITEM_CONFIG)];

struct SG_USABLE_ITEM_CONFIG : SG_ITEM_CONFIG				//可使用
{
	string<100>							functional;
};
task[GEN_STRUCT_SERIALIZE(SG_USABLE_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_USABLE_ITEM_CONFIG)];

struct SG_GEM_ITEM_CONFIG : SG_ITEM_CONFIG					//宝石
{
	string<SG_GEM_NAME_MAX>		gem_name;					//宝石名字
	string<SG_GEM_DESC_MAX>		desc;						//宝石描述
	_U8							quality;					//宝石品质
	_U8							level;						//宝石等级
	_U8							attri_type;					//宝石属性
	_U32						attri;						//宝石属性值
	_U32						compose_next;				//合成的下一个等级
	_U32						compose_num;				//合成需要的数量
};
task[GEN_STRUCT_SERIALIZE(SG_GEM_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_GEM_ITEM_CONFIG)];

struct SG_ENEMY_CONFIG : A_CONTENT_OBJECT
{
	_U32								enemy_id;			//敌人id
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
	_S32								levelup_gold;		//升级所需金钱
	_S32								levelup_rmb;		//升级所需金钱
	_S32								levelup_XP;			//升级所需经验
	_U16								mod_scale;			//模型缩放			
};
task[GEN_STRUCT_SERIALIZE(SG_SOLDIER_LEVEL_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_SOLDIER_LEVEL_CONFIG)];

struct SG_GENERAL_LEVEL_CONFIG : A_CONTENT_OBJECT
{
	_U32								attr_id;
	_U32								level;
	SG_PAWN_CONFIG						PawnConfig;
	_S32								levelup_gold;		//升级所需金钱
	_S32								levelup_rmb;		//升级所需金钱
	_S32								levelup_XP;			//升级所需经验
};

task[GEN_STRUCT_SERIALIZE(SG_GENERAL_LEVEL_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_GENERAL_LEVEL_CONFIG)];

struct SG_SOLDIER_CONFIG : A_CONTENT_OBJECT
{
	_U32								soldier_id;
	_U32								attr_id;
	
	_U8									type;				//小兵类型
	_U8									atk_type;			//攻击类型
	_U8									def_type;			//防御类型
	string<SG_DESCRIPTION_MAX>			description;		//描述
	_S32								req_gold;			//需要的金币
	_S32								req_rmb;			//需要的人民币
	_S32								unlock_level;		//解锁等级
	_S32								pre_general_id;		//前置兵种
	_S32								pre_level;			//前置等级
	string<ARCHETYPE_URL_LENGTH_MAX>    archetype;			//archetype
};
task[GEN_STRUCT_SERIALIZE(SG_SOLDIER_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_SOLDIER_CONFIG)];

struct SG_GENERAL_CONFIG : A_CONTENT_OBJECT
{
	_U32								general_id;
	_U32								attr_id;

	string<SG_DESCRIPTION_MAX>			description;		//描述
	_S32								req_title;			//要求的官职
	_S32								req_gold;			//需要的金币
	_S32								req_rmb;			//需要的人民币
	_S32								unlock_level;		//解锁等级
	_S32								pre_general_id;		//前置兵种
	_S32								pre_level;			//前置等级
	string<ARCHETYPE_URL_LENGTH_MAX>    archetype;			//archetype
	_U32								gift_id;			//天赋ID
	string<SG_SKILL_DESC_MAX>			skill_name;			//技能名称
	string<SG_DESCRIPTION_MAX>			skill_desc;			//技能表述
	_S32								rank;				//阶级
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
	A_UUID								head;				//头部
	A_UUID								weapon;				//武器
	A_UUID								shoulder;			//肩部
	A_UUID								chest;				//胸部
	A_UUID								leg;				//腿部
	A_UUID								decoration;			//reserve饰品		
};
task[GEN_STRUCT_SERIALIZE(SG_EQUIP_SLOTS)];
task[GEN_STRUCT_REFLECT(SG_EQUIP_SLOTS)];

struct SG_GENERAL : A_LIVE_OBJECT
{
	_U32								general_id;
	_U16								level;
	_U32								exp;
	_U8									status;				//武将状态 0-未招募 1-在酒馆未在队 2-在酒馆在队
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
	_U8									increase_equipt_times;//提高过冷却时间次数
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
	_U8									turbo_level;				//当前强化等级
	array<A_UUID, SG_EUQIPT_GEM_SLOTS_MAX>		gems_slots;			//镶嵌的宝石
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
