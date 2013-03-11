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
		ContentObject::Register(DDLReflect::GetStruct<KNIGHT_SKILL_CONFIG>());
	}

	void InitLiveObjects()
	{
		Atlas::Engine::InitLiveObjects();
	}

};
