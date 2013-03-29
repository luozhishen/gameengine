
include "../../LibCommon/CommonDDL.ddl"

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
const _U32 SG_SOLDIER_NAME_MAX = 12;
const _U32 SG_LEVEL_SOLDIERS_INFO_MAX = 6;
const _U32 SG_SKILL_NAME_MAX = 10;
const _U32 SG_SKILL_DESC_MAX = 256;

const _U32 SG_GENERAL_NOT_EMPLOYEE		= 0;	//0-δ��ļ
const _U32 SG_GENERAL_EMPLOYEED_OUTQUEUE = 1;	//1-�ھƹ�δ�ڶ�
const _U32 SG_GENERAL_EMPLOYEED_INQUEUE = 2;	//2-�ھƹ��ڶ�

//Item
const _U32 SG_ITEM_USABLE_ID_START  = 10000;
const _U32 SG_ITEM_USABLE_ID_END	= 19999;
const _U32 SG_ITEM_MATERIAL_ID_START  = 20000;
const _U32 SG_ITEM_MATERIAL_ID_END	  = 29999;
const _U32 SG_ITEM_GEM_ID_START		= 30000;
const _U32 SG_ITEM_GEM_ID_END		= 39999;
const _U32 SG_ITEM_EQUIPT_ID_START  = 40000;
const _U32 SG_ITEM_EQUIPT_ID_END	= 49999;
const _U32 SG_ITEM_NAME_MAX			= 32;

//Material
const _U32 SG_MATERIAL_DESC_MAX = 256;

//Gem
const _U32 SG_GEM_DESC_MAX = 256;

//Equipt
const _U32 SG_EQUIPT_TEX_MAX = 256;
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

//Quest
const _U32 SG_QUEST_DIALOG_MAX = 512;
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
const _U32 SG_DAILY_ACTION_TYPE_MAX = 12;		//Ŀǰ�ճ�����ֵ
const _U8 SG_DAILY_ACTION_TYPE_PAID = 0;		//0-ٺ»
const _U8 SG_DAILY_ACTION_TYPE_PVP_LEVEL = 1;	//1-ÿ�չؿ��ɽ���
const _U8 SG_DAILY_ACTION_TYPE_PVP_BATTLE = 2;	//2-pvp��ս����
const _U8 SG_DAILY_ACTION_TYPE_PVP_REWARD = 3;	//3-pvpÿ�ս���
const _U8 SG_DAILY_ACTION_TYPE_SALARY = 4;		//4-ÿ�վ��ÿ������
const _U32 SG_DAILY_ACTION_NAME_MAX = 128;		
const _U32 SG_DAILY_ACTION_DESC = 512;			

//vip
const _U32 SG_VIP_ICON_MAX = 256;

//turbo
const _U32 SG_TURBO_CHARPTER_NAME_MAX = 128;
const _U32 SG_TURBO_UNLOCK_SKILL_MAX = 15;
const _U32 SG_TURBO_EQUIP_SKILL_MAX = 3;

//turbo ��˫
struct SG_TURBO_CONFIG : A_CONTENT_OBJECT
{
	_U32						general_id;							//General_ID
	_U32						turbo_level;						//��˫�ȼ�
	string<SG_SKILL_NAME_MAX>	skill_name;							//��˫��������
	_U8							charpter_id;						//����ƪ��ID
	string<SG_TURBO_CHARPTER_NAME_MAX>	charpter_name;				//����ƪ������
	_U32						req_wake_pt;						//������ѵ�
	_U8							skill_type;							//�������� 0-���� 1-����
	_U32						HP;									//�����ӳ�
	_U32						POW;								//�����ӳ�
	_U32						INT;								//�����ӳ�
	_F32						ATK;								//�������ӳ�%
	_F32						DEF;								//���׼ӳ�%
	_F32						HIT;								//���мӳ�%
	_F32						EVASION;							//���ܼӳ�%
	_F32						CRIT;								//�����ʼӳ�%
	_F32						X;									//��˫��X����
	_F32						Y;									//��˫��Y����
	string<ARCHETYPE_URL_LENGTH_MAX>	skill_archetype;			//������������archettype
	string<SG_SKILL_DESC_MAX>			skill_desc;					//������������
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
	_U32						vip_level;						//vip�ȼ�
	_U32						recharge_rmb;					//�����ֵrmb
	_U32						pay_times;						//����ȡ���ô���
	_U32						energy_times;					//�ɹ�����������
	_U32						energy_upper;					//��������
	_U32						reset_instance_times;			//�����ø�������
	_U8							hangup_free;					//�һ���� 0-�� 1-��
	_U8							enhance_free;					//ǿ����� 0-�� 1-��


