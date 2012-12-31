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
	}

	void InitLiveObjects()
	{
		Atlas::Engine::InitLiveObjects();
		LiveObject::Register(DDLReflect::GetStruct<SG_PLAYER>());
	}

};
