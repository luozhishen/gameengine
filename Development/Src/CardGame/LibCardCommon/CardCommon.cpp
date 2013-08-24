#include <ZionBase.h>
#include <ZionCommon.h>

#include "CardGameDDL.h"

namespace Zion
{

	const char* ZionGameName()
	{
		return "CardGame";
	}

	void InitDDLStub()
	{
		Zion::Engine::InitDDLStub();

		RegisterClientStub<CARDGAME_S2C>();
		RegisterServerStub<CARDGAME_C2S>(0);
	}

	void InitContentObjects()
	{
		Zion::Engine::InitContentObjects();

		ContentObject::IContentGroup* pContentGroup;
		
		pContentGroup = ContentObject::CreateContentGroup("Card", "card.json", true);
		pContentGroup->Register(DDLReflect::GetStruct<CARD_AVATAR_OPERATOR>(), true, "uuid");
	}

	void InitLiveObjects()
	{
		Zion::Engine::InitLiveObjects();

		Zion::LiveObject::Register(DDLReflect::GetStruct<CARD_AVATAR_DESC>());
		Zion::LiveObject::Register(DDLReflect::GetStruct<CARD_AVATAR>());
		Zion::LiveObject::Register(DDLReflect::GetStruct<CARD_AVATAR_OWNOBJ>());
	}

};
