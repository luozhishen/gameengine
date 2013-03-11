#include <AtlasBase.h>
#include <AtlasClient.h>
#include <AtlasCommon.h>

#include <time.h>
#include "SGCommon.h"
#include "SGClient.h"
#include "SGSyncDataManager.h"
#include "SGClientUtil.h"


namespace Atlas
{
	CSGClient::CSGClient(CClientApp* pClientApp, _U32 recvsize) : CClient(pClientApp, recvsize), m_C2S(this), m_S2C(this)
	{
		m_callback = NULL;
		m_nServerTimeDelta = 0;
		m_nConnectPingTime = 0;
		m_pSyncMgr = ATLAS_NEW Atlas::CSGSyncDataManager(this);
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

	void CSGClient::EquipItem(_U32 general_id, const SG_EQUIP_SLOTS& slots)
	{
		SG_EQUIP_SLOTS tidy_slots;
		for(Atlas::Vector<SG_EQUIPT_ITEM>::iterator it = m_equipts.begin(); it != m_equipts.end(); ++it)
		{
			const DDLReflect::STRUCT_INFO* struct_info = Atlas::ContentObject::GetType("SG_EQUIPT_ITEM_CONFIG");
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
			Atlas::Vector<A_UUID> content_list;
			if(!Atlas::ContentObject::GetList(struct_info, content_list, true))
			{
				continue;
			}

			for(Atlas::Vector<A_UUID>::iterator it_uuid = content_list.begin(); it_uuid != content_list.end(); ++it_uuid)
			{
				content_obj = Atlas::ContentObject::QueryByUUID(*it_uuid, struct_info);
				if(((SG_EQUIPT_ITEM_CONFIG*)content_obj)->item_id == it->item_id)
				{
					SGClientUtil::SetRightLocation(content_obj, tidy_slots, uuid);	
					break;
				}
			}
		}

		m_C2S.EquipItem(general_id, tidy_slots);
	}

	void CSGClient::EquipGenerals(const _U32* generals, _U32 count)
	{
		m_C2S.EquipGenerals(generals, count);
	}

	void CSGClient::EquipSoldiers(const _U32* soldiers, _U32 count)
	{
		m_C2S.EquipSoldiers(soldiers, count);
	}

	void CSGClient::UnLockSoldier(_U32 soldier_id)
	{
		m_C2S.UnLockSoldier(soldier_id);

		//help to sync data
		Atlas::Vector<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		vecSync.push_back(CSGSyncDataManager::eSyncSoldiers);
		SyncSet(vecSync);
	}

	void CSGClient::EnhanceSoldier(_U32 soldier_id)
	{
		m_C2S.EnhanceSoldier(soldier_id);

		//help to sync data
		Atlas::Vector<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		vecSync.push_back(CSGSyncDataManager::eSyncSoldiers);
		SyncSet(vecSync);
	}

	void CSGClient::EnhanceEquipt(A_UUID& uuid)
	{
		m_C2S.EnhanceEquipt(uuid);

		//help to sync data
		Atlas::Vector<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
		SyncSet(vecSync);
	}

	void CSGClient::ExtendEquipt(A_UUID& uuid, A_UUID& puuid)
	{
		m_C2S.ExtendEquipt(uuid, puuid);
		
		//help to sync data
		Atlas::Vector<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
		SyncSet(vecSync);
	}

	void CSGClient::EnhanceCoolDown()
	{
		m_C2S.EnhanceCoolDown();
	}

	void CSGClient::EnhanceCoolDownClear()
	{
		m_C2S.EnhanceCoolDownClear();
	}

	void CSGClient::IncreaseEquipCoolDown()
	{
		m_C2S.IncreaseEquipCoolDown();

		//help to sync data
		Atlas::Vector<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		SyncSet(vecSync);
	}

	void CSGClient::RefreshEquipNormal(A_UUID& uuid)
	{
		m_C2S.RefreshEquipNormal(uuid);
		//help to sync data
		Atlas::Vector<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
		SyncSet(vecSync);
	}
	
	void CSGClient::RefreshEquipProperty(A_UUID& uuid)
	{
		m_C2S.RefreshEquipProperty(uuid);
		//help to sync data
		Atlas::Vector<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
		SyncSet(vecSync);
	}

	void CSGClient::RefreshEquipAbility(A_UUID& uuid)
	{
		m_C2S.RefreshEquipAbility(uuid);
	}
	
	void CSGClient::RefreshEquipDecideAccept(A_UUID& uuid)
	{
		m_C2S.RefreshEquipDecideAccept(uuid);

		//help to sync data
		Atlas::Vector<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		vecSync.push_back(CSGSyncDataManager::eSyncBagBegin);
		SyncSet(vecSync);
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

	void CSGClient::EndBattle(const char* name, _U32 result)
	{
		m_C2S.EndBattle(name, result);
	}

	void CSGClient::QueryServerTime()
	{
		m_C2S.QueryServerTime();
	}

	void CSGClient::EquipGem(const A_UUID& item_uuid, _U32 gem_id)
	{
		m_C2S.EquipGem(item_uuid, gem_id);
	}

	void CSGClient::UnequipGem(const A_UUID& item_uuid, _U32 gem_id)
	{
		m_C2S.UnequipGem(item_uuid, gem_id);
	}

	void CSGClient::GemCombine(_U32 item_id)
	{
		m_C2S.GemCombine(item_id);

		//help to sync data
		Atlas::Vector<_U8> vecSync;
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
			Atlas::Vector<A_UUID> uuid_list;
			if(!ContentObject::GetList(struct_info, uuid_list, true))
			{
				return;
			}

			_U32 general_limit_num = 0;
			for(Atlas::Vector<A_UUID>::iterator it = uuid_list.begin(); it != uuid_list.end(); ++it)
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
		Atlas::Vector<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		vecSync.push_back(CSGSyncDataManager::eSyncGenerals);
		SyncSet(vecSync);
	}

	void CSGClient::QueryPlayerQuest(_U8 nSync)												
	{
		m_C2S.QueryPlayerQuest(nSync);
	}

	void CSGClient::SaveQuestData(const SG_QUEST_LIVE_INFO& quest_info)
	{
		Atlas::Vector<SG_QUEST_LIVE_INFO>::iterator it; 
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
	
	void CSGClient::FinishQuest(_U32 quest_id)
	{
		m_C2S.FinishQuest(quest_id);
	}

	void CSGClient::GetPaid()
	{
		m_C2S.GetPaid();
		
		Atlas::Vector<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		SyncSet(vecSync);
	}
	
	void CSGClient::UpgradeTitle()
	{
		m_C2S.UpgradeTitle();

		Atlas::Vector<_U8> vecSync;
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
	}

	void CSGClient::PVPBattle(_U32 defender, _U8 result)
	{
		m_C2S.PVPBattle(defender, result);
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

	void CSGClient::EndInstanceBattle(const char* map_url, _U32 result)
	{
		m_C2S.EndInstanceBattle(map_url, result);
	}

	void CSGClient::ResetInstance(_U32 instance_id)
	{
		m_C2S.ResetInstance(instance_id);
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
		if(m_callback && (m_lastServerID == SG_INVALID_SERVER_ID))
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
			return;
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
		
		static bool bQueryed = false;
		if(!bQueryed)
		{
			SyncForInit();
			bQueryed = true;
		}
	}

	void CSGClient::CreateAvatarResult(CSGClient* pClient, _U32 code)
	{
		if(m_callback) m_callback->CreateAvatarResult(code);
		if(code == 0)
		{
			SyncForInit();
		}
	}

	void CSGClient::QueryPlayerResult(CSGClient* pClient, const SG_PLAYER& player, _U8 nSync)
	{
		if(SGClientUtil::DiffPlayerInfo(m_player, player)
			&& !SGClientUtil::IsEmptyPlayerInfo(m_player))
		{
			if(m_callback) 
			{
				m_callback->DataUpdate(Atlas::CSGSyncDataManager::eSyncPlayer);
			}
		}

		m_player = player;
		
		//sync waitting...
		if(nSync) 
		{
			m_pSyncMgr->ReceiveRequest(Atlas::CSGSyncDataManager::eSyncPlayer);
			return;
		}

		if(m_callback) m_callback->QueryPlayerResult(m_player);
	}

	void CSGClient::QueryGeneralResult(CSGClient* pClient, const SG_GENERAL* generals, _U32 count, _U8 nSync)
	{
		Atlas::Vector<SG_GENERAL> generals_new;
		SGClientUtil::GenerateTempNewVec<SG_GENERAL>(generals, count, generals_new);

		if(SGClientUtil::DiffGenerals(m_generals, generals_new))
			//&& !SGClientUtil::IsEmptyGeneral(m_generals))
		{
			if(m_callback) 
			{
				m_callback->DataUpdate(Atlas::CSGSyncDataManager::eSyncGenerals);
			}
		}

		m_generals.clear();
		for(_U32 i = 0; i < count; ++i)
		{
			m_generals.push_back(generals[i]);
		}

		//sync waitting...
		if(nSync)
		{
			m_pSyncMgr->ReceiveRequest(Atlas::CSGSyncDataManager::eSyncGenerals);
			return;
		}
			
		if(m_callback) m_callback->QueryGeneralsDone(m_generals);
	}

	void CSGClient::QuerySoldierResult(CSGClient* pClient, const SG_SOLDIER* soldiers, _U32 count, _U8 nSync)
	{
		Atlas::Vector<SG_SOLDIER> soldiers_new;
		SGClientUtil::GenerateTempNewVec<SG_SOLDIER>(soldiers, count, soldiers_new);

		if(SGClientUtil::DiffSoldiers(m_soldiers, soldiers_new))
			//&& !SGClientUtil::IsEmptySoldier(m_soldiers))
		{
			if(m_callback) 
			{
				m_callback->DataUpdate(Atlas::CSGSyncDataManager::eSyncSoldiers);
			}
		}

		m_soldiers.clear();
		for(_U32 i = 0; i < count; ++i)
		{
			m_soldiers.push_back(soldiers[i]);
		}

		if(nSync)
		{
			m_pSyncMgr->ReceiveRequest(Atlas::CSGSyncDataManager::eSyncSoldiers);
			return;
		}

		if(m_callback) m_callback->QuerySoldiersDone(m_soldiers);
	}

	void CSGClient::QueryBagBegin(CSGClient* pClient)
	{
		if(m_pSyncMgr->ReceiveRequest(Atlas::CSGSyncDataManager::eSyncBagBegin))
			return;
	}

	void CSGClient::QueryBagEquipt(CSGClient* pClient, const SG_EQUIPT_ITEM* items, _U32 count)
	{
		Atlas::Vector<SG_EQUIPT_ITEM> equipts_new;
		SGClientUtil::GenerateTempNewVec<SG_EQUIPT_ITEM>(items, count, equipts_new);

		if(SGClientUtil::DiffEquipt(m_equipts, equipts_new))
			//&& !SGClientUtil::IsEmptyEquipt(m_equipts))
		{
			if(m_callback) 
			{
				m_callback->DataUpdate(Atlas::CSGSyncDataManager::eSyncEquipt);
			}
		}

		m_equipts.clear();
		for(_U32 i = 0; i < count; ++i)
		{
			m_equipts.push_back(items[i]);
		}

		if(m_pSyncMgr->ReceiveRequest(Atlas::CSGSyncDataManager::eSyncEquipt))
			return;
	}

	void CSGClient::QueryBagUsable(CSGClient* pClient, const SG_USABLE_ITEM* items, _U32 count)
	{
		Atlas::Vector<SG_USABLE_ITEM> usables_new;
		SGClientUtil::GenerateTempNewVec<SG_USABLE_ITEM>(items, count, usables_new);

		if(SGClientUtil::DiffUsable(m_usables, usables_new))
			//&& !SGClientUtil::IsEmptyUsable(m_usables))
		{
			if(m_callback) 
			{
				m_callback->DataUpdate(Atlas::CSGSyncDataManager::eSyncUsable);
			}
		}

		m_usables.clear();
		for(_U32 i = 0; i < count; ++i)
		{
			m_usables.push_back(items[i]);
		}

		if(m_pSyncMgr->ReceiveRequest(Atlas::CSGSyncDataManager::eSyncUsable))
			return;
	}

	void CSGClient::QueryBagGen(CSGClient* pClient, const SG_GEM_ITEM* items, _U32 count)
	{
		Atlas::Vector<SG_GEM_ITEM> gems_new;
		SGClientUtil::GenerateTempNewVec<SG_GEM_ITEM>(items, count, gems_new);

		if(SGClientUtil::DiffGem(m_gems, gems_new))
			//&& !SGClientUtil::IsEmptyGem(m_gems))
		{
			if(m_callback) 
			{
				m_callback->DataUpdate(Atlas::CSGSyncDataManager::eSyncGem);
			}
		}

		m_gems.clear();
		for(_U32 i = 0; i < count; ++i)
		{
			m_gems.push_back(items[i]);
		}

		if(m_pSyncMgr->ReceiveRequest(Atlas::CSGSyncDataManager::eSyncGem))
			return;
	}

	void CSGClient::QueryBagEnd(CSGClient* pClient, _U8 nSync)
	{
		if(nSync && m_pSyncMgr->ReceiveRequest(Atlas::CSGSyncDataManager::eSyncBagEnd))
		{
			return;
		}

		if(m_callback) m_callback->QueryBagDone(m_equipts, m_usables, m_gems);
	}

	void CSGClient::BeginBattleResult(CSGClient* pClient, const SG_PLAYER_PVE& PlayerPVE)
	{
		if(m_callback) m_callback->BeginBattleDone(PlayerPVE);
	}

	void CSGClient::EndBattleResult(CSGClient* pClient, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count)
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
				SGClientUtil::GetUnlockSoldierByLevel(m_player.level, level, m_newSoldiers, m_soldiers);
				m_player.level = (_U16)level;
			}

			m_player.exp = exp;
			m_player.gold += gold;
			for(_U32 i = 0; i < drop_count; ++i)
			{
				m_newItemList.push_back(drops[i].uuid);
			}

			m_callback->EndBattleDone(level, exp_addition, gold, drop_lists, drop_count);
			m_callback->DataUpdate(CSGSyncDataManager::eSyncPlayer);
		}
	}
	
	void CSGClient::EnhanceCoolDownResult(CSGClient* pClient, _U32 time)
	{
		if(m_callback)
		{
			m_callback->EnhanceCoolDownResult(time);
		}
	}

	void CSGClient::RefreshEquipDone(CSGClient* pClient, SG_EQUIPT_ITEM& equipt)
	{
		for(Atlas::Vector<SG_EQUIPT_ITEM>::iterator it = m_equipts.begin(); it != m_equipts.end(); ++it)
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

	void CSGClient::QueryServerTimeResult(CSGClient* pClient, _U32 server_time)
	{
		time_t t;
		time(&t);
		m_nServerTimeDelta = (int)(server_time - (_U32)t);
		if(m_pSyncMgr->ReceiveRequest(Atlas::CSGSyncDataManager::eSyncServerTime))
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
		if(nSync && m_pSyncMgr->ReceiveRequest(Atlas::CSGSyncDataManager::eSyncPlayerQuest))
			return;

		if(m_callback)
		{
			m_callback->QueryPlayerQuestResult(quest_list, count);
		}
	}
	
	void CSGClient::FinishQuestDone(CSGClient* pClient, _U32 quest_id, _U32 exp_addition, _U32 exp, _U32 level, _U32 gold,  _U32 rmb, _U32 reputation, _U32 energy, const SG_DROP_ITEM_BASE* drops, _U32 drop_count)
	{
		for(Atlas::Vector<SG_QUEST_LIVE_INFO>::iterator it = m_quests.begin(); it != m_quests.end(); ++it)
		{
			SG_QUEST_LIVE_INFO& quest_item = (*it);
			if(quest_item.quest_id == quest_id)
			{
				quest_item.status = SG_QUEST_STATUS_FINISHED;
				break;
			}
		}

		if(m_callback)
		{
			if(m_player.level != level)
			{
				SGClientUtil::GetUnlockSoldierByLevel(m_player.level, level, m_newSoldiers, m_soldiers);
				m_player.level = (_U16)level;
			}

			m_player.exp = exp;
			m_player.gold += gold;
			m_player.rmb += rmb;
			m_player.reputation += reputation;

			for(_U32 i = 0; i < drop_count; ++i)
			{
				m_newItemList.push_back(drops[i].uuid);
			}

			m_callback->FinishQuestDone(quest_id, exp_addition, gold, rmb, reputation, energy, drops, drop_count);
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

	void CSGClient::QueryPlayerRankListResult(CSGClient* pClient, SG_PLAYER* players, _U32 count)
	{
		if(m_callback)
		{
			m_callback->QueryPlayerRankListResult(players, count);
		}
	}

	void CSGClient::BuyGoodsResult(CSGClient* pClient, const A_UUID* goods, _U32 count)
	{
		for(_U32 i = 0; i < count; ++i)
		{
			m_newItemList.push_back(goods[i]);
		}

		//help to sync data
		Atlas::Vector<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		SyncSet(vecSync);
	}

	void CSGClient::PVPCoolDownResult(CSGClient* pClient, _U32 time)
	{
		if(m_callback)
		{
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

	void CSGClient::PVPHeroListRecord(CSGClient* pClient, const SG_PLAYER* players, _U32 count)
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
				m_newItemList.push_back(items->uuid);
			}

			//sync player and bag
		}
	}

	void CSGClient::QueryInstanceResult(CSGClient* pClient, const SG_INSTANCE_INFO* instances, _U32 count)
	{
		if(m_callback)
		{
			
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

	void CSGClient::EndInstanceBattleResult(CSGClient* pClient, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, const SG_DROP_ITEM_CONFIG* drops, _U32 drop_count)
	{
		if(m_callback)
		{
			if(m_player.level != level)
			{
				SGClientUtil::GetUnlockSoldierByLevel(m_player.level, level, m_newSoldiers, m_soldiers);
				m_player.level = (_U16)level;
			}

			m_player.exp = exp;
			m_player.gold += gold;

			for(_U32 i = 0; i < drop_count; ++i)
			{
				m_newItemList.push_back(drops[i].uuid);
			}
			
			m_callback->EndInstanceBattleResult(level, exp_addition, gold, drops, drop_count);
			m_callback->DataUpdate(CSGSyncDataManager::eSyncPlayer);
		}
	}

	void CSGClient::ResetInstanceResult(CSGClient* pClient, _U8 result, _U32 gold, const SG_INSTANCE_INFO& instance)
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

			m_player.gold += gold;

			m_callback->ResetInstanceResult(result, gold, instance);
		}
	}


	void CSGClient::PVPBattleResult(CSGClient* pClient, _U32 reputation)
	{
		//help to sync data
		Atlas::Vector<_U8> vecSync;
		vecSync.push_back(CSGSyncDataManager::eSyncPlayer);
		SyncSet(vecSync);

		if(m_callback)
		{
			m_player.reputation += reputation;

			m_callback->PVPBattleResult(reputation);
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
	}

	const Atlas::Vector<SG_GENERAL>& CSGClient::GetGenerals()
	{
		return m_generals;
	}

	const Atlas::Vector<SG_SOLDIER>& CSGClient::GetSoldiers()
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

	const Atlas::Vector<SG_EQUIPT_ITEM>& CSGClient::GetEquiptItem()
	{
		return m_equipts;
	}
	
	const Atlas::Vector<SG_GEM_ITEM>& CSGClient::GetGemItem()
	{
		return m_gems;
	}

	const Atlas::Vector<SG_USABLE_ITEM>& CSGClient::GetUsableItem()
	{
		return m_usables;
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
		
		ATLAS_ASSERT(0);
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
		for(Atlas::Vector<SG_EQUIPT_ITEM>::iterator it = m_equipts.begin(); it != m_equipts.end(); ++it)
		{
			if((*it).uuid == uuid)
			{
				return &(*it);
			}
		}
		
		for(Atlas::Vector<SG_USABLE_ITEM>::iterator it = m_usables.begin(); it != m_usables.end(); ++it)
		{
			if((*it).uuid == uuid)
			{
				return &(*it);
			}
		}

		for(Atlas::Vector<SG_GEM_ITEM>::iterator it = m_gems.begin(); it != m_gems.end(); ++it)
		{
			if((*it).uuid == uuid)
			{
				return &(*it);
			}
		}		
		
		return NULL;
	}

	void CSGClient::GetFinishedQuest(Atlas::Vector<SG_QUEST_LIVE_INFO>& quest_vec)
	{
		for(Atlas::Vector<SG_QUEST_LIVE_INFO>::iterator it = m_quests.begin(); it != m_quests.end(); ++it)
		{
			if((*it).status == SG_QUEST_STATUS_FINISHED)
			{
				quest_vec.push_back(*it);
			}
		}
	}

	void CSGClient::GetNewSoldierList(Atlas::Vector<_U32>& soldier_lists)
	{
		soldier_lists = m_newSoldiers;
	}

	bool CSGClient::HasNewSoldier()
	{
		return !m_newSoldiers.empty();
	}

	bool CSGClient::IsNewSoldier(_U32 soldier_id)
	{
		for(size_t i = 0; i < m_newSoldiers.size(); ++i)
		{
			if(m_newSoldiers[i] == soldier_id)
			{
				return true;
			}
		}

		return false;
	}

	void CSGClient::ClearNewSoldierList()
	{
		m_newSoldiers.clear();
	}

	void CSGClient::GetNewItemList(Atlas::Vector<A_UUID>& item_lists)
	{
		item_lists = m_newItemList;
	}

	bool CSGClient::HasNewItem()
	{
		return !m_newItemList.empty();
	}

	bool CSGClient::IsNewItem(const A_UUID& uuid)
	{
		for(size_t i = 0; i < m_newItemList.size(); ++i)
		{
			if(m_newItemList[i] == uuid)
			{
				return true;
			}
		}
		return false;
	}

	void CSGClient::ClearNewItemList()
	{
		m_newItemList.clear();
	}

	void CSGClient::SyncForInit()
	{
		m_pSyncMgr->AddAllRequest();
		m_pSyncMgr->SendSyncRequest();
	}

	void CSGClient::SyncSet(const Atlas::Vector<_U8> vecSync)
	{
		for(size_t i = 0; i < vecSync.size(); ++i)
		{
			m_pSyncMgr->AddRequest(vecSync[i]);
		}

		m_pSyncMgr->SendSyncRequest();
	}

	void CSGClient::SyncSuccNotify()
	{
		m_callback->SyncDone();
	}
}
