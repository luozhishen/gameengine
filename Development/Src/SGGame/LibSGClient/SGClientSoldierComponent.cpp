#include <AtlasBase.h>
#include <AtlasClient.h>
#include <AtlasCommon.h>

#include "SGCommon.h"
#include "SGClient.h"

#include "SGClientSoldierComponent.h"

namespace Atlas
{
	CSGClientSoldierComponent::CSGClientSoldierComponent(CSGClient* pClient) : CClientComponent(pClient)
	{
	
	}
	
	CSGClientSoldierComponent::~CSGClientSoldierComponent()
	{
	
	}

	void CSGClientSoldierComponent::GetAvaliableSoldiers(std::vector<_U32>& soldiers)
	{
		for(std::vector<SG_SOLDIER>::iterator it = m_vecSoldiers.begin(); it != m_vecSoldiers.end(); ++it)
		{
			soldiers.push_back(it->type);
		}
	}

	const SG_SOLDIER* CSGClientSoldierComponent::GetSoldierlInfo(_U32 type)
	{
		for(std::vector<SG_SOLDIER>::iterator it = m_vecSoldiers.begin(); it != m_vecSoldiers.end(); ++it)
		{
			if(type==it->type)
			{
				return &*it;
			}
		}

		return NULL;
	}

}
