#include <AtlasBase.h>
#include <AtlasCommon.h>

#include "SGGameDDL.h"
#include <ContentObject.h>

namespace Atlas
{
	struct JsonContent
	{
		const char* struct_type;
		const char* file;
		const char* keys;
	};

	JsonContent g_JsonContent[] = 
	{
		{"SG_PAY_CONSUME_CONFIG", "SG_PAY_CONSUME_CONFIG.json", "apply_times"},
		{"SG_PAY_REWARD_CONFIG", "SG_PAY_REWARD_CONFIG.json", "player_level"},
		{"SG_INSTANCE_CONFIG", "SG_INSTANCE_CONFIG.json", "instance_id"},
		{"SG_LEAGUE_CONFIG", "SG_LEAGUE_CONFIG.json", "league_level"},
		{"SG_LEAGUE_POW_CONFIG", "SG_LEAGUE_POW_CONFIG.json", "pow_type"},

		{"SG_LEAGUE_MEMBER_SETTING_CONFIG", "SG_LEAGUE_MEMBER_SETTING_CONFIG.json", "master_num"},
		{"SG_DAILY_ACTION_CONFIG", "SG_DAILY_ACTION_CONFIG.json", "action_id"},
		{"SG_HALO_CONFIG", "SG_HALO_CONFIG.json", "halo_level"},
		{"SG_HALO_LEVELUP_CONFIG", "SG_HALO_LEVELUP_CONFIG.json", "level"},
		{"SG_PLAYER_TITLE_CONFIG", "SG_PLAYER_TITLE_CONFIG.json", "title_level"},

		{"SG_QUEST_INFO_CONFIG", "SG_QUEST_INFO_CONFIG.json", "quest_id"},
		{"SG_LEVEL_INFO_CONFIG", "SG_LEVEL_INFO_CONFIG.json", "level_id"},
		{"SG_DIRTY_WORD_CONFIG", "SG_DIRTY_WORD_CONFIG.json", "dirty_word"},
		{"SG_PLAYER_RANDOM_NAME_CONFIG", "SG_PLAYER_RANDOM_NAME_CONFIG.json", "family_name,male_name,female_name"},
		{"SG_DROP_ITEM_CONFIG", "SG_DROP_ITEM_CONFIG.json", "group_id,item_id"},

		{"SG_LEVEL_DROP_CONFIG", "SG_LEVEL_DROP_CONFIG.json", "level_name"},
		{"SG_ENEMY_CONFIG", "SG_ENEMY_CONFIG.json", "enemy_id"},
		{"SG_GENERAL_CONFIG", "SG_GENERAL_CONFIG.json", "general_id,attr_id"},
		{"SG_SOLDIER_CONFIG", "SG_SOLDIER_CONFIG.json", "soldier_id,attr_id"},
		{"SG_GENERAL_LEVEL_CONFIG", "SG_GENERAL_LEVEL_CONFIG.json", "attr_id,level"},

		{"SG_SOLDIER_LEVEL_CONFIG", "SG_SOLDIER_LEVEL_CONFIG.json", "attr_id,level"},
		{"SG_ITEM_CONFIG", "SG_ITEM_CONFIG.json", "item_id"},
		{"SG_EQUIPT_ITEM_CONFIG", "SG_EQUIPT_ITEM_CONFIG.json", "item_id"},
		{"SG_USABLE_ITEM_CONFIG", "SG_USABLE_ITEM_CONFIG.json", "item_id"},
		{"SG_GEM_ITEM_CONFIG", "SG_GEM_ITEM_CONFIG.json", "item_id"},

		{"SG_GOODS_CONFIG", "SG_GOODS_CONFIG.json", "good_id"},
		{"SG_GOODS_GROUP_CONFIG", "SG_GOODS_GROUP_CONFIG.json", "goods_group_id,good_id"},
		{"SG_SHOP_CONFIG", "SG_SHOP_CONFIG.json", "shop_id,goods_group_id"},
		{"SG_PVP_INFO_CONFIG", "SG_PVP_INFO_CONFIG.json", "server_level_lower,server_level_upper,lower,upper"},
		{"SG_PVP_SETTING_CONFIG", "SG_PVP_SETTING_CONFIG.json", "increase_pay_rmb"},

		{"SG_VIP_CONFIG", "SG_VIP_CONFIG.json", "vip_level"},
		{"SG_TURBO_CONFIG", "SG_TURBO_CONFIG.json", "general_id,turbo_level"},
		{"SG_LEAGUE_ACTION_CONFIG", "SG_LEAGUE_ACTION_CONFIG.json", "action_id"},
		{"SG_MATERIAL_CONFIG", "SG_MATERIAL_CONFIG.json", "item_id"},
		{"SG_LEAGUE_TOAST_CONFIG", "SG_LEAGUE_TOAST_CONFIG.json", "wine_id"},

		{"SG_DAILY_ACTION_DESC_CONFIG", "SG_DAILY_ACTION_DESC_CONFIG.json", "action_id"},
		{"SG_CHAT_CHANNEL_CONFIG", "SG_CHAT_CHANNEL_CONFIG.json", "channel_id"},
		{"SG_CHAT_TAB_CONFIG", "SG_CHAT_TAB_CONFIG.json", "tab_id"},
		{"SG_LEAGUE_DIANJIANG_CONFIG", "SG_LEAGUE_DIANJIANG_CONFIG.json", "consume_npc_gold"},
		{"SG_ENERGY_CONFIG", "SG_ENERGY_CONFIG.json", "restore_energy"},

		{"SG_ENERGY_BUY_CONFIG", "SG_ENERGY_BUY_CONFIG.json", "times"},
		{"SG_FRIEND_NUM_CONFIG", "SG_FRIEND_NUM_CONFIG.json", "player_level"},
		{"SG_ASTROLOGY_BALL_CONFIG", "SG_ASTROLOGY_BALL_CONFIG.json", "item_id"},
		{"SG_ASTROLOGER_CONFIG", "SG_ASTROLOGER_CONFIG.json", "astrologer_id"},
		{"SG_GENERAL_ASTROLOG_SLOT_CONFIG", "SG_GENERAL_ASTROLOG_SLOT_CONFIG.json", "general_level"},
		
	};

