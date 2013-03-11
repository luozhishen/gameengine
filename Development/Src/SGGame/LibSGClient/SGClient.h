#ifndef __SGGAME_CLIENT__
#define __SGGAME_CLIENT__

namespace Atlas
{
	class CSGSyncDataManager;

	class CSGClientCallback
	{
	public:
		virtual ~CSGClientCallback();
		
		virtual void NetworkTestResult(bool bSucc) = 0;
		virtual void LoginResult(int LoginResult) = 0;
		virtual void LogoutDone() = 0;
		virtual void DisconnectNotify() = 0;
		virtual void HttpStateNotify(bool IsOnline)= 0;

		virtual void SyncDone() = 0;
		virtual void DataUpdate(int nDataType) = 0;

		virtual void GetServerListResult(const SG_SERVER_INFO* infos, _U32 count, _U32 last_server) = 0;
		virtual void QueryAvatarFailed(_U32 code) = 0;
		virtual void QueryAvatarDone(const SG_PLAYER& player) = 0;
		virtual void CreateAvatarResult(_U32 code) = 0;
		virtual void QueryPlayerResult(const SG_PLAYER& player) = 0;

		virtual void QueryGeneralsDone(const Atlas::Vector<SG_GENERAL>& generals) = 0;
		virtual void QuerySoldiersDone(const Atlas::Vector<SG_SOLDIER>& soldiers) = 0;
		virtual void QueryBagDone(Atlas::Vector<SG_EQUIPT_ITEM>& equipts, Atlas::Vector<SG_USABLE_ITEM>& usables, Atlas::Vector<SG_GEM_ITEM>& gems) = 0;
		virtual void EnhanceCoolDownResult(_U32 time) = 0;
		virtual void RefreshEquipDone(SG_EQUIPT_ITEM& euipt) = 0;
		virtual void GemCombineResult(const SG_GEM_ITEM& gem) = 0;
		virtual void HaloGetCoolDownResult(_U32 time) = 0;
		virtual void HaloIncreaseEXPResult(_U32 time) = 0;

		virtual void BeginBattleDone(const SG_PLAYER_PVE& PlayerPVE) = 0;
		virtual void EndBattleDone(_U32 level, _U32 exp, _U32 gold, const SG_DROP_ITEM_BASE* drops, _U32 drop_count) = 0;

		virtual void QueryPlayerQuestResult(const SG_QUEST_LIVE_INFO* quest_list, _U32 count) = 0;
		virtual void FinishQuestDone(_U32 quest_id, _U32 exp, _U32 gold,  _U32 rmb, _U32 reputation, _U32 energy, const SG_DROP_ITEM_BASE* drops, _U32 drop_count) = 0;

		virtual void QueryPlayerPVPInfoResult(const SG_PLAYER_PVE& pve) = 0;
		virtual void QueryPlayerRankListResult(SG_PLAYER* players, _U32 count) = 0;

		virtual void PVPCoolDownResult(_U32 time) = 0;												
		virtual void PVPGetRestTimeResult(_U32 rest_time) = 0;										
		virtual void PVPRecordResult(const SG_PVP_RECORD_ITEM* record, _U32 count) = 0;				
		virtual void PVPHeroListRecord(const SG_PLAYER* players, _U32 count) = 0;					
		virtual void PVPDailyReward(_U32 gold, _U32 reputation, const SG_ITEM* items, _U32 count) = 0;
		virtual void PVPBattleResult(_U32 reputation) = 0;

		virtual void QueryInstanceResult(const SG_INSTANCE_INFO* instances, _U32 count) = 0;					
		virtual void BeginInstanceBattleResult(const SG_PLAYER_PVE& PlayerPVE) = 0;								
		virtual void EnterInstanceResult(const SG_INSTANCE_INFO& instance) = 0;			
		virtual void EndInstanceBattleResult(_U32 level, _U32 exp, _U32 gold, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count) = 0;
		virtual void ResetInstanceResult(_U8 result, _U32 gold, const SG_INSTANCE_INFO& instance) = 0;
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

		void ResetAccount();
		void GetServerList();
		void EnterServer(_U32 server_id);
		void QueryAvatar();
		void CreateAvatar(const char* nick, _U32 general_id);
		void DeleteAvatar();
		void EnterGame();
		void LeaveGame();
		void QueryPlayer(_U8 nSync = 0);									//0-nSync ��query��һ�β�ѯ������ʱֱ�ӵ���callback 1-��ͬ������������ͬ���������ѡ���Ƿ�callback֪ͨ
		void QueryGenerals(_U8 nSync = 0);
		void QuerySoldiers(_U8 nSync = 0);
		void QueryBag(_U8 nSync = 0);

