#ifndef __SGGAME_CLIENT__
#define __SGGAME_CLIENT__

namespace Atlas
{

	class CSGClientCallback
	{
	public:
		virtual ~CSGClientCallback();
		virtual void Pong() = 0;
		virtual void GetServerListResult(const SG_SERVER_INFO* infos, _U32 count) = 0;
		virtual void QueryAvatarFailed(_U32 code) = 0;
		virtual void QueryAvatarDone() = 0;
		virtual void CreateAvatarResult(_U32 code) = 0;
		virtual void QueryPlayerResult() = 0;
		virtual void QueryGeneralResult() = 0;
		virtual void QuerySoldierResult() = 0;
		virtual void QueryBagDone() = 0;
		virtual void BeginBattleResult(const A_UUID& battle) = 0;
		virtual void EndBattleResult(_U32 level, _U32 exp, _U32 gold, const SG_DROP_GROUP_BASE* drops, _U32 drop_count) = 0;
	};

	class CSGClient : public CClient
	{
	public:
		CSGClient(CClientApp* pClientApp, _U32 recvsize=6*1024);
		virtual ~CSGClient();

		void SetCallback(CSGClientCallback* callback);

		virtual void InitializeComponents();

		virtual void OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data);

		DDLProxy::SGGAME_C2S<CSGClient, DDL::TMemoryWriter<10000>>	m_C2S;
		DDLStub::SGGAME_S2C<CSGClient, CSGClient>					m_S2C;

		// UC server
		void Ping();
		void GetServerList();
		void EnterServer(_U32 server_id);
		void QueryAvatar();
		void CreateAvatar(const char* nick, _U32 general_id);
		void DeleteAvatar();
		void EnterGame();
		void LeaveGame();
		void QueryPlayer();
		void QueryGenerals();
		void QuerySoldiers();
		void QueryBag();
		void EquipItem(_U32 general_id, A_UUID item_uuid);
		void BeginBattle(const char* name);
		void EndBattle(const char* name, _U32 result);

		// UC data

		//
		void Pong(CSGClient* pClient);
		void GetServerListResult(CSGClient* pClient, const SG_SERVER_INFO* infos, _U32 count);
		void QueryAvatarFailed(CSGClient* pClient, _U32 code);
		void QueryAvatarResult(CSGClient* pClient, const SG_PLAYER& player);
		void CreateAvatarResult(CSGClient* pClient, _U32 code);
		void QueryPlayerResult(CSGClient* pClient, const SG_PLAYER& player);
		void QueryGeneralResult(CSGClient* pClient, const SG_GENERAL* generals, _U32 count);
		void QuerySoldierResult(CSGClient* pClient, const SG_SOLDIER* soldiers, _U32 count);
		void QueryBagBegin(CSGClient* pClient);
		void QueryBagEquipt(CSGClient* pClient, const SG_EQUIPT_ITEM* items, _U32 count);
		void QueryBagUsable(CSGClient* pClient, const SG_USABLE_ITEM* items, _U32 count);
		void QueryBagGen(CSGClient* pClient, const SG_GEM_ITEM* items, _U32 count);
		void QueryBagEnd(CSGClient* pClient);
		void BeginBattleResult(CSGClient* pClient, const A_UUID& battle);
		void EndBattleResult(CSGClient* pClient, _U32 level, _U32 exp, _U32 gold, const SG_DROP_GROUP_BASE* drops, _U32 drop_count);
	};

}

#endif
