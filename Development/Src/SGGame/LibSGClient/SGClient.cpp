#include <AtlasBase.h>
#include <AtlasClient.h>
#include <AtlasCommon.h>

#include "SGCommon.h"
#include "SGClient.h"

namespace Atlas
{
	
	CSGClient::CSGClient(CClientApp* pClientApp, _U32 recvsize) : CClient(pClientApp, recvsize), m_C2S(this), m_S2C(this)
	{
		m_nQueryBagRef = 0;
		m_QueryState = STATE_NA;
		m_callback = NULL;
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
		m_C2S.Ping();
	}

	void CSGClient::GetServerList()
	{
		//m_C2S.GetServerList();
		//test data
		SG_SERVER_INFO infos[4];
		memset(infos, 0, sizeof(SG_SERVER_INFO)*4);
		_U32 count = 1;

		infos[0].server_id = 0;
		infos[0].server_name = "test";
		infos[0].server_state = 1;
		infos[0].avatar_nick = "mumu";
		infos[0].general_id = 11001;
		infos[0].level = 1;

		GetServerListResult(this, infos, 1);
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
	}

	void CSGClient::EnterGame()
	{
		m_C2S.EnterGame();
	}

	void CSGClient::LeaveGame()
	{
		m_C2S.LeaveGame();
	}

	void CSGClient::QueryPlayer()
	{
		m_C2S.QueryPlayer();
	}

	void CSGClient::QueryGenerals()
	{
		m_C2S.QueryGenerals();
	}

	void CSGClient::QuerySoldiers()
	{
		m_C2S.QuerySoldiers();
	}

	void CSGClient::QueryBag()
	{
		m_C2S.QueryBag();
	}

	void CSGClient::EquipItem(_U32 general_id, A_UUID item_uuid)
	{
		m_C2S.EquipItem(general_id, item_uuid);
	}

	void CSGClient::BeginBattle(const char* name)
	{
		//m_C2S.BeginBattle(name);
		//temp return data
		SG_PLAYER_PVE PlayerPVE;
		memset(&PlayerPVE, 0, sizeof(SG_PLAYER_PVE));

		const DDLReflect::STRUCT_INFO* struct_info = ContentObject::GetType("SG_GENERAL_LEVEL_CONFIG");
		std::vector<A_UUID> list;
		if(!ContentObject::GetList(struct_info, list, true))
		{
			ATLAS_ASSERT(0);
			return;
		}
		
		for(size_t i = 0; i < list.size(); ++i)
		{
			SG_GENERAL_LEVEL_CONFIG* config = (SG_GENERAL_LEVEL_CONFIG*)ContentObject::Query(list[i]);
			if(config)
			{
				if(config->attr_id == 11001&& config->level == 1)
				{
					PlayerPVE.PawnConfig.BaseConfig = config->PawnConfig.BaseConfig;
				}

				if(config->attr_id == 21011&& config->level == 1)
				{
					PlayerPVE.EquippedGenerals._Array[0].PawnConfig.BaseConfig = config->PawnConfig.BaseConfig;
				}

				//no 21022 diaochan 21006 instead of it
				if(config->attr_id == 21006&& config->level == 1)
				{
					PlayerPVE.EquippedGenerals._Array[1].PawnConfig.BaseConfig = config->PawnConfig.BaseConfig;
				}
			}
		}
		
		PlayerPVE.ArchetypeURL = "Archetypes.FriendlyHeroes.WuSheng";
		PlayerPVE.EquippedGenerals._Count = 2;
		PlayerPVE.EquippedGenerals._Array[0].ArchetypeURL = "Archetypes.FriendlyGenerals.ZhangLiao";
		PlayerPVE.EquippedGenerals._Array[1].ArchetypeURL = "Archetypes.FriendlyGenerals.DiaoChan";
		PlayerPVE.EquippedSoldiers._Count = 3;
		PlayerPVE.EquippedSoldiers._Array[0].ArchetypeURL = "Archetypes.FriendlySoldiers.ChangQiangBing";
		PlayerPVE.EquippedSoldiers._Array[1].ArchetypeURL = "Archetypes.FriendlySoldiers.GongJianBing";
		PlayerPVE.EquippedSoldiers._Array[2].ArchetypeURL = "Archetypes.FriendlySoldiers.YinShi";
		
		struct_info = ContentObject::GetType("SG_SOLDIER_LEVEL_CONFIG");
		list.clear();
		if(!ContentObject::GetList(struct_info, list, true))
		{
			ATLAS_ASSERT(0);
			return;
		}
		
		for(size_t i = 0; i < list.size(); ++i)
		{
			SG_SOLDIER_LEVEL_CONFIG* config = (SG_SOLDIER_LEVEL_CONFIG*)ContentObject::Query(list[i]);
			if(config)
			{
				if(config->attr_id == 31002&& config->level == 1)
				{
					PlayerPVE.EquippedSoldiers._Array[0].PawnConfig.BaseConfig = config->PawnConfig.BaseConfig;
				}

				if(config->attr_id == 31004&& config->level == 1)
				{
					PlayerPVE.EquippedSoldiers._Array[1].PawnConfig.BaseConfig = config->PawnConfig.BaseConfig;
				}

				//no 21022 diaochan 21006 instead of it
				if(config->attr_id == 31005&& config->level == 1)
				{
					PlayerPVE.EquippedSoldiers._Array[2].PawnConfig.BaseConfig = config->PawnConfig.BaseConfig;
				}
			}
		}

		this->BeginBattleResult(this, PlayerPVE);
	}

