#ifndef __SGCLIENT_PLAYER_COMPONENT_H__
#define __SGCLIENT_PLAYER_COMPONENT_H__

#include "SGGameDDL.h"

namespace Atlas
{
	class CSGClientPlayerComponent : public CClientComponent
	{
	public:
		CSGClientPlayerComponent(CSGClient* pClient);
		virtual ~CSGClientPlayerComponent();

		const SG_PLAYER* GetPlayerInfo();

	private:
		SG_PLAYER* m_pPlayerInfo;
	};
}

#endif //__SGCLIENT_PLAYER_COMPONENT_H__