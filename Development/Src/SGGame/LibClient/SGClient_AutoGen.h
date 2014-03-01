sigslot::signal0<> _OnPong;
sigslot::signal1<_U8> _OnBindAccountResult;
sigslot::signal1<_U32> _OnQueryServerTimeResult;
sigslot::signal3<const SG_SERVER_INFO*, _U32, _U32> _OnGetServerListResult;
sigslot::signal1<_U32> _OnQueryAvatarFailed;
sigslot::signal1<const SG_PLAYER&> _OnQueryAvatarResult;
sigslot::signal1<_U8> _OnEnterGameResult;
sigslot::signal1<_U32> _OnCreateAvatarResult;
sigslot::signal2<const SG_PLAYER&, _U8> _OnQueryPlayerResult;
sigslot::signal3<const SG_GENERAL*, _U32, _U8> _OnQueryGeneralResult;
sigslot::signal3<const SG_SOLDIER*, _U32, _U8> _OnQuerySoldierResult;
sigslot::signal0<> _OnQueryBagBegin;
sigslot::signal2<const SG_EQUIPT_ITEM*, _U32> _OnQueryBagEquipt;
sigslot::signal2<const SG_USABLE_ITEM*, _U32> _OnQueryBagUsable;
sigslot::signal2<const SG_GEM_ITEM*, _U32> _OnQueryBagGen;
sigslot::signal2<const SG_MATERIAL_ITEM*, _U32> _OnQueryBagMaterial;
sigslot::signal1<_U8> _OnQueryBagEnd;
sigslot::signal7<const SG_PLAYER&, const SG_GENERAL*, _U32, const SG_SOLDIER*, _U32, const SG_EQUIPT_ITEM*, _U32> _OnQueryOtherPlayerResult;
sigslot::signal2<const SG_PLAYER_INFO*, _U32> _OnQueryOtherPlayersResult;
sigslot::signal3<_U8, _U32, const SG_EQUIPT_ITEM&> _OnEnhanceEquiptResult;
sigslot::signal3<_U8, const SG_EQUIPT_ITEM&, const A_UUID&> _OnExtendEquiptResult;
sigslot::signal1<_U32> _OnEnhanceCoolDownResult;
sigslot::signal1<const SG_EQUIPT_ITEM&> _OnRefreshEquipDone;
sigslot::signal1<const SG_EQUIPT_ITEM&> _OnRefreshEquipDecideAcceptResult;
sigslot::signal1<const SG_GEM_ITEM&> _OnGemCombineResult;
sigslot::signal2<_U8, _U32> _OnUpgradeTitleResult;
sigslot::signal3<_U32, _U32, _U32> _OnHaloIncreaseEXPResult;
sigslot::signal2<_U8, _U32> _OnHaloCoolDownResult;
sigslot::signal1<_U32> _OnHaloGetCoolDownResult;
sigslot::signal1<const SG_PLAYER_PVE&> _OnBeginBattleResult;
sigslot::signal7<_U32, _U32, _U32, _U32, _U32, const SG_DROP_ITEM_CONFIG*, _U32> _OnEndBattleResult;
sigslot::signal3<const SG_QUEST_LIVE_INFO*, _U32, _U8> _OnQueryPlayerQuestResult;
sigslot::signal12<_U8, _U32, _U32, _U32, _U32, _U32, _U32, _U32, _U32, const SG_DROP_ITEM_BASE*, _U32, _U32> _OnFinishQuestDone;
sigslot::signal4<_U8, const A_UUID*, const _U32*, _U32> _OnBuyGoodsResult;
sigslot::signal4<_U8, const A_UUID&, _U32, _U32> _OnSellItemResult;
sigslot::signal1<const SG_PLAYER_PVE&> _OnQueryPlayerPVPInfoResult;
sigslot::signal2<const SG_PVP_RANK_PLAYER*, _U32> _OnQueryPlayerRankListResult;
sigslot::signal1<_U32> _OnPVPCoolDownResult;
sigslot::signal1<_U32> _OnPVPGetRestTimeResult;
sigslot::signal2<const SG_PVP_RECORD_ITEM*, _U32> _OnPVPRecordResult;
sigslot::signal2<const SG_PVP_HEROLIST_PLAYER*, _U32> _OnPVPHeroListRecord;
sigslot::signal4<_U32, _U32, const SG_ITEM*, _U32> _OnPVPDailyReward;
sigslot::signal2<const SG_PLAYER_PVE&, const SG_PLAYER_PVE&> _OnPVPBattleBeginResult;
sigslot::signal1<_U32> _OnPVPBattleEndResult;
sigslot::signal2<_U8, _U32> _OnPVPCoolDownClearResult;
sigslot::signal1<_U32> _OnPVPIncreateBattleTimesResult;
sigslot::signal2<const SG_INSTANCE_INFO*, _U32> _OnQueryInstanceResult;
sigslot::signal1<const SG_PLAYER_PVE&> _OnBeginInstanceBattleResult;
sigslot::signal1<const SG_INSTANCE_INFO&> _OnEnterInstanceResult;
sigslot::signal8<_U32, _U32, _U32, _U32, _U32, _U8, const SG_DROP_ITEM_CONFIG*, _U32> _OnEndInstanceBattleResult;
sigslot::signal3<_U8, _U32, const SG_INSTANCE_INFO&> _OnResetInstanceResult;
sigslot::signal2<_U8, const SG_LEAGUE&> _OnCreateLeagueResult;
sigslot::signal2<const SG_LEAGUE_APPLYER*, _U32> _OnQueryLeagueApplyListResult;
sigslot::signal1<const SG_LEAGUE&> _OnQueryLeagueResult;
sigslot::signal2<const SG_LEAGUE*, _U32> _OnQueryLeagueListResult;
sigslot::signal2<const SG_LEAGUE_MEMBER*, _U32> _OnQueryLeagueMemberListResult;
sigslot::signal1<const SG_LEAGUE_MEMBER&> _OnQueryLeagueMemberInfoResult;
sigslot::signal2<const SG_LEAGUE_MEMBER&, const SG_LEAGUE&> _OnContributeLeagueResult;
sigslot::signal2<_U8, const SG_LEAGUE_MEMBER&> _OnHandleApplyResult;
sigslot::signal1<const char*> _OnQueryLeagueNoticeResult;
sigslot::signal2<_U8, const char*> _OnSetLeagueNoticeResult;
sigslot::signal2<_U8, _U32> _OnSetLeagueOwnerResult;
sigslot::signal3<_U8, _U32, _U8> _OnSetMemberPositionResult;
sigslot::signal2<_U8, _U32> _OnDismissMemberResult;
sigslot::signal1<_U8> _OnExitLeagueResult;
sigslot::signal2<const SG_LEAGUE_LOG*, _U32> _OnQueryLeagueLogResult;
sigslot::signal5<_U8, _U32, _U32, _U32, _U32> _OnLeagueToastResult;
sigslot::signal3<_U8, _U32, _U32> _OnSalaryGetResult;
sigslot::signal4<_U8, _U32, _U32, _U32> _OnSalaryGetBatResult;
sigslot::signal4<_U8, _U32, _U32, _U32> _OnBuyEnergyResult;
sigslot::signal3<_U8, _U32, _U32> _OnEnhanceTurboResult;
sigslot::signal4<_U8, const SG_EQUIPT_ITEM&, const SG_MATERIAL_ITEM&, const SG_MATERIAL_ITEM&> _OnMakeEquiptResult;
sigslot::signal3<const _U32*, const _U32*, _U32> _OnQueryActionAvailableResult;
sigslot::signal2<const SG_FRIEND_INFO*, _U32> _OnQueryFriendListResult;
sigslot::signal2<const SG_FRIEND_SEARCH_INFO*, _U32> _OnQueryFriendListSearchResult;
sigslot::signal2<const SG_FRIEND_INVITATION*, _U32> _OnQueryFriendInvitationListResult;
sigslot::signal2<_U8, _U32> _OnAcceptFriendResult;
sigslot::signal1<_U8> _OnSendChatResult;
sigslot::signal2<const SG_CHAT_RECV_INFO*, _U32> _OnReceiveChatResult;
sigslot::signal3<_U8, const SG_LEAGUE_DIANJIANG_JOINER*, _U32> _OnQueryLeagueDianjiangListResult;
sigslot::signal4<_U8, const SG_LEAGUE_DIANJIANG_JOINER&, const SG_LEAGUE_DIANJIANG_JOINER*, _U32> _OnJoinLeagueDianjiangResult;
sigslot::signal2<_U8, _U32> _OnQueryLeagueDianjiangRewardResult;
sigslot::signal6<_U8, _U32, _U32, const SG_LEAGUE_DIANJIANG_JOINER&, const SG_LEAGUE_DIANJIANG_JOINER*, _U32> _OnLeagueDianjiangSummonNPCResult;
sigslot::signal4<const _U32*, _U32, const _U32*, _U32> _OnQueryAstrologyBagResult;
sigslot::signal2<_U8, _U32> _OnSaveToBagResult;
sigslot::signal2<_U8, const SG_GENERAL&> _OnSetAstrologyBallStatusResult;
sigslot::signal4<_U8, _U32, _U32, _U32> _OnEnhanceAstrologyBallResult;
sigslot::signal5<_U8, _U32, _U32, _U32, _U32> _OnStrologyResult;
sigslot::signal5<_U8, _U32, const _U32*, _U32, _U32> _OnStrologyAutoResult;
sigslot::signal6<_U8, _U8, const _U32*, _U32, const _U32*, _U32> _OnDevourResult;
sigslot::signal8<_U8, const A_UUID&, _U32, _U32, const SG_PLAYER&, const SG_GENERAL&, const SG_ITEM*, _U32> _OnUseItemResult;
sigslot::signal8<_U8, _U32, _U32, _U32, _U8, _U8, _U32, _U32> _OnFeedHorseResult;
sigslot::signal3<const SG_BOSSRUSH_INFO&, const SG_BOSSRUSH_FRIEND_DAMAGE*, _U32> _OnQueryBossRushInfoResult;
sigslot::signal2<const SG_BOSSRUSH_SUPPORT_INFO*, _U32> _OnQueryBossRushSupportInfoResult;
sigslot::signal1<const SG_PLAYER_PVE&> _OnBeginBossRushBattleResult;
sigslot::signal8<_U8, _U32, _U32, _U32, _U32, _U32, const SG_DROP_ITEM_CONFIG*, _U32> _OnEndBossRushBattleResult;
sigslot::signal2<_U8, const SG_PLAYER_PVE&> _OnBeginBossRushSupportBattleResult;
sigslot::signal8<_U8, _U32, _U32, _U32, _U32, _U32, const SG_DROP_ITEM_CONFIG*, _U32> _OnEndBossRushSupportBattleResult;
sigslot::signal3<_U8, _U32, _U8> _OnAddBossRushRemainingTimesResult;
sigslot::signal8<_U8, _U32, _U32, _U32, _U32, _U32, const SG_DROP_ITEM_CONFIG*, _U32> _OnAwardBossRushResult;
sigslot::signal8<_U8, _U32, _U32, _U32, _U32, _U32, const SG_DROP_ITEM_CONFIG*, _U32> _OnAwardBossRushSupportResult;
sigslot::signal9<_U8, _U32, _U32, _U32, _U32, const SG_DROP_ITEM_CONFIG*, _U32, _U32, _U32> _OnQueryDiceNumResult;
sigslot::signal2<const SG_NEWCOMER_GUIDE_INFO*, _U32> _OnQueryNewcomerGuideInfoResult;
sigslot::signal1<const SG_WORLDBOSS_RANK_INFO&> _OnQueryWorldBossRankInfoResult;
sigslot::signal13<_U8, _U32, _U32, _U32, const SG_PLAYER_PVE&, const SG_WORLDBOSS_INFO&, const SG_WORLDBOSS_RANK_INFO&, const SG_PLAYER_INFO*, _U32, _U32, const SG_DROP_ITEM_BASE*, _U32, _U32> _OnBeginWorldBossBattleResult;
sigslot::signal2<_U8, _U32> _OnInspireWorldBossBattleResult;
sigslot::signal5<_U8, _U32, _U32, const SG_DROP_ITEM_BASE*, _U32> _OnAwardWorldBossRankResult;
sigslot::signal2<const SG_WORLDBOSS_INFO&, const SG_WORLDBOSS_RANK_INFO&> _OnUpdateWorldBossBattleResult;
sigslot::signal0<> _OnEndWorldBossBattleResult;
sigslot::signal5<_U8, _U32, _U32, const SG_DROP_ITEM_BASE*, _U32> _OnAwardWorldBossAttendanceResult;
sigslot::signal3<_U8, _U32, const SG_PLAYER&> _OnRechargeResult;
sigslot::signal6<const SG_LEAGUE_BATTLE_MEMBER*, _U32, const SG_LEAGUE_BATTLE_MEMBER*, _U32, _U32, _U32> _OnQueryLeagueBattleResult;
sigslot::signal11<_U8, _U32, _U32, _U32, _U32, _U32, _U32, _U32, _U32, const SG_DROP_ITEM_CONFIG*, _U32> _OnAwardLeagueBattleResult;
sigslot::signal8<_U8, _U32, _U32, _U32, _U32, _U32, const SG_DROP_ITEM_CONFIG*, _U32> _OnQueryLeagueBattleRewardResult;
sigslot::signal6<const SG_LEAGUE_BATTLE_LOG*, _U32, _U32, _U8, const SG_LEAGUE_BATTLE_MEMBER*, _U32> _OnQueryLeagueBattleInfoResult;
sigslot::signal4<const SG_LEAGUE_BATTLE_RANK*, _U32, const SG_LEAGUE_BATTLE_PLAYER_RANK*, _U32> _OnQueryLeagueBattleRankInfoResult;
sigslot::signal3<_U8, _U32, _U32> _OnSignUpResult;
sigslot::signal2<const SG_GOAL_CHAPTER_INFO*, _U32> _OnQueryGoalResult;
sigslot::signal6<_U8, const SG_GOAL_CHAPTER_INFO&, _U32, _U32, const SG_DROP_ITEM_CONFIG*, _U32> _OnAwardGoalChapterResult;
sigslot::signal2<_U8, _U32> _OnAwardActionResult;
sigslot::signal2<const SG_BATTLE_RESULT_INFO*, _U32> _OnAutoCombatPVEResult;
sigslot::signal2<const SG_BATTLE_RESULT_INFO*, _U32> _OnAutoCombatInstanceResult;
sigslot::signal1<_U32> _OnQueryFightValueResult;
sigslot::signal2<const SG_MAIL_INFO*, _U32> _OnCheckMailResult;
sigslot::signal6<_U8, _U32, _U32, _U32, const SG_ITEM*, _U32> _OnReceiveMailResult;
sigslot::signal8<_U8, _U32, const SG_GENERAL*, _U32, const SG_SOLDIER*, _U32, const SG_ITEM*, _U32> _OnRechargeBonusResult;