	string<SG_VIP_ICON_MAX>				res;					//ͼ����Դ
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
	_U32						apply_times;					//��ȡ���� ��101�������һ������
	_U32						consume_rmb;					//����rmb
	_F32						reward_rate;					//ϵ��
};
task[GEN_STRUCT_SERIALIZE(SG_PAY_CONSUME_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_PAY_CONSUME_CONFIG)];

struct SG_PAY_REWARD_CONFIG : A_CONTENT_OBJECT
{
	_U32						player_level;					//��ҵȼ�
	_U32						reward_gold;					//ÿ�λ�ý��
};
task[GEN_STRUCT_SERIALIZE(SG_PAY_REWARD_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_PAY_REWARD_CONFIG)];

//instance
struct SG_INSTANCE_CONFIG : A_CONTENT_OBJECT
{
	_U32								instance_id;			//����ID
	_U8									total_maps;				//������ͼ����
	string<SG_INSTANCE_REWARD_DES_MAX>	reward_normal1;			//��ͨ�Ѷ�С�ؽ�����Ϣ
	string<SG_INSTANCE_REWARD_DES_MAX>	reward_normal2;			//��ͨ�Ѷȴ�ؽ�����Ϣ
	string<SG_INSTANCE_REWARD_DES_MAX>	reward_hard1;			//�����Ѷ�С�ؽ�����Ϣ
	string<SG_INSTANCE_REWARD_DES_MAX>	reward_hard2;			//�����Ѷȴ�ؽ�����Ϣ
	_U32								reset_rmb;				//���õĻ���
	_U32								wake_pt;				//���ѵ�
};
task[GEN_STRUCT_SERIALIZE(SG_INSTANCE_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_INSTANCE_CONFIG)];

struct SG_INSTANCE_INFO : A_LIVE_OBJECT
{
	_U32								instance_id;			//����ID
	_U8									difficulty;				//�Ѷ� 0-��ͨ 1-����
	_S8									progress;				//���� -1 - δ���븱�� 0-���븱��δ�������С�� 1-�����С����Ŀ
	_U8									num_today;				//�����Ѿ���ս����
	_U8									normal_completed;		//��ͨ�Ѷ��Ƿ��ѹ� 0-δ�� 1-�ѹ�
};
task[GEN_STRUCT_SERIALIZE(SG_INSTANCE_INFO)];
task[GEN_STRUCT_REFLECT(SG_INSTANCE_INFO)];

//league
struct SG_LEAGUE_CONFIG : A_CONTENT_OBJECT
{
	_U32								league_level;			//ս�˵ȼ�
	_U32								XP;						//ս�˾���
	_U32								members_count;			//��Ա��
	_F32								rmb_xp_rate;			//1��Ҷһ�ս�˾�����
	_F32								rmb_contribution_rate;	//1��Ҷһ�ս�˹�����
	_F32								energy_xp_rate;			//1�����һ�ս�˾�����
	_F32								energy_contribution_rate;///1�����һ�ս�˹�����
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_CONFIG)];

struct SG_LEAGUE_POW_CONFIG : A_CONTENT_OBJECT
{
	_U8									pow_type;				//Ȩ������ 0-����Ȩ�� 1ת��ս��Ȩ�� 2��ɢս��Ȩ�� 3��׼���� 4������Ա 5�޸Ĺ��� 6�鿴����״̬ 7���� 8�μ�ս�˻ 9�˳�ս��
	_U8									master_pow;				//�ų�Ȩ�� 0-��Ȩ 1-��Ȩ
	_U8									vice_master_pow;		//���ų�Ȩ�� 0-��Ȩ 1-��Ȩ
	_U8									member_pow;				//��ԱȨ�� 0-��Ȩ 1-��Ȩ				
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_POW_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_POW_CONFIG)];

struct SG_LEAGUE_MEMBER_SETTING_CONFIG : A_CONTENT_OBJECT
{
	_U32								master_num;				//�ų����� Ӧ����Զ��1
	_U32						        vice_master_num;		//���ų�����
	_U32								member_num;				//��Ա����
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_MEMBER_SETTING_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_MEMBER_SETTING_CONFIG)];

struct SG_LEAGUE_ACTION_CONFIG : A_CONTENT_OBJECT
{
	_U32								action_id;				//�id
	string<SG_DAILY_ACTION_NAME_MAX>	action_name;			//�����
	string<SG_DAILY_ACTION_DESC>		desc;					//�����
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_ACTION_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_ACTION_CONFIG)];

struct SG_LEAGUE_TOAST_CONFIG : A_CONTENT_OBJECT
{
	_U8									wine_id;				//�Ƶ�id
	string<SG_LEAGUE_LOG_MAX>			wine_name;				//������
	_U32								req_vid;				//vip�ȼ�Ҫ��
	_U32								consume_gold;			//����ͭǮ
	_U32								consume_rmb;			//���Ľ��
	_U32								reward_reputation;		//����(����)����
	_U32								reward_league_xp;		//ս�˾���
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_TOAST_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_TOAST_CONFIG)];


