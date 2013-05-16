
include "../../LibCommon/CommonDDL.ddl"

//SG_SERVER_RESULT
const _U8 SG_SERVER_RESULT_SUCC			= 0;

//SG_CLIENT
const _U8 SG_CLIENT_STATUS_READY		= 0;
const _U8 SG_CLIENT_STATUS_UNINIT		= 1;
const _U8 SG_CLIENT_STATUS_SYNCING		= 2;
const _U8 SG_CLIENT_STATUS_DISCONNECT	= 3;
const _U8 SG_CLIENT_STATUS_TERMINED		= 4;
const _U8 SG_CLIENT_STATUS_UNKOWN		= 5;

const _U8 SG_CLIENT_PING_TIMEOUT  = 10;
const _U8 SG_CLIENT_EVENT_POLL_TIMEOUT = 10;

//Basic
const _U32 ARCHETYPE_URL_LENGTH_MAX = 256;
const _U32 SG_DESCRIPTION_MAX = 512;
const _U32 SG_INVALID_SERVER_ID = 255;

const _U32 SG_ICON_MAX = 256;
const _U32 SG_MAP_URL_MAX = 256;
const _U32 SG_TEX_MAX = 256;

//Sync data
const _U8 SG_SYNC_NONE = 0;
const _U8 SG_SYNC_PLAYER = 1;
const _U8 SG_SYNC_Generals = 2;
const _U8 SG_SYNC_SOLDIERS = 3;
const _U8 SG_SYNC_BAG = 4;
const _U8 SG_SYNC_EQUIPT = 5;
const _U8 SG_SYNC_USABLE = 6;
const _U8 SG_SYNC_GEM = 7;
const _U8 SG_SYNC_PLAYERQUEST = 9;

//Player
const _U32 SG_PLAYER_NAME_MAX = 32;
const _U32 SG_PLAYER_TITLE_NAME_MAX = 12;
const _U32 SG_PLAYER_WUSHENG_MALE_ID = 11001;
const _U32 SG_PLAYER_WUSHENG_FEMALE_ID = 11002;

//Soldier General Skill 
const _U32 SG_SOLDIER_NAME_MAX = 24;
const _U32 SG_LEVEL_SOLDIERS_INFO_MAX = 6;
const _U32 SG_SKILL_NAME_MAX = 10;
const _U32 SG_SKILL_DESC_MAX = 256;

const _U32 SG_GENERAL_NOT_EMPLOYEE		= 0;	//0-未招募
const _U32 SG_GENERAL_EMPLOYEED_OUTQUEUE = 1;	//1-在酒馆未在队
const _U32 SG_GENERAL_EMPLOYEED_INQUEUE = 2;	//2-在酒馆在队

//Item
const _U32 SG_ITEM_USABLE_ID_START  = 10000;
const _U32 SG_ITEM_USABLE_ID_END	= 19999;
const _U32 SG_ITEM_MATERIAL_ID_START  = 20000;
const _U32 SG_ITEM_MATERIAL_ID_END	  = 29999;
const _U32 SG_ITEM_GEM_ID_START		= 30000;
const _U32 SG_ITEM_GEM_ID_END		= 39999;
const _U32 SG_ITEM_EQUIPT_ID_START  = 40000;
const _U32 SG_ITEM_EQUIPT_ID_END	= 49999;
const _U32 SG_ITEM_ASTOLOGY_BALL_ID_START	= 50000;
const _U32 SG_ITEM_ASTOLOGY_BALL_ID_END		= 59999;
const _U32 SG_ITEM_NAME_MAX			= 32;

//Material
const _U32 SG_MATERIAL_DESC_MAX = 256;

//Gem
const _U32 SG_GEM_DESC_MAX = 256;

//Equipt
const _U32 SG_EUQIPT_GEM_SLOTS_MAX = 8;
const _U32 SG_EQUIPT_GENERAL_MAX = 2;
const _U32 SG_EQUIPT_SOLDIER_MAX = 3;

const _U8 SG_EQUIPT_TYPE_WEAPON_JINZHAN		= 0;
const _U8 SG_EQUIPT_TYPE_WEAPON_YUANCHENG	= 1;
const _U8 SG_EQUIPT_TYPE_WEAPON_FASHU		= 2;
const _U8 SG_EQUIPT_TYPE_HEAD		= 3;
const _U8 SG_EQUIPT_TYPE_SHOULDER	= 4;
const _U8 SG_EQUIPT_TYPE_CHEST		= 5;
const _U8 SG_EQUIPT_TYPE_LEG		= 6;
const _U8 SG_EQUIPT_TYPE_DECORATION = 7;

//usable
const _U32 SG_USABLE_DESC_MAX = 256;

//Quest
const _U32 SG_QUEST_DIALOG_MAX = 768;
const _U32 SG_QUEST_NAME_MAX = 32;
const _U32 SG_QUEST_URL_MAX = 128;
const _U32 SG_QUEST_COND_MAX = 128;
const _U32 SG_QUEST_GOAL_MAX = 512;
const _U32 SG_QUEST_GOAL_DESC_MAX = 256;

const _U8 SG_QUEST_STATUS_NOTAVALIABLE = 0;
const _U8 SG_QUEST_STATUS_AVALIABLE = 1;
const _U8 SG_QUEST_STATUS_AVALIABLE_NEXT_LEVEL = 2;
const _U8 SG_QUEST_STATUS_WIP = 3;
const _U8 SG_QUEST_STATUS_READY = 4;
const _U8 SG_QUEST_STATUS_FINISHED = 5;

//Title
const _U32 SG_TITLE_GENERAL_DESC = 512;
const _U32 SG_TITLE_PROP_DESC = 768;
const _U32 SG_TITLE_ACRHTYPE_MAX = 512;

//League
const _U32 SG_LEAGUE_NAME_MAX = 32;
const _U32 SG_LEAGUE_NOTICE_MAX = 512;
const _U32 SG_LEAGUE_LOG_MAX = 256;

//shop
const _U32 SG_GOODS_GROUP_NAME_MAX = 128;
const _U32 SG_INSTANCE_REWARD_DES_MAX = 128;

//league
const _U32 SG_LEAGUE_APPLY_MAX = 10;
const _U8 SG_LEAGUE_CREATE_SUCC = 0;
const _U8 SG_LEAGUE_CREATE_FAILED = 1;
const _U8 SG_LEAUGE_WINE_NAME_MAX = 32;

//daily action type
const _U32 SG_DAILY_ACTION_TYPE_MAX = 12;		//目前日常活动最大值
const _U8 SG_DAILY_ACTION_TYPE_PAID = 0;		//0-俸禄
const _U8 SG_DAILY_ACTION_TYPE_PVP_LEVEL = 1;	//1-每日关卡可进行
const _U8 SG_DAILY_ACTION_TYPE_PVP_BATTLE = 2;	//2-pvp挑战次数
const _U8 SG_DAILY_ACTION_TYPE_PVP_REWARD = 3;	//3-pvp每日奖励
const _U8 SG_DAILY_ACTION_TYPE_SALARY = 4;		//4-每日军饷可领次数
const _U32 SG_DAILY_ACTION_NAME_MAX = 128;		
const _U32 SG_DAILY_ACTION_DESC = 512;			

//vip
const _U32 SG_VIP_ICON_MAX = 256;

//turbo
const _U32 SG_TURBO_CHARPTER_NAME_MAX = 128;
const _U32 SG_TURBO_UNLOCK_SKILL_MAX = 15;
const _U32 SG_TURBO_EQUIP_SKILL_MAX = 3;

//auto combat
const _U32 SG_AUTO_COMBAT_REWARD_MAX = 64;

//struct SG_MAP_URL
//{
//	string<SG_MAP_URL_MAX>				map_url;				
//};
//task[GEN_STRUCT_SERIALIZE(SG_MAP_URL)];
//task[GEN_STRUCT_REFLECT(SG_MAP_URL)];

//chat
const _U32 SG_CHAT_CHANNEL_NAME_MAX = 32;
const _U32 SG_CHAT_CHANNEL_COLOR_MAX = 32;
const _U32 SG_CHAT_TAP_NAME_MAX = 32;
const _U32 SG_CHAT_CONTENT_MAX = 512;

//strology
const _U32 SG_ASTROLOGY_BALL_NAME_MAX = 32;
const _U32 SG_ASTROLOGY_BALL_DESC_MAX = 128;

const _U32 SG_ASTROLOGER_NAME_MAX = 32;
const _U32 SG_ASTROLOGER_DESC_MAX = 128;

//horse
const _U32 SG_HORSE_NAME_MAX = 32;
const _U32 SG_HORSE_MESH_MAX = 512;
const _U32 SG_HORSE_DESC_MAX = 512;

//zhanjiang level
const _U32 SG_ZHANJIANG_LEVEL_LD_MAX = 512;

//newcomer guide
const _U32 SG_NEWCOMER_GUIDE_DESC_MAX = 512;
const _U32 SG_NEWCOMER_GUIDE_PIC_MAX = 128;
const _U32 SG_NEWCOMER_GUIDE_ICON_MAX = 128;
const _U32 SG_NEWCOMER_GUIDE_TIP_MAX = 128;
const _U32 SG_NEWCOMER_UI_MAX = 128;
const _U32 SG_NEWCOMER_GUIDER_LABEL_MAX = 128;

//vip
const _U32 SG_VIP_DESC_MAX = 512;

//rmb_recharge
const _U32 SG_RECHARGE_YUANBAO_DESC = 512;
const _U32 SG_RECHARGE_PRICE_DESC = 512;
const _U32 SG_RECHARGE_OTHER_DESC = 512;

//world boss
const _U32 SG_WORLDBOSS_RANK_LIST_MAX = 10;

//world boss
struct SG_WORLD_BOSS_CONFIG : A_CONTENT_OBJECT
{
	_U32										world_level;		//世界等级
	_U32										boss_id;			//bossid
	_U32										base_gold;			//基础银币
	_U32										total_gold;			//银币总量
	_U32										total_reputation;	//声望总量

	_U32										loot;				//参与宝石宝箱
	_U32										reward_gold;		//单次银币奖励
	_U32										reward_loot1;		//单次宝箱1奖励
	_F32										reward_loot1_rate;	//单次宝箱1几率
	_U32										reward_loot2;		//单次宝箱2奖励
	_F32										reward_loot2_rate;	//单次宝箱2几率

