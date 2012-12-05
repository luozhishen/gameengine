#ifndef __SGGAME_CLIENT__
#define __SGGAME_CLIENT__

namespace Atlas
{

	class CSGClientPlayerComponent;
	class CSGClientGeneralComponent;
	class CSGClientSoldierComponent;

	class CSGClient : public CClient
	{
	public:
		CSGClient(CClientApp* pClientApp, _U32 recvsize=6*1024);
		virtual ~CSGClient();

		inline CSGClientPlayerComponent* GetPlayerComponent()		{ return m_pPlayerComponent; }
		inline CSGClientGeneralComponent* GetGeneralComponent()		{ return m_pGeneralComponent; }
		inline CSGClientSoldierComponent* GetSoldierComponent()		{ return m_pSoldierComponent; }

		virtual void InitializeComponents();

	private:
		CSGClientPlayerComponent*	m_pPlayerComponent;
		CSGClientGeneralComponent*	m_pGeneralComponent;
		CSGClientSoldierComponent*	m_pSoldierComponent;
	};

}

#endif
