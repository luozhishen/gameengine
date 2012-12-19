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
		if(iid==GetClientStubID<SGGAME_S2C>())
		{
			DDL::MemoryReader reader(data, len);
			m_S2C.Dispatcher(this, fid, reader);
			return;
		}
	}

	void CSGClient::QueryAvatarFailed(CSGClient* pClient, _U32 code)
	{
	}

	void CSGClient::QueryAvatarResult(CSGClient* pClient, const SG_PLAYER& player)
	{
	}

	void CSGClient::CreatAvatarResult(CSGClient* pClient, _U32 code)
	{
	}

	void CSGClient::QueryBagBegin(CSGClient* pClient)
	{
	}

	void CSGClient::QueryBagEquipt(CSGClient* pClient, const SG_EQUIPT_ITEM& item)
	{
	}

	void CSGClient::QueryBagUsable(CSGClient* pClient, const SG_USABLE_ITEM& item)
	{
	}

	void CSGClient::QueryBagGen(CSGClient* pClient, const SG_GEM_ITEM& item)
	{
	}

	void CSGClient::QueryBagEnd(CSGClient* pClient)
	{
	}

	void CSGClient::Pong(CSGClient* pClient)
	{
	}

}
