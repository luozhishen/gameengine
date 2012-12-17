#include <AtlasBase.h>
#include <AtlasClient.h>
#include <AtlasCommon.h>

#include "SGCommon.h"
#include "SGClient.h"

#include "SGClientGeneralComponent.h"

namespace Atlas
{

	CSGClientGeneralComponent::CSGClientGeneralComponent(CSGClient* pClient) : CClientComponent(pClient)
	{

	}

	CSGClientGeneralComponent::~CSGClientGeneralComponent()
	{

	}

	void CSGClientGeneralComponent::GetGeneralComponent(std::vector<_U32>& genrels)
	{
		for(std::vector<SG_GENERAL>::iterator it = m_vecGenerals.begin(); it != m_vecGenerals.end(); ++it)
		{
			genrels.push_back(it->general_id);
		}
	}

	const SG_GENERAL* CSGClientGeneralComponent::GetGenerallInfo(_U32 type)
	{
		for(std::vector<SG_GENERAL>::iterator it = m_vecGenerals.begin(); it != m_vecGenerals.end(); ++it)
		{
			if(type==it->general_id)
			{
				return &*it;
			}
		}

		return NULL;
	}

}