	void CSGClient::EndBattle(const char* name, _U32 result)
	{
		//temp return data
		//m_C2S.EndBattle(name, result);
		_U32 level = 1;
		_U32 exp = 500;
		_U32 gold = 999;
		SG_DROP_ITEM_BASE drops[4];
		memset(drops, 0, sizeof(SG_DROP_ITEM_BASE)*4);
		drops[0].count = 1;
		drops[0].item_id = 10001;

		drops[1].count = 2;
		drops[1].item_id = 11001;

		_U32 drop_count = 2;

		this->EndBattleResult(this, level, exp, gold, drops, drop_count);
	}

	void CSGClient::Pong(CSGClient* pClient)
	{
	}

	void CSGClient::GetServerListResult(CSGClient* pClient, const SG_SERVER_INFO* infos, _U32 count)
	{	
		if(m_callback) m_callback->GetServerListResult(infos, count);
	}

	void CSGClient::QueryAvatarFailed(CSGClient* pClient, _U32 code)
	{
		if(m_callback) m_callback->QueryAvatarFailed(code);
	}

	void CSGClient::QueryAvatarResult(CSGClient* pClient, const SG_PLAYER& player)
	{
		m_player = player;
	}

	void CSGClient::CreateAvatarResult(CSGClient* pClient, _U32 code)
	{
		//according to code
	}

	void CSGClient::QueryPlayerResult(CSGClient* pClient, const SG_PLAYER& player)
	{
		m_player = player;
		m_player.equip_generals._Count = player.equip_generals._Count;
		m_player.equip_soldiers._Count = player.equip_soldiers._Count;

		memcpy(m_player.equip_generals._Array, player.equip_generals._Array, sizeof(player.equip_generals._Array[0])*player.equip_generals._Count);
		memcpy(m_player.equip_soldiers._Array, player.equip_soldiers._Array, sizeof(player.equip_soldiers._Array[0])*player.equip_soldiers._Count);
	}

	void CSGClient::QueryGeneralResult(CSGClient* pClient, const SG_GENERAL* generals, _U32 count)
	{
		m_generals.clear();
		for(_U32 i = 0; i < count; ++i)
		{
			m_generals.push_back(generals[i]);
		}

		if(m_callback) m_callback->QueryGeneralsDone(m_generals);
	}

	void CSGClient::QuerySoldierResult(CSGClient* pClient, const SG_SOLDIER* soldiers, _U32 count)
	{
		m_soldiers.clear();
		for(_U32 i = 0; i < count; ++i)
		{
			m_soldiers.push_back(soldiers[i]);
		}

		if(m_callback) m_callback->QuerySoldiersDone(m_soldiers);
	}

	void CSGClient::QueryBagBegin(CSGClient* pClient)
	{
		if(m_QueryState == STATE_NA || m_QueryState == STATE_SUCC || m_QueryState == STATE_FAILED)
		{
			m_nQueryBagRef = 0;
			m_QueryState = STATE_PENDING;
		}
	}

	void CSGClient::QueryBagEquipt(CSGClient* pClient, const SG_EQUIPT_ITEM* items, _U32 count)
	{
		++m_nQueryBagRef;
		m_equipts.clear();
		for(_U32 i = 0; i < count; ++i)
		{
			m_equipts.push_back(items[i]);
		}
	}

	void CSGClient::QueryBagUsable(CSGClient* pClient, const SG_USABLE_ITEM* items, _U32 count)
	{
		++m_nQueryBagRef;
		m_usables.clear();
		for(_U32 i = 0; i < count; ++i)
		{
			m_usables.push_back(items[i]);
		}
	}

	void CSGClient::QueryBagGen(CSGClient* pClient, const SG_GEM_ITEM* items, _U32 count)
	{
		++m_nQueryBagRef;
		m_gems.clear();
		for(_U32 i = 0; i < count; ++i)
		{
			m_gems.push_back(items[i]);
		}
	}

	void CSGClient::QueryBagEnd(CSGClient* pClient)
	{
		if(m_nQueryBagRef == 2)
		{
			if(m_callback) m_callback->QueryBagDone(m_equipts, m_usables, m_gems);
			m_QueryState = STATE_SUCC;
			m_nQueryBagRef = 0;
		}
	}

	void CSGClient::BeginBattleResult(CSGClient* pClient, const SG_PLAYER_PVE& PlayerPVE)
	{
		if(m_callback) m_callback->BeginBattleDone(PlayerPVE);
	}

	void CSGClient::EndBattleResult(CSGClient* pClient, _U32 level, _U32 exp, _U32 gold, const SG_DROP_ITEM_BASE* drops, _U32 drop_count)
	{
		if(m_callback) m_callback->EndBattleDone(level, exp, gold, drops, drop_count);
	}

}