	const char* AtlasGameName()
	{
		return "SGGame";
	}

	void InitDDLStub()
	{
		Atlas::Engine::InitDDLStub();

		RegisterClientStub<SGGAME_S2C>();
		RegisterServerStub<SGGAME_C2S>(0);
	}

	void InitContentObjects()
	{

		//Atlas::Engine::InitContentObjects();
		//ContentObject::IContentGroup* pContentGroup = ContentObject::CreateContentGroup("SGGame", "sggame.json", true);

		int nSum = sizeof(g_JsonContent)/sizeof(g_JsonContent[0]);
		int i = 0;

		ContentObject::IContentGroup* pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_PAY_CONSUME_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_PAY_REWARD_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_INSTANCE_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_LEAGUE_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_LEAGUE_POW_CONFIG >(), true, g_JsonContent[i++].keys);
		
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_LEAGUE_MEMBER_SETTING_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_DAILY_ACTION_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_HALO_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_HALO_LEVELUP_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_PLAYER_TITLE_CONFIG >(), true, g_JsonContent[i++].keys);

		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_QUEST_INFO_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_LEVEL_INFO_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_DIRTY_WORD_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_PLAYER_RANDOM_NAME_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_DROP_ITEM_CONFIG >(), true, g_JsonContent[i++].keys);
		
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_LEVEL_DROP_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_ENEMY_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_GENERAL_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_SOLDIER_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_GENERAL_LEVEL_CONFIG >(), true, g_JsonContent[i++].keys);

		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_SOLDIER_LEVEL_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_ITEM_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_EQUIPT_ITEM_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_USABLE_ITEM_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_GEM_ITEM_CONFIG >(), true, g_JsonContent[i++].keys);

		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_GOODS_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_GOODS_GROUP_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_SHOP_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_PVP_INFO_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_PVP_SETTING_CONFIG >(), true, g_JsonContent[i++].keys);
		
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_VIP_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_TURBO_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_LEAGUE_ACTION_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_MATERIAL_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_LEAGUE_TOAST_CONFIG >(), true, g_JsonContent[i++].keys);

		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_DAILY_ACTION_DESC_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_CHAT_CHANNEL_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_CHAT_TAB_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_LEAGUE_DIANJIANG_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_ENERGY_CONFIG >(), true, g_JsonContent[i++].keys);

		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_ENERGY_BUY_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_FRIEND_NUM_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_ASTROLOGY_BALL_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_ASTROLOGER_CONFIG >(), true, g_JsonContent[i++].keys);
		pContentGroup = ContentObject::CreateContentGroup(g_JsonContent[i].struct_type, g_JsonContent[i].file, true);
		pContentGroup->Register(DDLReflect::GetStruct< SG_GENERAL_ASTROLOG_SLOT_CONFIG >(), true, g_JsonContent[i++].keys);
	}

	void InitLiveObjects()
	{
		Atlas::Engine::InitLiveObjects();
		LiveObject::Register(DDLReflect::GetStruct<SG_PLAYER>());
	}

};
