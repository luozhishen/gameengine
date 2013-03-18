#include <AtlasBase.h>
#include <AtlasCommon.h>

#include "SGGameDDL.h"
#include <ContentObject.h>

namespace Atlas
{

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

#define CREATE_CONTENT_GROUP_AND_REGISTER(CONFIG, EXPAND_NAME, KEYS) \
	pContentGroup = ContentObject::CreateContentGroup(#CONFIG, #CONFIG##EXPAND_NAME, true); \
	pContentGroup->Register(DDLReflect::GetStruct< CONFIG >(), true, KEYS);

		//ContentObject::IContentGroup* pContentGroup = ContentObject::CreateContentGroup("SGGame", "sggame.json", true);
		ContentObject::IContentGroup* pContentGroup = NULL;

		CREATE_CONTENT_GROUP_AND_REGISTER(SG_PAY_CONSUME_CONFIG, ".json", "apply_times");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_PAY_REWARD_CONFIG, ".json", "player_level");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_INSTANCE_CONFIG, ".json", "instance_id");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_LEAGUE_CONFIG, ".json", "league_level");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_LEAGUE_POW_CONFIG, ".json", "pow_type");

		CREATE_CONTENT_GROUP_AND_REGISTER(SG_LEAGUE_MEMBER_SETTING_CONFIG, ".json", "master_num");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_DAILY_ACTION_CONFIG, ".json", "type");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_HALO_CONFIG, ".json", "halo_level");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_HALO_LEVELUP_CONFIG, ".json", "level");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_PLAYER_TITLE_CONFIG, ".json", "title_level");

		CREATE_CONTENT_GROUP_AND_REGISTER(SG_QUEST_INFO_CONFIG, ".json", "quest_id");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_LEVEL_INFO_CONFIG, ".json", "level_id");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_DIRTY_WORD_CONFIG, ".json", "dirty_word");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_PLAYER_RANDOM_NAME_CONFIG, ".json", "family_name");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_DROP_ITEM_CONFIG, ".json", "group_id,item_id");

		CREATE_CONTENT_GROUP_AND_REGISTER(SG_LEVEL_DROP_CONFIG, ".json", "level_name");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_ENEMY_CONFIG, ".json", "enemy_id");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_GENERAL_CONFIG, ".json", "general_id,attr_id");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_SOLDIER_CONFIG, ".json", "soldier_id,attr_id");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_GENERAL_LEVEL_CONFIG, ".json", "attr_id,level");

		CREATE_CONTENT_GROUP_AND_REGISTER(SG_SOLDIER_LEVEL_CONFIG, ".json", "attr_id,level");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_ITEM_CONFIG, ".json", "item_id");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_EQUIPT_ITEM_CONFIG, ".json", "item_id");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_USABLE_ITEM_CONFIG, ".json", "item_id");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_GEM_ITEM_CONFIG, ".json", "item_id");

		CREATE_CONTENT_GROUP_AND_REGISTER(SG_GOODS_CONFIG, ".json", "good_id");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_GOODS_GROUP_CONFIG, ".json", "goods_group_id,good_id");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_SHOP_CONFIG, ".json", "shop_id,goods_group_id");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_PVP_INFO_CONFIG, ".json", "server_level_lower,server_level_upper,lower,upper");
		CREATE_CONTENT_GROUP_AND_REGISTER(SG_PVP_SETTING_CONFIG, ".json", "increase_pay_rmb");

		CREATE_CONTENT_GROUP_AND_REGISTER(SG_VIP_CONFIG, ".json", "vip_level");


		/*pContentGroup->Register(DDLReflect::GetStruct<SG_PAY_CONSUME_CONFIG>(), true, "apply_times");
		pContentGroup->Register(DDLReflect::GetStruct<SG_PAY_REWARD_CONFIG>(), true, "player_level");
		pContentGroup->Register(DDLReflect::GetStruct<SG_INSTANCE_CONFIG>(), true, "instance_id");
		pContentGroup->Register(DDLReflect::GetStruct<SG_LEAGUE_CONFIG>(), true, "league_level");
		pContentGroup->Register(DDLReflect::GetStruct<SG_LEAGUE_POW_CONFIG>(), true,  "pow_type");

		pContentGroup->Register(DDLReflect::GetStruct<SG_LEAGUE_MEMBER_SETTING_CONFIG>(), true,  "master_num");
		pContentGroup->Register(DDLReflect::GetStruct<SG_DAILY_ACTION_CONFIG>(), true,  "type");
		pContentGroup->Register(DDLReflect::GetStruct<SG_HALO_CONFIG>(), true,  "halo_level");
		pContentGroup->Register(DDLReflect::GetStruct<SG_HALO_LEVELUP_CONFIG>(), true,  "level");
		pContentGroup->Register(DDLReflect::GetStruct<SG_PLAYER_TITLE_CONFIG>(), true,  "title_level");

		pContentGroup->Register(DDLReflect::GetStruct<SG_QUEST_INFO_CONFIG>(), true,  "quest_id");
		pContentGroup->Register(DDLReflect::GetStruct<SG_LEVEL_INFO_CONFIG>(), true,  "level_id");
		pContentGroup->Register(DDLReflect::GetStruct<SG_DIRTY_WORD_CONFIG>(), true,  "dirty_word");
		pContentGroup->Register(DDLReflect::GetStruct<SG_PLAYER_RANDOM_NAME_CONFIG>(), true,  "family_name");
		pContentGroup->Register(DDLReflect::GetStruct<SG_DROP_ITEM_CONFIG>(), true,  "group_id,item_id");

		pContentGroup->Register(DDLReflect::GetStruct<SG_LEVEL_DROP_CONFIG>(), true,  "level_name");
		pContentGroup->Register(DDLReflect::GetStruct<SG_ENEMY_CONFIG>(), true,  "enemy_id");
		pContentGroup->Register(DDLReflect::GetStruct<SG_GENERAL_CONFIG>(), true,  "general_id,attr_id");
		pContentGroup->Register(DDLReflect::GetStruct<SG_SOLDIER_CONFIG>(), true,  "soldier_id,attr_id");
		pContentGroup->Register(DDLReflect::GetStruct<SG_GENERAL_LEVEL_CONFIG>(), true,  "attr_id,level");

		pContentGroup->Register(DDLReflect::GetStruct<SG_SOLDIER_LEVEL_CONFIG>(), true,  "attr_id,level");
		pContentGroup->Register(DDLReflect::GetStruct<SG_ITEM_CONFIG>(), true,  "item_id");
		pContentGroup->Register(DDLReflect::GetStruct<SG_EQUIPT_ITEM_CONFIG>(), true,  "item_id");
		pContentGroup->Register(DDLReflect::GetStruct<SG_USABLE_ITEM_CONFIG>(), true,  "item_id");
		pContentGroup->Register(DDLReflect::GetStruct<SG_GEM_ITEM_CONFIG>(), true,  "item_id");

		pContentGroup->Register(DDLReflect::GetStruct<SG_GOODS_CONFIG>(), true,  "good_id");
		pContentGroup->Register(DDLReflect::GetStruct<SG_GOODS_GROUP_CONFIG>(), true,  "goods_group_id,good_id");
		pContentGroup->Register(DDLReflect::GetStruct<SG_SHOP_CONFIG>(), true,  "shop_id,goods_group_id");
		pContentGroup->Register(DDLReflect::GetStruct<SG_PVP_INFO_CONFIG>(), true,  "server_level_lower,server_level_upper,lower,upper");
		*/

#undef CREATE_CONTENT_GROUP_AND_REGISTER
	}

	void InitLiveObjects()
	{
		Atlas::Engine::InitLiveObjects();
		LiveObject::Register(DDLReflect::GetStruct<SG_PLAYER>());
	}

};
