#ifndef __ATLAS_SERVER_BASE__
#define __ATLAS_SERVER_BASE__

#include <list>

namespace Zion
{
	class CServerBase;
	class CServerComponent;

	void SLog(const char* format, ...);

	class IServerConfig
	{
	public:
		virtual const char* GetConfig(const char* pName, const char* pDefault="") = 0;
	};

	class CServerBase : public CNoCopy
	{
	public:
		CServerBase(CServerApp* pServerApp);
		virtual ~CServerBase();

		CServerApp* GetServerApp() { return m_pServerApp; }

		void AddComponent(CServerComponent* pComponent);

		virtual bool LoadConfig(IServerConfig* pConfig) { return true; }
		virtual bool Start();
		virtual void Stop();

	private:
		CServerApp* m_pServerApp;
		Zion::List<CServerComponent*> m_ComponentList;
	};

	class CServerComponent : public CNoCopy
	{
	public:
		CServerComponent(CServerBase* pServerBase);
		virtual ~CServerComponent();

		virtual void InitComponent() { }

		CServerBase* GetServer() { return m_pServer; }

	private:
		CServerBase* m_pServer;
	};
	
}

#endif
