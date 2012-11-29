#ifndef __ATLAS_SERVER_BASE__
#define __ATLAS_SERVER_BASE__

#include <list>

namespace Atlas
{
	class CServerBase;
	class CServerComponent;

	void SLog(const char* format, ...);

	class IServerConfig
	{
	public:
		virtual const char* GetConfig(const char* pName, const char* pDefault="") = 0;
	};

	// 1. 管理Server Component
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
		std::list<CServerComponent*> m_ComponentList;
	};

	// 1. Server的扩展模式
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