	string<100>									reward1;			//第n名奖励
	string<100>									reward2;
	string<100>									reward3;
	string<100>									reward4;
	string<100>									reward5;
	string<100>									reward6;
	string<100>									reward7;
	string<100>									reward8;
	string<100>									reward9;
	string<100>									reward10;
};
task[GEN_STRUCT_SERIALIZE(SG_WORLD_BOSS_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_WORLD_BOSS_CONFIG)];

struct SG_WORLD_BOSS_INSPIRE_CONFIG : A_CONTENT_OBJECT
{
	_U32										times;				//次数
	_U32										consume_rmb;		//花费元宝数
};
task[GEN_STRUCT_SERIALIZE(SG_WORLD_BOSS_INSPIRE_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_WORLD_BOSS_INSPIRE_CONFIG)];

//rmb_recharge
struct SG_RECHARGE_CONFIG : A_CONTENT_OBJECT
{
	_U32										index;				//序号
	_U32										yuanbao;			//元宝数
	_U32										extra_yuanbao;		//额外赠送元宝数
	string<SG_RECHARGE_YUANBAO_DESC>			yuanbao_desc;		//元宝表述
	_U32										price;				//价格rmb
	string<SG_RECHARGE_PRICE_DESC>				price_desc;			//价格描述
	string<SG_RECHARGE_OTHER_DESC>				other_desc;

	string<SG_ICON_MAX>							icon;				//图片资源

	_U32										U;					
	_U32										V;					
	_U32										UL;					
	_U32										VL;	
};		
task[GEN_STRUCT_SERIALIZE(SG_RECHARGE_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_RECHARGE_CONFIG)];

//newcomer guide
struct SG_NEWCOMER_GUIDE_CONFIG : A_CONTENT_OBJECT
{
	_U32										function_id;		//功能ID
	_U32										process_id;			//流程引导ID
	_U8											type;				//类型
	_U32										remain_time;		//停留时间
	_U8											condition1;			//开启条件1
	_U32										condition1_param1;	//开启参数1
	_U32										condition1_param2;	//开启参数2
																	
	_U8											condition2;			//开启条件2
	_U32										condition2_param1;	//开启2条件1
	_U32										condition2_param2;	//开启2条件2
																	
	string<SG_NEWCOMER_UI_MAX>					open_function;		//开启功能 string
	string<SG_NEWCOMER_GUIDE_DESC_MAX>			desc;				//文字说明
																	
	string<SG_NEWCOMER_GUIDE_PIC_MAX>			guider_pic;			//引导员图片
	_U32										U;					//guider_pic的UVULVL
	_U32										V;					
	_U32										UL;					
	_U32										VL;					
																	
	string<SG_ICON_MAX>							icon;				//开启功能icon
	_U32										U2;					//icon的UVULVL
	_U32										V2;					
	_U32										UL2;				
	_U32										VL2;				
	string<SG_NEWCOMER_GUIDER_LABEL_MAX>		guider_label;		//引导员标题
	string<SG_TEX_MAX>							tex;				
};
task[GEN_STRUCT_SERIALIZE(SG_NEWCOMER_GUIDE_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_NEWCOMER_GUIDE_CONFIG)];

struct SG_NEWCOMER_GUIDE_PROCESS_CONFIG : A_CONTENT_OBJECT
{
	_U32										process_id;			//引导ID
	_U8											type;				//引导类型
	_U32										group;				//引导组
	_U32										close_level;		//关闭等级
	_U8											termial_type;		//中断类型
	_U32										step1;				//步骤1
	_U32										step2;
	_U32										step3;
	_U32										step4;
	_U32										step5;
	_U32										step6;
	_U32										step7;
	_U32										step8;
	_U32										step9;
	_U32										step10;
	_U32										step11;
	_U32										step12;
	_U32										step13;
	_U32										step14;
	_U32										step15;				//步骤15

};
task[GEN_STRUCT_SERIALIZE(SG_NEWCOMER_GUIDE_PROCESS_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_NEWCOMER_GUIDE_PROCESS_CONFIG)];

struct SG_NEWCOMER_GUIDE_STEP_CONFIG : A_CONTENT_OBJECT
{
	_U32										step_id;			//步骤ID
	string<SG_NEWCOMER_UI_MAX>					open_ui;			//自动开启界面 string
	string<SG_NEWCOMER_UI_MAX>					ui_condition;		//界面条件 string
	_U8											complete_type;		//完成类型
	string<SG_NEWCOMER_UI_MAX>					complete_param1;	//完成参数1 string
	_U32										complete_param2;	//完成参数2
	_U32										button_tip;			//按键提示
	string<SG_NEWCOMER_UI_MAX>					button_param1;		//指向按键参数1 string
	_U32										button_param2;		//指向按键参数2
	_U8											arrow_direction;	//箭头方向
	_U32										offset_x;			//偏移量X
	_U32										offset_y;			//偏移量Y
	string<SG_NEWCOMER_GUIDE_TIP_MAX>			tip_desc;			//提示框文字显示
	_U32										up;
	_U32										down;
	_U32										left;
	_U32										right;
};
task[GEN_STRUCT_SERIALIZE(SG_NEWCOMER_GUIDE_STEP_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_NEWCOMER_GUIDE_STEP_CONFIG)];

//daily search
struct SG_DAILY_SEARCH_CONFIG : A_CONTENT_OBJECT
{
	_U32										road_pt;			//路点ID
	_U8											road_type;			//路点类型
	_U32										param;				//类型参数
};
task[GEN_STRUCT_SERIALIZE(SG_DAILY_SEARCH_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_DAILY_SEARCH_CONFIG)];

struct SG_DAILY_SEARCH_REWARD_CONFIG : A_CONTENT_OBJECT
{
	_U32										circle_num;			//圈数
	_U32										reward_energy;		//体力奖励
	_U32										reward_rmb;			//元宝奖励
	_U32										reward_item1;		//道具奖励1
	_U32										reward_item1_count;	//道具数量1
	_U32										reward_item2;		//道具奖励2
	_U32										reward_item2_count;	//道具数量2
};
task[GEN_STRUCT_SERIALIZE(SG_DAILY_SEARCH_REWARD_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_DAILY_SEARCH_REWARD_CONFIG)];


struct SG_ZHANJIANG_LEVEL_CONFIG : A_CONTENT_OBJECT
{
	_U32										level_id;			//关卡
	//_U32										general_id;			//武将
	string<SG_TEX_MAX>							general_tex;		//武将tex
	_U32										U;					
	_U32										V;					
	_U32										UL;
	_U32										VL;
			
	string<SG_ZHANJIANG_LEVEL_LD_MAX>			LD_map;				//对应LD地图
	_U32										reward_gold;		//铜钱奖励
	_U32										reward_exp;			//经验奖励
	_U32										reward_reputation;	//奖励军功

	_U32										reward_item1;		//道具奖励1
	_F32										reward_rate1;		//道具几率1
	_U32										reward_item2;		//奖励2
	_F32										reward_rate2;		//几率2
	_U32										reward_item3;		//奖励3
	_F32										reward_rate3;		//几率3

};
task[GEN_STRUCT_SERIALIZE(SG_ZHANJIANG_LEVEL_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_ZHANJIANG_LEVEL_CONFIG)];

//horse
struct SG_HORSE_CONFIG : A_CONTENT_OBJECT
{
	_U32										horse_level;		//坐骑等级
	string<SG_HORSE_NAME_MAX>					horse_name;			//坐骑名字
	_U8											horse_star;			//坐骑星级
	_U32										req_exp;			//需求经验
	_U32										consume_gold;		//消耗铜钱
	_U32										gold_exp;			//铜钱经验

	_F32										gold_10_rate;		//铜钱十倍率
	_U32										consume_rmb;		//消耗元宝
	_U32										rmb_exp;			//元宝经验

	_F32										rmb_levelup_rate;	//元宝直升率
	_F32										rmb_10_rate;		//元宝十倍率

	_F32										restore_hp_rate;	//每秒回血

	_U32										soldier_hp;			//小兵生命
	_U32										soldier_atk;		//小兵攻击
	_U32										soldier_def;		//小兵防御

	_U32										general_hp;			//武将生命
	_U32										general_atk;		//武将攻击
	_U32										general_def;		//武将防御

	string<SG_HORSE_MESH_MAX>					mesh;				//坐骑mesh
	string<SG_TEX_MAX>							tex;				//icon tex
	_U32										U;					
	_U32										V;					
	_U32										UL;
	_U32										VL;
};
task[GEN_STRUCT_SERIALIZE(SG_HORSE_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_HORSE_CONFIG)];

struct SG_HORSE_SETTING_CONFIG : A_CONTENT_OBJECT
{
	_U32										unlock_level;		//解锁等级
	string<SG_HORSE_NAME_MAX>					horse_name;			//坐骑名字
	string<SG_HORSE_DESC_MAX>					desc;				//描述文字
	string<SG_TEX_MAX>							tex;				//坐骑tex
	_U32										U;					
	_U32										V;					
	_U32										UL;
	_U32										VL;
};
task[GEN_STRUCT_SERIALIZE(SG_HORSE_SETTING_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_HORSE_SETTING_CONFIG)];


//命魂
struct SG_ASTROLOGER_CONFIG : A_CONTENT_OBJECT
{
	_U32								astrologer_id;				//占星师ID
	string<SG_ASTROLOGER_NAME_MAX>		astrologer_name;			//名字
	_U32								level;						//等级
	string<SG_ASTROLOGER_DESC_MAX>		desc;						//描述
	_U32								consume_gold;				//花费金币/铜钱
	_F32								level_up_percent;			//占星level提升概率
	_F32								failed_percent;				//占星归1概率
	
	_U32								reward1;					//1品质命魂道具组ID
	_F32								reward_percent1;			//出现概率1
	
	_U32								reward2;					//2品质命魂道具组ID
	_F32								reward_percent2;			//出现概率2
	
	_U32								reward3;					//3品质命魂道具组ID
	_F32								reward_percent3;			//出现概率3
	
	_U32								reward4;					//4品质命魂道具组ID
	_F32								reward_percent4;			//出现概率4
	
	_U32								reward5;					//5品质命魂道具组ID
	_F32								reward_percent5;			//出现概率5
	
	_U32								U;					
	_U32								V;
	_U32								UL;
	_U32								VL;
	
	string<SG_TEX_MAX>					tex;						//纹理

	_U32								U2;					
	_U32								V2;
	_U32								UL2;
	_U32								VL2;
	
	string<SG_TEX_MAX>					tex2;						//纹理

};
task[GEN_STRUCT_SERIALIZE(SG_ASTROLOGER_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_ASTROLOGER_CONFIG)];

