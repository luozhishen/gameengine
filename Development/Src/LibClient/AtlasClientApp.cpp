#include "AtlasBase.h"
#include "AtlasCommon.h"
#include "AtlasClientApp.h"
#include "AtlasClient.h"
#include "mosdk.h"
#include "ClientConnection.h"
#include "AsyncIOConnection.h"
#include <fstream>

namespace Atlas
{
	static CClientApp* __global_client_app = NULL;

	CClientApp::CClientApp(const char* appname, bool bThread)
	{
		ATLAS_ASSERT(!__global_client_app);
		__global_client_app = this;
		m_bThread = bThread;
		m_bEnableTick = true;
		A_MUTEX_INIT(&m_mtxQueue);

		m_nRecvSize = 20*1024;
		m_nSendSize = 1*1024;

		MOInit(appname);

		LoadParams();
	}

	CClientApp::~CClientApp()
	{
		A_MUTEX_DESTROY(&m_mtxQueue);

		MOFini();

		SaveParams();
	}

	void CClientApp::SetPacketSize(_U32 sendsize, _U32 recvsize)
	{
		m_nSendSize = sendsize;
		m_nRecvSize = recvsize;
	}

	_U32 CClientApp::GetUpPacketSize()
	{
		return m_nSendSize;
	}
	
	_U32 CClientApp::GetDownPacketSize()
	{
		return m_nRecvSize;
	}

	void CClientApp::SetParam(const char* name, const char* value)
	{
		m_Params[name] = value;
	}
	
	const char* CClientApp::GetParam(const char* name, const char* default_value)
	{
		Atlas::Map<Atlas::String, Atlas::String>::iterator i;
		i = m_Params.find(name);
		if(i==m_Params.end()) return default_value;
		return i->second.c_str();
	}
	
	const Atlas::Map<Atlas::String, Atlas::String>& CClientApp::GetParams()
	{
		return m_Params;
	}

	bool CClientApp::LoadParams(const char* path)
	{
		m_Params.clear();

		Atlas::String strXmlFile = path?path:Atlas::AtlasGameDir();
		if(path)
		{
			strXmlFile = Atlas::StringFormat("%s%s", path, "Client.json");
		}
		else
		{
			strXmlFile += "\\Config\\Client.json";
		}
		
		std::ifstream ifs;
		ifs.open(strXmlFile.c_str());
		if(!ifs.is_open()) return false;

		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(ifs, root, false))
		{
		    return false;
		}
		
		for(Json::UInt i = 0; i < root.size(); ++i)
		{
			Atlas::Vector<Atlas::String> vec = root.getMemberNames();
			Atlas::String strKey = vec[i];
			Atlas::String strValue = root[strKey.c_str()].asString();

			m_Params[strKey] = strValue;
		}

		ifs.close();
		
