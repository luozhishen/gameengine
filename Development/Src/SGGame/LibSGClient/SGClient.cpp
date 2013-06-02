#include <ZionBase.h>
#include <ZionClient.h>
#include <ZionCommon.h>

#include <time.h>
#include "SGCommon.h"
#include "SGClient.h"
#include "SGSyncDataManager.h"
#include "SGClientUtil.h"

#include "SGActionStatusCache.h"
#include "SGClientTipsHelper.h"

namespace Zion
{
	int CSGClient::ms_nLastRanderTime = 0;
	static Zion::SGActionStatusCache g_actionStatusCache;
	static Zion::CSGSyncDataManager g_syncDataManager;

	Zion::SGClientTipsHelper<_U32>		g_newSoldiers;		//新获得的可解锁的soldier
	Zion::SGClientTipsHelper<A_UUID>	g_newItemList;		//新获得的物品
	Zion::SGClientTipsHelper<_U32>		g_newApplyers;		//申请的战盟的玩家的信息

	CSGClient::CSGClient(CClientApp* pClientApp, _U32 recvsize) : CClient(pClientApp, recvsize), m_C2S(this), m_S2C(this)
	{
		m_callback = NULL;
		m_nServerTimeDelta = 0;
		m_nConnectPingTime = 0;
		g_syncDataManager.SetSGClient(this);
		m_lastServerID = SG_INVALID_SERVER_ID;
	}	

	CSGClient::~CSGClient()
	{

	}

	CSGClientCallback::~CSGClientCallback()
	{
	}

	void CSGClient::SetCallback(CSGClientCallback* callback)
	{
		m_callback = callback;
	}

	void CSGClient::InitializeComponents()
	{
		
	}

	void CSGClient::OnData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		CClient::OnData(iid, fid, len, data);

