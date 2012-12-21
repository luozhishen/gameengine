#include <AtlasBase.h>
#include <AtlasClient.h>
#include <AtlasCommon.h>

#include "SGCommon.h"
#include "SGClient.h"

namespace Atlas
{
	
	CSGClient::CSGClient(CClientApp* pClientApp, _U32 recvsize) : CClient(pClientApp, recvsize), m_C2S(this), m_S2C(this)
	{
	}	

	CSGClient::~CSGClient()
	{

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

	void CSGClient::Pong(CSGClient* pClient)
	{
	}

	void CSGClient::GetServerListResult(CSGClient* pClient, const SG_SERVER_INFO* infos, _U32 count)
	{
	}

	void CSGClient::QueryAvatarFailed(CSGClient* pClient, _U32 code)
	{
	}

	void CSGClient::QueryAvatarResult(CSGClient* pClient, const SG_PLAYER& player)
	{
	}

	void CSGClient::CreateAvatarResult(CSGClient* pClient, _U32 code)
	{
	}

	void CSGClient::QueryPlayerResult(CSGClient* pClient, const SG_PLAYER& player)
	{
	}

	void CSGClient::QueryGeneralResult(CSGClient* pClient, const SG_GENERAL* generals, _U32 count)
	{
	}

	void CSGClient::QuerySoldierResult(CSGClient* pClient, const SG_SOLDIER* soldiers, _U32 count)
	{
	}

	void CSGClient::QueryBagBegin(CSGClient* pClient)
	{
	}

	void CSGClient::QueryBagEquipt(CSGClient* pClient, const SG_EQUIPT_ITEM* items, _U32 count)
	{
	}

	void CSGClient::QueryBagUsable(CSGClient* pClient, const SG_USABLE_ITEM* items, _U32 count)
	{
	}

	void CSGClient::QueryBagGen(CSGClient* pClient, const SG_GEM_ITEM* items, _U32 count)
	{
	}

	void CSGClient::QueryBagEnd(CSGClient* pClient)
	{
	}

	void CSGClient::BeginBattleResult(CSGClient* pClient, const A_UUID& battle)
	{
	}

	void CSGClient::EndBattleResult(CSGClient* pClient, _U32 level, _U32 exp, _U32 gold, const SG_DROP_GROUP_BASE* drops, _U32 drop_count)
	{
	}

}