		void EquipItem(_U32 general_id, const SG_EQUIP_SLOTS& slots);
		void EquipGenerals(const _U32* generals, _U32 count);
		void EquipSoldiers(const _U32* soldiers, _U32 count);
		void UnLockSoldier(_U32 soldier_id);
		void EnhanceSoldier(_U32 soldier_id);
		void EnhanceEquipt(A_UUID& uuid);

		void ExtendEquipt(A_UUID& uuid, A_UUID& puuid);
		void EnhanceCoolDown();
		void EnhanceCoolDownClear();
		void IncreaseEquipCoolDown();

		void RefreshEquipNormal(A_UUID& uuid);
		void RefreshEquipProperty(A_UUID& uuid);
		void RefreshEquipAbility(A_UUID& uuid);
		void RefreshEquipDecideAccept(A_UUID& uuid);

		void HaloCoolDown();												//������ȴ
		void HaloIncreaseEXP(_U8 halo_type);								//�����⻷
		void HaloGetCoolDown();												//��ȡ�⻷��ȴʱ��

		void BeginBattle(const char* name);
		void EndBattle(const char* name, _U32 result);
		void QueryServerTime();

		void EquipGem(const A_UUID& item_uuid, _U32 gem_id);
		void UnequipGem(const A_UUID& item_uuid, _U32 gem_id);
		void GemCombine(_U32 item_id);
		void SetGeneralStatus(_U32 general_id, _U8 status);
		
		void QueryPlayerQuest(_U8 nSync = 0);									//��ѯ���������Ϣ
		void SaveQuestData(const SG_QUEST_LIVE_INFO& quest_info);				//��������״̬
		void FinishQuest(_U32 quest_id);										//�ύ�������

		void GetPaid();															//daily salary
		void UpgradeTitle();													//��������

		void QueryPlayerPVPInfo(_U32 avatar_id);								//pvp ��Ϣ
		void QueryPlayerRankList();												//pvp���а�

		void BuyGoods(_U32 item_id);											//�̵깺��

		void PVPCoolDown();														//��ս��ȴʱ��
		void PVPGetRestTime();													//��սʣ�����
		void PVPRecord();														//��ս��¼		
		void PVPHeroList();														//Ӣ�۰�			
		void PVPDailyReward();													//pvpÿ�ս���֮��
		void PVPBattle(_U32 defender, _U8 result);								//pvpս�� 0-succ 1-failed

		void QueryInstance();													//����
		void EnterInstance(_U32 instance_id, _U8 difficulty);					//���븱�� 0-��ͨ 1-����
		void BeginInstanceBattle(_U32 instance_id, const char* map_url);				//��ʼ����ս��
		void EndInstanceBattle(const char* map_url, _U32 result);					//��������ս��
		void ResetInstance(_U32 instance_id);									//���ø���
		
		//result
		void Pong(CSGClient* pClient);

		void GetServerListResult(CSGClient* pClient, const SG_SERVER_INFO* infos, _U32 count, _U32 last_server);
		void QueryAvatarFailed(CSGClient* pClient, _U32 code);
		void QueryAvatarResult(CSGClient* pClient, const SG_PLAYER& player);
		void CreateAvatarResult(CSGClient* pClient, _U32 code);
		void QueryPlayerResult(CSGClient* pClient, const SG_PLAYER& player, _U8 nSync = 0);
		void QueryGeneralResult(CSGClient* pClient, const SG_GENERAL* generals, _U32 count, _U8 nSync = 0);
		void QuerySoldierResult(CSGClient* pClient, const SG_SOLDIER* soldiers, _U32 count, _U8 nSync = 0);
		void QueryBagBegin(CSGClient* pClient);
		void QueryBagEquipt(CSGClient* pClient, const SG_EQUIPT_ITEM* items, _U32 count);
		void QueryBagUsable(CSGClient* pClient, const SG_USABLE_ITEM* items, _U32 count);
		void QueryBagGen(CSGClient* pClient, const SG_GEM_ITEM* items, _U32 count);
		void QueryBagEnd(CSGClient* pClient, _U8 nSync = 0);
		void BeginBattleResult(CSGClient* pClient, const SG_PLAYER_PVE& PlayerPVE);
		void EndBattleResult(CSGClient* pClient, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count);
		void EnhanceCoolDownResult(CSGClient* pClient, _U32 time);
		void RefreshEquipDone(CSGClient* pClient, SG_EQUIPT_ITEM& euipt);
		void GemCombineResult(CSGClient* pClient, const SG_GEM_ITEM& gem);
		void HaloGetCoolDownResult(CSGClient* pClient, _U32 time);								//��ȡ�⻷��ȴʱ��
		void HaloIncreaseEXPResult(CSGClient* pClient, _U32 time);
		void QueryServerTimeResult(CSGClient* pClient, _U32 time);
		