//PVP�¼���¼
struct SG_PVP_RECORD_ITEM : A_LIVE_OBJECT
{
	_U32								challenger;
	_U32								defender;
	string<SG_PLAYER_NAME_MAX>			defender_name;			//����
	_U8									type;					//0 - challengerսʤ defender 1-�෴
	_U32								time;					//�¼�ʱ��
	_U32								param;					//һ��������
};
task[GEN_STRUCT_SERIALIZE(SG_PVP_RECORD_ITEM)];
task[GEN_STRUCT_REFLECT(SG_PVP_RECORD_ITEM)];


//PVP����
struct SG_PVP_INFO_CONFIG : A_CONTENT_OBJECT
{
	_U32								server_level_lower;		//�������ȼ�����
	_U32								server_level_upper;		//�������ȼ�����
	_U32								lower;					//����
	_U32								upper;					//����
	_U32								span;					//��ս���
	_U32								daily_reputation;		//ÿ�ս�������
	_U32								reward_gold;			//ÿ��ͭǮ����
	_U32								reward_item;			//ÿ�յ��߽���
	_U32								reward_item_count;		//��������
	_U32								succ_reputation;		//����ʤ����������
	_U32								failed_reputation;		//����ʧ�ܿ۳�����
};
task[GEN_STRUCT_SERIALIZE(SG_PVP_INFO_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_PVP_INFO_CONFIG)];

struct SG_PVP_SETTING_CONFIG : A_CONTENT_OBJECT
{
	_U32								increase_pay_rmb;		//������������Ԫ��
};
task[GEN_STRUCT_SERIALIZE(SG_PVP_SETTING_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_PVP_SETTING_CONFIG)];

//Shop
struct SG_GOODS_CONFIG : A_CONTENT_OBJECT
{
	_U32								good_id;				//��ƷID
	_U8									display_condition;		//��ʾ����/���� 0-������ 1-��ְ���� 2-�ȼ�Ҫ�� 3-VIP�ȼ� 4-ս�˵ȼ�
	_U32								display_param1;			//��������1
	_U32								display_param2;			//��������2
	_U8									buy_condition;			//������������ 0-������ 1-��ְ���� 2-�ȼ�����  3-VIP�ȼ� 4-ս�˵ȼ�
	_U32								buy_param1;				//��������1
	_U32								buy_param2;				//��������2
	_U32								item_id;				//����ID			
	_U32								count;					//��������
	_U8									price_condition;		//�һ��������� 0-���κ����� 1-gold 2-rmb 3-ս�˹��� 4-����
	_U32								price_param1;			//���Ĳ���1
	_U32								price_param2;			//���Ĳ���2
	_U32								price_discount;			//��������	
};
task[GEN_STRUCT_SERIALIZE(SG_GOODS_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_GOODS_CONFIG)];

struct SG_GOODS_GROUP_CONFIG : A_CONTENT_OBJECT
{
	_U32								goods_group_id;			//��Ʒ��ID
	_U32								good_id;				//��ƷID
};
task[GEN_STRUCT_SERIALIZE(SG_GOODS_GROUP_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_GOODS_GROUP_CONFIG)];

struct SG_SHOP_CONFIG : A_CONTENT_OBJECT
{
	_U32								shop_id;				//�̵�ID
	string<SG_GOODS_GROUP_NAME_MAX>		goods_group_name;		//��Ʒ������
	_U32								goods_group_id;			//��Ʒ��ID
};
task[GEN_STRUCT_SERIALIZE(SG_SHOP_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_SHOP_CONFIG)];

//League
struct SG_LEAGUE : A_LIVE_OBJECT
{
	_U32								league_id;				//ս��ID

	string<SG_LEAGUE_NAME_MAX>			league_name;			//ս������
	_U32								rank;					//ս������
	_U32								member_num;				//Ŀǰ��Ա����
	_U32								level;					//�ȼ�
	_U32								exp;					//ս�˾���
	_U32								create_time;			//����ʱ��
	_U32								owner_id;				//�ų�ID
	string<SG_PLAYER_NAME_MAX>			owner_name;				//�ų�����
	string<SG_LEAGUE_NOTICE_MAX>		post_content;			//ս�˹���
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE)];

struct SG_LEAGUE_MEMBER : A_LIVE_OBJECT
{
	_U32								league_id;				//����ս��ID
	_U32								member_id;				//��ԱID avatar_id
	_U32								general_id;				//��Աgeneral_id
	string<SG_PLAYER_NAME_MAX>			member_name;			//��Ա����
	_U32								member_level;			//��ԱLevel
	_U32								member_battle_value;	//��Աս����
	_U32								pvp_rank;				//��Աpvp����

