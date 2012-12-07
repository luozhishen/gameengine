#include <AtlasBase.h>
#include <AtlasCommon.h>
#include <AtlasServer.h>

#include "SGCommon.h"
#include "SGNode.h"

namespace Atlas
{
	CSGGameServerComponent::CSGGameServerComponent(CSGNodeServer* pServer) : CServerComponent(pServer), m_Stub(this)
	{
		pServer->RegisterStub(GetServerStubID<::SGGAME_C2S>(),
		std::tr1::bind( &SGGAME_STUB::OnData, &m_Stub, std::tr1::placeholders::_1, std::tr1::placeholders::_2, std::tr1::placeholders::_3, std::tr1::placeholders::_4)
		);
	}

	CSGGameServerComponent::~CSGGameServerComponent()
	{
	}

	void CSGGameServerComponent::CreateAvatar(CSGNodeClient* pClient, _S8 name[])
	{
	
	}

	void CSGGameServerComponent::QueryAvatar(CSGNodeClient* pClient)
	{
	
	}

	CSGNodeServer::CSGNodeServer(CServerApp* pServerApp) : CNodeServer(pServerApp, 0)
	{
		AddComponent(ATLAS_NEW CSGGameServerComponent(this));
	}

	CSGNodeServer::~CSGNodeServer()
	{
	}

	CNodeClient* CSGNodeServer::NewNodeClient(_U64 nndx, _U64 cndx, HSERVER hCluster)
	{
		return ATLAS_NEW CSGNodeClient(this, nndx, cndx, hCluster);
	}

	CSGNodeClient::CSGNodeClient(CSGNodeServer* pServer, _U64 nndx, _U64 cndx, HSERVER hCluster) : CNodeClient(pServer, nndx, cndx, hCluster), m_Sender(this)
	{
	}

	CSGNodeClient::~CSGNodeClient()
	{
	}

	void CSGNodeClient::OnConnected(_U32 len, const _U8* data)
	{
		/*Lobby.LoginDone();*/
		char szInfo[256];
		_U32 nUID = *((_U32*)data);
		sprintf(szInfo, "len:%d, data:%d\n", len, nUID);
		OutputDebugString(szInfo);

		//if(!m_DBPlayer)
		//{
		//	m_DBPlayer = ATLAS_NEW CSimpleDBFile<Simple_Info>(nUID);
		//}

		//if(!m_DBPlayer->IsExist())
		//{
		//	m_DBPlayer->Generate();
		//}

		//if(!m_DBGenral)
		//{
		//	m_DBGenral = ATLAS_NEW CSimpleDBFile<Simple_Info>(nUID);
		//}

		//if(!m_DBGenral->IsExist())
		//{
		//	m_DBGenral->Generate();
		//}

		//if(!m_DBSoldier)
		//{
		//	m_DBSoldier = ATLAS_NEW CSimpleDBFile<Simple_Info>(nUID);
		//}

		//if(!m_DBSoldier->IsExist())
		//{
		//	m_DBSoldier->Generate();
		//}

		//if(!m_DBEquip)
		//{
		//	m_DBEquip = ATLAS_NEW CSimpleDBFile<Simple_Info>(nUID);
		//}

		//if(!m_DBEquip->IsExist())
		//{
		//	m_DBEquip->Generate();
		//}

	}

	void CSGNodeClient::OnDisconnected()
	{
		CNodeClient::OnDisconnected();
		//if(m_DBPlayer)
		//{
		//	//ATLAS_FREE m_DBFile;
		//	m_DBPlayer->Save();
		//}

		//if(m_DBGenral)
		//{
		//	//ATLAS_FREE m_DBFile;
		//	m_DBGenral->Save();
		//}

		//if(m_DBSoldier)
		//{
		//	//ATLAS_FREE m_DBFile;
		//	m_DBSoldier->Save();
		//}

		//if(m_DBEquip)
		//{
		//	//ATLAS_FREE m_DBFile;
		//	m_DBEquip->Save();
		//}
	}

	void CSGNodeClient::OnData(_U16 code, _U32 len, const _U8* data)
	{
	}

	void CSGNodeClient::OnEvent(_U16 code, _U32 len, const _U8* data)
	{
	}
}