		return true;
	}

	bool CClientApp::SaveParams()
	{
		Json::Value root;
		Json::FastWriter writer;
		
		for(Atlas::Map<Atlas::String, Atlas::String>::const_iterator it = m_Params.begin(); it != m_Params.end(); ++it)
		{
			root[it->first] = it->second;
		}

		Atlas::String json_file = writer.write(root);
		Atlas::String strXmlFile = Atlas::AtlasGameDir();
		strXmlFile += "\\Config\\Client.json";
		std::ofstream ofs;
		ofs.open(strXmlFile.c_str());
		if(!ofs.is_open()) return false;
		ofs<<json_file;
		ofs.close();

		return true;
	}

	CClientApp* CClientApp::GetDefault()
	{
		return __global_client_app;
	}

	void CClientApp::InitApp()
	{
		InitDDLStub();
		InitContentObjects();
		InitLiveObjects();

		if(!ContentObject::BuildIndex())
		{
			ATLAS_ASSERT(0);
		}

#ifndef WITHOUT_ATLAS_ASYNCIO
		CAsyncIOConnection::Init(1);
#endif
	}

	void CClientApp::FiniApp()
	{
#ifndef WITHOUT_ATLAS_ASYNCIO
		CAsyncIOConnection::Fini();
#endif
	}

	CClient* CClientApp::NewClient()
	{
		return ATLAS_NEW CClient(this);
	}

	void CClientApp::EnableTick(bool bEnable)
	{
		m_bEnableTick = bEnable;
	}

	bool CClientApp::Tick(_U32 nTime)
	{
		if(m_bThread) return false;

		bool bQuit = false, bRet = false;
		CLIENTAPP_ITEM i;
		while(!bQuit)
		{
			A_MUTEX_LOCK(&m_mtxQueue);
			if(m_Queue.size()==0)
			{
				bQuit = true;
				A_MUTEX_UNLOCK(&m_mtxQueue);
				break;
			}
			i = m_Queue.front();
			m_Queue.pop_front();
			bRet = true;
			A_MUTEX_UNLOCK(&m_mtxQueue);

			switch(i.nCode)
			{
			case 0:
				i.pClient->OnLoginDone();
				break;
			case 1:
				i.pClient->OnLoginFailed();
				break;
			case 2:
				i.pClient->OnDisconnected();
				break;
			case 3:
				i.pClient->OnData(i.iid, i.fid, i.len, i.data);
				ATLAS_FREE(i.data);
				break;
			}
		}

		Atlas::Set<CClient*>::iterator it;
		for(it=m_Clients.begin(); it!=m_Clients.end(); it++)
		{
			(*it)->Tick();
		}

		return bRet;
	}

	void CClientApp::QueueLoginDone(CClient* pClient)
	{
		if(m_bThread)
		{
			pClient->OnLoginDone();
			return;
		}

		CLIENTAPP_ITEM i = { 0, pClient, 0, 0, 0, NULL };
		A_MUTEX_LOCK(&m_mtxQueue);
		m_Queue.push_back(i);
		A_MUTEX_UNLOCK(&m_mtxQueue);
	}

	void CClientApp::QueueLoginFailed(CClient* pClient)
	{
		if(m_bThread)
		{
			pClient->OnLoginFailed();
			return;
		}

		CLIENTAPP_ITEM i = { 1, pClient, 0, 0, 0, NULL };
		A_MUTEX_LOCK(&m_mtxQueue);
		m_Queue.push_back(i);
		A_MUTEX_UNLOCK(&m_mtxQueue);
	}

	void CClientApp::QueueDisconnected(CClient* pClient)
	{
		if(m_bThread)
		{
			pClient->OnDisconnected();
			return;
		}

		CLIENTAPP_ITEM i = { 2, pClient, 0, 0, 0, NULL };
		A_MUTEX_LOCK(&m_mtxQueue);
		m_Queue.push_back(i);
		A_MUTEX_UNLOCK(&m_mtxQueue);
	}

	void CClientApp::QueueData(CClient* pClient, _U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		if(m_bThread)
		{
			pClient->OnData(iid, fid, len, data);
			return;
		}

		CLIENTAPP_ITEM i = { 3, pClient, iid, fid, len, NULL };
		i.data = (_U8*)ATLAS_ALLOC(len);
		memcpy(i.data, data, len);
		A_MUTEX_LOCK(&m_mtxQueue);
		m_Queue.push_back(i);
		A_MUTEX_UNLOCK(&m_mtxQueue);
	}

	void CClientApp::QueueTask(CClient* pClient, CClientTask* pTask)
	{
		ATLAS_ASSERT(!m_bThread);

		CLIENTAPP_ITEM i = { 4, pClient, 0, 0, 0, NULL };
		A_MUTEX_LOCK(&m_mtxQueue);
		m_Queue.push_back(i);
		A_MUTEX_UNLOCK(&m_mtxQueue);
	}

	void CClientApp::RegisterClient(CClient* pClient)
	{
		m_Clients.insert(pClient);
	}
	
	void CClientApp::UnregisterClient(CClient* pClient)
	{
		m_Clients.erase(pClient);
	}

}