	_U8									position;				//����ս��ְλ 0-��ͨ��Ա 1-���ų� 2-�ų�
	_U32								contribution_value;		//����ֵ
	_U32								total_contribution_value;//�ܹ���ֵ
	_U32								last_ol_time;			//�ϴ��û�������ʱ�� ��ʾ�Ƿ�����
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_MEMBER)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_MEMBER)];

struct SG_LEAGUE_APPLYER : A_LIVE_OBJECT
{
	_U32								applyer_id;				//������ID avatar_id
	string<SG_PLAYER_NAME_MAX>			applyer_name;			//����������
	_U32								general_id;				//������ְҵ(����ID)
	_U32								level;					//�����ߵȼ�
	_U32								rank;					//������pvp����
	_U32								league_id;				//�������ս�˵�ID 
	_U8									reason;					//?
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_APPLYER)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_APPLYER)];

struct SG_LEAGUE_LOG	: A_LIVE_OBJECT
{
	_U32								league_id;				
	_U32								result_time;			//����log��¼ʱ��
	_U8									type;					//����log���¼�����
	string<SG_LEAGUE_LOG_MAX>			log;					//log����
};
task[GEN_STRUCT_SERIALIZE(SG_LEAGUE_LOG)];
task[GEN_STRUCT_REFLECT(SG_LEAGUE_LOG)];

//�⻷
struct SG_HALO_CONFIG					: A_CONTENT_OBJECT 
{
	_U32								halo_level;				//�⻷�ȼ�
	_U32								EXP;					//�������辭��
	_F32								ATK;					//����ATK
	_F32								DEF;					//����DEF
	_F32								HIT;					//��������(percent)
	_F32								EVASION;				//��������(percent)
	_F32								CRIT;					//��������(percent)
};
task[GEN_STRUCT_SERIALIZE(SG_HALO_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_HALO_CONFIG)];

//�⻷ǿ��
struct SG_HALO_LEVELUP_CONFIG			: A_CONTENT_OBJECT
{
	_U32								level;					//ǿ��ѡ�� 1~3
	_U32								EXP;					//��þ���
	_U32								CD;						//��ȴʱ��
};
task[GEN_STRUCT_SERIALIZE(SG_HALO_LEVELUP_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_HALO_LEVELUP_CONFIG)];

//��Ҿ���
struct SG_PLAYER_TITLE_CONFIG			: A_CONTENT_OBJECT
{
	_U8									title_level;			//���εȼ�
	string<SG_PLAYER_TITLE_NAME_MAX>	title_name;				//��������
	_U8									title_star;				//��������
	_U32								req_level_up_reputaion;	//������������
	_U32								daily_reputation_deduct;//ÿ�տ۳�����
	_U8									general_limit_num;		//��ӵ���佫����
	_U32								daily_salary_gold;		//ÿ��ٺ»����ͭǮ
	_U32								daily_reward_reserve;	//ÿ��Ԥ������
	_U32								STRAddition;			//����STR����
	_U32								INTAddition;			//����INT����
	_U32								HP_AddExtra;			//ȫ��HP��������
	_U32								ATK_AddExtra;			//ȫ��ATK��������
	_U32								DEF_AddExtra;			//ȫ��DEF��������

	string<SG_TITLE_PROP_DESC>			employ_general_prop_desc;	//������������
	string<SG_TITLE_GENERAL_DESC>		employ_general_desc;		//����ļ�佫����
	
	string<SG_TITLE_ACRHTYPE_MAX>							wusheng_male;			//����ʥarchtype
	string<SG_TITLE_ACRHTYPE_MAX>							wusheng_female;
	string<SG_TITLE_ACRHTYPE_MAX>							junshi_male;
	string<SG_TITLE_ACRHTYPE_MAX>							junshi_female;
	string<SG_TITLE_ACRHTYPE_MAX>							jianshen_male;
	string<SG_TITLE_ACRHTYPE_MAX>							jianshen_female;		//�м���archtype

};
task[GEN_STRUCT_SERIALIZE(SG_PLAYER_TITLE_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_PLAYER_TITLE_CONFIG)];

//����live
struct SG_QUEST_LIVE_INFO				: A_LIVE_OBJECT	
{
	_U32								quest_id;				//����ID
	_U32								counter;				//���������
	_U8									status;					//����ǰ״̬ 0-not avaliable 1-avaliable 2-wip 3-ready to submit 4-finished
};
task[GEN_STRUCT_SERIALIZE(SG_QUEST_LIVE_INFO)];
task[GEN_STRUCT_REFLECT(SG_QUEST_LIVE_INFO)];

