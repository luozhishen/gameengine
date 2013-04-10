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
		RegisterClientStub<KNIGHT_S2C>();
		RegisterServerStub<KNIGHT_C2S>(0);
	}

	void InitContentObjects()
	{
		Atlas::Engine::InitContentObjects();

		ContentObject::CreateContentGroup("AtlasDataTest", "AtlasDataTest.json", true)
			->Register(DDLReflect::GetStruct<DATATEST_BASE_CONFIG>(),false,"name");

		ContentObject::CreateContentGroup("KnightConfig_Item", "KnightConfig_Item.json", false)
			->Register(DDLReflect::GetStruct<KNIGHT_ITEM_CONFIG>(),false,"name");
		ContentObject::CreateContentGroup("KnightConfig_Equipment", "KnightConfig_Equipment.json", false)
			->Register(DDLReflect::GetStruct<KNIGHT_EQUIPMENT_CONFIG>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_ARMOR_CONFIG>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_HELMET_CONFIG>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_SHIELD_CONFIG>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_WEAPON_CONFIG>(),false,"name");
		ContentObject::CreateContentGroup("KnightConfig_Skill", "KnightConfig_Skill.json", false)
			->Register(DDLReflect::GetStruct<KNIGHT_SKILL_CONFIG>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_SKILL_ACTIVE_CONFIG>(),false,"name");
		ContentObject::CreateContentGroup("KnightConfig_Buff", "KnightConfig_Buff.json", false)
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_CONFIG>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_STATS>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_DAMAGE_CORRECTION>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_MAGIC_CORRECTION>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_BUFF_CORRECTION>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_ATTACK_SLICES_CORRECTION>(),false,"name");
		ContentObject::CreateContentGroup("KnightConfig_Calc", "KnightConfig_Calc.json", false)
			->Register(DDLReflect::GetStruct<KNIGHT_CALC_CONFIG>(),false,"name");
	}

	void InitLiveObjects()
	{
		Atlas::Engine::InitLiveObjects();
	}

};