struct SG_GENERAL_ASTROLOG_SLOT_CONFIG : A_CONTENT_OBJECT
{
	_U32								general_level_upper;		//武将等级上限
	_U32								general_level_lower;		//武将等级下限
	_U32								slot_num;					//命魂格数
};
task[GEN_STRUCT_SERIALIZE(SG_GENERAL_ASTROLOG_SLOT_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_GENERAL_ASTROLOG_SLOT_CONFIG)];

//friend
struct SG_FRIEND_NUM_CONFIG : A_CONTENT_OBJECT
{
	_U32								player_level;				//玩家等级
	_U32								upper_friend_limit;			//好友数量
};
task[GEN_STRUCT_SERIALIZE(SG_FRIEND_NUM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_FRIEND_NUM_CONFIG)];

//energy
struct SG_ENERGY_CONFIG : A_CONTENT_OBJECT
{
	_U32								restore_energy;				//每半小时恢复体力值
	_U32								buy_unit_energy;			//每次购买体力值
	_U32								reset_time;					//每日体力更新时间
	_U32								consume_pve;				//pve每次扣除体力值
};
task[GEN_STRUCT_SERIALIZE(SG_ENERGY_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_ENERGY_CONFIG)];

struct SG_ENERGY_BUY_CONFIG : A_CONTENT_OBJECT
{
	_U32								times;						//购买的次数
	_U32								consume_rmb;				//购买所花费的体力
};
task[GEN_STRUCT_SERIALIZE(SG_ENERGY_BUY_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_ENERGY_BUY_CONFIG)];

//chat
struct SG_CHAT_CHANNEL_CONFIG : A_CONTENT_OBJECT
{
	_U32								channel_id;					//频道ID
	string<SG_CHAT_CHANNEL_NAME_MAX>	channel_name;				//频道名称
	_U8									scope;						//范围
	_U8									shield;						//屏蔽情况
	_U32								interval_times;				//间隔次数
	_U32								interval_period;			//发话间隔
	_U32								req_level;					//等级要求
	_U32								content_length_max;			//内容长度限制
	string<SG_CHAT_CHANNEL_COLOR_MAX>   color;						//描边颜色
};
task[GEN_STRUCT_SERIALIZE(SG_CHAT_CHANNEL_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_CHAT_CHANNEL_CONFIG)];

struct SG_CHAT_TAB_CONFIG : A_CONTENT_OBJECT
{
	_U32								tab_id;
	string<SG_CHAT_TAP_NAME_MAX>		tab_name;
	_U32								channel_id1;
	_U32								channel_id2;
	_U32								channel_id3;
	_U32								channel_id4;
	_U32								channel_id5;

	_U32								channel_id6;
	_U32								channel_id7;
	_U32								channel_id8;
	_U32								channel_id9;
	_U32								channel_id10;

	_U32								channel_id11;
	_U32								channel_id12;
	_U32								channel_id13;
	_U32								channel_id14;
	_U32								channel_id15;
};
task[GEN_STRUCT_SERIALIZE(SG_CHAT_TAB_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_CHAT_TAB_CONFIG)];


//turbo 无双
struct SG_TURBO_CONFIG : A_CONTENT_OBJECT
{
	_U32						general_id;							//General_ID
	_U32						turbo_level;						//无双等级
	string<SG_SKILL_NAME_MAX>	skill_name;							//无双技能名称
	_U8							charpter_id;						//所属篇章ID
	string<SG_TURBO_CHARPTER_NAME_MAX>	charpter_name;				//所属篇章名称
	_U32						req_wake_pt;						//所需觉醒点
	_U8							skill_type;							//技能类型 0-被动 1-主动
	_U32						HP;									//生命加成
	_U32						POW;								//力量加成
	_U32						INT;								//智力加成
	_F32						ATK;								//攻击力加成%
	_F32						DEF;								//护甲加成%
	_F32						HIT;								//命中加成%
	_F32						EVASION;							//闪避加成%
	_F32						CRIT;								//暴击率加成%
	_F32						X;									//无双点X坐标
	_F32						Y;									//无双点Y坐标
	string<ARCHETYPE_URL_LENGTH_MAX>	skill_archetype;			//主动解锁技能archettype
	string<SG_SKILL_DESC_MAX>			skill_desc;					//解锁技能描述
};
task[GEN_STRUCT_SERIALIZE(SG_TURBO_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_TURBO_CONFIG)];

struct SG_TURBO_SKILL_SLOT
{
	string<ARCHETYPE_URL_LENGTH_MAX>	skill_archetype1;
	string<ARCHETYPE_URL_LENGTH_MAX>	skill_archetype2;
	string<ARCHETYPE_URL_LENGTH_MAX>	skill_archetype3;
};
task[GEN_STRUCT_SERIALIZE(SG_TURBO_SKILL_SLOT)];
task[GEN_STRUCT_REFLECT(SG_TURBO_SKILL_SLOT)];

//Vip
struct SG_VIP_CONFIG : A_CONTENT_OBJECT
{
	_U32						vip_level;						//vip等级
	_U32						recharge_rmb;					//所需充值rmb
	_U32						pay_times;						//可领取军饷次数
	_U32						energy_times;					//可购买体力次数
	_U32						energy_upper;					//体力上限
	_U32						reset_instance_times;			//可重置副本次数
	_U8							hangup_free;					//挂机免费 0-否 1-是
	_U8							enhance_free;					//强化免费 0-否 1-是
	_U32						enhance_friend_num;				//vip好友上限增加量
	_U8							auto_atrology;					//占星vip是否开启
	string<SG_VIP_DESC_MAX>     desc;							//描述

	string<SG_VIP_ICON_MAX>				res;					//图标资源
	_U32								U;					
	_U32								V;
	_U32								UL;
	_U32								VL;
};
task[GEN_STRUCT_SERIALIZE(SG_VIP_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_VIP_CONFIG)];

//Pay 
struct SG_PAY_CONSUME_CONFIG : A_CONTENT_OBJECT
{
	_U32						apply_times;					//领取次数 从101到最后都是一样数据
	_U32						consume_rmb;					//花费rmb
	_F32						reward_rate;					//系数
};
task[GEN_STRUCT_SERIALIZE(SG_PAY_CONSUME_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_PAY_CONSUME_CONFIG)];

struct SG_PAY_REWARD_CONFIG : A_CONTENT_OBJECT
{
	_U32						player_level;					//玩家等级
	_U32						reward_gold;					//每次获得金币
};
task[GEN_STRUCT_SERIALIZE(SG_PAY_REWARD_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_PAY_REWARD_CONFIG)];

//instance
struct SG_INSTANCE_CONFIG : A_CONTENT_OBJECT
{
	_U32								instance_id;			//副本ID
	_U8									total_maps;				//副本地图个数
	string<SG_INSTANCE_REWARD_DES_MAX>	reward_normal1;			//普通难度小关奖励信息
	string<SG_INSTANCE_REWARD_DES_MAX>	reward_normal2;			//普通难度大关奖励信息
	string<SG_INSTANCE_REWARD_DES_MAX>	reward_hard1;			//困难难度小关奖励信息
	string<SG_INSTANCE_REWARD_DES_MAX>	reward_hard2;			//困难难度大关奖励信息
	_U32								reset_rmb;				//重置的花费
	_U32								wake_pt;				//觉醒点
};
task[GEN_STRUCT_SERIALIZE(SG_INSTANCE_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_INSTANCE_CONFIG)];

struct SG_INSTANCE_INFO : A_LIVE_OBJECT
{
	_U32								instance_id;			//副本ID
	_U8									difficulty;				//难度 0-普通 1-困难
	_S8									progress;				//进度 -1 - 未进入副本 0-进入副本未完成任意小关 1-已完成小关数目
	_U8									num_today;				//今日已经挑战次数
	_U8									normal_completed;		//普通难度是否已过 0-未过 1-已过
	_S8									furthest_normal;		//普通难度最远进度
	_S8									furthest_hard;			//困难难度最远进度
};
task[GEN_STRUCT_SERIALIZE(SG_INSTANCE_INFO)];
task[GEN_STRUCT_REFLECT(SG_INSTANCE_INFO)];

//boss rush
struct SG_BOSSRUSH_INFO : A_LIVE_OBJECT
{
	_U32								level_id;				//关卡id
	_U32								boss_hp;				//当前boss血量
	_U32								next_reset_time;		//下次重置时间
	_U8									status;					//状态 0-Initial 1-started 2-waiting for support 3-canceled 4-ready to reward 5-Rewarded

	array<_U8, 128>						daily_progress;			//0- daily havent 1-have	
};
task[GEN_STRUCT_SERIALIZE(SG_BOSSRUSH_INFO)];
task[GEN_STRUCT_REFLECT(SG_BOSSRUSH_INFO)];

struct SG_BOSSRUSH_SUPPORT_INFO : A_LIVE_OBJECT
{
	_U32								friend_id;				//好友id
	string<SG_PLAYER_NAME_MAX>			friend_nick;
	_U32								level_id;		
	_U32								boss_hp;				//boss剩余血量
	_U8									status;					//状态 0-waiting for support; 1-supported; 2-canceled; 3-ready to reward; 4-rewarded; 
	_U32								next_reset_time;		//下次重置时间
};
task[GEN_STRUCT_SERIALIZE(SG_BOSSRUSH_SUPPORT_INFO)];
task[GEN_STRUCT_REFLECT(SG_BOSSRUSH_SUPPORT_INFO)];

//league
struct SG_LEAGUE_CONFIG : A_CONTENT_OBJECT
{
	_U32								league_level;			//战盟等级
	_U32								XP;						//战盟经验
	_U32								members_count;			//成员数
	_F32								rmb_xp_rate;			//1金币兑换战盟经验数
	_F32								rmb_contribution_rate;	//1金币兑换战盟贡献数
	_F32								energy_xp_rate;			//1体力兑换战盟经验数
	_F32								energy_contribution_rate;///1体力兑换战盟贡献数
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_CONFIG)];

struct SG_LEAGUE_POW_CONFIG : A_CONTENT_OBJECT
{
	_U8									pow_type;				//权限类型 0-任命权限 1转让战盟权限 2解散战盟权限 3比准加入 4开除成员 5修改公告 6查看具体状态 7捐献 8参加战盟活动 9退出战盟
	_U8									master_pow;				//团长权限 0-无权 1-有权
	_U8									vice_master_pow;		//副团长权限 0-无权 1-有权
	_U8									member_pow;				//成员权限 0-无权 1-有权				
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_POW_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_POW_CONFIG)];

