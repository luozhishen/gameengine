#include "ZionBase.h"
#include "ZionCommon.h"
#include "ZionClient.h"
#include "DataSyncClient.h"

namespace Zion
{

	CDataSyncClient::CDataSyncClient(CClient* pClient) : CClientComponent(pClient), m_Binary(pClient), m_Json(pClient)
	{
		pClient->RegisterStub(new DDLStub::DATASYNC_BINARY_S2C<CDataSyncClient>(this));
		pClient->RegisterStub(new DDLStub::DATASYNC_JSON_S2C<CDataSyncClient>(this));
		m_Flag = (_U32)-1;
	}

	CDataSyncClient::~CDataSyncClient()
	{
		ClearQueue();
	}

	A_LIVE_OBJECT* CDataSyncClient::CreateObject(const DDLReflect::STRUCT_INFO* pInfo)
	{
		if((m_Flag&SYNCFLAG_CLIENT_ACTIVE)==0) return NULL;
		if(!DDLReflect::IsChild(DDLReflect::GetStruct<A_LIVE_OBJECT>(), pInfo)) return NULL;

		A_LIVE_OBJECT* data = (A_LIVE_OBJECT*)DDLReflect::CreateObject(pInfo);
		if(!data) return NULL;
		m_NewQ.objs.push_back(data);
		m_NewQ.infos.push_back(pInfo);
		return NULL;
	}

	bool CDataSyncClient::RemoveObject(const A_UUID& _uuid)
	{
		if((m_Flag&SYNCFLAG_CLIENT_ACTIVE)==0) return false;

		if(m_Manager.Remove(_uuid))
		{
			m_DelList.insert(_uuid);
			return true;
		}
		else
		{
			return false;
		}
	}

	LiveObject::CObject* CDataSyncClient::GetObject(const A_UUID& _uuid)
	{
		return m_Manager.Get(_uuid);
	}

	void CDataSyncClient::Sync()
	{
/*
		while(!m_NewQ.objs.empty())
		{
			DDLReflect::DestoryObject(m_NewQ.infos.front(), m_NewQ.objs.front());
			m_NewQ.objs.pop_front();
			m_NewQ.infos.pop_front();
		}
		while(!m_WatQ.objs.empty())
		{
			DDLReflect::DestoryObject(m_WatQ.infos.front(), m_WatQ.objs.front());
			m_WatQ.objs.pop_front();
			m_WatQ.infos.pop_front();
		}

		if((m_Flag&SYNCFLAG_MODEMASK)==SYNCFLAG_BINARY)
		{
			DDL::TMemoryWriter<1024> buf;
			m_Binary.DS_CreateObject(LiveObject::GetTypeId(pInfo->name), buf.GetBuf(), buf.GetSize();
		}
		else
		{
			String json;
			m_Json.DS_CreateObject(pInfo->name, json.c_str());
		}
*/
	}

	bool CDataSyncClient::InProcess()
	{
		return m_Flag==0 || !m_bReady || !m_NewQ.objs.empty() || !m_WatQ.objs.empty();
	}

	void CDataSyncClient::DS_SyncOpen(_U32 flag)
	{
		ZION_ASSERT(m_Flag==(_U32)-1);
		m_Flag = flag;
		m_bReady = false;
	}

	void CDataSyncClient::DS_SyncReady()
	{
		ZION_ASSERT(m_Flag!=(_U32)-1);
		ZION_ASSERT(!m_bReady);
		m_bReady = true;
	}

	void CDataSyncClient::DS_SyncClose()
	{
		ZION_ASSERT(m_Flag!=(_U32)-1);
		ZION_ASSERT(m_bReady);
		m_Flag = (_U32)-1;
		ClearQueue();
	}

	void CDataSyncClient::DS_CreateObjectDone(const A_UUID& _uuid)
	{
		if(m_WatQ.objs.empty() || m_WatQ.infos.empty())
		{
			ZION_ASSERT("invalid function call");
			return;
		}

		const DDLReflect::STRUCT_INFO* _info = m_WatQ.infos.front();
		A_LIVE_OBJECT* _obj = m_WatQ.objs.front();
		_obj->_uuid = _uuid;
		m_WatQ.infos.pop_front();
		m_WatQ.objs.pop_front();
		if(!m_Manager.Append(_info, _obj))
		{
			DDLReflect::DestoryObject(_info, _obj);
			return;
		}
	}

	void CDataSyncClient::DS_CreateObject(const char* type, const char* data)
	{
		const DDLReflect::STRUCT_INFO* _info = LiveObject::GetType(type);
		if(!_info)
		{
			ZION_ASSERT(!"invalid type");
			return;
		}

		m_Manager.Append(_info, data);
	}

	void CDataSyncClient::DS_CreateObject(_U16 type, const _U8* data, _U32 len)
	{
		const DDLReflect::STRUCT_INFO* _info = LiveObject::GetType(type);
		if(!_info)
		{
			ZION_ASSERT(!"invalid type");
			return;
		}

		m_Manager.Append(_info, data, len);
	}

	void CDataSyncClient::DS_UpdateObject(const A_UUID& _uuid, const char* data)
	{
	}

	void CDataSyncClient::DS_UpdateObject(const A_UUID& _uuid, const _U8* data, _U32 len)
	{
	}

	void CDataSyncClient::DS_RemoveObjects(const A_UUID* _uuids, _U32 count)
	{
		for(_U32 i=0; i<count; i++)
		{
			m_Manager.Remove(_uuids[i]);
		}
	}

	void CDataSyncClient::ClearQueue()
	{
		while(!m_NewQ.objs.empty())
		{
			DDLReflect::DestoryObject(m_NewQ.infos.front(), m_NewQ.objs.front());
			m_NewQ.objs.pop_front();
			m_NewQ.infos.pop_front();
		}

		while(!m_WatQ.objs.empty())
		{
			DDLReflect::DestoryObject(m_WatQ.infos.front(), m_WatQ.objs.front());
			m_WatQ.objs.pop_front();
			m_WatQ.infos.pop_front();
		}
	}

}
