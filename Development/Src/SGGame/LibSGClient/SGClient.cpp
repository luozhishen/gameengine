#include <AtlasBase.h>
#include <AtlasClient.h>
#include <AtlasCommon.h>

#include "SGCommon.h"
#include "SGClient.h"

#include "SGClientSoldierComponent.h"
#include "SGClientPlayerComponent.h"
#include "SGClientGeneralComponent.h"

namespace Atlas
{
	
	CSGClient::CSGClient(CClientApp* pClientApp, _U32 recvsize) : CClient(pClientApp, recvsize)
	{

		InitializeComponents();

		AddComponent(m_pPlayerComponent);
		AddComponent(m_pGeneralComponent);
		AddComponent(m_pSoldierComponent);

	}	

	CSGClient::~CSGClient()
	{

	}

	void CSGClient::InitializeComponents()
	{
		m_pPlayerComponent = ATLAS_NEW CSGClientPlayerComponent(this);
		m_pGeneralComponent = ATLAS_NEW CSGClientGeneralComponent(this);
		m_pSoldierComponent = ATLAS_NEW CSGClientSoldierComponent(this);
	}
}