struct SG_LEAGUE_MEMBER_SETTING_CONFIG : A_CONTENT_OBJECT
{
	_U32								master_num;				//团长人数 应该永远是1
	_U32						        vice_master_num;		//副团长人数
	_U32								member_num;				//成员人数
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_MEMBER_SETTING_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_MEMBER_SETTING_CONFIG)];

struct SG_LEAGUE_ACTION_CONFIG : A_CONTENT_OBJECT
{
	_U32								action_id;				//活动id
	string<SG_DAILY_ACTION_NAME_MAX>	action_name;			//活动名字
	string<SG_DAILY_ACTION_DESC>		desc;					//活动描述
	_U32								sort_seq;				//显示顺序
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_ACTION_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_ACTION_CONFIG)];

struct SG_LEAGUE_TOAST_CONFIG : A_CONTENT_OBJECT
{
	_U8									wine_id;				//酒的id
	string<SG_LEAGUE_LOG_MAX>			wine_name;				//酒名字
	_U32								req_vid;				//vip等级要求
	_U32								consume_gold;			//消耗铜钱
	_U32								consume_rmb;			//消耗金币
	_U32								reward_reputation;		//军功(声望)奖励
	_U32								reward_league_xp;		//战盟经验
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_TOAST_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_TOAST_CONFIG)];

struct SG_LEAGUE_DIANJIANG_CONFIG : A_CONTENT_OBJECT
{
	_U32								consume_npc_gold;		//召唤npc所需金币
	_U32								reward_reputation;		//奖励军功声望
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_DIANJIANG_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_DIANJIANG_CONFIG)];

struct SG_LEAGUE_BATTLE_RANK_CONFIG : A_CONTENT_OBJECT
{
	_U32								server_level_lower;		//服务器等级上限
	_U32								server_level_upper;		//服务器等级下限
	_U32								lower;					//战盟排名下限
	_U32								upper;					//战盟排名上限
	
	_U32								reward_exp;				//经验奖励
	_U32								reward_gold;			//铜钱奖励
	_U32								reward_rmb;				//元宝奖励
	_U32								reward_reputation;		//奖励军功
	_U32								reward_contribution;	//贡献度奖励

	_U32								reward_item1;			//道具1奖励
	_U32								reward_item_count1;		//道具1数量
	_U32								reward_item2;			//道具2奖励
	_U32								reward_item_count2;		//道具2数量
	_U32								reward_item3;			//道具3奖励
	_U32								reward_item_count3;		//道具3数量
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_BATTLE_RANK_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_BATTLE_RANK_CONFIG)];

struct SG_LEAGUE_MEMBER_WIN_CONFIG : A_CONTENT_OBJECT
{
	_U32								server_level_lower;		//服务器等级上限
	_U32								server_level_upper;		//服务器等级下限

	_U32								win_times;				//连胜次数

	_U32								reward_exp;				//经验奖励
	_U32								reward_gold;			//铜钱奖励
	_U32								reward_rmb;				//元宝奖励
	_U32								reward_reputation;		//奖励军功
	_U32								reward_contribution;	//贡献度奖励

	_U32								reward_item1;			//道具1奖励
	_U32								reward_item_count1;		//道具1数量
	_U32								reward_item2;			//道具2奖励
	_U32								reward_item_count2;		//道具2数量
	_U32								reward_item3;			//道具3奖励
	_U32								reward_item_count3;		//道具3数量
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_MEMBER_WIN_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_MEMBER_WIN_CONFIG)];


//PVP事件记录
struct SG_PVP_RECORD_ITEM : A_LIVE_OBJECT
{
	_U32								challenger;
	_U32								defender;
	string<SG_PLAYER_NAME_MAX>			defender_name;			//名字
	_U8									type;					//0 - challenger战胜 defender 1-相反
	_U32								time;					//事件时间
	_U32								param;					//一般是排名
};
task[GEN_STRUCT_SERIALIZE(SG_PVP_RECORD_ITEM)];
task[GEN_STRUCT_REFLECT(SG_PVP_RECORD_ITEM)];


//PVP配置
struct SG_PVP_INFO_CONFIG : A_CONTENT_OBJECT
{
	_U32								server_level_lower;		//服务器等级上限
	_U32								server_level_upper;		//服务器等级下限
	_U32								lower;					//下限
	_U32								upper;					//上限
	_U32								span;					//挑战跨度
	_U32								daily_reputation;		//每日奖励声望
	_U32								reward_gold;			//每日铜钱奖励
	_U32								reward_item;			//每日道具奖励
	_U32								reward_item_count;		//道具数量
	_U32								succ_reputation;		//单场胜利奖励声望
	_U32								failed_reputation;		//单场失败扣除声望
};
task[GEN_STRUCT_SERIALIZE(SG_PVP_INFO_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_PVP_INFO_CONFIG)];

struct SG_PVP_SETTING_CONFIG : A_CONTENT_OBJECT
{
	_U32								increase_pay_rmb;		//提升次数所需元宝
};
task[GEN_STRUCT_SERIALIZE(SG_PVP_SETTING_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_PVP_SETTING_CONFIG)];

//Shop
struct SG_GOODS_CONFIG : A_CONTENT_OBJECT
{
	_U32								good_id;				//商品ID
	_U8									display_condition;		//显示条件/类型 0-无条件 1-官职需求 2-等级要求 3-VIP等级 4-战盟等级
	_U32								display_param1;			//条件参数1
	_U32								display_param2;			//条件参数2
	_U8									buy_condition;			//购买条件类型 0-无条件 1-官职需求 2-等级需求  3-VIP等级 4-战盟等级
	_U32								buy_param1;				//条件参数1
	_U32								buy_param2;				//条件参数2
	_U32								item_id;				//道具ID			
	_U32								count;					//道具数量
	_U8									price_condition;		//兑换消耗类型 0-无任何消耗 1-gold 2-rmb 3-战盟贡献 4-道具
	_U32								price_param1;			//消耗参数1
	_U32								price_param2;			//消耗参数2
	_U32								price_discount;			//打折消耗	
};
task[GEN_STRUCT_SERIALIZE(SG_GOODS_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_GOODS_CONFIG)];

struct SG_GOODS_GROUP_CONFIG : A_CONTENT_OBJECT
{
	_U32								goods_group_id;			//货品组ID
	_U32								good_id;				//商品ID
};
task[GEN_STRUCT_SERIALIZE(SG_GOODS_GROUP_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_GOODS_GROUP_CONFIG)];

struct SG_SHOP_CONFIG : A_CONTENT_OBJECT
{
	_U32								shop_id;				//商店ID
	string<SG_GOODS_GROUP_NAME_MAX>		goods_group_name;		//货品组名字
	_U32								goods_group_id;			//货品组ID
};
task[GEN_STRUCT_SERIALIZE(SG_SHOP_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_SHOP_CONFIG)];

//League
struct SG_LEAGUE : A_LIVE_OBJECT
{
	_U32								league_id;				//战盟ID

	string<SG_LEAGUE_NAME_MAX>			league_name;			//战盟名称
	_U32								rank;					//战盟排名
	_U32								member_num;				//目前成员数量
	_U32								level;					//等级
	_U32								exp;					//战盟经验
	_U32								create_time;			//创建时间
	_U32								owner_id;				//团长ID
	string<SG_PLAYER_NAME_MAX>			owner_name;				//团长名字
	string<SG_LEAGUE_NOTICE_MAX>		post_content;			//战盟公告
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE)];

struct SG_LEAGUE_MEMBER : A_LIVE_OBJECT
{
	_U32								league_id;				//所在战盟ID
	_U32								member_id;				//成员ID avatar_id
	_U32								general_id;				//成员general_id
	string<SG_PLAYER_NAME_MAX>			member_name;			//成员名字
	_U32								member_level;			//成员Level
	_U32								member_battle_value;	//成员战斗力
	_U32								pvp_rank;				//成员pvp排名

	_U8									position;				//所在战盟职位 0-普通成员 1-副团长 2-团长
	_U32								contribution_value;		//贡献值
	_U32								total_contribution_value;//总贡献值
	_U32								last_ol_time;			//上次用户活动的最后时间 表示是否在线
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_MEMBER)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_MEMBER)];

struct SG_LEAGUE_APPLYER : A_LIVE_OBJECT
{
	_U32								applyer_id;				//申请者ID avatar_id
	string<SG_PLAYER_NAME_MAX>			applyer_name;			//申请者名字
	_U32								general_id;				//申请者职业(主将ID)
	_U32								level;					//申请者等级
	_U32								rank;					//申请者pvp排名
	_U32								league_id;				//申请加入战盟的ID 
	_U8									reason;					//?
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_APPLYER)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_APPLYER)];

struct SG_LEAGUE_LOG : A_LIVE_OBJECT
{
	_U32								league_id;				
	_U32								result_time;			//该条log记录时间
	_U32								type;					//该条log的事件类型
	string<SG_LEAGUE_LOG_MAX>			log;					//log内容
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_LOG)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_LOG)];

struct SG_LEAGUE_DIANJIANG_JOINER : A_LIVE_OBJECT
{
	_U32								league_id;				//所在战盟ID
	_U32								member_id;				//成员ID avatar_id
	_U32								general_id;				//成员general_id
	string<SG_PLAYER_NAME_MAX>			member_name;			//成员名字
	_U8 								role;					//0-主动参与 1-协助
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_DIANJIANG_JOINER)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_DIANJIANG_JOINER)];

//光环
struct SG_HALO_CONFIG					: A_CONTENT_OBJECT 
{
	_U32								halo_level;				//光环等级
	_U32								EXP;					//升级所需经验
	_F32								ATK;					//提升ATK
	_F32								DEF;					//提升DEF
	_F32								HIT;					//提升命中(percent)
	_F32								EVASION;				//提升闪避(percent)
	_F32								CRIT;					//提升暴击(percent)
};
task[GEN_STRUCT_SERIALIZE(SG_HALO_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_HALO_CONFIG)];

//光环强化
struct SG_HALO_LEVELUP_CONFIG			: A_CONTENT_OBJECT
{
	_U32								level;					//强化选项 1~3
	_U32								EXP;					//获得经验
	_U32								CD;						//冷却时间
};
task[GEN_STRUCT_SERIALIZE(SG_HALO_LEVELUP_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_HALO_LEVELUP_CONFIG)];

