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

		CSGClientPlayerComponent* GetPlayerComponent()		{ return pPlayerComponent; }
		CSGClientGeneralComponent* GetGeneralComponent()	{ return pGeneralComponent; }
		CSGClientSoldierComponent* GetSoldierComponent()	{ return pSoldierComponent; }

	private:
		CSGClientPlayerComponent*	pPlayerComponent;
		CSGClientGeneralComponent*	pGeneralComponent;
		CSGClientSoldierComponent*	pSoldierComponent;
	};

	class CSGClientPlayerComponent : public CClientComponent
	{
	public:
		CSGClientPlayerComponent(CSGClient* pClient);
		virtual ~CSGClientPlayerComponent();
	};

	class CSGClientGeneralComponent : public CClientComponent
	{
	public:
		CSGClientGeneralComponent(CSGClient* pClient);
		virtual ~CSGClientGeneralComponent();
	};

	class CSGClientSoldierComponent : public CClientComponent
	{
	public:
		CSGClientSoldierComponent(CSGClient* pClient);
		virtual ~CSGClientSoldierComponent();
	};

}

#endif
