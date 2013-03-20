#include <AtlasBase.h>
#include <AtlasCommon.h>

#include "KnightGameDDL.h"

namespace Atlas
{

	const char* AtlasGameName()
	{
		return "KnightGame";
	}

	void InitDDLStub()
	{
		Atlas::Engine::InitDDLStub();
	}

	void InitContentObjects()
	{
		Atlas::Engine::InitContentObjects();

		ContentObject::CreateContentGroup("AtlasDataTest", "AtlasDataTest.json", true)
			->Register(DDLReflect::GetStruct<DATATEST_BASE_CONFIG>(),false,"name");

		ContentObject::CreateContentGroup("KnightConfig", "KnightConfig.json", true)
			->Register(DDLReflect::GetStruct<KNIGHT_SKILL_ACTIVE_CONFIG>(),true,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_SKILL_PASSIVE_CONFIG>(),true,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_CONFIG>(),true,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_STATS>(),true,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_DAMAGE_CORRECTION>(),true,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_MAGIC_CORRECTION>(),true,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_BUFF_CORRECTION>(),true,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_ITEM_CONFIG>(),true,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_EQUIPMENT_CONFIG>(),true,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_ARMOR_CONFIG>(),true,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_HELMET_CONFIG>(),true,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_SHIELD_CONFIG>(),true,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_WEAPON_CONFIG>(),true,"name");
	}

	void InitLiveObjects()
	{
		Atlas::Engine::InitLiveObjects();
	}

};