//玩家军衔
struct SG_PLAYER_TITLE_CONFIG			: A_CONTENT_OBJECT
{
	_U8									title_level;			//军衔等级
	string<SG_PLAYER_TITLE_NAME_MAX>	title_name;				//军衔名称
	_U8									title_star;				//军衔星数
	_U32								req_level_up_reputaion;	//升级所需声望
	_U32								daily_reputation_deduct;//每日扣除声望
	_U8									general_limit_num;		//可拥有武将数量
	_U32								daily_salary_gold;		//每日俸禄奖励铜钱
	_U32								daily_reward_reserve;	//每日预留奖励
	_U32								STRAddition;			//主角STR叠加
	_U32								INTAddition;			//主角INT叠加
	_U32								HP_AddExtra;			//全军HP额外增加
	_U32								ATK_AddExtra;			//全军ATK额外增加
	_U32								DEF_AddExtra;			//全军DEF额外增加

	string<SG_TITLE_PROP_DESC>			employ_general_prop_desc;	//军衔属性描述
	string<SG_TITLE_GENERAL_DESC>		employ_general_desc;		//可招募武将描述
	
	string<SG_TITLE_ACRHTYPE_MAX>							wusheng_male;			//男武圣archtype
	string<SG_TITLE_ACRHTYPE_MAX>							wusheng_female;
	string<SG_TITLE_ACRHTYPE_MAX>							junshi_male;
	string<SG_TITLE_ACRHTYPE_MAX>							junshi_female;
	string<SG_TITLE_ACRHTYPE_MAX>							jianshen_male;
	string<SG_TITLE_ACRHTYPE_MAX>							jianshen_female;		//男箭神archtype

};
task[GEN_STRUCT_SERIALIZE(SG_PLAYER_TITLE_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_PLAYER_TITLE_CONFIG)];

//任务live
struct SG_QUEST_LIVE_INFO				: A_LIVE_OBJECT	
{
	_U32								quest_id;				//任务ID
	_U32								counter;				//任务计数器
	_U8									status;					//任务当前状态 0-not avaliable 1-avaliable 2-wip 3-ready to submit 4-finished
};
task[GEN_STRUCT_SERIALIZE(SG_QUEST_LIVE_INFO)];
task[GEN_STRUCT_REFLECT(SG_QUEST_LIVE_INFO)];

//任务信息配置
struct SG_QUEST_INFO_CONFIG				: A_CONTENT_OBJECT
{
	_U32								quest_id;				//任务id
	_U8									quest_type;				//任务类型 0-主线 1-支线
	string<SG_QUEST_NAME_MAX>			quest_name;				//任务名称
	string<SG_QUEST_GOAL_MAX>			quest_goal_desc;		//任务目标描述
	string<SG_QUEST_GOAL_DESC_MAX>		quest_goal_short_desc;	//任务目标简述

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
	_U32								reward_reputation;		//军功
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
	_U32								next_level;				//下一关的关卡配置id
	string<SG_DESCRIPTION_MAX>			description;			//关卡描述
	_U32								req_player_level;		//可解锁要求的玩家等级
	_U32								wake_pt;				//觉醒点
	
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

//脏词配置
struct SG_DIRTY_WORD_CONFIG				: A_CONTENT_OBJECT
{
	string<SG_PLAYER_NAME_MAX>			dirty_word;
};
task[GEN_STRUCT_SERIALIZE(SG_DIRTY_WORD_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_DIRTY_WORD_CONFIG)];

//起名配置
struct SG_PLAYER_RANDOM_NAME_CONFIG : A_CONTENT_OBJECT
{
	string<SG_PLAYER_NAME_MAX>			family_name;
	string<SG_PLAYER_NAME_MAX>			male_name;
	string<SG_PLAYER_NAME_MAX>			female_name;
};
task[GEN_STRUCT_SERIALIZE(SG_PLAYER_RANDOM_NAME_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_PLAYER_RANDOM_NAME_CONFIG)];

//掉落组配置
struct SG_LEVEL_DROP_CONFIG : A_CONTENT_OBJECT
{
	string<100>							level_name;
	_U32								exp;
	_U32								gold;
	_U32								reputation;
	_U32								level_id;	
	_U32								next_level;

	_U32								group1_id;
	_F32								group1_rate;
	_U32								group2_id;
	_F32								group2_rate;
	_U32								group3_id;
	_F32								group3_rate;
	_U32								group4_id;
	_F32								group4_rate;
	_U32								wake_pt;				//觉醒点
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

//掉落组
struct SG_DROP_ITEM_CONFIG : SG_DROP_ITEM_BASE
{
	_U32								group_id;
	_F32								rate;
};
task[GEN_STRUCT_SERIALIZE(SG_DROP_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_DROP_ITEM_CONFIG)];

//宝箱掉落
struct SG_LOOT_ITEM_CONFIG : A_CONTENT_OBJECT
{
	_U32								loot_id;
	_U32								num;
	_U32								group_id1;
	_U8									group_rate1;
	_U32								group_id2;
	_U8									group_rate2;
	_U32								group_id3;
	_U8									group_rate3;
	_U32								group_id4;
	_U8									group_rate4;
	_U32								group_id5;
	_U8									group_rate5;
	_U32								group_id6;
	_U8									group_rate6;
};
task[GEN_STRUCT_SERIALIZE(SG_LOOT_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_LOOT_ITEM_CONFIG)];

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

//物品配置
struct SG_ITEM_CONFIG : A_CONTENT_OBJECT
{
	_U32								item_id;			//物品id
	_U32								stack_max;			//叠加最大数
	_U32								price;				//出售价格
	
	_U32								U;					
	_U32								V;
	_U32								UL;
	_U32								VL;
	
	string<SG_TEX_MAX>					tex;				//纹理
	string<SG_ITEM_NAME_MAX>			item_name;			//物品名字
	_U8									quality;			//品质
};
task[GEN_STRUCT_SERIALIZE(SG_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_ITEM_CONFIG)];

//材料
struct SG_MATERIAL_CONFIG : SG_ITEM_CONFIG
{
	string<SG_MATERIAL_DESC_MAX>		desc;				//描述
	_U32								req_level;			//等级
};
task[GEN_STRUCT_SERIALIZE(SG_MATERIAL_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_MATERIAL_CONFIG)];

//装备配置
struct SG_EQUIPT_ITEM_CONFIG : SG_ITEM_CONFIG				//装备
{
	SG_ATTR_MOD_CONFIG					mod_config;
	_U8									item_type;			//装备类型
	_F32								quality_rate;		//品质系数
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

//可使用配置
struct SG_USABLE_ITEM_CONFIG : SG_ITEM_CONFIG				
{
	string<SG_USABLE_DESC_MAX>			desc;				//描述
	_U8									salablity;			//是否可出售 0-否 1-是
	_U32								req_level;			//等级要求
	_U8									usage_type;			//使用类型
	_F32								attri_value1;		//属性值1
	_F32								attri_value2;		//属性值2
	_F32								attri_value3;		//属性值3
};
task[GEN_STRUCT_SERIALIZE(SG_USABLE_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_USABLE_ITEM_CONFIG)];

//宝石配置
struct SG_GEM_ITEM_CONFIG : SG_ITEM_CONFIG					
{
	string<SG_GEM_DESC_MAX>		desc;						//宝石描述
	_U8							level;						//宝石等级
	_U8							attri_type;					//宝石属性
	_F32						attri;						//宝石属性值
	_U32						compose_next;				//合成的宝石ID
	_U32						compose_num;				//合成需要的数量
};
task[GEN_STRUCT_SERIALIZE(SG_GEM_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_GEM_ITEM_CONFIG)];

//命魂
struct SG_ASTROLOGY_BALL_CONFIG : SG_ITEM_CONFIG
{
	_U32								level;				//命魂等级
	string<SG_ASTROLOGY_BALL_DESC_MAX>	desc;				//命魂描述
	_U8									attri_type;			//属性枚举 加什么属性
	_F32								attri_num;			//属性数值 加多少
	_U32								levelup_ball;		//升级后的命魂ID
	_U32								req_levelup;		//升级所需的魂力值
	_U32								swllaow_obtain;		//命魂吞噬后所得的魂力值
	string<ARCHETYPE_URL_LENGTH_MAX>	archetype;			//archetype
};
task[GEN_STRUCT_SERIALIZE(SG_ASTROLOGY_BALL_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_ASTROLOGY_BALL_CONFIG)];


//敌人配置
struct SG_ENEMY_CONFIG : A_CONTENT_OBJECT
{
	_U32								enemy_id;			//敌人id
	SG_PAWN_CONFIG						PawnConfig;
};
task[GEN_STRUCT_SERIALIZE(SG_ENEMY_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_ENEMY_CONFIG)];

//Soldier等级属性配置
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

//General等级属性配置
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

//Soldier配置
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

//General配置
struct SG_GENERAL_CONFIG : A_CONTENT_OBJECT
{
	_U32								general_id;
	_U32								attr_id;

	_U8									type;				//武将类型
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
	_U8									atk_type;			//攻击类型
	_U8									def_type;			//防御类型
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
	string<SG_PLAYER_NAME_MAX>			Nick;
	_U32								Avatar_ID;			//如果是查询则返回查询时带入的avatar_id
	_U32								horse_level;
	_U32								halo_level;
};
task[GEN_STRUCT_SERIALIZE(SG_PLAYER_PVE)];
task[GEN_STRUCT_REFLECT(SG_PLAYER_PVE)];

//装备Slot
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
	array<_U32, 6>						minghun_slots;		//命魂槽 ball_id
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


//daily action
struct SG_DAILY_ACTION_CONFIG			: A_CONTENT_OBJECT
{
	_U32								action_id;
	string<SG_DAILY_ACTION_NAME_MAX>	action_name;			//活动名称
	_U8									display_position;		//右上角显示位置

	_U8									type;					//活动类型 0-日常玩法 世界活动 战盟活动
	_U32								req_league_level;		//要求的战盟等级
	_U32								req_level;				//要求的玩家等级

	_U8									condition1_type;		//条件一类型
	_U32								condition1_param1;		//条件一参数一
	
	_U8									condition2_type;        //条件二类型
	_U32								condition2_param1;		//条件二参数一

	_U32								hour;					//开启时间
	_U32								min;					//开启时间

	_U32								end_hour;				//结束时间
	_U32								end_min;				//结束时间

	_U32								times;					//次数

	_U32								prepare_min;			//准备时间min

	_U32								reset_hour;				//重置时间
	_U32								reset_min;				//重置时间

