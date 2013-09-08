#include <ZionBase.h>
#include <AsyncSockIO.h>
#include <AsyncRPC.h>

#include "ServerApp.h"
#include "ServerBase.h"

namespace Zion
{

	void SLog(const char* format, ...)
	{
		va_list args;
		char buffer[2000];
		va_start(args, format);
		vsprintf(buffer, format, args);
		puts(buffer);
	}

	CServerBase::CServerBase(CServerApp* pServerApp) : m_pServerApp(pServerApp)
	{
	}

	CServerBase::~CServerBase()
	{
		Zion::List<CServerComponent*>::iterator i;
		for(i=m_ComponentList.begin(); i!=m_ComponentList.end(); i++)
		{
			delete *i;
		}
		m_ComponentList.clear();
	}

	bool CServerBase::Start()
	{
		Zion::List<CServerComponent*>::iterator i;
		for(i=m_ComponentList.begin(); i!=m_ComponentList.end(); i++)
		{
			(*i)->InitComponent();
		}
		return true;
	}

	void CServerBase::Stop()
	{
		while(!m_ComponentList.empty())
		{
			delete m_ComponentList.back();
			m_ComponentList.pop_back();
		}
	}

	void CServerBase::AddComponent(CServerComponent* pComponent)
	{
		m_ComponentList.push_back(pComponent);
	}

	CServerComponent::CServerComponent(CServerBase* pServer) : m_pServer(pServer)
	{
		m_pServer->AddComponent(this);
	}

	CServerComponent::~CServerComponent()
	{
	}

}