		if(iid==GetClientStubID<SGGAME_S2C>())
		{
			DDL::MemoryReader reader(data, len);
			m_S2C.Dispatcher(this, fid, reader);
			return;
		}
	}

	void CSGClient::Ping()
	{
		if(m_nConnectPingTime != 0)
		{
			return;	//still waitting
		}
		else
		{
			time_t tv;
			time(&tv);
			m_nConnectPingTime = (_U64)tv;
		}

		m_C2S.Ping();
	}

	void CSGClient::GetServerList()
	{
		m_C2S.GetServerList();
	}

	void CSGClient::EnterServer(_U32 server_id)
	{
		m_C2S.EnterServer(server_id);
		m_lastServerID = server_id;
	}

	void CSGClient::QueryAvatar()
	{
		m_C2S.QueryAvatar();
	}

	void CSGClient::CreateAvatar(const char* nick, _U32 general_id)
	{
		m_C2S.CreateAvatar(nick, general_id);
	}

	void CSGClient::DeleteAvatar()
	{
		m_C2S.DeleteAvatar();

		memset(&m_player, 0, sizeof(SG_PLAYER));
		m_generals.clear();
		m_soldiers.clear();
		m_equipts.clear();
		m_usables.clear();
		m_gems.clear();
		m_quests.clear();

		m_nConnectPingTime = 0;
	}

	void CSGClient::EnterGame()
	{
		m_C2S.EnterGame();
	}

	void CSGClient::LeaveGame()
	{
		m_C2S.LeaveGame();
	}

	void CSGClient::QueryPlayer(_U8 nSync)
	{
		m_C2S.QueryPlayer(nSync);
	}

	void CSGClient::QueryGenerals(_U8 nSync)
	{
		m_C2S.QueryGenerals(nSync);
	}

	void CSGClient::QuerySoldiers(_U8 nSync)
	{
		m_C2S.QuerySoldiers(nSync);
	}

	void CSGClient::QueryBag(_U8 nSync)
	{
		m_C2S.QueryBag(nSync);
	}

	void CSGClient::QueryOtherPlayers(const char* szCurrentTownMap, _U32 nMaxNum)
	{
		m_C2S.QueryOtherPlayers(szCurrentTownMap, nMaxNum);
	}

	void CSGClient::EquipItem(_U32 general_id, const SG_EQUIP_SLOTS& slots)
	{
		SG_EQUIP_SLOTS tidy_slots;
		for(Zion::Array<SG_EQUIPT_ITEM>::iterator it = m_equipts.begin(); it != m_equipts.end(); ++it)
		{
			const DDLReflect::STRUCT_INFO* struct_info = Zion::ContentObject::GetType("SG_EQUIPT_ITEM_CONFIG");
			A_UUID uuid; 
			if(it->uuid == slots.head
				||it->uuid == slots.weapon
				||it->uuid == slots.weapon
				||it->uuid == slots.shoulder
				||it->uuid == slots.chest
				||it->uuid == slots.leg
				||it->uuid == slots.decoration)
			{
				uuid = it->uuid;
			}

			if(uuid.isEmpty())
				continue;

			const A_CONTENT_OBJECT* content_obj = NULL;
			Zion::Array<A_UUID> content_list;
			if(!Zion::ContentObject::GetList(struct_info, content_list, true))
			{
				continue;
			}

			for(Zion::Array<A_UUID>::iterator it_uuid = content_list.begin(); it_uuid != content_list.end(); ++it_uuid)
			{
				content_obj = Zion::ContentObject::QueryByUUID(*it_uuid, struct_info);
				if(((SG_EQUIPT_ITEM_CONFIG*)content_obj)->item_id == it->item_id)
				{
					SGClientUtil::SetRightLocation(content_obj, tidy_slots, uuid);	
					break;
				}
			}
		}

		m_C2S.EquipItem(general_id, tidy_slots);

		if(m_player.general_id == general_id)
		{
			m_player.equip_slots = tidy_slots;
		}
		else
		{
			for(_U32 i = 0; i < m_generals.size(); ++i)
			{
				if(m_generals[i].general_id == general_id)
				{
					m_generals[i].equip_slots = tidy_slots;
					break;
				}
			}
		}
	}

	void CSGClient::EquipGenerals(const _U32* generals, _U32 count)
	{
		m_C2S.EquipGenerals(generals, count);

		m_player.equip_generals.Resize(count);
		for(_U32 i = 0; i < count; ++i)
		{
			m_player.equip_generals._Array[i] = generals[i];
		}
	}

	void CSGClient::EquipSoldiers(const _U32* soldiers, _U32 count)
	{
		m_C2S.EquipSoldiers(soldiers, count);

		m_player.equip_soldiers.Resize(count);
		for(_U32 i = 0; i < count; ++i)
		{
			m_player.equip_soldiers._Array[i] = soldiers[i];
		}
	}

	void CSGClient::UnLockSoldier(_U32 soldier_id)
	{
		m_C2S.UnLockSoldier(soldier_id);

		if(SGClientUtil::UnlockSoldier(m_player, m_soldiers, soldier_id))
		{
			m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
			m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncSoldiers);
		}
	}

	void CSGClient::EnhanceSoldier(_U32 soldier_id)
	{
		m_C2S.EnhanceSoldier(soldier_id);

		if(SGClientUtil::EnhanceSoldier(m_player, m_soldiers, soldier_id))
		{
			m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
			m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncSoldiers);
		}
	}

	void CSGClient::EnhanceEquipt(A_UUID& uuid)
	{
		m_C2S.EnhanceEquipt(uuid);
	}

	void CSGClient::ExtendEquipt(A_UUID& uuid, A_UUID& puuid)
	{
		m_C2S.ExtendEquipt(uuid, puuid);
	}

	void CSGClient::EnhanceCoolDown()
	{
		m_C2S.EnhanceCoolDown();
	}

	void CSGClient::EnhanceCoolDownClear()
	{
		m_C2S.EnhanceCoolDownClear();

		//help to sync data
		Zion::Array<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		SyncSet(vecSync);
	}

	void CSGClient::IncreaseEquipCoolDown()
	{
		m_C2S.IncreaseEquipCoolDown();

		//help to sync data
		Zion::Array<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		SyncSet(vecSync);
	}

	void CSGClient::RefreshEquipNormal(A_UUID& uuid)
	{
		m_C2S.RefreshEquipNormal(uuid);
	}
	
	void CSGClient::RefreshEquipProperty(A_UUID& uuid)
	{
		m_C2S.RefreshEquipProperty(uuid);
	}

	void CSGClient::RefreshEquipAbility(A_UUID& uuid)
	{
		m_C2S.RefreshEquipAbility(uuid);
	}
	
	void CSGClient::RefreshEquipDecideAccept(A_UUID& uuid)
	{
		m_C2S.RefreshEquipDecideAccept(uuid);

		////help to sync data
		//Zion::Array<_U8> vecSync;
		//vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		//vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
		//SyncSet(vecSync);
	}

	
	void CSGClient::HaloCoolDown()		
	{
		m_C2S.HaloCoolDown();
	}

	void CSGClient::HaloIncreaseEXP(_U8 halo_type)								
	{
		m_C2S.HaloIncreaseEXP(halo_type);
	}

	void CSGClient::HaloGetCoolDown()											
	{
		m_C2S.HaloGetCoolDown();
	}

	void CSGClient::BeginBattle(const char* name)
	{
		m_C2S.BeginBattle(name);
	}

	void CSGClient::EndBattle(const char* name, _U32 result, _U8 auto_combat)
	{
		m_C2S.EndBattle(name, result, auto_combat);
	}

	void CSGClient::QueryServerTime()
	{
		m_C2S.QueryServerTime();
	}

	void CSGClient::EquipGem(const A_UUID& item_uuid, _U32 gem_id)
	{
		m_C2S.EquipGem(item_uuid, gem_id);

		//help to sync data
		Zion::Array<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
		SyncSet(vecSync);
	}

	void CSGClient::UnequipGem(const A_UUID& item_uuid, _U32 gem_id)
	{
		m_C2S.UnequipGem(item_uuid, gem_id);

		//help to sync data
		Zion::Array<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
		SyncSet(vecSync);
	}

	void CSGClient::GemCombine(_U32 item_id)
	{
		m_C2S.GemCombine(item_id);

		//help to sync data
		Zion::Array<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
		SyncSet(vecSync);
	}

	void CSGClient::SetGeneralStatus(_U32 general_id, _U8 status)
	{
		bool bEmployeed = false;
		for(size_t i = 0; i < m_generals.size(); ++i)
		{
			if(general_id == m_generals[i].general_id)
			{
				m_generals[i].status = status;
				bEmployeed = true;
				break;
			}
		}

		if(!bEmployeed)
		{
			SG_GENERAL general_inst;
			memset(&general_inst, 0, sizeof(SG_GENERAL));

			general_inst.general_id = general_id;
			const DDLReflect::STRUCT_INFO* struct_info = DDLReflect::GetStruct<SG_PLAYER_TITLE_CONFIG>();
			Zion::Array<A_UUID> uuid_list;
			if(!ContentObject::GetList(struct_info, uuid_list, true))
			{
				return;
			}

			_U32 general_limit_num = 0;
			for(Zion::Array<A_UUID>::iterator it = uuid_list.begin(); it != uuid_list.end(); ++it)
			{
				const SG_PLAYER_TITLE_CONFIG* title_config = (const SG_PLAYER_TITLE_CONFIG*)ContentObject::QueryByUUID(*it, struct_info);
				if(!title_config) continue;

				if(title_config->title_level == m_player.title)
				{
					general_limit_num = title_config->general_limit_num;
					break;
				}
			}

			general_inst.level = 1;
			general_inst.exp = 0;

			general_inst.status = m_generals.size() >= general_limit_num ? SG_GENERAL_EMPLOYEED_OUTQUEUE : SG_GENERAL_EMPLOYEED_INQUEUE;
			m_generals.push_back(general_inst);
		}

		m_C2S.SetGeneralStatus(general_id, status);

		//help to sync data
		Zion::Array<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		//vecSync.push_back(CSGSyncDataManager::eSyncGenerals);
		SyncSet(vecSync);
	}

	void CSGClient::QueryPlayerQuest(_U8 nSync)												
	{
		m_C2S.QueryPlayerQuest(nSync);
	}

	void CSGClient::SaveQuestData(const SG_QUEST_LIVE_INFO& quest_info)
	{
		Zion::Array<SG_QUEST_LIVE_INFO>::iterator it; 
		for(it = m_quests.begin(); it != m_quests.end(); ++it)
		{
			SG_QUEST_LIVE_INFO& quest_item = (*it);
			if(quest_item.quest_id == quest_info.quest_id)
			{
				quest_item.counter = quest_info.counter;
				quest_item.status = quest_info.status;
				break;
			}
		}

		if(it == m_quests.end())
		{
			m_quests.push_back(quest_info);
		}

		m_C2S.SaveQuestData(quest_info.quest_id, quest_info.counter, quest_info.status);
	}
	
	void CSGClient::FinishQuest(_U32 quest_id, _U32 rmb)
	{
		m_C2S.FinishQuest(quest_id, rmb);
	}

	void CSGClient::GetPaid()
	{
		m_C2S.GetPaid();
		
		Zion::Array<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		SyncSet(vecSync);
	}
	
	void CSGClient::UpgradeTitle()
	{
		m_C2S.UpgradeTitle();

		Zion::Array<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		SyncSet(vecSync);
	}

	void CSGClient::QueryPlayerPVPInfo(_U32 avatar_id)
	{
		m_C2S.QueryPlayerPVPInfo(avatar_id);
	}

	void CSGClient::QueryPlayerRankList()
	{
		m_C2S.QueryPlayerRankList();
	}

	void CSGClient::BuyGoods(_U32 item_id)
	{
		m_C2S.BuyGoods(item_id);
	}

	void CSGClient::SellItem(const A_UUID& uuid, const _U32 item_id, const _U32 count)
	{
		m_C2S.SellItem(uuid, item_id, count);
	}

	void CSGClient::PVPCoolDown()
	{
		m_C2S.PVPCoolDown();
	}

	void CSGClient::PVPGetRestTime()
	{
		m_C2S.PVPGetRestTime();
	}

	void CSGClient::PVPRecord()		
	{
		m_C2S.PVPRecord();
	}

	void CSGClient::PVPHeroList()	
	{
		m_C2S.PVPHeroList();
	}

	void CSGClient::PVPDailyReward()
	{
		m_C2S.PVPDailyReward();

		//sync player and bag
		Zion::Array<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		SyncSet(vecSync);
	}

	void CSGClient::PVPBattleBegin(_U32 defender)
	{
		m_C2S.PVPBattleBegin(defender);
	}

	void CSGClient::PVPBattleEnd(_U32 defender, _U8 ret)
	{
		m_C2S.PVPBattleEnd(defender, ret);
	}

	void CSGClient::PVPCoolDownClear()
	{
		m_C2S.PVPCoolDownClear();

		//sync player and bag
		Zion::Array<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		SyncSet(vecSync);
	}

	void CSGClient::PVPIncreateBattleTimes()
	{
		m_C2S.PVPIncreateBattleTimes();
	}

	void CSGClient::QueryInstance()
	{
		m_C2S.QueryInstance();
	}

	void CSGClient::EnterInstance(_U32 instance_id, _U8 difficulty)
	{
		m_C2S.EnterInstance(instance_id, difficulty);
	}

	void CSGClient::BeginInstanceBattle(_U32 instance_id, const char* map_url)
	{
		m_C2S.BeginInstanceBattle(instance_id, map_url);
	}

	void CSGClient::EndInstanceBattle(_U32 instance_id, const char* map_url, _U32 result, _U8 auto_combat)
	{
		m_C2S.EndInstanceBattle(instance_id, map_url, result, auto_combat);
	}

	void CSGClient::ResetInstance(_U32 instance_id)
	{
		m_C2S.ResetInstance(instance_id);
	}

	void CSGClient::SaveLastTownMap(const char* last_town_map)
	{
		m_C2S.SaveLastTownMap(last_town_map);

		m_player.last_town_map = last_town_map;
	}

	void CSGClient::CreateLeague(const char* league_name)
	{
		m_C2S.CreateLeague(league_name);

		//help to sync data
		Zion::Array<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		SyncSet(vecSync);
	}

	void CSGClient::ApplyJoinLeague(_U32 league_id)
	{
		m_C2S.ApplyJoinLeague(league_id);

		//help to sync data
		Zion::Array<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		SyncSet(vecSync);
	}

	void CSGClient::QueryLeagueApplyList(_U32 league_id)
	{
		m_C2S.QueryLeagueApplyList(league_id);
	}

	void CSGClient::QueryLeague(_U32 league_id)
	{
		m_C2S.QueryLeague(league_id);
	}

	void CSGClient::QueryLeagueList()
	{
		m_C2S.QueryLeagueList();
	}

	void CSGClient::QueryLeagueMemberList(_U32 league_id)
	{
		m_C2S.QueryLeagueMemberList(league_id);
	}

	void CSGClient::QueryLeagueMemberInfo(_U32 member_id)
	{
		m_C2S.QueryLeagueMemberInfo(member_id);
	}

	void CSGClient::ContributeLeague(_U32 rmb, _U32 energy)
	{
		m_C2S.ContributeLeague(rmb, energy);
	}

	void CSGClient::HandleApply(_U32 applyer_id, _U8 allowed)
	{
		m_C2S.HandleApply(applyer_id, allowed);
	}

	void CSGClient::QueryLeagueNotice()
	{
		m_C2S.QueryLeagueNotice();
	}

	void CSGClient::SetLeagueNotice(_U32 league_id, const char* notice_content)
	{
		m_C2S.SetLeagueNotice(league_id, notice_content);
	}

	void CSGClient::SetLeagueOwner(_U32 member_id)
	{
		m_C2S.SetLeagueOwner(member_id);
	}

	void CSGClient::DissolveLeague()
	{
		m_C2S.DissolveLeague();
		m_player.league_id = 0;
	}

	void CSGClient::SetMemberPosition(_U32 member_id, _U8 position)
	{
		m_C2S.SetMemberPosition(member_id, position);
	}

	void CSGClient::DismissMember(_U32 member_id)
	{
		m_C2S.DismissMember(member_id);
	}

	void CSGClient::ExitLeague()
	{
		m_C2S.ExitLeague();
	}

	void CSGClient::QueryLeagueLog()
	{
		m_C2S.QueryLeagueLog();
	}

	void CSGClient::LeagueToast(_U8 wine_id)
	{
		m_C2S.LeagueToast(wine_id);
	}

	void CSGClient::SalaryGet()
	{
		m_C2S.SalaryGet();
	}
	
	void CSGClient::SalaryGetBat()
	{
		m_C2S.SalaryGetBat();
	}

	void CSGClient::BuyEnergy()
	{
		m_C2S.BuyEnergy();	
	}

	void CSGClient::EnhanceTurbo()
	{
		m_C2S.EnhanceTurbo();
	}
		
	void CSGClient::EquipTurboSkill(const SG_TURBO_SKILL_SLOT& skill_slot)
	{
		m_C2S.EquipTurboSkill(skill_slot);
		
		//help to sync data
		Zion::Array<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		SyncSet(vecSync);
	}

	void CSGClient::MakeEquipt(_U32 equipt_id)
	{
		m_C2S.MakeEquipt(equipt_id);
	}

	void CSGClient::QueryActionAvailable(_U32 *action_list, _U32 count)
	{
		m_C2S.QueryActionAvailable(action_list, count);
	}

	void CSGClient::QueryFriendList()
	{
		m_C2S.QueryFriendList();
	}

	void CSGClient::QueryFriendSearchList(const char* nick)
	{
		m_C2S.QueryFriendSearchList(nick);
	}

	void CSGClient::QueryFriendInvitationList()
	{
		m_C2S.QueryFriendInvitationList();
	}

	void CSGClient::InviteFriend(_U32 avatar_id)
	{
		m_C2S.InviteFriend(avatar_id);
	}

	void CSGClient::AcceptFriend(_U32 avatar_id)
	{
		m_C2S.AcceptFriend(avatar_id);
	}

	void CSGClient::RefuseInvitation(_U32 avatar_id)
	{
		m_C2S.RefuseInvitation(avatar_id);
	}

	void CSGClient::DelFriend(_U32 avatar_id)
	{
		m_C2S.DelFriend(avatar_id);
	}
	
	void CSGClient::SendChat(const SG_CHAT_SEND_INFO& send_info)
	{
		m_C2S.SendChat(send_info);
	}

	void CSGClient::ReceiveChat(const _U32* channel_list, const _U32 count)
	{
		m_C2S.ReceiveChat(channel_list, count);
	}

	void CSGClient::QueryLeagueDianjiangList(_U32 league_id)
	{
		m_C2S.QueryLeagueDianjiangList(league_id);
	}

	void CSGClient::JoinLeagueDianjiang(_U32 league_id)
	{
		m_C2S.JoinLeagueDianjiang(league_id);
	}

	void CSGClient::QueryLeagueDianjiangReward(_U32 league_id, _U32 avatar_id)
	{
		m_C2S.QueryLeagueDianjiangReward(league_id, avatar_id);
	}

	void CSGClient::LeagueDianjiangSummonNPC(_U32 league_id)
	{
		m_C2S.LeagueDianjiangSummonNPC(league_id);
	}

	void CSGClient::QueryAstrologyBag()
	{
		m_C2S.QueryAstrologyBag();
	}

	void CSGClient::SaveToBag(_U32 item_id)
	{
		m_C2S.SaveToBag(item_id);
	}

	void CSGClient::SetAstrologyBallStatus(_U32 general_id, _U32 ball_id, _U8 status)
	{
		m_C2S.SetAstrologyBallStatus(general_id, ball_id, status);
	}

	void CSGClient::EnhanceAstrologyBall(_U32 ball_id, _U32 general_id, _U32 index)
	{
		m_C2S.EnhanceAstrologyBall(ball_id, general_id, index);
	}

	//void CSGClient::BuyAstrologyBall(_U32 ball_id)
	//{
	//	m_C2S.BuyAstrologyBall(ball_id);
	//}

	void CSGClient::Strology(_U32 astrologer_id)
	{
		m_C2S.Strology(astrologer_id);
	}

	void CSGClient::StrologyAuto(_U32 RestSlotCount)
	{
		m_C2S.StrologyAuto(RestSlotCount);
	}

	void CSGClient::UseItem(const A_UUID& uuid, _U32 item_id, _U32 count, _U32 target_id)
	{
		m_C2S.UseItem(uuid, item_id, count, target_id);
	}

	void CSGClient::Devour(_U8 bag_type)
	{
		m_C2S.Devour(bag_type);
	}

	void CSGClient::FeedHorse(const _U8 feed_type)
	{
		m_C2S.FeedHorse(feed_type);
	}


	void CSGClient::QueryBossRushInfo()
	{
		m_C2S.QueryBossRushInfo();
	}

	void CSGClient::QueryBossRushSupportInfo()
	{
		m_C2S.QueryBossRushSupportInfo();
	}

	void CSGClient::BeginBossRushBattle(_U32 level_id, const char* level_url, _U32 init_boss_hp)
	{
		m_C2S.BeginBossRushBattle(level_id, level_url, init_boss_hp);
	}

	void CSGClient::EndBossRushBattle(const char* level_url, _U32 result, _U32 total_damage)
	{
		m_C2S.EndBossRushBattle(level_url, result, total_damage);
	}

	void CSGClient::BeginBossRushSupportBattle(_U32 friend_id, _U32 level_id, const char* level_url)
	{
		m_C2S.BeginBossRushSupportBattle(friend_id, level_id, level_url);
	}

	void CSGClient::EndBossRushSupportBattle(_U32 friend_id, const char* level_url, _U32 result, _U32 total_damage)
	{
		m_C2S.EndBossRushSupportBattle(friend_id, level_url, result, total_damage);
	}

	void CSGClient::RequestBossRushSupport()
	{
		m_C2S.RequestBossRushSupport();
	}

	void CSGClient::CancelBossRush()
	{
		m_C2S.CancelBossRush();
	}

	void CSGClient::AddBossRushRemainingTimes()
	{
		m_C2S.AddBossRushRemainingTimes();
	}

	void CSGClient::AwardBossRush()
	{
		m_C2S.AwardBossRush();
	}

	void CSGClient::AwardBossRushSupport(_U32 friend_id, _U32 level_id, const char* level_url)
	{
		m_C2S.AwardBossRushSupport(friend_id, level_id, level_url);
	}

	void CSGClient::QueryDiceNum()
	{
		m_C2S.QueryDiceNum();
	}

	void CSGClient::QueryNewcomerGuideInfo()
	{
		m_C2S.QueryNewcomerGuideInfo();
	}

	void CSGClient::ActivateNewcomerGuide(_U32 function_id)
	{
		m_C2S.ActivateNewcomerGuide(function_id);
	}
	
	void CSGClient::FinishNewcomerGuide(_U32 function_id)
	{
		m_C2S.FinishNewcomerGuide(function_id);
	}
		
	void CSGClient::QueryWorldBossRankInfo()
	{
		m_C2S.QueryWorldBossRankInfo();
	}

	void CSGClient::BeginWorldBossBattle(_U32 bInstantResurrection)
	{
		m_C2S.BeginWorldBossBattle(bInstantResurrection);
	}

	void CSGClient::InspireWorldBossBattle()
	{
		m_C2S.InspireWorldBossBattle();
	}

	void CSGClient::UpdateWorldBossBattle(_U32 damage)
	{
		m_C2S.UpdateWorldBossBattle(damage);
	}

	void CSGClient::EndWorldBossBattle(_U32 damage)
	{
		m_C2S.EndWorldBossBattle(damage);
	}

	void CSGClient::AwardWorldBossRank()
	{
		m_C2S.AwardWorldBossRank();
	}

	void CSGClient::AwardWorldBossAttendance()
	{
		m_C2S.AwardWorldBossAttendance();
	}

	void CSGClient::Recharge(_U32 index)
	{
		m_C2S.Recharge(index);
	}

	void CSGClient::Pong(CSGClient* pClient)
	{
		if(m_callback)
		{
			m_nConnectPingTime = 0;
			m_callback->NetworkTestResult(true);
		}
	}

	void CSGClient::GetServerListResult(CSGClient* pClient, const SG_SERVER_INFO* infos, _U32 count, _U32 last_server)
	{	
		m_serverList.clear();
		for(_U32 i = 0; i < count; ++i)
		{
			m_serverList.push_back(infos[i]);
		}

		//for login after LoginDone
		static bool bLogin = true;
		if(bLogin)
		{
			if(m_callback && m_lastServerID == SG_INVALID_SERVER_ID)
			{
				if(last_server == SG_INVALID_SERVER_ID)
				{
					m_lastServerID = infos[count-1].server_id;
				}
				else
				{
					m_lastServerID = last_server;
				}

				m_callback->LoginResult(0);
				bLogin = false;
				return;
			}
		}

		m_lastServerID = last_server;

		if(m_callback) m_callback->GetServerListResult(infos, count, last_server);
	}

	void CSGClient::QueryAvatarFailed(CSGClient* pClient, _U32 code)
	{
		if(m_callback) m_callback->QueryAvatarFailed(code);
	}

	void CSGClient::QueryAvatarResult(CSGClient* pClient, const SG_PLAYER& player)
	{
		m_player = player;
		if(m_callback) m_callback->QueryAvatarDone(m_player);
	}

	void CSGClient::EnterGameResult(CSGClient* pClient, _U8 ret)
	{
		if(!ret)
		{
			SyncForInit();
		}

		if(m_callback)
		{
			m_callback->EnterGameResult(ret);
		}
	}

	void CSGClient::CreateAvatarResult(CSGClient* pClient, _U32 code)
	{
		if(m_callback) m_callback->CreateAvatarResult(code);
	}

	void CSGClient::QueryPlayerResult(CSGClient* pClient, const SG_PLAYER& player, _U8 nSync)
	{
		SG_PLAYER player_tmp = m_player;

		m_player = player;
		if(SGClientUtil::DiffPlayerInfo(player_tmp, player)
			&& !SGClientUtil::IsEmptyPlayerInfo(m_player))
		{
			if(m_callback) 
			{
				m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
			}
		}
		
		//sync waitting...
		if(nSync) 
		{
			g_syncDataManager.ReceiveRequest(Zion::CSGSyncDataManager::eSyncPlayer);
			return;
		}

		if(m_callback) m_callback->QueryPlayerResult(m_player);
	}

	void CSGClient::QueryGeneralResult(CSGClient* pClient, const SG_GENERAL* generals, _U32 count, _U8 nSync)
	{
		Zion::Array<SG_GENERAL> generals_new;
		SGClientUtil::GenerateTempNewVec<SG_GENERAL>(generals, count, generals_new);

		//if(SGClientUtil::DiffGenerals(m_generals, generals_new))
		//&& !SGClientUtil::IsEmptyGeneral(m_generals))
		//{}
			
		m_generals.clear();
		SGClientUtil::GenerateTempNewVec<SG_GENERAL>(generals, count, m_generals);

		if(m_callback) 
		{
			m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncGenerals);
		}

		//sync waitting...
		if(nSync)
		{
			g_syncDataManager.ReceiveRequest(Zion::CSGSyncDataManager::eSyncGenerals);
			return;
		}
			
		if(m_callback) m_callback->QueryGeneralsDone(m_generals);
	}

	void CSGClient::QuerySoldierResult(CSGClient* pClient, const SG_SOLDIER* soldiers, _U32 count, _U8 nSync)
	{
		Zion::Array<SG_SOLDIER> soldiers_new;
		SGClientUtil::GenerateTempNewVec<SG_SOLDIER>(soldiers, count, soldiers_new);

		if(SGClientUtil::DiffSoldiers(m_soldiers, soldiers_new))
			//&& !SGClientUtil::IsEmptySoldier(m_soldiers))
		{
			m_soldiers.clear();
			SGClientUtil::GenerateTempNewVec<SG_SOLDIER>(soldiers, count, m_soldiers);
			
			if(m_callback) 
			{
				m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncSoldiers);
			}
		}

		if(nSync)
		{
			g_syncDataManager.ReceiveRequest(Zion::CSGSyncDataManager::eSyncSoldiers);
			return;
		}

		if(m_callback) m_callback->QuerySoldiersDone(m_soldiers);
	}

	void CSGClient::QueryBagBegin(CSGClient* pClient)
	{
		if(g_syncDataManager.ReceiveRequest(Zion::CSGSyncDataManager::eSyncBagBegin))
			return;
	}

	void CSGClient::QueryBagEquipt(CSGClient* pClient, const SG_EQUIPT_ITEM* items, _U32 count)
	{
		Zion::Array<SG_EQUIPT_ITEM> equipts_new;
		SGClientUtil::GenerateTempNewVec<SG_EQUIPT_ITEM>(items, count, equipts_new);

		if(SGClientUtil::DiffEquipt(m_equipts, equipts_new))
			//&& !SGClientUtil::IsEmptyEquipt(m_equipts))
		{
			m_equipts.clear();
			SGClientUtil::GenerateTempNewVec<SG_EQUIPT_ITEM>(items, count, m_equipts);

			if(m_callback) 
			{
				m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncEquipt);
			}
		}

		if(g_syncDataManager.ReceiveRequest(Zion::CSGSyncDataManager::eSyncEquipt))
			return;
	}

	void CSGClient::QueryBagUsable(CSGClient* pClient, const SG_USABLE_ITEM* items, _U32 count)
	{
		Zion::Array<SG_USABLE_ITEM> usables_new;
		SGClientUtil::GenerateTempNewVec<SG_USABLE_ITEM>(items, count, usables_new);

		if(SGClientUtil::DiffUsable(m_usables, usables_new))
			//&& !SGClientUtil::IsEmptyUsable(m_usables))
		{
			m_usables.clear();
			SGClientUtil::GenerateTempNewVec<SG_USABLE_ITEM>(items, count, m_usables);

			if(m_callback) 
			{
				m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncUsable);
			}
		}


		if(g_syncDataManager.ReceiveRequest(Zion::CSGSyncDataManager::eSyncUsable))
			return;
	}

	void CSGClient::QueryBagGen(CSGClient* pClient, const SG_GEM_ITEM* items, _U32 count)
	{
		Zion::Array<SG_GEM_ITEM> gems_new;
		SGClientUtil::GenerateTempNewVec<SG_GEM_ITEM>(items, count, gems_new);

		if(SGClientUtil::DiffGem(m_gems, gems_new))
			//&& !SGClientUtil::IsEmptyGem(m_gems))
		{
			m_gems.clear();
			SGClientUtil::GenerateTempNewVec<SG_GEM_ITEM>(items, count, m_gems);

			if(m_callback) 
			{
				m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncGem);
			}
		}

		if(g_syncDataManager.ReceiveRequest(Zion::CSGSyncDataManager::eSyncGem))
			return;
	}

	void CSGClient::QueryBagMaterial(CSGClient* pClient, const SG_MATERIAL_ITEM* items, _U32 count)
	{
		Zion::Array<SG_MATERIAL_ITEM> new_materials;
		SGClientUtil::GenerateTempNewVec<SG_MATERIAL_ITEM>(items, count, new_materials);

		if(SGClientUtil::DiffMaterial(m_materials, new_materials))
			//&& !SGClientUtil::IsEmptyGem(m_gems))
		{
			m_materials.clear();
			SGClientUtil::GenerateTempNewVec<SG_MATERIAL_ITEM>(items, count, m_materials);
			if(m_callback) 
			{
				m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncMaterial);
			}
		}

		if(g_syncDataManager.ReceiveRequest(Zion::CSGSyncDataManager::eSyncMaterial))
			return;
	}

	void CSGClient::QueryBagEnd(CSGClient* pClient, _U8 nSync)
	{
		if(nSync && g_syncDataManager.ReceiveRequest(Zion::CSGSyncDataManager::eSyncBagEnd))
		{
			return;
		}

		if(m_callback) m_callback->QueryBagDone(m_equipts, m_usables, m_gems, m_materials);
	}

	void CSGClient::QueryOtherPlayersResult(CSGClient* pClient, const SG_PLAYER_INFO* players, _U32 count)
	{
		if(m_callback) 
		{
			//to be optimize
			SG_PLAYER player_tmp_array[10];
			memset(player_tmp_array, 0, sizeof(SG_PLAYER)*10);

			for(_U32 i = 0; i < count; ++i)
			{
				player_tmp_array[i].nick		= players[i].nick;
				player_tmp_array[i].avatar_id	= players[i].avatar_id;
				player_tmp_array[i].general_id	= players[i].general_id;
				player_tmp_array[i].level		= players[i].level;
				player_tmp_array[i].title		= players[i].title;
				player_tmp_array[i].reputation	= players[i].reputation;
				player_tmp_array[i].rank		= players[i].rank;
				player_tmp_array[i].vip_level	= players[i].vip_level;
				player_tmp_array[i].league_id	= players[i].league_id;
				player_tmp_array[i].horse_level = players[i].horse_level;
			}

			m_callback->QueryOtherPlayersResult(player_tmp_array, count);
		}
	}

	void CSGClient::EnhanceEquiptResult(CSGClient* pClient, _U8 ret, _U32 gold, const SG_EQUIPT_ITEM& equip_item)
	{
		if(ret == SG_SERVER_RESULT_SUCC)
		{
			m_player.gold = gold;
			SGClientUtil::UpdateEquipt(m_equipts, equip_item);
			m_callback->DataUpdate(CSGSyncDataManager::eSyncPlayer);
		}
	}

	void CSGClient::ExtendEquiptResult(CSGClient* pClient, _U8 ret, const SG_EQUIPT_ITEM& equip_item, const A_UUID& reduce_uuid)
	{
		if(ret == SG_SERVER_RESULT_SUCC)
		{
			//help to sync data
			Zion::Array<_U8> vecSync;
			vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
			SyncSet(vecSync);

			SGClientUtil::UpdateEquiptTurboLevel(m_equipts, reduce_uuid, 0);
			SGClientUtil::UpdateEquipt(m_equipts, equip_item);

			m_callback->DataUpdate(CSGSyncDataManager::eSyncEquipt);
		}
	}

	void CSGClient::BeginBattleResult(CSGClient* pClient, const SG_PLAYER_PVE& PlayerPVE)
	{
		if(m_callback) m_callback->BeginBattleDone(PlayerPVE);
	}

	void CSGClient::EndBattleResult(CSGClient* pClient, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 wake_pt, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count)
	{
		SG_DROP_ITEM_BASE drop_lists[10];
		memset(drop_lists, 0, sizeof(drop_lists));
		
		for(_U32 i = 0; i < drop_count; ++i)
		{
			drop_lists[i].uuid = drops[i].uuid;
			drop_lists[i].name = drops[i].name;
			drop_lists[i].item_id = drops[i].item_id;
			drop_lists[i].count = drops[i].count;
		}

		if(m_callback)
		{
			if(m_player.level != level)
			{
				SGClientUtil::GetUnlockSoldierByLevel(m_player.level, level, g_newSoldiers.GetData(), m_soldiers);
				m_player.level = (_U16)level;
			}

			m_player.exp = exp;
			m_player.gold += gold;
			m_player.wake_pt += wake_pt;

			for(_U32 i = 0; i < drop_count; ++i)
			{
				g_newItemList.AddItem(drops[i].uuid);
			}

			m_callback->EndBattleDone(level, exp_addition, gold, wake_pt, drop_lists, drop_count);

			//help to sync data
			Zion::Array<_U8> vecSync;
			vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
			vecSync.push_back(CSGSyncDataManager::eSyncGenerals);
			vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
			SyncSet(vecSync);
		}
	}
	
	void CSGClient::EnhanceCoolDownResult(CSGClient* pClient, _U32 time)
	{
		if(m_callback)
		{
			m_callback->EnhanceCoolDownResult(time);
		}
	}

	void CSGClient::RefreshEquipDone(CSGClient* pClient, const SG_EQUIPT_ITEM& equipt)
	{
		for(Zion::Array<SG_EQUIPT_ITEM>::iterator it = m_equipts.begin(); it != m_equipts.end(); ++it)
		{
			if(it->uuid == equipt.uuid)
			{
				*it = equipt;
			}
		}

		if(m_callback)
		{
			m_callback->RefreshEquipDone(equipt);
		}

		//help to sync data
		Zion::Array<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		SyncSet(vecSync);
	}
	
	void CSGClient::GemCombineResult(CSGClient* pClient, const SG_GEM_ITEM& gem)
	{
		if(m_callback)
		{
			m_callback->GemCombineResult(gem);
		}
	}

	void CSGClient::HaloGetCoolDownResult(CSGClient* pClient, _U32 time)
	{
		if(m_callback)
		{
			m_callback->HaloGetCoolDownResult(time);
		}
	}

	void CSGClient::HaloIncreaseEXPResult(CSGClient* pClient, _U32 time)
	{
		if(m_callback)
		{
			m_callback->HaloIncreaseEXPResult(time);
		}
	}

	void CSGClient::HaloCoolDownResult(CSGClient* pClient, _U8 ret, _U32 consume_rmb)
	{
		if(m_callback)
		{
			if(ret == SG_SERVER_RESULT_SUCC)
			{
				m_player.rmb -= consume_rmb;
				m_callback->DataUpdate(CSGSyncDataManager::eSyncPlayer);
			}
		}
	}

	void CSGClient::QueryServerTimeResult(CSGClient* pClient, _U32 server_time)
	{
		time_t t;
		time(&t);
		m_nServerTimeDelta = (int)(server_time - (_U32)t);
		if(g_syncDataManager.ReceiveRequest(Zion::CSGSyncDataManager::eSyncServerTime))
			return;
	}

	void CSGClient::QueryPlayerQuestResult(CSGClient* pClient, const SG_QUEST_LIVE_INFO* quest_list, _U32 count, _U8 nSync)
	{
		m_quests.clear();
		for(_U32 i = 0; i < count; ++i)
		{
			m_quests.push_back(quest_list[i]);		
		}
		
		//sync ...
		if(nSync && g_syncDataManager.ReceiveRequest(Zion::CSGSyncDataManager::eSyncPlayerQuest))
			return;

		if(m_callback)
		{
			m_callback->QueryPlayerQuestResult(quest_list, count);
		}
	}
	
	void CSGClient::FinishQuestDone(CSGClient* pClient, _U8 ret, _U32 quest_id, _U32 exp_addition, _U32 exp, _U32 level, _U32 gold, _U32 rmb, _U32 reputation, _U32 energy, const SG_DROP_ITEM_BASE* drops, _U32 drop_count, _U32 consume_rmb)
	{
		if(ret == SG_SERVER_RESULT_SUCC)
		{
			for(Zion::Array<SG_QUEST_LIVE_INFO>::iterator it = m_quests.begin(); it != m_quests.end(); ++it)
			{
				SG_QUEST_LIVE_INFO& quest_item = (*it);
				if(quest_item.quest_id == quest_id
					&&ret == SG_SERVER_RESULT_SUCC)
				{
					quest_item.status = SG_QUEST_STATUS_FINISHED;
					break;
				}
			}
			
			if(m_player.level != level)
			{
				SGClientUtil::GetUnlockSoldierByLevel(m_player.level, level, g_newSoldiers.GetData(), m_soldiers);
				m_player.level = (_U16)level;
			}
			
			m_player.exp = exp;
			m_player.gold += gold;
			m_player.rmb += rmb;
			m_player.rmb -= consume_rmb;

			m_player.reputation += reputation;

			for(_U32 i = 0; i < drop_count; ++i)
			{
				g_newItemList.AddItem(drops[i].uuid);
			}
						
			//help to sync data
			Zion::Array<_U8> vecSync;
			vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
			SyncSet(vecSync);
		}

		if(m_callback)
		{
			m_callback->FinishQuestDone(ret, quest_id, exp_addition, gold, rmb, reputation, energy, drops, drop_count);
			m_callback->DataUpdate(CSGSyncDataManager::eSyncPlayer);
		}
	}

	void CSGClient::QueryPlayerPVPInfoResult(CSGClient* pClient, const SG_PLAYER_PVE& pve)
	{
		if(m_callback)
		{
			m_callback->QueryPlayerPVPInfoResult(pve);
		}
	}

	void CSGClient::QueryPlayerRankListResult(CSGClient* pClient, SG_PVP_RANK_PLAYER* players, _U32 count)
	{
		if(m_callback)
		{
			m_callback->QueryPlayerRankListResult(players, count);
		}
	}

	void CSGClient::BuyGoodsResult(CSGClient* pClient, _U8 ret, const A_UUID* goods, _U32* id_list, _U32 count)
	{
		for(_U32 i = 0; i < count; ++i)
		{
			g_newItemList.AddItem(goods[i]);
		}

		//help to sync data
		Zion::Array<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
		SyncSet(vecSync);

		if(m_callback)
		{
			m_callback->BuyGoodsResult(ret, id_list, count);
		}
	}

	void CSGClient::PVPCoolDownResult(CSGClient* pClient, _U32 time)
	{
		if(m_callback)
		{
			//help to sync data
			Zion::Array<_U8> vecSync;
			vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
			SyncSet(vecSync);

			m_callback->PVPCoolDownResult(time);
		}
	}

	void CSGClient::PVPGetRestTimeResult(CSGClient* pClient, _U32 rest_time)
	{
		if(m_callback)
		{
			m_callback->PVPGetRestTimeResult(rest_time);
		}
	}
	
	void CSGClient::PVPRecordResult(CSGClient* pClient, const SG_PVP_RECORD_ITEM* records, _U32 count)
	{
		if(m_callback)
		{
			m_callback->PVPRecordResult(records, count);
		}
	}

	void CSGClient::PVPHeroListRecord(CSGClient* pClient, const SG_PVP_HEROLIST_PLAYER* players, _U32 count)
	{
		if(m_callback)
		{
			m_callback->PVPHeroListRecord(players, count);
		}
	}
	
	void CSGClient::PVPDailyReward(CSGClient* pClient, _U32 gold, _U32 reputation, const SG_ITEM* items, _U32 count)
	{
		if(m_callback)
		{
			m_player.gold += gold;
			m_player.reputation += reputation;

			m_callback->PVPDailyReward(gold, reputation, items, count);

			for(_U32 i = 0; i < count; ++i)
			{
				g_newItemList.AddItem(items->uuid);
			}

		}
	}

	void CSGClient::PVPBattleBeginResult(CSGClient* pClient, const SG_PLAYER_PVE& SelfPVE, const SG_PLAYER_PVE& DefenderPVE)
	{
		if(m_callback)
		{
			m_callback->PVPBattleBeginResult(SelfPVE, DefenderPVE);
		}
	}
	void CSGClient::PVPBattleEndResult(CSGClient* pClient, _U32 reputation)
	{
		if(m_callback)
		{
			if(reputation)
			{
				m_player.reputation += reputation;
			}

			m_callback->PVPBattleEndResult(reputation);

			//sync player
			Zion::Array<_U8> vecSync;
			vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
			SyncSet(vecSync);
		}
	}

	void CSGClient::PVPCoolDownClearResult(CSGClient* pClient, _U8 ret, _U32 cost)
	{
		if(m_callback)
		{
			if(ret == SG_SERVER_RESULT_SUCC)
			{
				m_player.rmb -= cost;
				m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
			}

			m_callback->PVPCoolDownClearResult();
		}
	}

	void CSGClient::PVPIncreateBattleTimesResult(CSGClient* pClient, _U32 rest_times)
	{
		if(m_callback)
		{
			//sync player
			Zion::Array<_U8> vecSync;
			vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
			SyncSet(vecSync);

			m_callback->PVPIncreateBattleTimesResult(rest_times);
		}
	}

	void CSGClient::QueryInstanceResult(CSGClient* pClient, const SG_INSTANCE_INFO* instances, _U32 count)
	{
		if(m_callback)
		{
			m_callback->QueryInstanceResult(instances, count);
		}
	}

	void CSGClient::BeginInstanceBattleResult(CSGClient* pClient, const SG_PLAYER_PVE& PlayerPVE)
	{
		if(m_callback)
		{
			m_callback->BeginInstanceBattleResult(PlayerPVE);
		}
	}

	void CSGClient::EnterInstanceResult(CSGClient* pClient, const SG_INSTANCE_INFO& instance)
	{
		if(m_callback)
		{
			for(size_t i = 0; i < m_instances.size(); ++i)
			{
				if(instance.instance_id == m_instances[i].instance_id)
				{
					m_instances[i] = instance;
					break;
				}
			}

			m_callback->EnterInstanceResult(instance);
		}
	}

	void CSGClient::EndInstanceBattleResult(CSGClient* pClient, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 wake_pt, _U8 result, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count)
	{
		if(m_callback)
		{
			if(m_player.level != level)
			{
				SGClientUtil::GetUnlockSoldierByLevel(m_player.level, level, g_newSoldiers.GetData(), m_soldiers);
				m_player.level = (_U16)level;
			}

			m_player.exp = exp;
			m_player.gold += gold;
			m_player.wake_pt += wake_pt;

			for(_U32 i = 0; i < drop_count; ++i)
			{
				g_newItemList.AddItem(drops[i].uuid);
			}
			
			m_callback->EndInstanceBattleResult(level, exp_addition, gold, wake_pt, result, drops, drop_count);

			//help to sync data
			Zion::Array<_U8> vecSync;
			vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
			vecSync.push_back(CSGSyncDataManager::eSyncGenerals);
			vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
			SyncSet(vecSync);
		}
	}

	void CSGClient::ResetInstanceResult(CSGClient* pClient, _U8 result, _U32 rmb, const SG_INSTANCE_INFO& instance)
	{
		if(m_callback)
		{
			for(size_t i = 0; i < m_instances.size(); ++i)
			{
				if(instance.instance_id == m_instances[i].instance_id)
				{
					m_instances[i] = instance;
					break;
				}
			}

			m_player.rmb -= rmb;

			m_callback->ResetInstanceResult(result, rmb, instance);
		}
	}

	void CSGClient::CreateLeagueResult(CSGClient* pClient, _U8 ret, const SG_LEAGUE& league)
	{
		if(m_callback)
		{
			if(ret == SG_LEAGUE_CREATE_SUCC)
			{
				m_player.league_id = league.league_id;
			}

			m_callback->CreateLeagueResult(ret, league);
		}
	}

	void CSGClient::QueryLeagueApplyListResult(CSGClient* pClient, const SG_LEAGUE_APPLYER* applyers, _U32 count)
	{
		if(m_callback)
		{
			//SGClientUtil::GenerateTempNewVec<SG_LEAGUE_APPLYER>(applyers, count, g_newApplyers.GetData());
			Zion::Array<_U32>& applyerVec = g_newApplyers.GetData();
			g_newApplyers.Resize(count);

			for(_U32 i = 0; i < count; ++i)
			{
				applyerVec[i] = applyers[i].applyer_id;
			}

			m_callback->QueryLeagueApplyListResult(applyers, count);
		}
	}

	void CSGClient::QueryLeagueResult(CSGClient* pClient, const SG_LEAGUE& league)
	{
		if(m_callback)
		{
			m_callback->QueryLeagueResult(league);
		}
	}

	void CSGClient::QueryLeagueListResult(CSGClient* pClient, const SG_LEAGUE* league_list, _U32 count)
	{
		if(m_callback)
		{
			m_callback->QueryLeagueListResult(league_list, count);
		}
	}

	void CSGClient::QueryLeagueMemberListResult(CSGClient* pClient, const SG_LEAGUE_MEMBER* league_members, _U32 count)
	{
		if(m_callback)
		{
			m_callback->QueryLeagueMemberListResult(league_members, count);
		}
	}

	void CSGClient::QueryLeagueMemberInfoResult(CSGClient* pClient, const SG_LEAGUE_MEMBER& member_info)
	{
		if(m_callback)
		{
			m_callback->QueryLeagueMemberInfoResult(member_info);
		}
	}

	void CSGClient::ContributeLeagueResult(CSGClient* pClient, const SG_LEAGUE_MEMBER& self_info, const SG_LEAGUE& league_info)
	{
		if(m_callback)
		{
			//help to sync data
			Zion::Array<_U8> vecSync;
			vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
			SyncSet(vecSync);

			m_callback->ContributeLeagueResult(self_info, league_info);
		}
	}

	void CSGClient::HandleApplyResult(CSGClient* pClient, _U8 ret, const SG_LEAGUE_MEMBER& new_joiner)
	{
		if(m_callback)
		{
			m_callback->HandleApplyResult(ret, new_joiner);
		}
	}

	void CSGClient::QueryLeagueNoticeResult(CSGClient* pClient, const char* notice_content)
	{
		if(m_callback)
		{
			m_callback->QueryLeagueNoticeResult(notice_content);
		}
	}

	void CSGClient::SetLeagueNoticeResult(CSGClient* pClient, _U8 ret, const char* notice_content)
	{
		if(m_callback)
		{
			m_callback->SetLeagueNoticeResult(ret, notice_content);
		}
	}

	void CSGClient::SetLeagueOwnerResult(CSGClient* pClient, _U8 ret, _U32 member_id)
	{
		if(m_callback)
		{
			m_callback->SetLeagueOwnerResult(ret, member_id);
		}
	}

	void CSGClient::SetMemberPositionResult(CSGClient* pClient, _U8 ret, _U32 member_id, _U8 position)
	{
		if(m_callback)
		{
			m_callback->SetMemberPositionResult(ret, member_id, position);
		}
	}

	void CSGClient::DismissMemberResult(CSGClient* pClient, _U8 ret, _U32 member_id)
	{
		if(m_callback)
		{
			m_callback->DismissMemberResult(ret, member_id);
		}
	}

	void CSGClient::ExitLeagueResult(CSGClient* pClient, _U8 ret)
	{
		if(m_callback)
		{
			if(!ret)
			{
				m_player.league_id = 0;
			}
			
			m_callback->ExitLeagueResult(ret);
		}
	}

	void CSGClient::QueryLeagueLogResult(CSGClient* pClient, const SG_LEAGUE_LOG* league_log, _U32 count)
	{
		if(m_callback)
		{
			m_callback->QueryLeagueLogResult(league_log, count);
		}
	}

	void CSGClient::LeagueToastResult(CSGClient* pClient, _U8 ret, _U32 gold, _U32 rmb, _U32 reward_reputation, _U32 reward_league_xp)
	{
		if(m_callback)
		{
			if(!ret)
			{
				m_player.rmb -= rmb;
				m_player.gold -= gold;
				m_player.reputation += reward_reputation;

				_U32 times = SGClientUtil::GetDailyActionTime(m_player, 3001);
				if(times > 0)
					SGClientUtil::SetDailyActionTimeInCache(m_player, 3001, times-1);

				m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
			}

			m_callback->LeagueToastResult(ret, gold, rmb, reward_reputation, reward_league_xp);
		}
	}

	void CSGClient::SalaryGetResult(CSGClient* pClient, _U8 ret, _U32 rmb, _U32 gold)
	{
		if(m_callback)
		{
			if(!ret)
			{
				m_player.rmb -= rmb;
				m_player.gold += gold;
			
				//help to sync data
				Zion::Array<_U8> vecSync;
				vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
				SyncSet(vecSync);
			}
			
			m_callback->SalaryGetResult(ret, rmb, gold);
		}
	}
	
	void CSGClient::SalaryGetBatResult(CSGClient* pClient, _U8 ret, _U32 rmb, _U32 gold, _U32 times)
	{
		if(m_callback)
		{
			if(!ret)
			{
				m_player.rmb -= rmb;
				m_player.gold += gold;

				//help to sync data
				Zion::Array<_U8> vecSync;
				vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
				SyncSet(vecSync);
			}

			m_callback->SalaryGetBatResult(ret, rmb, gold, times);
		}
	}

	void CSGClient::BuyEnergyResult(CSGClient* pClient, _U8 ret, _U32 rmb, _U32 energy, _U32 times)
	{
		if(m_callback)
		{
			if(!ret)
			{
				m_player.rmb -= rmb;
				m_player.energy += energy;

				_U32 times = SGClientUtil::GetDailyActionTime(m_player, 1007);
				SGClientUtil::SetDailyActionTimeInCache(m_player, 1007, times+1);
			}

			m_callback->BuyEnergyResult(ret, rmb, energy, times);
			m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
		}
	}

	void CSGClient::EnhanceTurboResult(CSGClient* pClient, _U8 ret, _U32 turbo_level,  _U32 wake_pt)
	{
		if(m_callback)
		{
			if(ret == SG_SERVER_RESULT_SUCC)
			{
				m_player.turbo_level = turbo_level;
				m_player.wake_pt = wake_pt;

				Zion::String skill_archetype;
				SGClientUtil::GetUnlockTurboSkill(m_player.general_id, turbo_level, skill_archetype);

				if(!skill_archetype.empty())
				{
					DDL::String<ARCHETYPE_URL_LENGTH_MAX> unlock_archetype;
					unlock_archetype.Assign(skill_archetype.c_str());

					int nCount = m_player.skills._Count;
					m_player.skills.Resize(nCount + 1);
					m_player.skills._Array[nCount] = unlock_archetype;

				}
			}

			m_callback->EnhanceTurboResult(ret, turbo_level, wake_pt);
			m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
		}
	}

	void CSGClient::MakeEquiptResult(CSGClient* pClient, _U8 ret, const SG_EQUIPT_ITEM& new_euqipt, const SG_MATERIAL_ITEM& com_material, const SG_MATERIAL_ITEM& key_material)
	{
		if(m_callback)
		{
			if(!ret)
			{
				g_newItemList.AddItem(new_euqipt.uuid);

				//help to sync data
				Zion::Array<_U8> vecSync;
				vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
				vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
				SyncSet(vecSync);
			}

			m_callback->MakeEquiptResult(ret, new_euqipt, com_material, key_material);
		}
	}

	void CSGClient::QueryActionAvailableResult(CSGClient* pClient, _U32* action_list, _U32* available_list, _U32 count)
	{
		if(m_callback)
		{
			Zion::Array<_U8> statusVec;
			Zion::Array<_U32> actionVec;

			SGClientUtil::GenerateTempNewVec(action_list, count, actionVec);
			g_actionStatusCache.GetDailActionStatus(GetServerTime(), actionVec, statusVec);

			m_callback->QueryActionStatusResult(action_list, statusVec, available_list, count);
		}
	}
	
	void CSGClient::QueryFriendListResult(CSGClient* pClient, const SG_FRIEND_INFO* friend_list, _U32 count)
	{
		if(m_callback)
		{
			m_callback->QueryFriendListResult(friend_list, count);
		}
	}

	void CSGClient::QueryFriendListSearchResult(CSGClient* pClient, const SG_FRIEND_SEARCH_INFO* search_list, _U32 count)
	{
		if(m_callback)
		{
			m_callback->QueryFriendListSearchResult(search_list, count);
		}
	}

	void CSGClient::QueryFriendInvitationListResult(CSGClient* pClient, const SG_FRIEND_INVITATION* invitation_list, _U32 count)
	{
		if(m_callback)
		{
			m_callback->QueryFriendInvitationListResult(invitation_list, count);
		}
	}

	void CSGClient::AcceptFriendResult(CSGClient* pClient, _U8 ret, _U32 avatar_id)
	{
		if(m_callback)
		{
			m_callback->AcceptFriendResult(ret, avatar_id);
		}
	}

	void CSGClient::SendChatResult(CSGClient* pClient, const _U8 ret)
	{
		if(m_callback)
		{
			m_callback->SendChatResult(ret);
		}
	}
	
	void CSGClient::ReceiveChatResult(CSGClient* pClient, const SG_CHAT_RECV_INFO* chat_list, _U32 count)
	{
		if(m_callback)
		{
			m_callback->ReceiveChatResult(chat_list, count);
		}
	}

	void CSGClient::QueryLeagueDianjiangListResult(CSGClient* pClient, _U8 ret, const SG_LEAGUE_DIANJIANG_JOINER* joiner_list, _U32 count)
	{
		if(m_callback)
		{
			m_callback->QueryLeagueDianjiangListResult(ret, joiner_list, count);
		}
	}

	void CSGClient::JoinLeagueDianjiangResult(CSGClient* pClient, _U8 ret, const SG_LEAGUE_DIANJIANG_JOINER& self)
	{
		if(m_callback)
		{
			m_callback->JoinLeagueDianjiangResult(ret, self);
		}
	}

	void CSGClient::QueryLeagueDianjiangRewardResult(CSGClient* pClient, _U8 ret, _U32 reputation)
	{
		if(m_callback)
		{
			if(ret)
			{
				m_player.reputation += reputation;
			
				//help to sync data
				Zion::Array<_U8> vecSync;
				vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
				SyncSet(vecSync);
			}

			m_callback->QueryLeagueDianjiangRewardResult(ret, reputation);
		}
	}

	void CSGClient::LeagueDianjiangSummonNPCResult(CSGClient* pClient, _U8 ret, _U32 gold, _U32 rmb, const SG_LEAGUE_DIANJIANG_JOINER& npc_joiner)
	{
		if(m_callback)
		{
			if(ret)
			{
				m_player.gold -= gold;
				m_player.rmb -= rmb;
			}

			m_callback->LeagueDianjiangSummonNPCResult(ret, gold, rmb, npc_joiner);
			
			if(ret)
			{
				m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
			}
		}
	}

	void CSGClient::QueryAstrologyBagResult(CSGClient* pClient, const _U32* bag_list, _U32 count1, const _U32* tmp_bag_list, _U32 count2)
	{
		if(m_callback)
		{
			m_callback->QueryAstrologyBagResult(bag_list, count1, tmp_bag_list, count2);
		}
	}

	void CSGClient::SaveToBagResult(CSGClient* pClient, _U8 ret, _U32 item_id)
	{
		if(m_callback)
		{
			m_callback->SaveToBagResult(ret, item_id);
		}
	}

	void CSGClient::SetAstrologyBallStatusResult(CSGClient* pClient, _U8 ret, const SG_GENERAL& general)
	{
		if(m_callback)
		{
			if(!ret)
			{
				SGClientUtil::UpdateGeneralSoulBall(m_player, m_generals, general);
			}
			
			m_callback->SetAstrologyBallStatusResult(ret, general);
		}
	}

	void CSGClient::EnhanceAstrologyBallResult(CSGClient* pClient, _U8 ret, _U32 gold, _U32 ball_id, _U32 new_ball_id)
	{
		if(m_callback)
		{
			if(!ret)
			{
				//help to sync data
				Zion::Array<_U8> vecSync;
				vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
				SyncSet(vecSync);
			}

			m_callback->EnhanceAstrologyBallResult(ret, gold, ball_id, new_ball_id);
		}
	}

	//void CSGClient::BuyAstrologyBallResult(CSGClient* pClient, _U8 ret, _U32 astrology_value, _U32 ball_id)
	//{
	//	if(m_callback)
	//	{
	//		m_callback->BuyAstrologyBallResult(ret, astrology_value, ball_id);
	//	}
	//}

	void CSGClient::StrologyResult(CSGClient* pClient, _U8 ret, _U32 gold, _U32 ball_id, _U32 astrologer_id, _U32 last_astrologer_id)
	{
		if(m_callback)
		{
			if(!ret)
			{
				m_player.current_zhanxing_value = astrologer_id;
				m_player.gold -= gold;
				m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
			}
			m_callback->StrologyResult(ret, gold, ball_id, astrologer_id, last_astrologer_id);
		}
	}

	void CSGClient::StrologyAutoResult(CSGClient* pClient, _U8 ret, _U32 gold, _U32* ball_list, _U32 count, _U32 astrologer_id)
	{
		if(m_callback)
		{
			if(!ret)
			{
				m_player.current_zhanxing_value = astrologer_id;
				m_player.gold -= gold;
				m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
			}

			m_callback->StrologyAutoResult(ret, gold, ball_list, count, astrologer_id);
		}
	}

	void CSGClient::DevourResult(CSGClient* pClient, _U8 ret, _U8 bag_type, _U32* ball_list, _U32 count, _U32* devoured_list, _U32 count2)
	{
		if(m_callback)
		{
			//help to sync data
			Zion::Array<_U8> vecSync;
			vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
			SyncSet(vecSync);

			m_callback->DevourResult(ret, bag_type, ball_list, count, devoured_list, count2);
		}
	}

	void CSGClient::UseItemResult(CSGClient* pClient, _U8 ret, const A_UUID& uuid, _U32 count, _U32 target_id, const SG_PLAYER& player_info, const SG_GENERAL& general, const SG_ITEM* drops, const _U32 drop_count)
	{
		if(m_callback)
		{
			if(ret == SG_SERVER_RESULT_SUCC)
			{
				for(_U32 i = 0; i < drop_count; ++i)
				{
					const SG_ITEM& item = drops[i];

					if( drops[i].item_id >= SG_ITEM_USABLE_ID_START && drops[i].item_id <= SG_ITEM_USABLE_ID_END)
					{
						SGClientUtil::AddItem<SG_USABLE_ITEM, SG_USABLE_ITEM_CONFIG>(m_usables, item);
					}

					if( drops[i].item_id >= SG_ITEM_MATERIAL_ID_START && drops[i].item_id <= SG_ITEM_MATERIAL_ID_END)
					{
						SGClientUtil::AddItem<SG_MATERIAL_ITEM, SG_MATERIAL_CONFIG>(m_materials, item);
					}

					if( drops[i].item_id >= SG_ITEM_GEM_ID_START && drops[i].item_id <= SG_ITEM_GEM_ID_END)
					{
						SGClientUtil::AddItem<SG_GEM_ITEM, SG_GEM_ITEM_CONFIG>(m_gems, item);
					}

					if( drops[i].item_id >= SG_ITEM_EQUIPT_ID_START && drops[i].item_id <= SG_ITEM_EQUIPT_ID_END)
					{
						assert(0);
					}

					if( drops[i].item_id >= SG_ITEM_ASTOLOGY_BALL_ID_START && drops[i].item_id <= SG_ITEM_ASTOLOGY_BALL_ID_END)
					{
						assert(0);
					}
				}

				if(player_info.exp || player_info.level)
				{
					m_player.exp = player_info.exp;
					m_player.level = player_info.level;
				}

				if(player_info.gold)
				{
					m_player.gold += player_info.gold;
				}

				if(player_info.rmb)
				{
					m_player.rmb += player_info.rmb;
				}

				if(general.exp || general.level)
				{
					for(Zion::Array<SG_GENERAL>::iterator it = m_generals.begin(); it != m_generals.end(); ++it)
					{
						if((*it).general_id == general.general_id)
						{
							(*it).exp = general.exp;
							(*it).level = general.level;
						}
					}
				}

				//help to sync data no need
				Zion::Array<_U8> vecSync;
				vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
				SyncSet(vecSync);
			}

			m_callback->UseItemResult(ret, uuid, count, target_id, player_info, general, drops, drop_count);
		}
	}

	void CSGClient::FeedHorseResult(CSGClient* pClient, _U8 ret, _U32 xp, _U32 xp_add, _U32 level, _U8 xp_add_type, _U8 feed_type, _U32 gold, _U32 rmb)
	{
		if(m_callback)
		{
			if(!ret)
			{
				//xp_add = SGClientUtil::GetHorseXpAdd(m_player.horse_level, level, m_player.horse_exp, xp);
				m_player.horse_exp = xp;
				m_player.horse_level = level;
				m_player.gold = gold;
				m_player.rmb = rmb;

				m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
			}

			m_callback->FeedHorseResult(ret, xp, xp_add, level, xp_add_type, feed_type);
		}
	}

	void CSGClient::QueryBossRushInfoResult(CSGClient* pClient, const SG_BOSSRUSH_INFO& bossrush_info)
	{
		if(m_callback)
		{
			m_callback->QueryBossRushInfoResult(bossrush_info);
		}
	}

	void CSGClient::QueryBossRushSupportInfoResult(CSGClient* pClient, const SG_BOSSRUSH_SUPPORT_INFO* support_list, _U32 count)
	{
		if(m_callback)
		{
			m_callback->QueryBossRushSupportInfoResult(support_list, count);
		}
	}

	void CSGClient::BeginBossRushBattleResult(CSGClient* pClient, const SG_PLAYER_PVE& PlayerPVE)
	{
		if(m_callback)
		{
			_U32 times = SGClientUtil::GetDailyActionTime(m_player, 2002);
			if(times > 0)
					SGClientUtil::SetDailyActionTimeInCache(m_player, 2002, times-1);

			m_callback->BeginBossRushBattleResult(PlayerPVE);
		}
	}

	void CSGClient::EndBossRushBattleResult(CSGClient* pClient, _U8 ret, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 reputation, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count)
	{
		if(m_callback)
		{
			if(ret == SG_SERVER_RESULT_SUCC)
			{
				SG_DROP_ITEM_BASE drop_lists[10];
				memset(drop_lists, 0, sizeof(drop_lists));

				for(_U32 i = 0; i < drop_count; ++i)
				{
					drop_lists[i].uuid = drops[i].uuid;
					drop_lists[i].name = drops[i].name;
					drop_lists[i].item_id = drops[i].item_id;
					drop_lists[i].count = drops[i].count;
				}

				if(m_player.level != level)
				{
					SGClientUtil::GetUnlockSoldierByLevel(m_player.level, level, g_newSoldiers.GetData(), m_soldiers);
					m_player.level = (_U16)level;
				}

				m_player.exp = exp;
				m_player.gold += gold;
				m_player.reputation += reputation;

				for(_U32 i = 0; i < drop_count; ++i)
				{
					g_newItemList.AddItem(drops[i].uuid);
				}

				//help to sync data
				Zion::Array<_U8> vecSync;
				vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
				vecSync.push_back(CSGSyncDataManager::eSyncGenerals);
				vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
				SyncSet(vecSync);
			}


			m_callback->EndBossRushBattleResult(ret, level, exp_addition, exp, gold, reputation, drops, drop_count);
		}
	}

	void CSGClient::BeginBossRushSupportBattleResult(CSGClient* pClient, _U8 ret, const SG_PLAYER_PVE& PlayerPVE)
	{
		if(m_callback)
		{
			if(ret == SG_SERVER_RESULT_SUCC)
			{
				_U32 times = SGClientUtil::GetDailyActionTime(m_player, 2002);
				if(times > 0)
					SGClientUtil::SetDailyActionTimeInCache(m_player, 2002, times-1);
			}

			m_callback->BeginBossRushSupportBattleResult(ret, PlayerPVE);
		}
	}

	void CSGClient::EndBossRushSupportBattleResult(CSGClient* pClient, _U8 ret, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 reputation, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count)
	{
		if(m_callback)
		{
			if(ret == SG_SERVER_RESULT_SUCC)
			{
				SG_DROP_ITEM_BASE drop_lists[10];
				memset(drop_lists, 0, sizeof(drop_lists));

				for(_U32 i = 0; i < drop_count; ++i)
				{
					drop_lists[i].uuid = drops[i].uuid;
					drop_lists[i].name = drops[i].name;
					drop_lists[i].item_id = drops[i].item_id;
					drop_lists[i].count = drops[i].count;
				}

				if(m_player.level != level)
				{
					SGClientUtil::GetUnlockSoldierByLevel(m_player.level, level, g_newSoldiers.GetData(), m_soldiers);
					m_player.level = (_U16)level;
				}

				m_player.exp = exp;
				m_player.gold += gold;
				m_player.reputation += reputation;

				for(_U32 i = 0; i < drop_count; ++i)
				{
					g_newItemList.AddItem(drops[i].uuid);
				}

				//help to sync data
				Zion::Array<_U8> vecSync;
				vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
				vecSync.push_back(CSGSyncDataManager::eSyncGenerals);
				vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
				SyncSet(vecSync);
			}


			m_callback->EndBossRushSupportBattleResult(ret, level, exp_addition, exp, gold, reputation, drops, drop_count);
		}
	}

	void CSGClient::AddBossRushRemainingTimesResult(CSGClient* pClient, _U8 ret, _U32 rmb, _U8 remain_times)
	{
		if(m_callback)
		{
			if(ret == SG_SERVER_RESULT_SUCC)
			{
				m_player.rmb -= rmb;

				_U32 times = SGClientUtil::GetDailyActionTime(m_player, 2002);
				SGClientUtil::SetDailyActionTimeInCache(m_player, 2002, remain_times);
			}

			m_callback->AddBossRushRemainingTimesResult(ret, rmb, remain_times);
		}
	}

	void CSGClient::AwardBossRushResult(CSGClient* pClient, _U8 ret, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 reputation, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count)
	{
		if(m_callback)
		{
			if(ret == SG_SERVER_RESULT_SUCC)
			{
				SG_DROP_ITEM_BASE drop_lists[10];
				memset(drop_lists, 0, sizeof(drop_lists));

				for(_U32 i = 0; i < drop_count; ++i)
				{
					drop_lists[i].uuid = drops[i].uuid;
					drop_lists[i].name = drops[i].name;
					drop_lists[i].item_id = drops[i].item_id;
					drop_lists[i].count = drops[i].count;
				}

				if(m_player.level != level)
				{
					SGClientUtil::GetUnlockSoldierByLevel(m_player.level, level, g_newSoldiers.GetData(), m_soldiers);
					m_player.level = (_U16)level;
				}

				m_player.exp = exp;
				m_player.gold += gold;
				m_player.reputation += reputation;

				for(_U32 i = 0; i < drop_count; ++i)
				{
					g_newItemList.AddItem(drops[i].uuid);
				}

				//help to sync data
				Zion::Array<_U8> vecSync;
				vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
				vecSync.push_back(CSGSyncDataManager::eSyncGenerals);
				vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
				SyncSet(vecSync);
			}


			m_callback->AwardBossRushResult(ret, level, exp_addition, exp, gold, reputation, drops, drop_count);
		}
	}

	void CSGClient::AwardBossRushSupportResult(CSGClient* pClient, _U8 ret, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 reputation, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count)
	{
		if(m_callback)
		{
			if(ret == SG_SERVER_RESULT_SUCC)
			{
				SG_DROP_ITEM_BASE drop_lists[10];
				memset(drop_lists, 0, sizeof(drop_lists));

				for(_U32 i = 0; i < drop_count; ++i)
				{
					drop_lists[i].uuid = drops[i].uuid;
					drop_lists[i].name = drops[i].name;
					drop_lists[i].item_id = drops[i].item_id;
					drop_lists[i].count = drops[i].count;
				}

				if(m_player.level != level)
				{
					SGClientUtil::GetUnlockSoldierByLevel(m_player.level, level, g_newSoldiers.GetData(), m_soldiers);
					m_player.level = (_U16)level;
				}

				m_player.exp = exp;
				m_player.gold += gold;
				m_player.reputation += reputation;

				for(_U32 i = 0; i < drop_count; ++i)
				{
					g_newItemList.AddItem(drops[i].uuid);
				}

				//help to sync data
				Zion::Array<_U8> vecSync;
				vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
				vecSync.push_back(CSGSyncDataManager::eSyncGenerals);
				vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
				SyncSet(vecSync);
			}

			m_callback->AwardBossRushSupportResult(ret, level, exp_addition, exp, gold, reputation, drops, drop_count);
		}
	}

	void CSGClient::QueryDiceNumResult(CSGClient* pClient, _U8 ret, _U32 dice_num, _U32 reward_time, _U32 energy, _U32 rmb, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count, _U32 circle_num, _U32 step)
	{
		if(m_callback)
		{
			if(ret == SG_SERVER_RESULT_SUCC)
			{
				m_player.xunbao_circle = circle_num;
				m_player.xunbao_step = step;

				_U32 nTimes = SGClientUtil::GetDailyActionTime(m_player, 9001);
				if(nTimes)
				{
					--nTimes;
				}
				nTimes += reward_time;

				SGClientUtil::SetDailyActionTimeInCache(m_player, 9001, nTimes);
				
				SG_DROP_ITEM_BASE drop_lists[10];
				memset(drop_lists, 0, sizeof(drop_lists));

				for(_U32 i = 0; i < drop_count; ++i)
				{
					drop_lists[i].uuid = drops[i].uuid;
					drop_lists[i].name = drops[i].name;
					drop_lists[i].item_id = drops[i].item_id;
					drop_lists[i].count = drops[i].count;
				}
				for(_U32 i = 0; i < drop_count; ++i)
				{
					g_newItemList.AddItem(drops[i].uuid);
				}

				if(drop_count)
				{
					//help to sync data
					Zion::Array<_U8> vecSync;
					vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
					SyncSet(vecSync);
				}

				m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
			}

			m_callback->QueryDiceNumResult(ret, dice_num, reward_time, energy, rmb, drops, drop_count, circle_num, step);
		}
	}

	void CSGClient::QueryNewcomerGuideInfoResult(CSGClient* pClient, const SG_NEWCOMER_GUIDE_INFO* guide_list, _U32 count)
	{
		if(m_callback)
		{
			m_callback->QueryNewcomerGuideInfoResult(guide_list, count);
		}
	}

	void CSGClient::QueryWorldBossRankInfoResult(CSGClient* pClient, const SG_WORLDBOSS_RANK_INFO& rank_info)
	{
		if(m_callback)
		{
			m_callback->QueryWorldBossRankInfoResult(rank_info);
		}
	}

	void CSGClient::BeginWorldBossBattleResult(CSGClient* pClient, _U8 ret, const SG_PLAYER_PVE& selfPve, const SG_WORLDBOSS_INFO& bossInfo, const SG_WORLDBOSS_RANK_INFO& ranklist, const SG_PLAYER_INFO* otherPlayers, _U32 count1, _U32 gold, const SG_DROP_ITEM_BASE* drops, _U32 count2, _U32 rmb)
	{
		if(m_callback)
		{
			if(ret == SG_SERVER_RESULT_SUCC)
			{
				if(gold)
				{
					m_player.gold += gold;
					m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
				}

				if(rmb)
				{
					m_player.rmb -= rmb;
					m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
				}

				if(count2)
				{
					//help to sync data
					Zion::Array<_U8> vecSync;
					vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
				}
			}

			m_callback->BeginWorldBossBattleResult(ret, selfPve, bossInfo, ranklist, otherPlayers, count1, gold, drops, count2);
		}
	}

	void CSGClient::InspireWorldBossBattleResult(CSGClient* pClient, _U8 ret, _U32 rmb)
	{
		if(m_callback)
		{
			//m_callback->InspireWorldBossBattleResult(ret);
		}
	}

	void CSGClient::AwardWorldBossRankResult(CSGClient* pClient, _U8 ret, _U32 gold, _U32 reputation, const SG_DROP_ITEM_BASE* drops, _U32 count)
	{
		if(m_callback)
		{
			if(ret == SG_SERVER_RESULT_SUCC)
			{
				if(gold)
				{
					m_player.gold += gold;
					m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
				}

				if(reputation)
				{
					m_player.reputation += reputation;
					m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
				}

				if(count)
				{
					//help to sync data
					Zion::Array<_U8> vecSync;
					vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
				}
			}

			m_callback->AwardWorldBossRankResult(ret, gold, reputation, drops, count);
		}
	}

	void CSGClient::UpdateWorldBossBattleResult(CSGClient* pClient, const SG_WORLDBOSS_INFO& bossInfo, const SG_WORLDBOSS_RANK_INFO& rankinfo)
	{
		if(m_callback)
		{
			m_callback->UpdateWorldBossBattleResult(bossInfo, rankinfo);
		}
	}

	void CSGClient::EndWorldBossBattleResult(CSGClient* pClient)
	{
		if(m_callback)
		{
			m_callback->EndWorldBossBattleResult();
		}
	}

	void CSGClient::AwardWorldBossAttendanceResult(CSGClient* pClient, _U8 ret, _U32 gold, _U32 reputation, const SG_DROP_ITEM_BASE* drops, _U32 count)
	{
		if(m_callback)
		{
			if(ret == SG_SERVER_RESULT_SUCC)
			{
				if(gold)
				{
					m_player.gold += gold;
					m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
				}

				if(reputation)
				{
					m_player.reputation += reputation;
					m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
				}

				if(count)
				{
					//help to sync data
					Zion::Array<_U8> vecSync;
					vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
				}
			}

			m_callback->AwardWorldBossAttendanceResult(ret, gold, reputation, drops, count);
		}
	}

	void CSGClient::RechargeResult(CSGClient* pClient, _U8 ret, _U32 rmb, const SG_PLAYER& selfplayer)
	{
		if(m_callback)
		{
			if(ret == SG_SERVER_RESULT_SUCC)
			{
				m_player = selfplayer;
				m_callback->DataUpdate(Zion::CSGSyncDataManager::eSyncPlayer);
			}

			m_callback->RechargeResult(ret, rmb);
		}	
	}

	void CSGClient::SellItemResult(CSGClient* pClient, _U8 result, const A_UUID& uuid, const _U32 item_id, const _U32 count)
	{
		if(m_callback)
		{
			if(!result)
			{
				SGClientUtil::UpdateItemCountByUUID(this, uuid, count);
			}

			m_callback->SellItemResult(result, uuid, item_id, count);

			//help to sync data
			Zion::Array<_U8> vecSync;
			vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
			vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
			SyncSet(vecSync);
		}
	}

	void CSGClient::OnLoginDone()
	{
		CClient::OnLoginDone();
		
		m_lastServerID = SG_INVALID_SERVER_ID;
		GetServerList();
		//if(m_callback) m_callback->LoginResult(0);
	}
	
	void CSGClient::OnLoginFailed()
	{
		CClient::OnLoginFailed();
		if(m_callback) m_callback->LoginResult(1);
	}

	void CSGClient::OnDisconnected()
	{
		CClient::OnDisconnected();
		if(m_callback) m_callback->DisconnectNotify();
	}

	void CSGClient::Tick()
	{
		CClient::Tick();

		if(m_nConnectPingTime == 0) 
			return;

		time_t tv;
		time(&tv);
		if((_U64)tv - m_nConnectPingTime > SG_CLIENT_PING_TIMEOUT)
		{
			m_nConnectPingTime = 0;
			if(m_callback) m_callback->NetworkTestResult(false);
		}

		//action poll
		if(ms_nLastRanderTime == 0)
		{
			if(m_nServerTimeDelta != 0)
			{
				ms_nLastRanderTime = GetServerTime();
			}
			else
			{
				return;
			}
		}

		if(GetServerTime() - ms_nLastRanderTime >= SG_CLIENT_EVENT_POLL_TIMEOUT)
		{
			//GetDailyAction
			Zion::Array<_U32> actionVec;
			Zion::Array<_U8>	notifyTypeVec;
			g_actionStatusCache.GetDailyActionEvent(GetServerTime(), actionVec, notifyTypeVec);

			for(size_t i = 0; i < actionVec.size(); ++i)
			{
				if(m_callback)
				{
					m_callback->NotifyAction(actionVec[i], notifyTypeVec[i]);
				}
			}

			ms_nLastRanderTime = GetServerTime();
		}
	}

	const Zion::Array<SG_GENERAL>& CSGClient::GetGenerals()
	{
		return m_generals;
	}

	const Zion::Array<SG_SOLDIER>& CSGClient::GetSoldiers()
	{
		return m_soldiers;
	}

	const _U32 CSGClient::GetLastServerID()
	{
		return m_lastServerID;
	}

	const SG_PLAYER& CSGClient::GetPlayerInfo()
	{
		return m_player;
	}

	const Zion::Array<SG_EQUIPT_ITEM>& CSGClient::GetEquiptItem()
	{
		return m_equipts;
	}
	
	const Zion::Array<SG_GEM_ITEM>& CSGClient::GetGemItem()
	{
		return m_gems;
	}

	const Zion::Array<SG_USABLE_ITEM>& CSGClient::GetUsableItem()
	{
		return m_usables;
	}

	const Zion::Array<SG_MATERIAL_ITEM>& CSGClient::GetMaterialItem()
	{
		return m_materials;
	}

	const SG_SERVER_INFO& CSGClient::GetCurrentServerInfo()
	{
		for(size_t i = 0; i < m_serverList.size(); ++i)
		{
			if(m_serverList[i].server_id == m_lastServerID)
			{
				return m_serverList[i];
			}
		}
		
		//ZION_ASSERT(0);
		return m_serverList[0];
	}

	const int CSGClient::GetServerTime()
	{
		time_t tv;
		time(&tv);
		return m_nServerTimeDelta + (int)tv;
	}

	SG_ITEM* CSGClient::GetItemByUUID(const A_UUID& uuid)
	{
		SG_ITEM* item = NULL;
		for(Zion::Array<SG_EQUIPT_ITEM>::iterator it = m_equipts.begin(); it != m_equipts.end(); ++it)
		{
			if((*it).uuid == uuid)
			{
				return &(*it);
			}
		}
		
		for(Zion::Array<SG_USABLE_ITEM>::iterator it = m_usables.begin(); it != m_usables.end(); ++it)
		{
			if((*it).uuid == uuid)
			{
				return &(*it);
			}
		}

		for(Zion::Array<SG_GEM_ITEM>::iterator it = m_gems.begin(); it != m_gems.end(); ++it)
		{
			if((*it).uuid == uuid)
			{
				return &(*it);
			}
		}		
		
		return NULL;
	}

	void CSGClient::GetFinishedQuest(Zion::Array<SG_QUEST_LIVE_INFO>& quest_vec)
	{
		for(Zion::Array<SG_QUEST_LIVE_INFO>::iterator it = m_quests.begin(); it != m_quests.end(); ++it)
		{
			if((*it).status == SG_QUEST_STATUS_FINISHED)
			{
				quest_vec.push_back(*it);
			}
		}
	}

	void CSGClient::GetNewSoldierList(Zion::Array<_U32>& soldier_lists)
	{
		g_newSoldiers.GetNewList(soldier_lists);
	}

	bool CSGClient::HasNewSoldier()
	{
		return g_newSoldiers.HasNewItem();
	}

	bool CSGClient::IsNewSoldier(_U32 soldier_id)
	{
		return g_newSoldiers.IsNewItem(soldier_id);
	}

	void CSGClient::ClearNewSoldierList()
	{
		g_newSoldiers.ClearNewList();
	}

	void CSGClient::GetNewItemList(Zion::Array<A_UUID>& item_lists)
	{
		g_newItemList.GetNewList(item_lists);
	}

	bool CSGClient::HasNewItem()
	{
		return g_newItemList.HasNewItem();
	}

	bool CSGClient::IsNewItem(const A_UUID& uuid)
	{
		return g_newItemList.IsNewItem(uuid);
	}

	void CSGClient::ClearNewItemList()
	{
		g_newItemList.ClearNewList();
	}

	void CSGClient::GetNewApplyerList(Zion::Array<_U32>& applyer_list)
	{
		g_newApplyers.GetNewList(applyer_list);
	}
	
	bool CSGClient::HasNewApplyer()
	{
		return g_newApplyers.HasNewItem();
	}

	bool CSGClient::IsNewApplyer(_U32 applyer_id)
	{
		return g_newApplyers.IsNewItem(applyer_id);
	}
	
	void CSGClient::ClearNewApplyerList()
	{
		return g_newApplyers.ClearNewList();
	}

	void CSGClient::SyncForInit()
	{
		g_syncDataManager.AddAllRequest();
		g_syncDataManager.SendSyncRequest();
	}

	void CSGClient::SyncSet(const Zion::Array<_U8> vecSync)
	{
		for(size_t i = 0; i < vecSync.size(); ++i)
		{
			g_syncDataManager.AddRequest(vecSync[i]);
		}

		g_syncDataManager.SendSyncRequest();
	}

	void CSGClient::SyncSuccNotify()
	{
		if(m_callback)
		{
			m_callback->SyncDone();
		}
	}
}
