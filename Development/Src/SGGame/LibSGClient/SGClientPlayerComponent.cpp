#include <AtlasBase.h>
#include <AtlasClient.h>
#include <AtlasCommon.h>

#include "SGCommon.h"
#include "SGClient.h"

#include "SGClientPlayerComponent.h"

namespace Atlas
{

	CSGClientPlayerComponent::CSGClientPlayerComponent(CSGClient* pClient) : CClientComponent(pClient)
	{
		m_pPlayerInfo = ATLAS_NEW SG_PLAYER;
		m_pPlayerInfo->nick = "nickname";
		m_pPlayerInfo->gold = 999;
		m_pPlayerInfo->rmb = 888;
		m_pPlayerInfo->sex = 12;
		//m_pPlayerInfo->uuid = 999;

		//m_pPlayerInfo->equip_generals;
		//m_pPlayerInfo->equip_soldiers;
	}

	CSGClientPlayerComponent::~CSGClientPlayerComponent()
	{

	}

	const SG_PLAYER* CSGClientPlayerComponent::GetPlayerInfo()
	{
		return m_pPlayerInfo;
	}

}