//������Ϣ����
struct SG_QUEST_INFO_CONFIG				: A_CONTENT_OBJECT
{
	_U32								quest_id;				//����id
	_U8									quest_type;				//�������� 0-���� 1-֧��
	string<SG_QUEST_NAME_MAX>			quest_name;				//��������
	string<SG_QUEST_GOAL_MAX>			quest_goal_desc;		//����Ŀ������
	string<SG_QUEST_GOAL_DESC_MAX>		quest_goal_short_desc;	//����Ŀ�����

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
	_U32								reward_reputation;		//����
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
	_U32								next_level;				//��һ�صĹؿ�����id
	string<SG_DESCRIPTION_MAX>			description;			//�ؿ�����
	_U32								req_player_level;		//�ɽ���Ҫ�����ҵȼ�
	_U32								wake_pt;				//���ѵ�
	
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

//�������
struct SG_DIRTY_WORD_CONFIG				: A_CONTENT_OBJECT
{
	string<SG_PLAYER_NAME_MAX>			dirty_word;
};
task[GEN_STRUCT_SERIALIZE(SG_DIRTY_WORD_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_DIRTY_WORD_CONFIG)];

//��������
struct SG_PLAYER_RANDOM_NAME_CONFIG : A_CONTENT_OBJECT
{
	string<SG_PLAYER_NAME_MAX>			family_name;
	string<SG_PLAYER_NAME_MAX>			male_name;
	string<SG_PLAYER_NAME_MAX>			female_name;
};
task[GEN_STRUCT_SERIALIZE(SG_PLAYER_RANDOM_NAME_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_PLAYER_RANDOM_NAME_CONFIG)];

//����������
struct SG_LEVEL_DROP_CONFIG : A_CONTENT_OBJECT
{
	string<100>							level_name;
	_U32								exp;
	_U32								gold;
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
	_U32								wake_pt;				//���ѵ�
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

//������
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

//��Ʒ����
struct SG_ITEM_CONFIG : A_CONTENT_OBJECT
{
	_U32								item_id;			//��Ʒid
	_U32								stack_max;			//���������
	_U32								price;				//���ۼ۸�
	
	_U32								U;					
	_U32								V;
	_U32								UL;
	_U32								VL;
	
	string<SG_EQUIPT_TEX_MAX>			tex;				//����
	string<SG_ITEM_NAME_MAX>			item_name;			//��Ʒ����
	_U8									quality;			//Ʒ��
};
task[GEN_STRUCT_SERIALIZE(SG_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_ITEM_CONFIG)];

//����
struct SG_MATERIAL_CONFIG : SG_ITEM_CONFIG
{
	string<SG_MATERIAL_DESC_MAX>		desc;				//����
	_U32								req_level;			//�ȼ�
};
task[GEN_STRUCT_SERIALIZE(SG_MATERIAL_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_MATERIAL_CONFIG)];

//װ������
struct SG_EQUIPT_ITEM_CONFIG : SG_ITEM_CONFIG				//װ��
{
	SG_ATTR_MOD_CONFIG					mod_config;
	_U8									item_type;			//װ������
	_F32								quality_rate;		//Ʒ��ϵ��
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

//��ʹ������
struct SG_USABLE_ITEM_CONFIG : SG_ITEM_CONFIG				
{
	string<100>							functional;
};
task[GEN_STRUCT_SERIALIZE(SG_USABLE_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_USABLE_ITEM_CONFIG)];

//��ʯ����
struct SG_GEM_ITEM_CONFIG : SG_ITEM_CONFIG					
{
	string<SG_GEM_DESC_MAX>		desc;						//��ʯ����
	_U8							level;						//��ʯ�ȼ�
	_U8							attri_type;					//��ʯ����
	_F32						attri;						//��ʯ����ֵ
	_U32						compose_next;				//�ϳɵı�ʯID
	_U32						compose_num;				//�ϳ���Ҫ������
};
task[GEN_STRUCT_SERIALIZE(SG_GEM_ITEM_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_GEM_ITEM_CONFIG)];

//��������
struct SG_ENEMY_CONFIG : A_CONTENT_OBJECT
{
	_U32								enemy_id;			//����id
	SG_PAWN_CONFIG						PawnConfig;
};
task[GEN_STRUCT_SERIALIZE(SG_ENEMY_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_ENEMY_CONFIG)];

//Soldier�ȼ���������
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

//General�ȼ���������
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

//Soldier����
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

//General����
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
	string<SG_PLAYER_NAME_MAX>			Nick;
	_U32								Avatar_ID;			//����ǲ�ѯ�򷵻ز�ѯʱ�����avatar_id
};
task[GEN_STRUCT_SERIALIZE(SG_PLAYER_PVE)];
task[GEN_STRUCT_REFLECT(SG_PLAYER_PVE)];

//װ��Slot
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

//daily action
struct SG_DAILY_ACTION_CONFIG			: A_CONTENT_OBJECT
{
	_U32								action_id;
	string<SG_DAILY_ACTION_NAME_MAX>	action_name;			//�����
	_U8									display_position;		//���Ͻ���ʾλ��

	_U8									type;					//����� 0-�ճ��淨 ���� ս�˻
	_U32								req_league_level;		//Ҫ���ս�˵ȼ�
	_U32								req_level;				//Ҫ�����ҵȼ�

	_U8									condition1_type;		//����һ����
	_U32								condition1_param1;		//����һ����һ
	
	_U8									condition2_type;        //����������
	_U32								condition2_param1;		//����������һ

	_U32								hour;					//����ʱ��
	_U32								min;					//����ʱ��

	_U32								end_hour;				//����ʱ��
	_U32								end_min;				//����ʱ��

	_U32								times;					//����

	_U32								prepare_min;			//׼��ʱ��min

	_U32								reset_hour;				//����ʱ��
	_U32								reset_min;				//����ʱ��

	string<SG_ICON_MAX>					icon;					//ͼ����Դ�ļ�
	_U32								U;					
	_U32								V;
	_U32								UL;
	_U32								VL;


};
task[GEN_STRUCT_SERIALIZE(SG_DAILY_ACTION_CONFIG)];
task[GEN_STRUCT_REFLECT(SG_DAILY_ACTION_CONFIG)];

struct SG_DAILY_ACTION_INFO				: A_LIVE_OBJECT
{
	_U32								action_id;				//1001-ٺ» 1-ÿ�չؿ��ɽ��� 1002-pvp 1004-pvpÿ�ս��� 1003-ÿ�վ���
	_U32								times;					//ʣ�����ȡٺ» 1-ÿ�տɽ��йؿ�ʣ����� -pvpʣ����� -pvpÿ�ս���������� -ÿ�վ�������ȡ����
	_U32								reset_time;				//ٺ»��ȴʱ�� 1-���õ�ʱ���ǩ -���õ�ʱ���ǩ -���õ�ʱ���ǩ -���õ�ʱ���ǩ
};
task[GEN_STRUCT_SERIALIZE(SG_DAILY_ACTION_INFO)];
task[GEN_STRUCT_REFLECT(SG_DAILY_ACTION_INFO)];

struct SG_PLAYER : SG_GENERAL
{
	string<SG_PLAYER_NAME_MAX>			nick;					//�ǳ�
	_U32								avatar_id;				//�൱��userid
	_U32								total_rmb;				//�ܳ�ֵ����
	_U32								gold;					//��Ϸ��
	_U32								rmb;					//Ԫ��
	array<_U32, 2>						equip_generals;
	array<_U32, 3>						equip_soldiers;
	_U8									increase_equipt_times;	//��߹���ȴʱ�����
	_U8									title;					//��ְ
	_U32								reputation;				//����
	_U32								energy;					//����
	
	_U32								halo_level;				//�⻷�ȼ�
	_U32								halo_exp;				//�⻷����

	array<SG_DAILY_ACTION_INFO, SG_DAILY_ACTION_TYPE_MAX>		daily_actions;			//�ճ���Ϊ
	_U32								next_level;				//��һ��ͨ���Ĺؿ�ID ��ʼֵ10001
	_U32								rank;					//���а�����
	_U32								last_rank;				//����ʱ�������
	_U32								vip_level;				//vip�ȼ�
	
	_U32								league_id;				//����ս��ID
	array<_U32, SG_LEAGUE_APPLY_MAX>	league_apply_list;		//����

	_U32								wake_pt;				//���ѵ�
	_U32								turbo_level;			//��˫�ȼ�
	array<string<ARCHETYPE_URL_LENGTH_MAX>, SG_TURBO_UNLOCK_SKILL_MAX>					skills;					//�Ѿ������ļ��� ���15
	SG_TURBO_SKILL_SLOT																	turbo_skill_slot;		//Ŀǰ�Ѿ�ѡ�еļ��� ���3

	_U32								last_operation_time;	//���һ�β���ʱ��
	string<SG_MAP_URL_MAX>				last_town_map;			//���һ�εĵ�ͼurl
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
	SG_ATTR_MOD_CONFIG							mod_config;			
	_U8											turbo_level;		//��ǰǿ���ȼ�
	array<_U32, SG_EUQIPT_GEM_SLOTS_MAX>		gems_slots;			//��Ƕ�ı�ʯ
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
	_U8										status;					//��ʯ�Ƿ���Ƕ 0-δ��Ƕ 1-��Ƕ
};
task[GEN_STRUCT_SERIALIZE(SG_GEM_ITEM)];
task[GEN_STRUCT_REFLECT(SG_GEM_ITEM)];

struct SG_MATERIAL_ITEM : SG_ITEM
{
	
};
task[GEN_STRUCT_SERIALIZE(SG_MATERIAL_ITEM)];
task[GEN_STRUCT_REFLECT(SG_MATERIAL_ITEM)];


//Misc
struct SG_SERVER_INFO
{
	_U32								server_id;
	string<100>							server_name;
	_U32								server_state;
	string<100>							avatar_nick;
	_U32								general_id;
	_U32								level;						//��ɫ�ڱ����ȼ�
	_U32								server_level;				//�����ȼ� ��������pvp���
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
	QueryServerTime();												//��ѯ������ʱ��

