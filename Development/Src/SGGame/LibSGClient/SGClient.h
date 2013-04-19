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
		virtual void NotifyAction(_U32 nActoinID, _U8 nActionType) = 0;

		virtual void GetServerListResult(const SG_SERVER_INFO* infos, _U32 count, _U32 last_server) = 0;
		virtual void QueryAvatarFailed(_U32 code) = 0;
		virtual void QueryAvatarDone(const SG_PLAYER& player) = 0;
		virtual void EnterGameResult(_U8 ret) = 0;
		virtual void CreateAvatarResult(_U32 code) = 0;
		virtual void QueryPlayerResult(const SG_PLAYER& player) = 0;
		virtual void QueryOtherPlayersResult(const SG_PLAYER* players, _U32 count) = 0;

		virtual void QueryGeneralsDone(const Atlas::Vector<SG_GENERAL>& generals) = 0;
		virtual void QuerySoldiersDone(const Atlas::Vector<SG_SOLDIER>& soldiers) = 0;
		virtual void QueryBagDone(Atlas::Vector<SG_EQUIPT_ITEM>& equipts, Atlas::Vector<SG_USABLE_ITEM>& usables, Atlas::Vector<SG_GEM_ITEM>& gems, Atlas::Vector<SG_MATERIAL_ITEM>& material) = 0;
		virtual void EnhanceCoolDownResult(_U32 time) = 0;
		virtual void RefreshEquipDone(SG_EQUIPT_ITEM& euipt) = 0;
		virtual void GemCombineResult(const SG_GEM_ITEM& gem) = 0;
		virtual void HaloGetCoolDownResult(_U32 time) = 0;
		virtual void HaloIncreaseEXPResult(_U32 time) = 0;

		virtual void BeginBattleDone(const SG_PLAYER_PVE& PlayerPVE) = 0;
		virtual void EndBattleDone(_U32 level, _U32 exp, _U32 gold, _U32 wake_pt, const SG_DROP_ITEM_BASE* drops, _U32 drop_count) = 0;

		virtual void QueryPlayerQuestResult(const SG_QUEST_LIVE_INFO* quest_list, _U32 count) = 0;
		virtual void FinishQuestDone(_U32 quest_id, _U32 exp, _U32 gold,  _U32 rmb, _U32 reputation, _U32 energy, const SG_DROP_ITEM_BASE* drops, _U32 drop_count) = 0;

		virtual void QueryPlayerPVPInfoResult(const SG_PLAYER_PVE& pve) = 0;
		virtual void QueryPlayerRankListResult(SG_PLAYER* players, _U32 count) = 0;

		virtual void PVPCoolDownResult(_U32 time) = 0;												
		virtual void PVPGetRestTimeResult(_U32 rest_time) = 0;										
		virtual void PVPRecordResult(const SG_PVP_RECORD_ITEM* record, _U32 count) = 0;				
		virtual void PVPHeroListRecord(const SG_PLAYER* players, _U32 count) = 0;					
		virtual void PVPDailyReward(_U32 gold, _U32 reputation, const SG_ITEM* items, _U32 count) = 0;
		virtual void PVPBattleBeginResult(const SG_PLAYER_PVE& SelfPVE, const SG_PLAYER_PVE& DefenderPVE, const SG_PLAYER& DefenderPlayerInfo) = 0;
		virtual void PVPBattleEndResult(_U32 reputation) = 0;	
		virtual void PVPCoolDownClearResult() = 0;						
		virtual void PVPIncreateBattleTimesResult(_U32 rest_times) = 0;

		virtual void QueryInstanceResult(const SG_INSTANCE_INFO* instances, _U32 count) = 0;					
		virtual void BeginInstanceBattleResult(const SG_PLAYER_PVE& PlayerPVE) = 0;								
		virtual void EnterInstanceResult(const SG_INSTANCE_INFO& instance) = 0;			
		virtual void EndInstanceBattleResult(_U32 level, _U32 exp, _U32 gold, _U32 wake_pt, _U8 result, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count) = 0;
		virtual void ResetInstanceResult(_U8 result, _U32 rmb, const SG_INSTANCE_INFO& instance) = 0;

		virtual void CreateLeagueResult(_U8 ret, const SG_LEAGUE& league) = 0;							//0-succ other-failed
		virtual void QueryLeagueApplyListResult(const SG_LEAGUE_APPLYER* applyers, _U32 count) = 0;
		virtual void QueryLeagueResult(const SG_LEAGUE& league) = 0;							
		virtual void QueryLeagueListResult(const SG_LEAGUE* league_list, _U32 count) = 0;
		virtual void QueryLeagueMemberListResult(const SG_LEAGUE_MEMBER* league_members, _U32 count) = 0;
		virtual void QueryLeagueMemberInfoResult(const SG_LEAGUE_MEMBER& member_info) = 0;

		virtual void ContributeLeagueResult(const SG_LEAGUE_MEMBER& self_info, const SG_LEAGUE& league_info) = 0;
		virtual void HandleApplyResult(_U8 ret, const SG_LEAGUE_MEMBER& new_joiner) = 0;								
		virtual void QueryLeagueNoticeResult(const char* notice_content) = 0;
		virtual void SetLeagueNoticeResult(_U8 ret, const char* notice_content) = 0;								
		virtual void SetLeagueOwnerResult(_U8 ret, _U32 member_id) = 0;																
		virtual void SetMemberPositionResult(_U8 ret, _U32 member_id, _U8 position) = 0;							
		virtual void DismissMemberResult(_U8 ret, _U32 member_id) = 0;																
		virtual void ExitLeagueResult(_U8 ret) = 0;										
		virtual void QueryLeagueLogResult(const SG_LEAGUE_LOG* league_log, _U32 count) = 0;
		virtual void LeagueToastResult(_U8 ret, _U32 gold, _U32 rmb, _U32 reward_reputation, _U32 reward_league_xp) = 0;

		virtual void SalaryGetResult(_U8 ret, _U32 rmb, _U32 gold) = 0;				
		virtual void SalaryGetBatResult(_U8 ret, _U32 rmb, _U32 gold, _U32 times) = 0;
		virtual void BuyEnergyResult(_U8 ret, _U32 rmb, _U32 energy, _U32 times) = 0;

		virtual void EnhanceTurboResult(_U8 ret, _U32 turbo_level,  _U32 wake_pt) = 0;
		virtual void MakeEquiptResult(_U8 ret, const SG_EQUIPT_ITEM& new_euqipt, const SG_MATERIAL_ITEM& com_material, const SG_MATERIAL_ITEM& key_material) = 0;
		virtual void QueryActionStatusResult(_U32 *action_list, Atlas::Vector<_U8> statusVec, _U32 *available_list, _U32 count) = 0;

		virtual void SellItemResult(_U8 ret, const A_UUID& uuid, _U32 item_id, _U32 count) = 0;

		virtual void QueryFriendListResult(const SG_FRIEND_INFO* friend_list, _U32 count) = 0;
		virtual void QueryFriendListSearchResult(const SG_FRIEND_SEARCH_INFO* search_list, _U32 count) = 0;
		virtual void QueryFriendInvitationListResult(const SG_FRIEND_INVITATION* invitation_list, _U32 count) = 0;
		virtual void AcceptFriendResult(_U8 ret, _U32 avatar_id) = 0;

		virtual void SendChatResult(const _U8 ret) = 0;
		virtual void ReceiveChatResult(const SG_CHAT_RECV_INFO* chat_list, _U32 count) = 0;

		virtual void QueryLeagueDianjiangListResult(_U8 ret, const SG_LEAGUE_DIANJIANG_JOINER* joiner_list, _U32 count) = 0;	
		virtual void JoinLeagueDianjiangResult(_U8 ret, const SG_LEAGUE_DIANJIANG_JOINER& self)= 0;								
		virtual void QueryLeagueDianjiangRewardResult(_U8 ret, _U32 reputation)= 0;											
		virtual void LeagueDianjiangSummonNPCResult(_U8 ret, _U32 gold, _U32 rmb, const SG_LEAGUE_DIANJIANG_JOINER& npc_joiner)= 0;

		virtual void QueryAstrologyBagResult(const _U32* bag_list, _U32 count1, const _U32* tmp_bag_list, _U32 count2) = 0;
		virtual void SaveToBagResult(_U8 ret, _U32 item_id) = 0;
		virtual void SetAstrologyBallStatusResult(_U8 ret, const SG_GENERAL& general) = 0;
		virtual void EnhanceAstrologyBallResult(_U8 ret, _U32 gold, _U32 ball_id, _U32 new_ball_id) = 0;
		//virtual void BuyAstrologyBallResult(_U8 ret, _U32 astrology_value, _U32 ball_id) = 0;
		virtual void StrologyResult(_U8 ret, _U32 gold, _U32 ball_id, _U32 astrologer_id) = 0;
		virtual void StrologyAutoResult(_U8 ret, _U32 gold, _U32* ball_list, _U32 count, _U32 atrologer_id) = 0;
		virtual void DevourResult(_U8 ret, _U8 bag_type, _U32* ball_list, _U32 count) = 0;
		virtual void UseItemResult(_U8 ret, const A_UUID& uuid, _U32 count, _U32 target_id, const SG_PLAYER& player_info, const SG_GENERAL& general, const SG_ITEM* drops, const _U32 drop_count) = 0;

		virtual void FeedHorseResult(_U8 ret, _U32 xp, _U32 level, _U8 xp_add_type) = 0;

		virtual void BuyGoodsResult(_U8 ret, _U32* id_list, _U32 count) = 0;
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
		void QueryPlayer(_U8 nSync = 0);									//0-nSync 该query是一次查询，返回时直接调用callback 1-是同步操作视所有同步操作情况选择是否callback通知
		void QueryGenerals(_U8 nSync = 0);
		void QuerySoldiers(_U8 nSync = 0);
		void QueryBag(_U8 nSync = 0);
		void QueryOtherPlayers(const char* szCurrentTownMap, _U32 nMaxNum);

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

		void HaloCoolDown();													//立刻冷却
		void HaloIncreaseEXP(_U8 halo_type);									//升级光环
		void HaloGetCoolDown();													//获取光环冷却时间

		void BeginBattle(const char* name);
		void EndBattle(const char* name, _U32 result);
		void QueryServerTime();

		void EquipGem(const A_UUID& item_uuid, _U32 gem_id);
		void UnequipGem(const A_UUID& item_uuid, _U32 gem_id);
		void GemCombine(_U32 item_id);
		void SetGeneralStatus(_U32 general_id, _U8 status);
		
		void QueryPlayerQuest(_U8 nSync = 0);									//查询玩家任务信息
		void SaveQuestData(const SG_QUEST_LIVE_INFO& quest_info);				//更改任务状态
		void FinishQuest(_U32 quest_id);										//提交完成任务

		void GetPaid();															//daily salary
		void UpgradeTitle();													//提升军衔

		void QueryPlayerPVPInfo(_U32 avatar_id);								//pvp 信息
		void QueryPlayerRankList();												//pvp排行榜

		void BuyGoods(_U32 item_id);											//商店购买
		void SellItem(const A_UUID& uuid, const _U32 item_id, const _U32 count);//卖出物品

		void PVPCoolDown();														//挑战冷却时间
		void PVPGetRestTime();													//挑战剩余次数
		void PVPRecord();														//对战记录		
		void PVPHeroList();														//英雄榜			
		void PVPDailyReward();													//pvp每日奖励之类
		void PVPBattleBegin(_U32 defender);										//pvp战斗开始 
		void PVPBattleEnd(_U32 defender, _U8 ret);								//pvp战斗结束 0-succ 1-failed
		void PVPCoolDownClear();												//pvp清楚挑战冷却时间
		void PVPIncreateBattleTimes();											//pvp增加挑战次数

		void QueryInstance();													//副本
		void EnterInstance(_U32 instance_id, _U8 difficulty);					//进入副本 0-普通 1-困难
		void BeginInstanceBattle(_U32 instance_id, const char* map_url);		//开始副本战斗
		void EndInstanceBattle(_U32 instance_id, const char* map_url, _U32 result, _U8 auto_combat = 0);//结束副本战斗 auto_combat 0-no 1-yese
		void ResetInstance(_U32 instance_id);									//重置副本
		void SaveLastTownMap(const char* last_town_map);						//保存最后一次的地图信息

		void CreateLeague(const char* league_name);								//战盟 创建
		void ApplyJoinLeague(_U32 league_id);									//申请加入战盟		
		void QueryLeagueApplyList(_U32 league_id);								//查询当前申请加入战盟的人
		void QueryLeague(_U32 league_id);										//查询league_id战盟信息
		void QueryLeagueList();													//查询当前所有战盟的列表
		void QueryLeagueMemberList(_U32 league_id);								//查询league_id战盟当前成员
		void QueryLeagueMemberInfo(_U32 member_id);								//显示成员选中tips
		
		void ContributeLeague(_U32 rmb, _U32 energy);							//战盟捐献
		void HandleApply(_U32 applyer_id, _U8 allowed);							//批准加入 1-同意 0-拒绝
		void QueryLeagueNotice();												//查询战盟公告
		void SetLeagueNotice(_U32 league_id, const char* notice_content);		//设置战盟公告
		void SetLeagueOwner(_U32 member_id);									//转交战盟
		void DissolveLeague();													//解散
		void SetMemberPosition(_U32 member_id, _U8 position);					//升职 0-普通成员 1-副团长 2-团长
		void DismissMember(_U32 member_id);										//开除
		void ExitLeague();														//退出战盟
		void QueryLeagueLog();													//战盟日志
		void LeagueToast(_U8 wine_id);											//战盟置酒

		void SalaryGet();														//获取每日军饷
		void SalaryGetBat();													//批量获取 max = 10
		void BuyEnergy();
		
		void EnhanceTurbo();
		void EquipTurboSkill(const SG_TURBO_SKILL_SLOT& skill_slot);			//装备无双技能
		void MakeEquipt(_U32 equipt_id);										//装备打造
		void QueryActionAvailable(_U32 *action_list, _U32 count);				//判断活动是否可以进入/激活

		void QueryFriendList();					
		void QueryFriendSearchList(const char* nick);	
		void QueryFriendInvitationList();		
		void InviteFriend(_U32 avatar_id);		
		void AcceptFriend(_U32 avatar_id);		
		void RefuseInvitation(_U32 avatar_id);	
		void DelFriend(_U32 avatar_id);			

		void SendChat(const SG_CHAT_SEND_INFO& send_info);						//发送聊天
		void ReceiveChat(const _U32* channel_list, const _U32 count);			//接受频道消息

		void QueryLeagueDianjiangList(_U32 league_id);							//获取当前点将的列表
		void JoinLeagueDianjiang(_U32 league_id);								//参加点将
		void QueryLeagueDianjiangReward(_U32 league_id, _U32 avatar_id);		//查询点将奖励
		void LeagueDianjiangSummonNPC(_U32 league_id);							//招募npc参加点将活动

		void QueryAstrologyBag();												//查询命魂
		void SaveToBag(_U32 item_id);											//存入命魂包中
		void SetAstrologyBallStatus(_U32 general_id, _U32 ball_id, _U8 status);	//0-卸下 1-装备
		void EnhanceAstrologyBall(_U32 ball_id, _U32 general_id);				//升级命魂
		//void BuyAstrologyBall(_U32 ball_id);									//购买命魂
		void Strology(_U32 astrologer_id);										//占星
		void StrologyAuto(_U32 RestSlotCount);									//一键占星
												
		void UseItem(const A_UUID& uuid, _U32 item_id, _U32 count, _U32 target_id);//使用可使用的物品
		void Devour(_U8 bag_type);

		void FeedHorse(const _U8 feed_type);

		//result
		void Pong(CSGClient* pClient);

		void GetServerListResult(CSGClient* pClient, const SG_SERVER_INFO* infos, _U32 count, _U32 last_server);
		void QueryAvatarFailed(CSGClient* pClient, _U32 code);
		void QueryAvatarResult(CSGClient* pClient, const SG_PLAYER& player);
		void EnterGameResult(CSGClient* pClient, _U8 ret);
		void CreateAvatarResult(CSGClient* pClient, _U32 code);
		void QueryPlayerResult(CSGClient* pClient, const SG_PLAYER& player, _U8 nSync = 0);
		void QueryGeneralResult(CSGClient* pClient, const SG_GENERAL* generals, _U32 count, _U8 nSync = 0);
		void QuerySoldierResult(CSGClient* pClient, const SG_SOLDIER* soldiers, _U32 count, _U8 nSync = 0);
		void QueryBagBegin(CSGClient* pClient);
		void QueryBagEquipt(CSGClient* pClient, const SG_EQUIPT_ITEM* items, _U32 count);
		void QueryBagUsable(CSGClient* pClient, const SG_USABLE_ITEM* items, _U32 count);
		void QueryBagGen(CSGClient* pClient, const SG_GEM_ITEM* items, _U32 count);
		void QueryBagMaterial(CSGClient* pClient, const SG_MATERIAL_ITEM* items, _U32 count);
		void QueryBagEnd(CSGClient* pClient, _U8 nSync = 0);
		void QueryOtherPlayersResult(CSGClient* pClient, const SG_PLAYER* players, _U32 count);

		void BeginBattleResult(CSGClient* pClient, const SG_PLAYER_PVE& PlayerPVE);
		void EndBattleResult(CSGClient* pClient, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 wake_pt, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count);
		void EnhanceCoolDownResult(CSGClient* pClient, _U32 time);
		void RefreshEquipDone(CSGClient* pClient, SG_EQUIPT_ITEM& euipt);
		void GemCombineResult(CSGClient* pClient, const SG_GEM_ITEM& gem);
		void HaloGetCoolDownResult(CSGClient* pClient, _U32 time);								//获取光环冷却时间
		void HaloIncreaseEXPResult(CSGClient* pClient, _U32 time);
		void QueryServerTimeResult(CSGClient* pClient, _U32 time);
		
		void QueryPlayerQuestResult(CSGClient* pClient,const SG_QUEST_LIVE_INFO* quest_list, _U32 count, _U8 nSync = 0);
		void FinishQuestDone(CSGClient* pClient, _U32 quest_id, _U32 exp_addition, _U32 exp, _U32 level, _U32 gold, _U32 rmb, _U32 reputation, _U32 energy, const SG_DROP_ITEM_BASE* drops, _U32 drop_count);

		void QueryPlayerPVPInfoResult(CSGClient* pClient, const SG_PLAYER_PVE& pve);
		void QueryPlayerRankListResult(CSGClient* pClient, SG_PLAYER* players, _U32 count);
		
		void BuyGoodsResult(CSGClient* pClient, _U8 ret, const A_UUID* goods, _U32* id_list, _U32 count);						
		void SellItemResult(CSGClient* pClient, _U8 result, const A_UUID& uuid, const _U32 item_id, const _U32 count);				//ret 0-succ 1-failed 

		void PVPCoolDownResult(CSGClient* pClient, _U32 time);													//挑战冷却时间
		void PVPGetRestTimeResult(CSGClient* pClient, _U32 rest_time);											//挑战剩余次数
		void PVPRecordResult(CSGClient* pClient, const SG_PVP_RECORD_ITEM* record, _U32 count);					//对战记录			
		void PVPHeroListRecord(CSGClient* pClient, const SG_PLAYER* players, _U32 count);						//英雄榜				
		void PVPDailyReward(CSGClient* pClient, _U32 gold, _U32 reputation, const SG_ITEM* items, _U32 count);	//pvp每日奖励 增量
		void PVPBattleBeginResult(CSGClient* pClient, const SG_PLAYER_PVE& SelfPVE, const SG_PLAYER_PVE& DefenderPVE, const SG_PLAYER& DefenderPlayerInfo);	//pvp战斗
		void PVPBattleEndResult(CSGClient* pClient, _U32 reputation);											//pvp战斗结束 
		void PVPCoolDownClearResult(CSGClient* pClient);														//pvp清楚挑战冷却时间
		void PVPIncreateBattleTimesResult(CSGClient* pClient, _U32 rest_times);									//pvp增加挑战次数

		void QueryInstanceResult(CSGClient* pClient, const SG_INSTANCE_INFO* instances, _U32 count);			//副本
		void BeginInstanceBattleResult(CSGClient* pClient, const SG_PLAYER_PVE& PlayerPVE);						//开始副本战斗
		void EnterInstanceResult(CSGClient* pClient, const SG_INSTANCE_INFO& instance);			
		void EndInstanceBattleResult(CSGClient* pClient, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 wake_pt, _U8 result, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count);
		void ResetInstanceResult(CSGClient* pClient, _U8 result, _U32 rmb, const SG_INSTANCE_INFO& instance);	//result 0-succ 1-failed

		void CreateLeagueResult(CSGClient* pClient, _U8 ret, const SG_LEAGUE& league);							//0-succ 1-failed
		void QueryLeagueApplyListResult(CSGClient* pClient, const SG_LEAGUE_APPLYER* applyers, _U32 count);
		void QueryLeagueResult(CSGClient* pClient, const SG_LEAGUE& league);							
		void QueryLeagueListResult(CSGClient* pClient, const SG_LEAGUE* league_list, _U32 count);
		void QueryLeagueMemberListResult(CSGClient* pClient, const SG_LEAGUE_MEMBER* league_members, _U32 count);	
		void QueryLeagueMemberInfoResult(CSGClient* pClient, const SG_LEAGUE_MEMBER& member_info);
		
		void ContributeLeagueResult(CSGClient* pClient, const SG_LEAGUE_MEMBER& self_info, const SG_LEAGUE& league_info);
		void HandleApplyResult(CSGClient* pClient, _U8 ret, const SG_LEAGUE_MEMBER& new_joiner);								//ret 0-succ 1-failed
		void QueryLeagueNoticeResult(CSGClient* pClient, const char* notice_content);
		void SetLeagueNoticeResult(CSGClient* pClient, _U8 ret, const char* notice_content);								//ret 0-succ 1-failed 失败带回原先的公告
		void SetLeagueOwnerResult(CSGClient* pClient, _U8 ret, _U32 member_id);												//ret 0-succ 1-failed
		void SetMemberPositionResult(CSGClient* pClient, _U8 ret, _U32 member_id, _U8 position);							//ret 0-succ 1-failed 如果成功则带回新职位失败不关心
		void DismissMemberResult(CSGClient* pClient, _U8 ret, _U32 member_id);												//ret 0-succ 1-failed	
		void ExitLeagueResult(CSGClient* pClient, _U8 ret);										
		void QueryLeagueLogResult(CSGClient* pClient, const SG_LEAGUE_LOG* league_log, _U32 count);
		void LeagueToastResult(CSGClient* pClient, _U8 ret, _U32 gold, _U32 rmb, _U32 reward_reputation, _U32 reward_league_xp);				//ret 0-succ 1-failed

		void SalaryGetResult(CSGClient* pClient, _U8 ret, _U32 rmb, _U32 gold);									//0-succ 1-failed rmb-消耗的rmb gold-获得的gold
		void SalaryGetBatResult(CSGClient* pClient, _U8 ret, _U32 rmb, _U32 gold, _U32 times);					//0-succ 1-failed rmb-消耗的rmb gold-获得的gold times-成功领取的次数
		void BuyEnergyResult(CSGClient* pClient, _U8 ret, _U32 rmb, _U32 energy, _U32 times);

		void EnhanceTurboResult(CSGClient* pClient, _U8 ret, _U32 turbo_level,  _U32 wake_pt);										//返回新的无双等级和消耗的觉醒点 
		void MakeEquiptResult(CSGClient* pClient, _U8 ret, const SG_EQUIPT_ITEM& new_euqipt, const SG_MATERIAL_ITEM& com_material, const SG_MATERIAL_ITEM& key_material);			//装备打造 com_material,key_material返回使用掉的材料
		void QueryActionAvailableResult(CSGClient* pClient, _U32* action_list, _U32* available_list, _U32 count);		//判断活动是否可以进入/激活 available_list[i] 0-available 1-none available

		void QueryFriendListResult(CSGClient* pClient, const SG_FRIEND_INFO* friend_list, _U32 count);
		void QueryFriendListSearchResult(CSGClient* pClient, const SG_FRIEND_SEARCH_INFO* search_list, _U32 count);
		void QueryFriendInvitationListResult(CSGClient* pClient, const SG_FRIEND_INVITATION* invitation_list, _U32 count);
		void AcceptFriendResult(CSGClient* pClient, _U8 ret, _U32 avatar_id);

		void SendChatResult(CSGClient* pClient, const _U8 ret);
		void ReceiveChatResult(CSGClient* pClient, const SG_CHAT_RECV_INFO* chat_list, _U32 count);

		void QueryLeagueDianjiangListResult(CSGClient* pClient, _U8 ret, const SG_LEAGUE_DIANJIANG_JOINER* joiner_list, _U32 count);	
		void JoinLeagueDianjiangResult(CSGClient* pClient, _U8 ret, const SG_LEAGUE_DIANJIANG_JOINER& self);								
		void QueryLeagueDianjiangRewardResult(CSGClient* pClient, _U8 ret, _U32 reputation);											
		void LeagueDianjiangSummonNPCResult(CSGClient* pClient, _U8 ret, _U32 gold, _U32 rmb, const SG_LEAGUE_DIANJIANG_JOINER& npc_joiner);

		void QueryAstrologyBagResult(CSGClient* pClient, const _U32* bag_list, _U32 count1, const _U32* tmp_bag_list, _U32 count2);
		void SaveToBagResult(CSGClient* pClient, _U8 ret, _U32 item_id);
		void SetAstrologyBallStatusResult(CSGClient* pClient, _U8 ret, const SG_GENERAL& general);
		void EnhanceAstrologyBallResult(CSGClient* pClient, _U8 ret, _U32 gold, _U32 ball_id, _U32 new_ball_id);
		//void BuyAstrologyBallResult(CSGClient* pClient, _U8 ret, _U32 astrology_value, _U32 ball_id);
		void StrologyResult(CSGClient* pClient, _U8 ret, _U32 gold, _U32 ball_id, _U32 astrologer_id);
		void StrologyAutoResult(CSGClient* pClient, _U8 ret, _U32 gold, _U32* ball_list, _U32 count, _U32 atrologer_id);
		void DevourResult(CSGClient* pClient, _U8 ret, _U8 bag_type, _U32* ball_list, _U32 count);

		void UseItemResult(CSGClient* pClient, _U8 ret, const A_UUID& uuid, _U32 count, _U32 target_id, const SG_PLAYER& player_info, const SG_GENERAL& general, const SG_ITEM* drops, const _U32 drop_count);
		void FeedHorseResult(CSGClient* pClient, _U8 ret, _U32 xp, _U32 level, _U8 xp_add_type);
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
		const Atlas::Vector<SG_MATERIAL_ITEM>& GetMaterialItem();
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

		void GetNewApplyerList(Atlas::Vector<_U32>& applyer_list);
		bool HasNewApplyer();
		bool IsNewApplyer(_U32 applyer_id);
		void ClearNewApplyerList();

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
		Atlas::Vector<SG_MATERIAL_ITEM> m_materials;
		Atlas::Vector<SG_QUEST_LIVE_INFO> m_quests;
		Atlas::Vector<SG_INSTANCE_INFO> m_instances;

		int m_nServerTimeDelta;
		_U64 m_nConnectPingTime;

		Atlas::Vector<SG_SERVER_INFO> m_serverList;		//目前的server
		_U32 m_lastServerID;							//最后一次登陆的server的ID

		static int ms_nLastRanderTime;					//上次一tick的时间
	};

}

#endif
