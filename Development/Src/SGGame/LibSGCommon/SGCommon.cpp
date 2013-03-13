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
		//Atlas::Engine::InitContentObjects();

		ContentObject::CreateContentGroup("SGGame", "sggame.json", true)
			->Register(DDLReflect::GetStruct<SG_INSTANCE_CONFIG>(), true, "instance_id")
			->Register(DDLReflect::GetStruct<SG_LEAGUE_CONFIG>(), true, "league_level")
			->Register(DDLReflect::GetStruct<SG_LEAGUE_POW_CONFIG>(), true,  "pow_type")
			->Register(DDLReflect::GetStruct<SG_LEAGUE_MEMBER_SETTING_CONFIG>(), true,  "master_num")
			->Register(DDLReflect::GetStruct<SG_DAILY_ACTION_CONFIG>(), true,  "type")
			->Register(DDLReflect::GetStruct<SG_HALO_CONFIG>(), true,  "halo_level")
			->Register(DDLReflect::GetStruct<SG_HALO_LEVELUP_CONFIG>(), true,  "level")
			->Register(DDLReflect::GetStruct<SG_PLAYER_TITLE_CONFIG>(), true,  "title_level")
			->Register(DDLReflect::GetStruct<SG_QUEST_INFO_CONFIG>(), true,  "quest_id")
			->Register(DDLReflect::GetStruct<SG_LEVEL_INFO_CONFIG>(), true,  "level_id")
			->Register(DDLReflect::GetStruct<SG_DIRTY_WORD_CONFIG>(), true,  "dirty_word")
			->Register(DDLReflect::GetStruct<SG_PLAYER_RANDOM_NAME_CONFIG>(), true,  "family_name")
			->Register(DDLReflect::GetStruct<SG_DROP_ITEM_CONFIG>(), true,  "group_id,item_id")
			->Register(DDLReflect::GetStruct<SG_LEVEL_DROP_CONFIG>(), true,  "level_name")
			->Register(DDLReflect::GetStruct<SG_ENEMY_CONFIG>(), true,  "enemy_id")
			->Register(DDLReflect::GetStruct<SG_GENERAL_CONFIG>(), true,  "general_id,attr_id")
			->Register(DDLReflect::GetStruct<SG_SOLDIER_CONFIG>(), true,  "soldier_id,attr_id")
			->Register(DDLReflect::GetStruct<SG_GENERAL_LEVEL_CONFIG>(), true,  "attr_id,level")
			->Register(DDLReflect::GetStruct<SG_SOLDIER_LEVEL_CONFIG>(), true,  "attr_id,level")
			->Register(DDLReflect::GetStruct<SG_ITEM_CONFIG>(), true,  "item_id")
			->Register(DDLReflect::GetStruct<SG_EQUIPT_ITEM_CONFIG>(), true,  "item_id")
			->Register(DDLReflect::GetStruct<SG_USABLE_ITEM_CONFIG>(), true,  "item_id")
			->Register(DDLReflect::GetStruct<SG_GEM_ITEM_CONFIG>(), true,  "item_id")
			->Register(DDLReflect::GetStruct<SG_GOODS_CONFIG>(), true,  "good_id")
			->Register(DDLReflect::GetStruct<SG_GOODS_GROUP_CONFIG>(), true,  "goods_group_id,good_id")
			->Register(DDLReflect::GetStruct<SG_SHOP_CONFIG>(), true,  "shop_id,goods_group_id")
			->Register(DDLReflect::GetStruct<SG_PVP_INFO_CONFIG>(), true,  "server_level_lower,server_level_upper,lower,upper");
	}

	void InitLiveObjects()
	{
		Atlas::Engine::InitLiveObjects();
		LiveObject::Register(DDLReflect::GetStruct<SG_PLAYER>());
	}

};