	QueryPlayer(_U8 nSync);											//0-nSync ��query��һ�β�ѯ������ʱֱ�ӵ���callback 1-��ͬ������������ͬ���������ѡ���Ƿ�callback֪ͨ
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
	EndBattle(string name, _U32 result);

	EquipGem(A_UUID item_uuid, _U32 gem_id);
	UnequipGem(A_UUID item_uuid, _U32 gem_id);
	GemCombine(_U32 item_id);

	SetGeneralStatus(_U32 general_id, _U8 status);					//�佫��ļ
	
	QueryPlayerQuest(_U8 nSync);									//��ѯ���������Ϣ
	SaveQuestData(_U32 quest_id, _U32 counter, _U8 status);			//��������״̬
	FinishQuest(_U32 quest_id);										//�ύ�������

	HaloCoolDown();													//������ȴ
	HaloIncreaseEXP(_U8 halo_type);									//�����⻷
	HaloGetCoolDown();												//��ȡ�⻷��ȴʱ��

	GetPaid();														//daily salary
	UpgradeTitle();													//��������
	
	BuyGoods(_U32 item_id);											//�̵깺��

	QueryPlayerPVPInfo(_U32 avatar_id);								//��ȡplayer pvp��Ϣ
	QueryPlayerRankList();											//��ȡpvp����

	PVPCoolDown();													//��ս��ȴʱ��
	PVPGetRestTime();												//��սʣ�����
	PVPRecord();													//��ս��¼			
	PVPHeroList();													//Ӣ�۰�			
	PVPDailyReward();												//pvpÿ�ս���֮��
	PVPBattleBegin(_U32 defender);									//pvpս����ʼ 
	PVPBattleEnd(_U32 defender, _U8 ret);							//pvpս������ 0-succ 1-failed
	PVPCoolDownClear();												//pvp�����ս��ȴʱ��
	PVPIncreateBattleTimes();										//pvp������ս����

	QueryInstance();												//����
	EnterInstance(_U32 instance_id, _U8 difficulty);				//���븱�� 0-��ͨ 1-����
	BeginInstanceBattle(_U32 instance_id, string map_url);			//��ʼ����ս��
	EndInstanceBattle(_U32 instance_id, string map_url, _U32 result);//��������ս��
	ResetInstance(_U32 instance_id);
	SaveLastTownMap(string last_town_map);							//�������һ�εĵ�ͼ��Ϣ

	CreateLeague(string league_name);								//ս�� ����
	ApplyJoinLeague(_U32 league_id);								//�������ս��		
	QueryLeagueApplyList(_U32 league_id);							//��ѯ��ǰ�������ս�˵���
	QueryLeague(_U32 league_id);									//��ѯleague_idս����Ϣ
	QueryLeagueList();												//��ѯ��ǰ����ս�˵��б�
	QueryLeagueMemberList(_U32 league_id);							//��ѯleague_idս�˵�ǰ��Ա
	QueryLeagueMemberInfo(_U32 member_id);							//��ʾ��Աѡ��tips
	
	ContributeLeague(_U32 rmb, _U32 energy);						//ս�˾���
	HandleApply(_U32 applyer_id, _U8 allowed);						//�������ս������ bAllowed ͬ��- 1 �ܾ�-0
	QueryLeagueNotice();											//��ѯս�˹���
	SetLeagueNotice(_U32 league_id, string notice_content);
	SetLeagueOwner(_U32 member_id);									//ת��ս��
	DissolveLeague();												//��ɢ
	SetMemberPosition(_U32 member_id, _U8 position);				//��ְ 0-��ͨ��Ա 1-���ų� 2-�ų�
	DismissMember(_U32 member_id);									//����
	ExitLeague();													//�˳�ս��
	QueryLeagueLog();												//ս����־
	LeagueToast(_U8 wine_id);										//ս���þ�

	SalaryGet();													//��ȡÿ�վ���
	SalaryGetBat();													//������ȡ max = 10

	EnhanceTurbo();													//������˫����
	EquipTurboSkill(SG_TURBO_SKILL_SLOT skill_slot);				//װ����˫����

	MakeEquipt(_U32 equipt_id);										//װ������
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
	QueryOtherPlayersResult(SG_PLAYER players[count], _U32 count);

	EnhanceCoolDownResult(_U32 time);
	RefreshEquipDone(SG_EQUIPT_ITEM euipt);
	GemCombineResult(SG_GEM_ITEM gem);

