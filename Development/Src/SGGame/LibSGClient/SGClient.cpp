#include <AtlasBase.h>
#include <AtlasClient.h>
#include <AtlasCommon.h>

#include "SGCommon.h"
#include "SGClient.h"

namespace Atlas
{

	CSGClient::CSGClient(CClientApp* pClientApp, _U32 recvsize) : CClient(pClientApp, recvsize), m_C2S(this), m_S2C(this)
	{
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

	void CSGClient::EquipItem(_U32 general_id, const SG_EQUIP_SLOTS& slots)
	{
		m_C2S.EquipItem(general_id, slots);
	}

	void CSGClient::EquipGenerals(const _U32* generals, _U32 count)
	{
		m_C2S.EquipGenerals(generals, count);
	}

	void CSGClient::EquipSoldiers(const _U32* soldiers, _U32 count)
	{
		m_C2S.EquipSoldiers(soldiers, count);
	}

	void CSGClient::EnhanceSoldier(_U32 soldier_id)
	{
		m_C2S.EnhanceSoldier(soldier_id);
	}

	void CSGClient::BeginBattle(const char* name)
	{
		m_C2S.BeginBattle(name);
	}

	void CSGClient::EndBattle(const char* name, _U32 result)
	{
		m_C2S.EndBattle(name, result);
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
		if(m_callback) m_callback->QueryAvatarDone(m_player);
	}

	void CSGClient::CreateAvatarResult(CSGClient* pClient, _U32 code)
	{
		if(m_callback) m_callback->CreateAvatarResult(code);
	}

	void CSGClient::QueryPlayerResult(CSGClient* pClient, const SG_PLAYER& player)
	{
		m_player = player;
		if(m_callback) m_callback->QueryPlayerResult(m_player);
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
	}

	void CSGClient::QueryBagEquipt(CSGClient* pClient, const SG_EQUIPT_ITEM* items, _U32 count)
	{
		m_equipts.clear();
		for(_U32 i = 0; i < count; ++i)
		{
			m_equipts.push_back(items[i]);
		}
	}

	void CSGClient::QueryBagUsable(CSGClient* pClient, const SG_USABLE_ITEM* items, _U32 count)
	{
		m_usables.clear();
		for(_U32 i = 0; i < count; ++i)
		{
			m_usables.push_back(items[i]);
		}
	}

	void CSGClient::QueryBagGen(CSGClient* pClient, const SG_GEM_ITEM* items, _U32 count)
	{
		m_gems.clear();
		for(_U32 i = 0; i < count; ++i)
		{
			m_gems.push_back(items[i]);
		}
	}

	void CSGClient::QueryBagEnd(CSGClient* pClient)
	{
		if(m_callback) m_callback->QueryBagDone(m_equipts, m_usables, m_gems);
	}

	void CSGClient::BeginBattleResult(CSGClient* pClient, const SG_PLAYER_PVE& PlayerPVE)
	{
		if(m_callback) m_callback->BeginBattleDone(PlayerPVE);
	}

	void CSGClient::EndBattleResult(CSGClient* pClient, _U32 level, _U32 exp, _U32 gold, const SG_DROP_ITEM_BASE* drops, _U32 drop_count)
	{
		if(m_callback) m_callback->EndBattleDone(level, exp, gold, drops, drop_count);
	}

	void CSGClient::OnLoginDone()
	{
		CClient::OnLoginDone();
		if(m_callback) m_callback->LoginResult(0);
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
}