void Pong(CSGClient* pClient)
{
	_OnPong();
}
void BindAccountResult(CSGClient* pClient, _U8 ret)
{
	_OnBindAccountResult(ret);
}
void QueryServerTimeResult(CSGClient* pClient, _U32 time)
{
	_OnQueryServerTimeResult(time);
}
void GetServerListResult(CSGClient* pClient, const SG_SERVER_INFO* infos, _U32 count, _U32 last_server)
{
	_OnGetServerListResult(infos, count, last_server);
}
void QueryAvatarFailed(CSGClient* pClient, _U32 code)
{
	_OnQueryAvatarFailed(code);
}
void QueryAvatarResult(CSGClient* pClient, const SG_PLAYER& player)
{
	_OnQueryAvatarResult(player);
}
void EnterGameResult(CSGClient* pClient, _U8 ret)
{
	_OnEnterGameResult(ret);
}
void CreateAvatarResult(CSGClient* pClient, _U32 code)
{
	_OnCreateAvatarResult(code);
}
void QueryPlayerResult(CSGClient* pClient, const SG_PLAYER& player, _U8 nSync)
{
	_OnQueryPlayerResult(player, nSync);
}
void QueryGeneralResult(CSGClient* pClient, const SG_GENERAL* generals, _U32 count, _U8 nSync)
{
	_OnQueryGeneralResult(generals, count, nSync);
}
void QuerySoldierResult(CSGClient* pClient, const SG_SOLDIER* soldiers, _U32 count, _U8 nSync)
{
	_OnQuerySoldierResult(soldiers, count, nSync);
}
void QueryBagBegin(CSGClient* pClient)
{
	_OnQueryBagBegin();
}
void QueryBagEquipt(CSGClient* pClient, const SG_EQUIPT_ITEM* items, _U32 count)
{
	_OnQueryBagEquipt(items, count);
}
void QueryBagUsable(CSGClient* pClient, const SG_USABLE_ITEM* items, _U32 count)
{
	_OnQueryBagUsable(items, count);
}
void QueryBagGen(CSGClient* pClient, const SG_GEM_ITEM* items, _U32 count)
{
	_OnQueryBagGen(items, count);
}
void QueryBagMaterial(CSGClient* pClient, const SG_MATERIAL_ITEM* items, _U32 count)
{
	_OnQueryBagMaterial(items, count);
}
void QueryBagEnd(CSGClient* pClient, _U8 nSync)
{
	_OnQueryBagEnd(nSync);
}
void QueryOtherPlayerResult(CSGClient* pClient, const SG_PLAYER& player_info, const SG_GENERAL* generals, _U32 count1, const SG_SOLDIER* soldiers, _U32 count2, const SG_EQUIPT_ITEM* equipts, _U32 count3)
{
	_OnQueryOtherPlayerResult(player_info, generals, count1, soldiers, count2, equipts, count3);
}
void QueryOtherPlayersResult(CSGClient* pClient, const SG_PLAYER_INFO* players, _U32 count)
{
	_OnQueryOtherPlayersResult(players, count);
}
void EnhanceEquiptResult(CSGClient* pClient, _U8 ret, _U32 gold, const SG_EQUIPT_ITEM& equip_item)
{
	_OnEnhanceEquiptResult(ret, gold, equip_item);
}
void ExtendEquiptResult(CSGClient* pClient, _U8 ret, const SG_EQUIPT_ITEM& equip_item, const A_UUID& reduce_uuid)
{
	_OnExtendEquiptResult(ret, equip_item, reduce_uuid);
}
void EnhanceCoolDownResult(CSGClient* pClient, _U32 time)
{
	_OnEnhanceCoolDownResult(time);
}
void RefreshEquipDone(CSGClient* pClient, const SG_EQUIPT_ITEM& euipt)
{
	_OnRefreshEquipDone(euipt);
}
void RefreshEquipDecideAcceptResult(CSGClient* pClient, const SG_EQUIPT_ITEM& equipt)
{
	_OnRefreshEquipDecideAcceptResult(equipt);
}
void GemCombineResult(CSGClient* pClient, const SG_GEM_ITEM& gem)
{
	_OnGemCombineResult(gem);
}
void UpgradeTitleResult(CSGClient* pClient, _U8 ret, _U32 title)
{
	_OnUpgradeTitleResult(ret, title);
}
void HaloIncreaseEXPResult(CSGClient* pClient, _U32 time, _U32 halo_level, _U32 halo_exp)
{
	_OnHaloIncreaseEXPResult(time, halo_level, halo_exp);
}
void HaloCoolDownResult(CSGClient* pClient, _U8 ret, _U32 consume_rmb)
{
	_OnHaloCoolDownResult(ret, consume_rmb);
}
void HaloGetCoolDownResult(CSGClient* pClient, _U32 time)
{
	_OnHaloGetCoolDownResult(time);
}
void BeginBattleResult(CSGClient* pClient, const SG_PLAYER_PVE& PlayerPVE)
{
	_OnBeginBattleResult(PlayerPVE);
}
void EndBattleResult(CSGClient* pClient, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 wake_pt, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count)
{
	_OnEndBattleResult(level, exp_addition, exp, gold, wake_pt, drops, drop_count);
}
void QueryPlayerQuestResult(CSGClient* pClient, const SG_QUEST_LIVE_INFO* quest_list, _U32 count, _U8 nSync)
{
	_OnQueryPlayerQuestResult(quest_list, count, nSync);
}
void FinishQuestDone(CSGClient* pClient, _U8 ret, _U32 quest_id, _U32 exp_addition, _U32 exp, _U32 level, _U32 gold, _U32 rmb, _U32 reputation, _U32 energy, const SG_DROP_ITEM_BASE* drops, _U32 drop_count, _U32 consume_rmb)
{
	_OnFinishQuestDone(ret, quest_id, exp_addition, exp, level, gold, rmb, reputation, energy, drops, drop_count, consume_rmb);
}
void BuyGoodsResult(CSGClient* pClient, _U8 ret, const A_UUID* goods, const _U32* id_list, _U32 count)
{
	_OnBuyGoodsResult(ret, goods, id_list, count);
}
void SellItemResult(CSGClient* pClient, _U8 ret, const A_UUID& uuid, _U32 item_id, _U32 count)
{
	_OnSellItemResult(ret, uuid, item_id, count);
}
void QueryPlayerPVPInfoResult(CSGClient* pClient, const SG_PLAYER_PVE& pve)
{
	_OnQueryPlayerPVPInfoResult(pve);
}
void QueryPlayerRankListResult(CSGClient* pClient, const SG_PVP_RANK_PLAYER* players, _U32 count)
{
	_OnQueryPlayerRankListResult(players, count);
}
void PVPCoolDownResult(CSGClient* pClient, _U32 time)
{
	_OnPVPCoolDownResult(time);
}
void PVPGetRestTimeResult(CSGClient* pClient, _U32 rest_time)
{
	_OnPVPGetRestTimeResult(rest_time);
}
void PVPRecordResult(CSGClient* pClient, const SG_PVP_RECORD_ITEM* record, _U32 count)
{
	_OnPVPRecordResult(record, count);
}
void PVPHeroListRecord(CSGClient* pClient, const SG_PVP_HEROLIST_PLAYER* players, _U32 count)
{
	_OnPVPHeroListRecord(players, count);
}
void PVPDailyReward(CSGClient* pClient, _U32 gold, _U32 reputation, const SG_ITEM* items, _U32 count)
{
	_OnPVPDailyReward(gold, reputation, items, count);
}
void PVPBattleBeginResult(CSGClient* pClient, const SG_PLAYER_PVE& SelfPVE, const SG_PLAYER_PVE& DefenderPVE)
{
	_OnPVPBattleBeginResult(SelfPVE, DefenderPVE);
}
void PVPBattleEndResult(CSGClient* pClient, _U32 reputation)
{
	_OnPVPBattleEndResult(reputation);
}
void PVPCoolDownClearResult(CSGClient* pClient, _U8 ret, _U32 cost)
{
	_OnPVPCoolDownClearResult(ret, cost);
}
void PVPIncreateBattleTimesResult(CSGClient* pClient, _U32 rest_times)
{
	_OnPVPIncreateBattleTimesResult(rest_times);
}
void QueryInstanceResult(CSGClient* pClient, const SG_INSTANCE_INFO* instances, _U32 count)
{
	_OnQueryInstanceResult(instances, count);
}
void BeginInstanceBattleResult(CSGClient* pClient, const SG_PLAYER_PVE& PlayerPVE)
{
	_OnBeginInstanceBattleResult(PlayerPVE);
}
void EnterInstanceResult(CSGClient* pClient, const SG_INSTANCE_INFO& instance)
{
	_OnEnterInstanceResult(instance);
}
void EndInstanceBattleResult(CSGClient* pClient, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 wake_pt, _U8 result, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count)
{
	_OnEndInstanceBattleResult(level, exp_addition, exp, gold, wake_pt, result, drops, drop_count);
}
void ResetInstanceResult(CSGClient* pClient, _U8 result, _U32 rmb, const SG_INSTANCE_INFO& instance)
{
	_OnResetInstanceResult(result, rmb, instance);
}
void CreateLeagueResult(CSGClient* pClient, _U8 ret, const SG_LEAGUE& league)
{
	_OnCreateLeagueResult(ret, league);
}
void QueryLeagueApplyListResult(CSGClient* pClient, const SG_LEAGUE_APPLYER* applyer, _U32 count)
{
	_OnQueryLeagueApplyListResult(applyer, count);
}
void QueryLeagueResult(CSGClient* pClient, const SG_LEAGUE& league)
{
	_OnQueryLeagueResult(league);
}
void QueryLeagueListResult(CSGClient* pClient, const SG_LEAGUE* league_list, _U32 count)
{
	_OnQueryLeagueListResult(league_list, count);
}
void QueryLeagueMemberListResult(CSGClient* pClient, const SG_LEAGUE_MEMBER* league_members, _U32 count)
{
	_OnQueryLeagueMemberListResult(league_members, count);
}
void QueryLeagueMemberInfoResult(CSGClient* pClient, const SG_LEAGUE_MEMBER& member_info)
{
	_OnQueryLeagueMemberInfoResult(member_info);
}
void ContributeLeagueResult(CSGClient* pClient, const SG_LEAGUE_MEMBER& self_info, const SG_LEAGUE& league_info)
{
	_OnContributeLeagueResult(self_info, league_info);
}
void HandleApplyResult(CSGClient* pClient, _U8 ret, const SG_LEAGUE_MEMBER& new_joiner)
{
	_OnHandleApplyResult(ret, new_joiner);
}
void QueryLeagueNoticeResult(CSGClient* pClient, const char* notice_content)
{
	_OnQueryLeagueNoticeResult(notice_content);
}
void SetLeagueNoticeResult(CSGClient* pClient, _U8 ret, const char* notice_content)
{
	_OnSetLeagueNoticeResult(ret, notice_content);
}
void SetLeagueOwnerResult(CSGClient* pClient, _U8 ret, _U32 member_id)
{
	_OnSetLeagueOwnerResult(ret, member_id);
}
void SetMemberPositionResult(CSGClient* pClient, _U8 ret, _U32 member_id, _U8 position)
{
	_OnSetMemberPositionResult(ret, member_id, position);
}
void DismissMemberResult(CSGClient* pClient, _U8 ret, _U32 member_id)
{
	_OnDismissMemberResult(ret, member_id);
}
void ExitLeagueResult(CSGClient* pClient, _U8 ret)
{
	_OnExitLeagueResult(ret);
}
void QueryLeagueLogResult(CSGClient* pClient, const SG_LEAGUE_LOG* league_log, _U32 count)
{
	_OnQueryLeagueLogResult(league_log, count);
}
void LeagueToastResult(CSGClient* pClient, _U8 ret, _U32 gold, _U32 rmb, _U32 reward_reputation, _U32 reward_league_xp)
{
	_OnLeagueToastResult(ret, gold, rmb, reward_reputation, reward_league_xp);
}
void SalaryGetResult(CSGClient* pClient, _U8 ret, _U32 rmb, _U32 gold)
{
	_OnSalaryGetResult(ret, rmb, gold);
}
void SalaryGetBatResult(CSGClient* pClient, _U8 ret, _U32 rmb, _U32 gold, _U32 times)
{
	_OnSalaryGetBatResult(ret, rmb, gold, times);
}
void BuyEnergyResult(CSGClient* pClient, _U8 ret, _U32 rmb, _U32 energy, _U32 times)
{
	_OnBuyEnergyResult(ret, rmb, energy, times);
}
void EnhanceTurboResult(CSGClient* pClient, _U8 ret, _U32 turbo_level, _U32 wake_pt)
{
	_OnEnhanceTurboResult(ret, turbo_level, wake_pt);
}
void MakeEquiptResult(CSGClient* pClient, _U8 ret, const SG_EQUIPT_ITEM& new_euqipt, const SG_MATERIAL_ITEM& com_material, const SG_MATERIAL_ITEM& key_material)
{
	_OnMakeEquiptResult(ret, new_euqipt, com_material, key_material);
}
void QueryActionAvailableResult(CSGClient* pClient, const _U32* action_list, const _U32* available_list, _U32 count)
{
	_OnQueryActionAvailableResult(action_list, available_list, count);
}
void QueryFriendListResult(CSGClient* pClient, const SG_FRIEND_INFO* friend_list, _U32 count)
{
	_OnQueryFriendListResult(friend_list, count);
}
void QueryFriendListSearchResult(CSGClient* pClient, const SG_FRIEND_SEARCH_INFO* search_list, _U32 count)
{
	_OnQueryFriendListSearchResult(search_list, count);
}
void QueryFriendInvitationListResult(CSGClient* pClient, const SG_FRIEND_INVITATION* invitation_list, _U32 count)
{
	_OnQueryFriendInvitationListResult(invitation_list, count);
}
void AcceptFriendResult(CSGClient* pClient, _U8 ret, _U32 avatar_id)
{
	_OnAcceptFriendResult(ret, avatar_id);
}
void SendChatResult(CSGClient* pClient, _U8 ret)
{
	_OnSendChatResult(ret);
}
void ReceiveChatResult(CSGClient* pClient, const SG_CHAT_RECV_INFO* chat_list, _U32 count)
{
	_OnReceiveChatResult(chat_list, count);
}
void QueryLeagueDianjiangListResult(CSGClient* pClient, _U8 ret, const SG_LEAGUE_DIANJIANG_JOINER* joiner_list, _U32 count)
{
	_OnQueryLeagueDianjiangListResult(ret, joiner_list, count);
}
void JoinLeagueDianjiangResult(CSGClient* pClient, _U8 ret, const SG_LEAGUE_DIANJIANG_JOINER& self, const SG_LEAGUE_DIANJIANG_JOINER* joiner_list, _U32 count)
{
	_OnJoinLeagueDianjiangResult(ret, self, joiner_list, count);
}
void QueryLeagueDianjiangRewardResult(CSGClient* pClient, _U8 ret, _U32 reputation)
{
	_OnQueryLeagueDianjiangRewardResult(ret, reputation);
}
void LeagueDianjiangSummonNPCResult(CSGClient* pClient, _U8 ret, _U32 gold, _U32 rmb, const SG_LEAGUE_DIANJIANG_JOINER& npc_joiner, const SG_LEAGUE_DIANJIANG_JOINER* joiner_list, _U32 count)
{
	_OnLeagueDianjiangSummonNPCResult(ret, gold, rmb, npc_joiner, joiner_list, count);
}
void QueryAstrologyBagResult(CSGClient* pClient, const _U32* bag_list, _U32 count1, const _U32* tmp_bag_list, _U32 count2)
{
	_OnQueryAstrologyBagResult(bag_list, count1, tmp_bag_list, count2);
}
void SaveToBagResult(CSGClient* pClient, _U8 ret, _U32 item_id)
{
	_OnSaveToBagResult(ret, item_id);
}
void SetAstrologyBallStatusResult(CSGClient* pClient, _U8 ret, const SG_GENERAL& general)
{
	_OnSetAstrologyBallStatusResult(ret, general);
}
void EnhanceAstrologyBallResult(CSGClient* pClient, _U8 ret, _U32 gold, _U32 ball_id, _U32 new_ball_id)
{
	_OnEnhanceAstrologyBallResult(ret, gold, ball_id, new_ball_id);
}
void StrologyResult(CSGClient* pClient, _U8 ret, _U32 gold, _U32 ball_id, _U32 atrologer_id, _U32 last_atrologer_id)
{
	_OnStrologyResult(ret, gold, ball_id, atrologer_id, last_atrologer_id);
}
void StrologyAutoResult(CSGClient* pClient, _U8 ret, _U32 gold, const _U32* ball_list, _U32 count, _U32 atrologer_id)
{
	_OnStrologyAutoResult(ret, gold, ball_list, count, atrologer_id);
}
void DevourResult(CSGClient* pClient, _U8 ret, _U8 bag_type, const _U32* ball_list, _U32 count, const _U32* devoured_list, _U32 count2)
{
	_OnDevourResult(ret, bag_type, ball_list, count, devoured_list, count2);
}
void UseItemResult(CSGClient* pClient, _U8 ret, const A_UUID& uuid, _U32 count, _U32 target_id, const SG_PLAYER& player_info, const SG_GENERAL& general, const SG_ITEM* drops, _U32 drop_count)
{
	_OnUseItemResult(ret, uuid, count, target_id, player_info, general, drops, drop_count);
}
void FeedHorseResult(CSGClient* pClient, _U8 ret, _U32 xp, _U32 xp_add, _U32 level, _U8 xp_add_type, _U8 feed_type, _U32 gold, _U32 rmb)
{
	_OnFeedHorseResult(ret, xp, xp_add, level, xp_add_type, feed_type, gold, rmb);
}
void QueryBossRushInfoResult(CSGClient* pClient, const SG_BOSSRUSH_INFO& bossrush_info, const SG_BOSSRUSH_FRIEND_DAMAGE* friend_damages, _U32 count)
{
	_OnQueryBossRushInfoResult(bossrush_info, friend_damages, count);
}
void QueryBossRushSupportInfoResult(CSGClient* pClient, const SG_BOSSRUSH_SUPPORT_INFO* support_list, _U32 count)
{
	_OnQueryBossRushSupportInfoResult(support_list, count);
}
void BeginBossRushBattleResult(CSGClient* pClient, const SG_PLAYER_PVE& PlayerPVE)
{
	_OnBeginBossRushBattleResult(PlayerPVE);
}
void EndBossRushBattleResult(CSGClient* pClient, _U8 ret, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 reputation, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count)
{
	_OnEndBossRushBattleResult(ret, level, exp_addition, exp, gold, reputation, drops, drop_count);
}
void BeginBossRushSupportBattleResult(CSGClient* pClient, _U8 ret, const SG_PLAYER_PVE& PlayerPVE)
{
	_OnBeginBossRushSupportBattleResult(ret, PlayerPVE);
}
void EndBossRushSupportBattleResult(CSGClient* pClient, _U8 ret, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 reputation, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count)
{
	_OnEndBossRushSupportBattleResult(ret, level, exp_addition, exp, gold, reputation, drops, drop_count);
}
void AddBossRushRemainingTimesResult(CSGClient* pClient, _U8 ret, _U32 rmb, _U8 remain_times)
{
	_OnAddBossRushRemainingTimesResult(ret, rmb, remain_times);
}
void AwardBossRushResult(CSGClient* pClient, _U8 ret, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 reputation, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count)
{
	_OnAwardBossRushResult(ret, level, exp_addition, exp, gold, reputation, drops, drop_count);
}
void AwardBossRushSupportResult(CSGClient* pClient, _U8 ret, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 reputation, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count)
{
	_OnAwardBossRushSupportResult(ret, level, exp_addition, exp, gold, reputation, drops, drop_count);
}
void QueryDiceNumResult(CSGClient* pClient, _U8 ret, _U32 dice_num, _U32 reward_time, _U32 energy, _U32 rmb, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count, _U32 circle_num, _U32 step)
{
	_OnQueryDiceNumResult(ret, dice_num, reward_time, energy, rmb, drops, drop_count, circle_num, step);
}
void QueryNewcomerGuideInfoResult(CSGClient* pClient, const SG_NEWCOMER_GUIDE_INFO* guide_list, _U32 count)
{
	_OnQueryNewcomerGuideInfoResult(guide_list, count);
}
void QueryWorldBossRankInfoResult(CSGClient* pClient, const SG_WORLDBOSS_RANK_INFO& rank_info)
{
	_OnQueryWorldBossRankInfoResult(rank_info);
}
void BeginWorldBossBattleResult(CSGClient* pClient, _U8 ret, _U32 NumResurrection, _U32 NumInspiration, _U32 BattleStartTime, const SG_PLAYER_PVE& selfPve, const SG_WORLDBOSS_INFO& bossInfo, const SG_WORLDBOSS_RANK_INFO& ranklist, const SG_PLAYER_INFO* otherPlayers, _U32 count1, _U32 gold, const SG_DROP_ITEM_BASE* drops, _U32 count2, _U32 rmb)
{
	_OnBeginWorldBossBattleResult(ret, NumResurrection, NumInspiration, BattleStartTime, selfPve, bossInfo, ranklist, otherPlayers, count1, gold, drops, count2, rmb);
}
void InspireWorldBossBattleResult(CSGClient* pClient, _U8 ret, _U32 rmb)
{
	_OnInspireWorldBossBattleResult(ret, rmb);
}
void AwardWorldBossRankResult(CSGClient* pClient, _U8 ret, _U32 Gold, _U32 Reputation, const SG_DROP_ITEM_BASE* ItemList, _U32 count)
{
	_OnAwardWorldBossRankResult(ret, Gold, Reputation, ItemList, count);
}
void UpdateWorldBossBattleResult(CSGClient* pClient, const SG_WORLDBOSS_INFO& bossInfo, const SG_WORLDBOSS_RANK_INFO& rankinfo)
{
	_OnUpdateWorldBossBattleResult(bossInfo, rankinfo);
}
void EndWorldBossBattleResult(CSGClient* pClient)
{
	_OnEndWorldBossBattleResult();
}
void AwardWorldBossAttendanceResult(CSGClient* pClient, _U8 ret, _U32 Gold, _U32 Reputation, const SG_DROP_ITEM_BASE* ItemList, _U32 count)
{
	_OnAwardWorldBossAttendanceResult(ret, Gold, Reputation, ItemList, count);
}
void RechargeResult(CSGClient* pClient, _U8 ret, _U32 rmb, const SG_PLAYER& selfplayer)
{
	_OnRechargeResult(ret, rmb, selfplayer);
}
void QueryLeagueBattleResult(CSGClient* pClient, const SG_LEAGUE_BATTLE_MEMBER* info_list, _U32 count1, const SG_LEAGUE_BATTLE_MEMBER* enemy_list, _U32 count2, _U32 rank, _U32 final_loop)
{
	_OnQueryLeagueBattleResult(info_list, count1, enemy_list, count2, rank, final_loop);
}
void AwardLeagueBattleResult(CSGClient* pClient, _U8 ret, _U32 type, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 rmb, _U32 reputation, _U32 contribution, const SG_DROP_ITEM_CONFIG* items, _U32 count)
{
	_OnAwardLeagueBattleResult(ret, type, level, exp_addition, exp, gold, rmb, reputation, contribution, items, count);
}
void QueryLeagueBattleRewardResult(CSGClient* pClient, _U8 ret, _U32 exp_addition, _U32 gold, _U32 rmb, _U32 reputation, _U32 contribution, const SG_DROP_ITEM_CONFIG* items, _U32 count)
{
	_OnQueryLeagueBattleRewardResult(ret, exp_addition, gold, rmb, reputation, contribution, items, count);
}
void QueryLeagueBattleInfoResult(CSGClient* pClient, const SG_LEAGUE_BATTLE_LOG* log_list, _U32 count1, _U32 rank, _U8 status, const SG_LEAGUE_BATTLE_MEMBER* member_list, _U32 count2)
{
	_OnQueryLeagueBattleInfoResult(log_list, count1, rank, status, member_list, count2);
}
void QueryLeagueBattleRankInfoResult(CSGClient* pClient, const SG_LEAGUE_BATTLE_RANK* league_rank_list, _U32 count1, const SG_LEAGUE_BATTLE_PLAYER_RANK* player_rank, _U32 count2)
{
	_OnQueryLeagueBattleRankInfoResult(league_rank_list, count1, player_rank, count2);
}
void SignUpResult(CSGClient* pClient, _U8 ret, _U32 aciton_id, _U32 times)
{
	_OnSignUpResult(ret, aciton_id, times);
}
void QueryGoalResult(CSGClient* pClient, const SG_GOAL_CHAPTER_INFO* chapter_list, _U32 count)
{
	_OnQueryGoalResult(chapter_list, count);
}
void AwardGoalChapterResult(CSGClient* pClient, _U8 ret, const SG_GOAL_CHAPTER_INFO& chapter, _U32 gold, _U32 rmb, const SG_DROP_ITEM_CONFIG* items, _U32 count)
{
	_OnAwardGoalChapterResult(ret, chapter, gold, rmb, items, count);
}
void AwardActionResult(CSGClient* pClient, _U8 ret, _U32 action_id)
{
	_OnAwardActionResult(ret, action_id);
}
void AutoCombatPVEResult(CSGClient* pClient, const SG_BATTLE_RESULT_INFO* battle_results, _U32 battle_count)
{
	_OnAutoCombatPVEResult(battle_results, battle_count);
}
void AutoCombatInstanceResult(CSGClient* pClient, const SG_BATTLE_RESULT_INFO* battle_results, _U32 battle_count)
{
	_OnAutoCombatInstanceResult(battle_results, battle_count);
}
void QueryFightValueResult(CSGClient* pClient, _U32 fight_value)
{
	_OnQueryFightValueResult(fight_value);
}
void CheckMailResult(CSGClient* pClient, const SG_MAIL_INFO* mail_list, _U32 count)
{
	_OnCheckMailResult(mail_list, count);
}
void ReceiveMailResult(CSGClient* pClient, _U8 ret, _U32 mail_id, _U32 reward_gold, _U32 reward_rmb, const SG_ITEM* reward_list, _U32 count)
{
	_OnReceiveMailResult(ret, mail_id, reward_gold, reward_rmb, reward_list, count);
}
void RechargeBonusResult(CSGClient* pClient, _U8 ret, _U32 bonus_id, const SG_GENERAL* general_list, _U32 count1, const SG_SOLDIER* soldier_list, _U32 count2, const SG_ITEM* item_list, _U32 count3)
{
	_OnRechargeBonusResult(ret, bonus_id, general_list, count1, soldier_list, count2, item_list, count3);
}

