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
		std::tr1::bind( &SGGAME_STUB::OnData, &m_Stub, std::tr1::placeholders::_1, std::tr1::placeholders::_2, std::tr1::placeholders::_3, std::tr1::placeholders::_4) );
	}

	CSGGameServerComponent::~CSGGameServerComponent()
	{
	}

	void CSGGameServerComponent::CreateAvatar(CSGNodeClient* pClient, const _S8 name[], _U32 type)
	{
	
	}

	void CSGGameServerComponent::QueryAvatar(CSGNodeClient* pClient)
	{
	
	}

	void CSGGameServerComponent::QueryBag(CSGNodeClient* pClient)
	{
	
	}

	void CSGGameServerComponent::Ping(CSGNodeClient* pClient)
	{
		pClient->m_Sender.Pong();
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
	}

	void CSGNodeClient::OnDisconnected()
	{
		CNodeClient::OnDisconnected();
	}

	void CSGNodeClient::OnData(_U16 code, _U32 len, const _U8* data)
	{
	}

	void CSGNodeClient::OnEvent(_U16 code, _U32 len, const _U8* data)
	{
	}

}
