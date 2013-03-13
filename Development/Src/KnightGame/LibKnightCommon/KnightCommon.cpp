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
			->Register(DDLReflect::GetStruct<DATATEST_BASE_CONFIG>(),true,"name");

		//ContentObject::Register(DDLReflect::GetStruct<KNIGHT_SKILL_CONFIG>(),true,"name");
		//ContentObject::Register(DDLReflect::GetStruct<KNIGHT_SKILL_ACTIVE_CONFIG>(),true,"name");
		//ContentObject::Register(DDLReflect::GetStruct<KNIGHT_ITEM_CONFIG>(),true,"name");
		//ContentObject::Register(DDLReflect::GetStruct<KNIGHT_ATTR_MOD_CONFIG>(),true,"name");
		//ContentObject::Register(DDLReflect::GetStruct<KNIGHT_SKILL_BUNDLE_CONFIG>(),true,"name");
		//ContentObject::Register(DDLReflect::GetStruct<KNIGHT_EQUIPMENT_CONFIG>(),true,"name");

		//ContentObject::CreateContentGroup(DDLReflect::GetStruct<KNIGHT_SKILL_CONFIG>(), "skill", "skill.json");
		//ContentObject::CreateContentGroup(DDLReflect::GetStruct<KNIGHT_SKILL_ACTIVE_CONFIG>(), "skill", "skill.json");
	}

	void InitLiveObjects()
	{
		Atlas::Engine::InitLiveObjects();
	}

};