	string<SG_ICON_MAX>					icon;					//图标资源文件
	_U32								U;					
	_U32								V;
	_U32								UL;
	_U32								VL;
	_U32								sort_seq;				//显示排序使用
};
task[GEN_STRUCT_SERIALIZE(SG_DAILY_ACTION_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_DAILY_ACTION_CONFIG)];

struct SG_DAILY_ACTION_DESC_CONFIG : A_CONTENT_OBJECT
{
	_U32								action_id;				//活动id
	string<SG_DAILY_ACTION_NAME_MAX>	action_name;			//活动名字
	string<SG_DAILY_ACTION_DESC>		desc;					//活动描述
	_U8									display_type;			//显示分类
	_U32								sort_seq;				//显示排序使用
};
task[GEN_STRUCT_SERIALIZE(SG_DAILY_ACTION_DESC_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_DAILY_ACTION_DESC_CONFIG)];

struct SG_DAILY_ACTION_INFO				: A_LIVE_OBJECT
{
	_U32								action_id;				//1001-俸禄 1-每日关卡可进行 1002-pvp 1004-pvp每日奖励 1003-每日军饷 1007-每日购买体力的次数 2002-boss rush
	_U32								times;					//剩余可领取俸禄 1-每日可进行关卡剩余次数 -pvp剩余次数 -pvp每日奖励可领次数 -每日军饷已领取次数
	_U32								reset_time;				//俸禄冷却时间 1-重置的时间标签 -重置的时间标签 -重置的时间标签 -重置的时间标签
};
task[GEN_STRUCT_SERIALIZE(SG_DAILY_ACTION_INFO)];
task[GEN_STRUCT_REFLECT(SG_DAILY_ACTION_INFO)];

struct SG_PLAYER : SG_GENERAL
{
	string<SG_PLAYER_NAME_MAX>			nick;					//昵称
	_U32								avatar_id;				//相当于userid
	_U32								total_rmb;				//总充值点数
	_U32								gold;					//游戏币
	_U32								rmb;					//元宝
	array<_U32, 2>						equip_generals;
	array<_U32, 3>						equip_soldiers;
	_U8									increase_equipt_times;	//提高过冷却时间次数
	_U8									title;					//官职
	_U32								reputation;				//军功
	_U32								energy;					//体力
	
	_U32								halo_level;				//光环等级
	_U32								halo_exp;				//光环经验

	array<SG_DAILY_ACTION_INFO, SG_DAILY_ACTION_TYPE_MAX>		daily_actions;			//日常行为
	_U32								next_level;				//下一个通过的关卡ID 初始值10001
	_U32								rank;					//排行榜排名
	_U32								last_rank;				//结算时候的排名
	_U32								vip_level;				//vip等级
	
	_U32								league_id;				//所在战盟ID
	array<_U32, SG_LEAGUE_APPLY_MAX>	league_apply_list;		//申请

	_U32								wake_pt;				//觉醒点
	_U32								turbo_level;			//无双等级
	array<string<ARCHETYPE_URL_LENGTH_MAX>, SG_TURBO_UNLOCK_SKILL_MAX>					skills;					//已经解锁的技能 最多15
	SG_TURBO_SKILL_SLOT																	turbo_skill_slot;		//目前已经选中的技能 最多3

	_U32								last_operation_time;	//最后一次操作时间
	string<SG_MAP_URL_MAX>				last_town_map;			//最后一次的地图url

	_U32								minghun_value;			//魂力值
	_U32								current_zhanxing_value;	//目前的占星等级

	_U32								horse_exp;
	_U32								horse_level;

	_U32								furthest_level;			//boss rush
	_U32								xunbao_circle;			//daily search
	_U32								xunbao_step;			
};
task[GEN_STRUCT_SERIALIZE(SG_PLAYER)];
task[GEN_STRUCT_REFLECT(SG_PLAYER)];

//simple player info
struct SG_PLAYER_INFO : SG_GENERAL
{
	string<SG_PLAYER_NAME_MAX>			nick;					//昵称
	_U32								avatar_id;				//相当于userid
	_U8									title;					//官职
	_U32								reputation;				//军功
	_U32								rank;					//排行榜排名
	_U32								vip_level;				//vip等级
	_U32								league_id;				//所在战盟ID
	_U32								horse_level;			
};
task[GEN_STRUCT_SERIALIZE(SG_PLAYER_INFO)];
task[GEN_STRUCT_REFLECT(SG_PLAYER_INFO)];

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
	SG_ATTR_MOD_CONFIG							mod_config;			
	_U8											turbo_level;		//当前强化等级
	array<_U32, SG_EUQIPT_GEM_SLOTS_MAX>		gems_slots;			//镶嵌的宝石
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
	_U8										status;					//宝石是否镶嵌 0-未镶嵌 1-镶嵌
};
task[GEN_STRUCT_SERIALIZE(SG_GEM_ITEM)];
task[GEN_STRUCT_REFLECT(SG_GEM_ITEM)];

struct SG_MATERIAL_ITEM : SG_ITEM
{
};
task[GEN_STRUCT_SERIALIZE(SG_MATERIAL_ITEM)];
task[GEN_STRUCT_REFLECT(SG_MATERIAL_ITEM)];

struct SG_ASTROLOGY_BALL_ITEM : SG_ITEM
{
};
task[GEN_STRUCT_SERIALIZE(SG_ASTROLOGY_BALL_ITEM)];
task[GEN_STRUCT_REFLECT(SG_ASTROLOGY_BALL_ITEM)];

//friend live
struct SG_FRIEND_BASE_INFO : A_LIVE_OBJECT
{
	_U32									avatar_id;				//avatar_id
	string<SG_PLAYER_NAME_MAX>				nick;
	_U32									title;
	_U32									level;
	_U32									battle_value;
};
task[GEN_STRUCT_SERIALIZE(SG_FRIEND_BASE_INFO)];
task[GEN_STRUCT_REFLECT(SG_FRIEND_BASE_INFO)];

struct SG_FRIEND_INFO : SG_FRIEND_BASE_INFO
{
	_U32									recent_op_time;			//在线时间戳
};
task[GEN_STRUCT_SERIALIZE(SG_FRIEND_INFO)];
task[GEN_STRUCT_REFLECT(SG_FRIEND_INFO)];

struct SG_FRIEND_SEARCH_INFO : SG_FRIEND_BASE_INFO
{
	_U32									current_friend_num;		//对方现在的好友数量
	_U32									max_friend_num;			//对方最多的好友数量
};
task[GEN_STRUCT_SERIALIZE(SG_FRIEND_SEARCH_INFO)];
task[GEN_STRUCT_REFLECT(SG_FRIEND_SEARCH_INFO)];

struct SG_FRIEND_INVITATION : SG_FRIEND_BASE_INFO
{
	_U32									recent_op_time;			//在线时间戳	
}
task[GEN_STRUCT_SERIALIZE(SG_FRIEND_INVITATION)];
task[GEN_STRUCT_REFLECT(SG_FRIEND_INVITATION)];

//Misc
struct SG_SERVER_INFO
{
	_U32								server_id;
	string<100>							server_name;
	_U32								server_state;
	string<100>							avatar_nick;
	_U32								general_id;
	_U32								level;						//角色在本服等级
	_U32								server_level;				//本服等级 与人数和pvp相关
};
task[GEN_STRUCT_SERIALIZE(SG_SERVER_INFO)];
task[GEN_STRUCT_REFLECT(SG_SERVER_INFO)];

struct SG_GENERAL_QUERY_INFO
{
	SG_GENERAL							general;
	SG_GENERAL_CONFIG					general_config;
	SG_GENERAL_LEVEL_CONFIG				level_config;
	SG_PAWN_CONFIG						PawnConfig;				
};
task[GEN_STRUCT_SERIALIZE(SG_GENERAL_QUERY_INFO)];
task[GEN_STRUCT_REFLECT(SG_GENERAL_QUERY_INFO)];

//chat
struct SG_CHAT_SEND_INFO : A_LIVE_OBJECT
{
	_U32 								channel_id;
	_U32								receiver_id;
	string<SG_PLAYER_NAME_MAX>			receiver_nick;
	_U32 								time;
	string<SG_CHAT_CONTENT_MAX>			content;
};
task[GEN_STRUCT_SERIALIZE(SG_CHAT_SEND_INFO)];
task[GEN_STRUCT_REFLECT(SG_CHAT_SEND_INFO)];

struct SG_CHAT_RECV_INFO : A_LIVE_OBJECT
{
	_U32 								channel_id;
	_U32								sender_id;
	string<SG_PLAYER_NAME_MAX>			sender_nick;
	_U32 								time;
	string<SG_CHAT_CONTENT_MAX> 		content;
};
task[GEN_STRUCT_SERIALIZE(SG_CHAT_RECV_INFO)];
task[GEN_STRUCT_REFLECT(SG_CHAT_RECV_INFO)];

//pvp
struct SG_PVP_HEROLIST_PLAYER : A_LIVE_OBJECT
{
	string<SG_PLAYER_NAME_MAX>			nick;
	_U32 								level;
	_U32								rank;
};
task[GEN_STRUCT_SERIALIZE(SG_PVP_HEROLIST_PLAYER)];
task[GEN_STRUCT_REFLECT(SG_PVP_HEROLIST_PLAYER)];

struct SG_PVP_RANK_PLAYER : A_LIVE_OBJECT
{
	_U32 								avatar_id;
	_U32								general_id;		
	string<SG_PLAYER_NAME_MAX>			nick;
	_U32 								level;
	_U32								rank;
};
task[GEN_STRUCT_SERIALIZE(SG_PVP_RANK_PLAYER)];
task[GEN_STRUCT_REFLECT(SG_PVP_RANK_PLAYER)];

//newcomer guide
struct SG_NEWCOMER_GUIDE_INFO : A_LIVE_OBJECT
{
	_U32								function_id;
	_U8									status;					//0-not never exist in list 1-activated 2- finished
};
task[GEN_STRUCT_SERIALIZE(SG_NEWCOMER_GUIDE_INFO)];
task[GEN_STRUCT_REFLECT(SG_NEWCOMER_GUIDE_INFO)];

//world boss
struct SG_WORLDBOSS_RANK_ITEM
{
	string<SG_PLAYER_NAME_MAX>			nick;
	_U32								rank;
	_U32								total_damage;
};
task[GEN_STRUCT_SERIALIZE(SG_WORLDBOSS_RANK_ITEM)];
task[GEN_STRUCT_REFLECT(SG_WORLDBOSS_RANK_ITEM)];

struct SG_WORLDBOSS_RANK_INFO
{
	array<SG_WORLDBOSS_RANK_ITEM, SG_WORLDBOSS_RANK_LIST_MAX> last_rank_list;
	SG_WORLDBOSS_RANK_ITEM									  my_last_rank;
	_U8														  attendance_reward;
	_U8														  rank_reward;
};
task[GEN_STRUCT_SERIALIZE(SG_WORLDBOSS_RANK_INFO)];
task[GEN_STRUCT_REFLECT(SG_WORLDBOSS_RANK_INFO)];

struct SG_WORLDBOSS_INFO
{
	_U32								boss_id;
	_U32								HP;
	_U8									status;
};
task[GEN_STRUCT_SERIALIZE(SG_WORLDBOSS_INFO)];
task[GEN_STRUCT_REFLECT(SG_WORLDBOSS_INFO)];

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
	QueryServerTime();												//查询服务器时间

