#ifndef __SGCLIENT_SOLDIER_COMPONENT_H__
#define __SGCLIENT_SOLDIER_COMPONENT_H__

#include "SGGameDDL.h"

namespace Atlas
{
	class CSGClientSoldierComponent : public CClientComponent
	{
	public:
		CSGClientSoldierComponent(CSGClient* pClient);
		virtual ~CSGClientSoldierComponent();

		void GetAvaliableSoldiers(std::vector<_U32>& soldiers);
		const SG_SOLDIER* GetSoldierlInfo(_U32 type);

	private:
		std::vector<SG_SOLDIER> m_vecSoldiers;
	};

}

#endif //__SGCLIENT_SOLDIER_COMPONENT_H__