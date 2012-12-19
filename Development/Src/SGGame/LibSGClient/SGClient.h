#ifndef __SGGAME_CLIENT__
#define __SGGAME_CLIENT__

namespace Atlas
{

	class CSGClient : public CClient
	{
	public:
		CSGClient(CClientApp* pClientApp, _U32 recvsize=6*1024);
		virtual ~CSGClient();

		virtual void InitializeComponents();

		virtual void OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data);

		DDLProxy::SGGAME_C2S<CSGClient, DDL::TMemoryWriter<10000>>	m_C2S;
		DDLStub::SGGAME_S2C<CSGClient, CSGClient>					m_S2C;

		void QueryAvatarFailed(CSGClient* pClient, _U32 code);
		void QueryAvatarResult(CSGClient* pClient, const SG_PLAYER& player);
		void CreatAvatarResult(CSGClient* pClient, _U32 code);
		void QueryBagBegin(CSGClient* pClient);
		void QueryBagEquipt(CSGClient* pClient, const SG_EQUIPT_ITEM& item);
		void QueryBagUsable(CSGClient* pClient, const SG_USABLE_ITEM& item);
		void QueryBagGen(CSGClient* pClient, const SG_GEM_ITEM& item);
		void QueryBagEnd(CSGClient* pClient);
		void Pong(CSGClient* pClient);
	};

}

#endif