	QueryPlayer(_U8 nSync);											//0-nSync 该query是一次查询，返回时直接调用callback 1-是同步操作视所有同步操作情况选择是否callback通知
	QueryGenerals(_U8 nSync);
	QuerySoldiers(_U8 nSync);
	QueryBag(_U8 nSync);
	QueryOtherPlayers(string current_town_map, _U32 num);

	EquipItem(_U32 general_id, SG_EQUIP_SLOTS slots);
	EquipGenerals(_U32 generals[count], _U32 count);
	EquipSoldiers(_U32 soldiers[count], _U32 count);

	UnLockSoldier(_U32 soldier_id);
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
	EndBattle(string name, _U32 result, _U8 auto_combat);			//auto_combat 0-no  1-yes

	EquipGem(A_UUID item_uuid, _U32 gem_id);
	UnequipGem(A_UUID item_uuid, _U32 gem_id);
	GemCombine(_U32 item_id);

	SetGeneralStatus(_U32 general_id, _U8 status);					//武将招募
	
	QueryPlayerQuest(_U8 nSync);									//查询玩家任务信息
	SaveQuestData(_U32 quest_id, _U32 counter, _U8 status);			//更改任务状态
	FinishQuest(_U32 quest_id, _U32 rmb);							//提交完成任务

	HaloCoolDown();													//立刻冷却
	HaloIncreaseEXP(_U8 halo_type);									//升级光环
	HaloGetCoolDown();												//获取光环冷却时间

	GetPaid();														//daily salary
	UpgradeTitle();													//军衔提升
	
	BuyGoods(_U32 item_id);											//商店购买
	SellItem(A_UUID uuid, _U32 item_id, _U32 count);				//卖出

	QueryPlayerPVPInfo(_U32 avatar_id);								//获取player pvp信息
	QueryPlayerRankList();											//获取pvp排行

	PVPCoolDown();													//挑战冷却时间
	PVPGetRestTime();												//挑战剩余次数
	PVPRecord();													//对战记录			
	PVPHeroList();													//英雄榜			
	PVPDailyReward();												//pvp每日奖励之类
	PVPBattleBegin(_U32 defender);									//pvp战斗开始 
	PVPBattleEnd(_U32 defender, _U8 ret);							//pvp战斗结束 0-succ 1-failed
	PVPCoolDownClear();												//pvp清除挑战冷却时间
	PVPIncreateBattleTimes();										//pvp增加挑战次数

	QueryInstance();												//副本
	EnterInstance(_U32 instance_id, _U8 difficulty);				//进入副本 0-普通 1-困难
	BeginInstanceBattle(_U32 instance_id, string map_url);			//开始副本战斗
	EndInstanceBattle(_U32 instance_id, string map_url, _U32 result, _U8 auto_combat);//结束副本战斗 auto_combat 0-no  1-yes
	ResetInstance(_U32 instance_id);
	SaveLastTownMap(string last_town_map);							//保存最后一次的地图信息

	CreateLeague(string league_name);								//战盟 创建
	ApplyJoinLeague(_U32 league_id);								//申请加入战盟		
	QueryLeagueApplyList(_U32 league_id);							//查询当前申请加入战盟的人
	QueryLeague(_U32 league_id);									//查询league_id战盟信息
	QueryLeagueList();												//查询当前所有战盟的列表
	QueryLeagueMemberList(_U32 league_id);							//查询league_id战盟当前成员
	QueryLeagueMemberInfo(_U32 member_id);							//显示成员选中tips
	
	ContributeLeague(_U32 rmb, _U32 energy);						//战盟捐献
	HandleApply(_U32 applyer_id, _U8 allowed);						//处理加入战盟申请 bAllowed 同意- 1 拒绝-0
	QueryLeagueNotice();											//查询战盟公告
	SetLeagueNotice(_U32 league_id, string notice_content);
	SetLeagueOwner(_U32 member_id);									//转交战盟
	DissolveLeague();												//解散
	SetMemberPosition(_U32 member_id, _U8 position);				//升职 0-普通成员 1-副团长 2-团长
	DismissMember(_U32 member_id);									//开除
	ExitLeague();													//退出战盟
	QueryLeagueLog();												//战盟日志
	LeagueToast(_U8 wine_id);										//战盟置酒

	SalaryGet();													//获取每日军饷
	SalaryGetBat();													//批量获取 max = 10
	BuyEnergy();													//购买体力

	EnhanceTurbo();													//提升无双技能
	EquipTurboSkill(SG_TURBO_SKILL_SLOT skill_slot);				//装备无双技能

	MakeEquipt(_U32 equipt_id);										//装备打造

	QueryActionAvailable(_U32 action_list[count], _U32 count);		//判断活动是否可以进入/激活

	QueryFriendList();												//获取好友列表
	QueryFriendSearchList(string nick);								//查询可以加为好友的列表
	QueryFriendInvitationList();									//查询自己被邀请的列表
	InviteFriend(_U32 avatar_id);									//邀请好友
	AcceptFriend(_U32 avatar_id);									//同意加为好友
	RefuseInvitation(_U32 avatar_id);								//拒绝加为好友
	DelFriend(_U32 avatar_id);										//删除好友

	SendChat(SG_CHAT_SEND_INFO send_info);							//发送消息
	ReceiveChat(_U32 channel_list[count], _U32 count);				//接受消息
	
	QueryLeagueDianjiangList(_U32 league_id);						//获取当前点将的列表
	JoinLeagueDianjiang(_U32 league_id);							//参加点将
	QueryLeagueDianjiangReward(_U32 league_id, _U32 avatar_id);		//查询点将奖励
	LeagueDianjiangSummonNPC(_U32 league_id);						//招募npc参加点将活动

	QueryAstrologyBag();												//查询命魂
	SaveToBag(_U32 item_id);											//存入命魂包中
	SetAstrologyBallStatus(_U32 general_id, _U32 ball_id, _U8 status);	//0-卸下 1-装备
	EnhanceAstrologyBall(_U32 ball_id, _U32 general_id, _U32 index);	//升级命魂 如果general_id 不为0则是升级装备在武将身上的id
	Strology(_U32 astrologer_id);										//占星
	StrologyAuto(_U32 RestSlotCount);									//一键占星
	Devour(_U8 bag_type);												//一键吞噬

	UseItem(A_UUID uuid, _U32 item_id, _U32 count, _U32 target_id);		//使用可使用物品 target_id 可填 [ 武将id ]

	FeedHorse(_U8 feed_type);											//养马

	QueryBossRushInfo();
	QueryBossRushSupportInfo();
	BeginBossRushBattle(_U32 level_id, string level_url, _U32 init_boss_hp);
	EndBossRushBattle(string level_url, _U32 result, _U32 total_damage);
	BeginBossRushSupportBattle(_U32 friend_id, _U32 level_id, string level_url);
	EndBossRushSupportBattle(_U32 friend_id, string level_url, _U32 result, _U32 total_damage);
	RequestBossRushSupport();
	CancelBossRush();
	AddBossRushRemainingTimes();
	AwardBossRush();
	AwardBossRushSupport(_U32 friend_id, _U32 level_id, string level_url);

	QueryDiceNum();														//每日寻宝筛子
	
	QueryNewcomerGuideInfo();											//新手引导
	ActivateNewcomerGuide(_U32 function_id);
	FinishNewcomerGuide(_U32 function_id);

	QueryWorldBossRankInfo();											//查询worldboss信息
	BeginWorldBossBattle(_U32 bInstantResurrection);
	InspireWorldBossBattle();
	UpdateWorldBossBattle(_U32 damage);									//damage to boss per some seconds
	EndWorldBossBattle(_U32 damage);
	AwardWorldBossRank();
	AwardWorldBossAttendance();

	Recharge(_U32 index);
};

class SGGAME_S2C
{
	Pong();

	QueryServerTimeResult(_U32 time);
	GetServerListResult(SG_SERVER_INFO infos[count], _U32 count, _U32 last_server);
	QueryAvatarFailed(_U32 code);
	QueryAvatarResult(SG_PLAYER player);
	EnterGameResult(_U8 ret);										//0 - succ
	CreateAvatarResult(_U32 code);

	QueryPlayerResult(SG_PLAYER player, _U8 nSync);
	QueryGeneralResult(SG_GENERAL generals[count], _U32 count, _U8 nSync);
	QuerySoldierResult(SG_SOLDIER soldiers[count], _U32 count, _U8 nSync);
	QueryBagBegin();
	QueryBagEquipt(SG_EQUIPT_ITEM items[count], _U32 count);
	QueryBagUsable(SG_USABLE_ITEM items[count], _U32 count);
	QueryBagGen(SG_GEM_ITEM items[count], _U32 count);
	QueryBagMaterial(SG_MATERIAL_ITEM items[count], _U32 count);
	QueryBagEnd(_U8 nSync);
	QueryOtherPlayersResult(SG_PLAYER_INFO players[count], _U32 count);

	EnhanceEquiptResult(_U8 ret, _U32 gold, SG_EQUIPT_ITEM equip_item);
	ExtendEquiptResult(_U8 ret, SG_EQUIPT_ITEM equip_item, A_UUID reduce_uuid);
	EnhanceCoolDownResult(_U32 time);
	RefreshEquipDone(SG_EQUIPT_ITEM euipt);
	GemCombineResult(SG_GEM_ITEM gem);

	HaloIncreaseEXPResult(_U32 time);
	HaloCoolDownResult(_U8 ret, _U32 consume_rmb);
	HaloGetCoolDownResult(_U32 time);

	BeginBattleResult(SG_PLAYER_PVE PlayerPVE);
	EndBattleResult(_U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 wake_pt, SG_DROP_ITEM_CONFIG drops[drop_count], _U32 drop_count);//level 战斗结束后等级  

	QueryPlayerQuestResult(SG_QUEST_LIVE_INFO quest_list[count], _U32 count, _U8 nSync);
	FinishQuestDone(_U8 ret, _U32 quest_id, _U32 exp_addition, _U32 exp, _U32 level, _U32 gold,  _U32 rmb, _U32 reputation, _U32 energy, SG_DROP_ITEM_BASE drops[drop_count], _U32 drop_count, _U32 consume_rmb);//level 任务完成之后
	
