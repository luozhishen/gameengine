#include <AtlasBase.h>
#include <AtlasCommon.h>

#include "SGDDL.h"

namespace Atlas
{

	const char* AtlasGameName()
	{
		return "SGGame";
	}

	void InitDDLStub()
	{
		Atlas::Engine::InitDDLStub();
	}

	void InitContentObjects()
	{
		Atlas::Engine::InitContentObjects();
		ContentObject::Register(DDLReflect::GetStruct<SG_ENEMY_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_EQUIP_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_GENERAL_CONFIG>());
		ContentObject::Register(DDLReflect::GetStruct<SG_SOLDIER_CONFIG>());

		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_ENEMY_CONFIG>(), "enemy", "enemy.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_EQUIP_CONFIG>(), "equip", "equip.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_GENERAL_CONFIG>(), "general", "general.json");
		ContentObject::CreateContentGroup(DDLReflect::GetStruct<SG_SOLDIER_CONFIG>(), "soldier", "soldier.json");
	}

	void InitLiveObjects()
	{
		Atlas::Engine::InitLiveObjects();
		LiveObject::Register(DDLReflect::GetStruct<SG_PLAYER>());
	}

	void InitSyncObjects()
	{
		Atlas::Engine::InitSyncObjects();
	}

};
