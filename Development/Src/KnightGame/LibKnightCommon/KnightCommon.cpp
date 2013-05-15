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
			->Register(DDLReflect::GetStruct<DATATEST_BASE_CONFIG>(),true,"name");
		ContentObject::CreateContentGroup("LuaCommonValidateInterface", "LuaValidateInterface.json", true)
			->Register(DDLReflect::GetStruct<LUA_COMMON_VALIDATE_INTERFACE>(),true,"name");
		ContentObject::CreateContentGroup("KnightConfig_Item", "KnightConfig_Item.json", true)
			->Register(DDLReflect::GetStruct<KNIGHT_ITEM_CONFIG>(),false,"name");
		ContentObject::CreateContentGroup("KnightConfig_Equipment", "KnightConfig_Equipment.json", true)
			->Register(DDLReflect::GetStruct<KNIGHT_EQUIPMENT_CONFIG>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_ARMOR_CONFIG>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_HELMET_CONFIG>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_SHIELD_CONFIG>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_WEAPON_CONFIG>(),false,"name");
		ContentObject::CreateContentGroup("KnightConfig_Mount", "KnightConfig_Mount.json", true)
			->Register(DDLReflect::GetStruct<KNIGHT_MOUNT_CONFIG>(),false,"name");
		ContentObject::CreateContentGroup("KnightConfig_Skill", "KnightConfig_Skill.json", true)
			->Register(DDLReflect::GetStruct<KNIGHT_SKILL_CONFIG>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_SKILL_ACTIVE_CONFIG>(),false,"name");
		ContentObject::CreateContentGroup("KnightConfig_Buff", "KnightConfig_Buff.json", true)
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_CONFIG>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_STATS>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_DAMAGE_CORRECTION>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_MAGIC_CORRECTION>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_BUFF_CORRECTION>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_DOT>(),false,"name")
			->Register(DDLReflect::GetStruct<KNIGHT_BUFF_ATTACK_SLICES_CORRECTION>(),false,"name");
		ContentObject::CreateContentGroup("KnightConfig_Calc", "KnightConfig_Calc.json", true)
			->Register(DDLReflect::GetStruct<KNIGHT_CALC_CONFIG>(),false,"name");
		ContentObject::CreateContentGroup("KnightConfig_Attack", "KnightConfig_Attack.json", false)
			->Register(DDLReflect::GetStruct<KNIGHT_ATTACK_CONFIG>(),false,"name");
		ContentObject::CreateContentGroup("Knightconfig_Mantra", "Knightconfig_Mantra.json", true)
			->Register(DDLReflect::GetStruct<KNIGHT_MANTRA_CONFIG>(),false,"name");
	}

	void InitLiveObjects()
	{
		Atlas::Engine::InitLiveObjects();
	}

};