	HaloIncreaseEXPResult(_U32 time);
	HaloGetCoolDownResult(_U32 time);

	BeginBattleResult(SG_PLAYER_PVE PlayerPVE);
	EndBattleResult(_U32 level, _U32 exp_addition, _U32 exp, _U32 gold, SG_DROP_ITEM_CONFIG drops[drop_count], _U32 drop_count);//level ս��������ȼ�

	QueryPlayerQuestResult(SG_QUEST_LIVE_INFO quest_list[count], _U32 count, _U8 nSync);
	FinishQuestDone(_U32 quest_id, _U32 exp_addition, _U32 exp, _U32 level, _U32 gold,  _U32 rmb, _U32 reputation, _U32 energy, SG_DROP_ITEM_BASE drops[drop_count], _U32 drop_count);//level �������֮��
	
	BuyGoodsResult(A_UUID goods[count], _U32 count);	

	QueryPlayerPVPInfoResult(SG_PLAYER_PVE pve);
	QueryPlayerRankListResult(SG_PLAYER players[count], _U32 count);

	PVPCoolDownResult(_U32 time);													//��ս��ȴʱ��
	PVPGetRestTimeResult(_U32 rest_time);											//��սʣ�����
	PVPRecordResult(SG_PVP_RECORD_ITEM record[count], _U32 count);					//��ս��¼			
	PVPHeroListRecord(SG_PLAYER players[count], _U32 count);						//Ӣ�۰�				
	PVPDailyReward(_U32 gold, _U32 reputation, SG_ITEM items[count], _U32 count);	//pvpÿ�ս��� ����
	PVPBattleBeginResult(SG_PLAYER_PVE SelfPVE, SG_PLAYER_PVE DefenderPVE, SG_PLAYER DefenderPlayerInfo);//pvpս��
	PVPBattleEndResult(_U32 reputation);											//pvpս������ 
	PVPCoolDownClearResult();														//pvp�����ս��ȴʱ��
	PVPIncreateBattleTimesResult(_U32 rest_times);									//pvp������ս����

	QueryInstanceResult(SG_INSTANCE_INFO instances[count], _U32 count);				//����
	BeginInstanceBattleResult(SG_PLAYER_PVE PlayerPVE);								//��ʼ����ս��
	EnterInstanceResult(SG_INSTANCE_INFO instance);			
	EndInstanceBattleResult(_U32 level, _U32 exp_addition, _U32 exp, _U32 gold, SG_DROP_ITEM_CONFIG drops[drop_count], _U32 drop_count);
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
	SetLeagueNoticeResult(_U8 ret, string notice_content);							//ret 0-succ 1-failed ʧ�ܴ���ԭ�ȵĹ���
	SetLeagueOwnerResult(_U8 ret, _U32 member_id);									//ret 0-succ 1-failed
	SetMemberPositionResult(_U8 ret, _U32 member_id, _U8 position);					//ret 0-succ 1-failed ����ɹ��������ְλʧ�ܲ�����
	DismissMemberResult(_U8 ret, _U32 member_id);									//ret 0-succ 1-failed	
	ExitLeagueResult(_U8 ret);										
	QueryLeagueLogResult(SG_LEAGUE_LOG league_log[count], _U32 count);
	LeagueToastResult(_U8 ret, _U32 gold, _U32 rmb, _U32 reward_reputation, _U32 reward_league_xp);		//0-succ 1-failed

	SalaryGetResult(_U8 ret, _U32 rmb, _U32 gold);														//0-succ 1-failed rmb-���ĵ�rmb gold-��õ�gold
	SalaryGetBatResult(_U8 ret, _U32 rmb, _U32 gold, _U32 times);										//0-succ 1-failed rmb-���ĵ�rmb gold-��õ�gold times-�ɹ���ȡ�Ĵ���

	EnhanceTurboResult(_U8 ret, _U32 turbo_level,  _U32 wake_pt);																//�����µ���˫�ȼ������ĵľ��ѵ� ret 0-�ɹ� other-failed
	MakeEquiptResult(_U8 ret, SG_EQUIPT_ITEM new_euqipt, SG_MATERIAL_ITEM com_material, SG_MATERIAL_ITEM key_material);			//װ������ com_material,key_material����ʹ�õ��Ĳ���
};

task[GEN_CLASS_STUB(SGGAME_C2S)];
task[GEN_CLASS_PROXY(SGGAME_C2S)];
task[GEN_CLASS_REFLECT(SGGAME_C2S)];
task[GEN_CLASS_STUB(SGGAME_S2C)];
task[GEN_CLASS_PROXY(SGGAME_S2C)];
task[GEN_CLASS_REFLECT(SGGAME_S2C)];

task[GEN_PHP_STUB(SGGAME_C2S)];
task[GEN_PHP_PROXY(SGGAME_S2C)];
	