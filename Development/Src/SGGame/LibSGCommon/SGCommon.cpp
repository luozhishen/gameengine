#include <AtlasBase.h>
#include <AtlasCommon.h>

#include "SGGameDDL.h"

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
		Atlas::Engine::InitContentObjects();
		
		ContentObject::Register(DDLReflect::GetStruct<SG_INSTANCE_CONFIG>(), true, "league_level");
		ContentObject::Register(DDLReflect::GetStruct<SG_LEAGUE_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_LEAGUE_POW_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_LEAGUE_MEMBER_SETTING_CONFIG>());

		ContentObject::Register(DDLReflect::GetStruct<SG_DAILY_ACTION_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_HALO_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_HALO_LEVELUP_CONFIG>());

		ContentObject::Register(DDLReflect::GetStruct<SG_PLAYER_TITLE_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_QUEST_INFO_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_LEVEL_INFO_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_DIRTY_WORD_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_PLAYER_RANDOM_NAME_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_DROP_ITEM_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_LEVEL_DROP_CONFIG>());

		ContentObject::Register(DDLReflect::GetStruct<SG_ENEMY_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_GENERAL_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_SOLDIER_CONFIG>());

		ContentObject::Register(DDLReflect::GetStruct<SG_GENERAL_LEVEL_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_SOLDIER_LEVEL_CONFIG>());
			
		ContentObject::Register(DDLReflect::GetStruct<SG_ITEM_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_EQUIPT_ITEM_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_USABLE_ITEM_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_GEM_ITEM_CONFIG>());
		
		ContentObject::Register(DDLReflect::GetStruct<SG_GOODS_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_GOODS_GROUP_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_SHOP_CONFIG>());

		ContentObject::Register(DDLReflect::GetStruct<SG_PVP_INFO_CONFIG>());

		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_ENEMY_CONFIG>(), "enemy", "enemy.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_GENERAL_CONFIG>(), "general", "general.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_SOLDIER_CONFIG>(), "soldier", "soldier.json");

		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_GENERAL_LEVEL_CONFIG>(), "general_level", "general_level.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_SOLDIER_LEVEL_CONFIG>(), "soldier_level", "soldier_level.json");

		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_ITEM_CONFIG>(), "item", "item.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_EQUIPT_ITEM_CONFIG>(), "equipt", "equipt.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_USABLE_ITEM_CONFIG>(), "usable", "usable.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_GEM_ITEM_CONFIG>(), "gem", "gem.json");

		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_DROP_ITEM_CONFIG>(), "drop_group", "drop_group.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_LEVEL_DROP_CONFIG>(), "level_drop", "level_drop.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_PLAYER_RANDOM_NAME_CONFIG>(), "random_name", "random_name.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_DIRTY_WORD_CONFIG>(), "dirty_word", "dirty_word.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_LEVEL_INFO_CONFIG>(), "level_info", "level_info.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_QUEST_INFO_CONFIG>(), "quest_info", "quest_info.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_PLAYER_TITLE_CONFIG>(), "player_title", "player_title.json");

		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_HALO_CONFIG>(), "halo_info", "halo_info.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_HALO_LEVELUP_CONFIG>(), "halo_levelup", "halo_levelup.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_DAILY_ACTION_CONFIG>(), "daily_action", "daily_action.json");

		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_GOODS_CONFIG>(), "goods_info", "goods_info.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_GOODS_GROUP_CONFIG>(), "goods_group", "goods_group.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_SHOP_CONFIG>(), "shop_info", "shop_info.json");

		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_PVP_INFO_CONFIG>(), "pvp_info", "pvp_info.json");

		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_LEAGUE_MEMBER_SETTING_CONFIG>(), "league_member_setting", "league_member_setting.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_LEAGUE_POW_CONFIG>(), "league_pow", "league_pow.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_LEAGUE_CONFIG>(), "league", "league.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_INSTANCE_CONFIG>(), "instance", "instance.json");
	}

	void InitLiveObjects()
	{
		Atlas::Engine::InitLiveObjects();
		LiveObject::Register(DDLReflect::GetStruct<SG_PLAYER>());
	}

};