	BuyGoodsResult(_U8 ret, A_UUID goods[count], _U32 id_list[count], _U32 count);	
	SellItemResult(_U8 ret, A_UUID uuid, _U32 item_id, _U32 count);					//ret 0-succ 1-failed 

	QueryPlayerPVPInfoResult(SG_PLAYER_PVE pve);
	QueryPlayerRankListResult(SG_PVP_RANK_PLAYER players[count], _U32 count);

	PVPCoolDownResult(_U32 time);													//挑战冷却时间
	PVPGetRestTimeResult(_U32 rest_time);											//挑战剩余次数
	PVPRecordResult(SG_PVP_RECORD_ITEM record[count], _U32 count);					//对战记录			
	PVPHeroListRecord(SG_PVP_HEROLIST_PLAYER players[count], _U32 count);			//英雄榜				
	PVPDailyReward(_U32 gold, _U32 reputation, SG_ITEM items[count], _U32 count);	//pvp每日奖励 增量
	PVPBattleBeginResult(SG_PLAYER_PVE SelfPVE, SG_PLAYER_PVE DefenderPVE);			//pvp战斗
	PVPBattleEndResult(_U32 reputation);											//pvp战斗结束 
	PVPCoolDownClearResult(_U8 ret, _U32 cost);										//pvp清除挑战冷却时间
	PVPIncreateBattleTimesResult(_U32 rest_times);									//pvp增加挑战次数

	QueryInstanceResult(SG_INSTANCE_INFO instances[count], _U32 count);				//副本
	BeginInstanceBattleResult(SG_PLAYER_PVE PlayerPVE);								//开始副本战斗
	EnterInstanceResult(SG_INSTANCE_INFO instance);					
	EndInstanceBattleResult(_U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 wake_pt, _U8 result, SG_DROP_ITEM_CONFIG drops[drop_count], _U32 drop_count);//0-succ other-failed
	ResetInstanceResult(_U8 result, _U32 rmb, SG_INSTANCE_INFO instance);			//result 0-succ 1-failed

	CreateLeagueResult(_U8 ret, SG_LEAGUE league);									//0-succ other-failed
	QueryLeagueApplyListResult(SG_LEAGUE_APPLYER applyer[count], _U32 count);
	QueryLeagueResult(SG_LEAGUE league);							
	QueryLeagueListResult(SG_LEAGUE league_list[count], _U32 count);
	QueryLeagueMemberListResult(SG_LEAGUE_MEMBER league_members[count], _U32 count);	
	QueryLeagueMemberInfoResult(SG_LEAGUE_MEMBER member_info);

	ContributeLeagueResult(SG_LEAGUE_MEMBER self_info, SG_LEAGUE league_info);
	HandleApplyResult(_U8 ret, SG_LEAGUE_MEMBER new_joiner);						//ret 0-succ 1-failed
	QueryLeagueNoticeResult(string notice_content);
	SetLeagueNoticeResult(_U8 ret, string notice_content);							//ret 0-succ 1-failed 失败带回原先的公告
	SetLeagueOwnerResult(_U8 ret, _U32 member_id);									//ret 0-succ 1-failed
	SetMemberPositionResult(_U8 ret, _U32 member_id, _U8 position);					//ret 0-succ 1-failed 如果成功则带回新职位失败不关心
	DismissMemberResult(_U8 ret, _U32 member_id);									//ret 0-succ 1-failed	
	ExitLeagueResult(_U8 ret);										
	QueryLeagueLogResult(SG_LEAGUE_LOG league_log[count], _U32 count);
	LeagueToastResult(_U8 ret, _U32 gold, _U32 rmb, _U32 reward_reputation, _U32 reward_league_xp);		//0-succ 1-failed

	SalaryGetResult(_U8 ret, _U32 rmb, _U32 gold);														//0-succ 1-failed rmb-消耗的rmb gold-获得的gold
	SalaryGetBatResult(_U8 ret, _U32 rmb, _U32 gold, _U32 times);										//0-succ 1-failed rmb-消耗的rmb gold-获得的gold times-成功领取的次数
	BuyEnergyResult(_U8 ret, _U32 rmb, _U32 energy, _U32 times);										//0-succ 1-failed rmb-消耗的rmb energy-获得的energy times-成功领取的次数

	EnhanceTurboResult(_U8 ret, _U32 turbo_level,  _U32 wake_pt);																//返回新的无双等级和消耗的觉醒点 ret 0-成功 other-failed
	MakeEquiptResult(_U8 ret, SG_EQUIPT_ITEM new_euqipt, SG_MATERIAL_ITEM com_material, SG_MATERIAL_ITEM key_material);			//装备打造 com_material,key_material返回使用掉的材料

	QueryActionAvailableResult(_U32 action_list[count], _U32 available_list[count], _U32 count);		//判断活动是否可以进入/激活 available_list[i] 0-available 1-none available

	QueryFriendListResult(SG_FRIEND_INFO friend_list[count], _U32 count);
	QueryFriendListSearchResult(SG_FRIEND_SEARCH_INFO search_list[count], _U32 count);
	QueryFriendInvitationListResult(SG_FRIEND_INVITATION invitation_list[count], _U32 count);
	AcceptFriendResult(_U8 ret, _U32 avatar_id);

	SendChatResult(_U8 ret);
	ReceiveChatResult(SG_CHAT_RECV_INFO chat_list[count], _U32 count);

	QueryLeagueDianjiangListResult(_U8 ret, SG_LEAGUE_DIANJIANG_JOINER joiner_list[count], _U32 count);	//0-succ other-failed no league 
	JoinLeagueDianjiangResult(_U8 ret, SG_LEAGUE_DIANJIANG_JOINER self);								//0-succ other-failed
	QueryLeagueDianjiangRewardResult(_U8 ret, _U32 reputation);											//0-succ other-failed
	LeagueDianjiangSummonNPCResult(_U8 ret, _U32 gold, _U32 rmb, SG_LEAGUE_DIANJIANG_JOINER npc_joiner);//0-succ other-failed

	QueryAstrologyBagResult(_U32 bag_list[count1], _U32 count1, _U32 tmp_bag_list[count2], _U32 count2);
	SaveToBagResult(_U8 ret, _U32 item_id);
	SetAstrologyBallStatusResult(_U8 ret, SG_GENERAL general);
	EnhanceAstrologyBallResult(_U8 ret, _U32 gold, _U32 ball_id, _U32 new_ball_id);
	StrologyResult(_U8 ret, _U32 gold, _U32 ball_id, _U32 atrologer_id, _U32 last_atrologer_id);
	StrologyAutoResult(_U8 ret, _U32 gold, _U32 ball_list[count], _U32 count, _U32 atrologer_id);
	DevourResult(_U8 ret, _U8 bag_type, _U32 ball_list[count], _U32 count, _U32 devoured_list[count2], _U32 count2);
	
	UseItemResult(_U8 ret, A_UUID uuid, _U32 count, _U32 target_id, SG_PLAYER player_info, SG_GENERAL general, SG_ITEM drops[drop_count], _U32 drop_count);													//使用可使用物品

	FeedHorseResult(_U8 ret, _U32 xp, _U32 xp_add, _U32 level, _U8 xp_add_type, _U8 feed_type, _U32 gold, _U32 rmb);

	QueryBossRushInfoResult(SG_BOSSRUSH_INFO bossrush_info);
	QueryBossRushSupportInfoResult(SG_BOSSRUSH_SUPPORT_INFO support_list[count], _U32 count);
	BeginBossRushBattleResult(SG_PLAYER_PVE PlayerPVE);
	EndBossRushBattleResult(_U8 ret, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 reputation, SG_DROP_ITEM_CONFIG drops[drop_count], _U32 drop_count);
	BeginBossRushSupportBattleResult(_U8 ret, SG_PLAYER_PVE PlayerPVE);
	EndBossRushSupportBattleResult(_U8 ret, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 reputation, SG_DROP_ITEM_CONFIG drops[drop_count], _U32 drop_count);
	AddBossRushRemainingTimesResult(_U8 ret, _U32 rmb, _U8 remain_times);
	AwardBossRushResult(_U8 ret, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 reputation, SG_DROP_ITEM_CONFIG drops[drop_count], _U32 drop_count);
	AwardBossRushSupportResult(_U8 ret, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 reputation, SG_DROP_ITEM_CONFIG drops[drop_count], _U32 drop_count);

	QueryDiceNumResult(_U8 ret, _U32 dice_num, _U32 reward_time, _U32 energy, _U32 rmb, SG_DROP_ITEM_CONFIG drops[drop_count], _U32 drop_count, _U32 circle_num, _U32 step);

	QueryNewcomerGuideInfoResult(SG_NEWCOMER_GUIDE_INFO guide_list[count], _U32 count);

	QueryWorldBossRankInfoResult(SG_WORLDBOSS_RANK_INFO rank_info);
	BeginWorldBossBattleResult(_U8 ret, SG_PLAYER_PVE selfPve, SG_WORLDBOSS_INFO bossInfo, SG_WORLDBOSS_RANK_INFO ranklist, SG_PLAYER_INFO otherPlayers[count1], _U32 count1, _U32 gold, SG_DROP_ITEM_BASE drops[count2], _U32 count2, _U32 rmb);
	//0-succ 1-the end 2-not begin 3-money out
	InspireWorldBossBattleResult(_U8 ret, _U32 rmb);
	AwardWorldBossRankResult(_U8 ret, _U32 Gold, _U32 Reputation, SG_DROP_ITEM_BASE ItemList[count], _U32 count);
	UpdateWorldBossBattleResult(SG_WORLDBOSS_INFO bossInfo, SG_WORLDBOSS_RANK_INFO rankinfo);
	EndWorldBossBattleResult();
	AwardWorldBossAttendanceResult(_U8 ret, _U32 Gold, _U32 Reputation, SG_DROP_ITEM_BASE ItemList[count], _U32 count);

	RechargeResult(_U8 ret, _U32 rmb, SG_PLAYER selfplayer);
};

task[GEN_CLASS_STUB(SGGAME_C2S)];
task[GEN_CLASS_PROXY(SGGAME_C2S)];
task[GEN_CLASS_REFLECT(SGGAME_C2S)];
task[GEN_CLASS_STUB(SGGAME_S2C)];
task[GEN_CLASS_PROXY(SGGAME_S2C)];
task[GEN_CLASS_REFLECT(SGGAME_S2C)];

task[GEN_PHP_STUB(SGGAME_C2S)];
task[GEN_PHP_PROXY(SGGAME_S2C)];
	