		void QueryPlayerQuestResult(CSGClient* pClient,const SG_QUEST_LIVE_INFO* quest_list, _U32 count, _U8 nSync = 0);
		void FinishQuestDone(CSGClient* pClient, _U32 quest_id, _U32 exp_addition, _U32 exp, _U32 level, _U32 gold, _U32 rmb, _U32 reputation, _U32 energy, const SG_DROP_ITEM_BASE* drops, _U32 drop_count);

		void QueryPlayerPVPInfoResult(CSGClient* pClient, const SG_PLAYER_PVE& pve);
		void QueryPlayerRankListResult(CSGClient* pClient, SG_PLAYER* players, _U32 count);
		
		void BuyGoodsResult(CSGClient* pClient, const A_UUID* goods, _U32 count);						

		void PVPCoolDownResult(CSGClient* pClient, _U32 time);													//��ս��ȴʱ��
		void PVPGetRestTimeResult(CSGClient* pClient, _U32 rest_time);											//��սʣ�����
		void PVPRecordResult(CSGClient* pClient, const SG_PVP_RECORD_ITEM* record, _U32 count);					//��ս��¼			
		void PVPHeroListRecord(CSGClient* pClient, const SG_PLAYER* players, _U32 count);						//Ӣ�۰�				
		void PVPDailyReward(CSGClient* pClient, _U32 gold, _U32 reputation, const SG_ITEM* items, _U32 count);	//pvpÿ�ս��� ����
		void PVPBattleResult(CSGClient* pClient, _U32 reputation);												//pvpս���������

		void QueryInstanceResult(CSGClient* pClient, const SG_INSTANCE_INFO* instances, _U32 count);			//����
		void BeginInstanceBattleResult(CSGClient* pClient, const SG_PLAYER_PVE& PlayerPVE);								//��ʼ����ս��
		void EnterInstanceResult(CSGClient* pClient, const SG_INSTANCE_INFO& instance);			
		void EndInstanceBattleResult(CSGClient* pClient, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count);
		void ResetInstanceResult(CSGClient* pClient, _U8 result, _U32 gold, const SG_INSTANCE_INFO& instance);	//result 0-succ 1-failed

	public:
		virtual void OnLoginDone();
		virtual void OnLoginFailed();
		virtual void OnDisconnected();

		virtual void Tick();
	public:
		const _U32 GetLastServerID();
		const SG_PLAYER& GetPlayerInfo();
		const Atlas::Vector<SG_GENERAL>& GetGenerals();
		const Atlas::Vector<SG_SOLDIER>& GetSoldiers();
		const Atlas::Vector<SG_EQUIPT_ITEM>& GetEquiptItem();
		const Atlas::Vector<SG_GEM_ITEM>& GetGemItem();
		const Atlas::Vector<SG_USABLE_ITEM>& GetUsableItem();
		const Atlas::Vector<SG_INSTANCE_INFO>& GetInstanceInfo();
		const SG_SERVER_INFO& GetCurrentServerInfo();

		const int GetServerTime();
		SG_ITEM* GetItemByUUID(const A_UUID& uuid);
		void GetFinishedQuest(Atlas::Vector<SG_QUEST_LIVE_INFO>& quest_vec);

		void GetNewSoldierList(Atlas::Vector<_U32>& soldier_lists);
		bool HasNewSoldier();
		bool IsNewSoldier(_U32 soldier_id);
		void ClearNewSoldierList();

		void GetNewItemList(Atlas::Vector<A_UUID>& item_lists);
		bool HasNewItem();
		bool IsNewItem(const A_UUID& uuid);
		void ClearNewItemList();

		void SyncForInit();
		void SyncSet(const Atlas::Vector<_U8> vecSync);
		void SyncSuccNotify();

	private:
		CSGClientCallback* m_callback;
		SG_PLAYER m_player;
		Atlas::Vector<SG_GENERAL> m_generals;
		Atlas::Vector<SG_SOLDIER> m_soldiers;
		Atlas::Vector<SG_EQUIPT_ITEM> m_equipts;
		Atlas::Vector<SG_USABLE_ITEM> m_usables;
		Atlas::Vector<SG_GEM_ITEM> m_gems;
		Atlas::Vector<SG_QUEST_LIVE_INFO> m_quests;
		Atlas::Vector<SG_INSTANCE_INFO> m_instances;

		Atlas::Vector<_U32> m_newSoldiers;				//�»�õĿɽ�����soldier
		Atlas::Vector<A_UUID> m_newItemList;			//�»�õ���Ʒ

		int m_nServerTimeDelta;
		Atlas::CSGSyncDataManager* m_pSyncMgr;
		_U64 m_nConnectPingTime;

		Atlas::Vector<SG_SERVER_INFO> m_serverList;		//Ŀǰ��server
		_U32 m_lastServerID;							//���һ�ε�½��server��ID
	};

}

#endif
