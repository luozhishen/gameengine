#ifndef __SGGAME_NODE__
#define __SGGAME_NODE__

namespace Atlas
{

	class CSGGameServerComponent;
	class CSGNodeServer;
	class CSGNodeClient;

	class CSGGameServerComponent : public CServerComponent
	{
	public:
		typedef DDLStub::SGGAME_C2S<CSGNodeClient, CSGGameServerComponent>			SGGAME_C2S_STUB;
		typedef CNodeStub<SGGAME_C2S_STUB, CSGNodeClient, CSGGameServerComponent>	SGGAME_STUB;

		CSGGameServerComponent(CSGNodeServer* pServer);
		virtual ~CSGGameServerComponent();

		void CreateAvatar(CSGNodeClient* pClient, _S8 name[]);
		void QueryAvatar(CSGNodeClient* pClient);
		void Ping(CSGNodeClient* pClient);

	private:
		SGGAME_STUB m_Stub;
	};

	class CSGNodeServer : public CNodeServer
	{
	public:
		CSGNodeServer(CServerApp* pServerApp);
		virtual ~CSGNodeServer();

		virtual CNodeClient* NewNodeClient(_U64 nndx, _U64 cndx, HSERVER hCluster);
	};

	class CSGNodeClient : public CNodeClient
	{
	public:
		CSGNodeClient(CSGNodeServer* pServer, _U64 nndx, _U64 cndx, HSERVER hCluster);
		virtual ~CSGNodeClient();

		virtual void OnConnected(_U32 len, const _U8* data);
		virtual void OnDisconnected();
		virtual void OnData(_U16 code, _U32 len, const _U8* data);
		virtual void OnEvent(_U16 code, _U32 len, const _U8* data);
		DDLProxy::SGGAME_S2C<CSGNodeClient, DDL::TMemoryWriter<1*1024>> m_Sender;

	};

}

#endif
