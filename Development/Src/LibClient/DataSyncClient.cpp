#include <ZionBase.h>
#include <ZionCommon.h>
#include "ZionClient.h"
#include "DataSyncClient.h"

namespace Zion
{

	CDataSyncClient::CDataSyncClient(CClient* pClient) : CClientComponent(pClient), m_Binary(pClient), m_Json(pClient)
	{
		pClient->RegisterStub(new DDLStub::DATASYNC_BINARY_S2C<CDataSyncClient>(this));
		pClient->RegisterStub(new DDLStub::DATASYNC_JSON_S2C<CDataSyncClient>(this));
		m_Mode = SYNCMODE_NONE;
		m_Flag = (_U32)-1;
	}

	CDataSyncClient::~CDataSyncClient()
	{
		Clear();
	}

	A_LIVE_OBJECT* CDataSyncClient::CreateObject(const DDLReflect::STRUCT_INFO* pInfo)
	{
		if((m_Flag&SYNCFLAG_CLIENT_ACTIVE)==0) return NULL;
		if(!DDLReflect::IsChild(DDLReflect::GetStruct<A_LIVE_OBJECT>(), pInfo)) return NULL;

		A_LIVE_OBJECT* data = (A_LIVE_OBJECT*)DDLReflect::CreateObject(pInfo);
		if(!data) return NULL;
		OBJECT_ADDITEM item = {data, pInfo};
		m_NewQ.push_back(item);
		return data;
	}

	bool CDataSyncClient::RemoveObject(const A_UUID& _uuid)
	{
		if((m_Flag&SYNCFLAG_CLIENT_ACTIVE)==0) return false;

		if(!m_Accesser.Remove(_uuid))
		{
			return false;
		}
		m_DelList.push_back(_uuid);
		return true;
	}

	LiveData::CObject* CDataSyncClient::GetObject(const A_UUID& _uuid)
	{
		return m_Accesser.Get(_uuid);
	}

	void CDataSyncClient::Sync()
	{
		while(!m_NewQ.empty())
		{
			OBJECT_ADDITEM& item = m_NewQ.front();
			switch((m_Flag&SYNCFLAG_MODEMASK))
			{
			case SYNCFLAG_BINARY:
				{
					DDL::TMemoryWriter<1024> buf;
					if(!item.info->write_proc(buf, item.obj))
					{
						ZION_ASSERT(0);
						return;
					}
					m_Binary.DS_CreateObject(LiveObject::GetTypeId(item.info->name), buf.GetBuf(), buf.GetSize());
				}
				break;
			case SYNCFLAG_JSON:
				{
					String json;
					if(!DDLReflect::Struct2Json(m_NewQ.front().info, (const _U8*)item.obj, json))
					{
						ZION_ASSERT(0);
						return;
					}
					m_Json.DS_CreateObject(item.info->name, json.c_str());
				}
				break;
			};
			m_WatQ.push_back(item);
			m_NewQ.pop_front();
		}

		if(!m_DelList.empty())
		{
			switch((m_Flag&SYNCFLAG_MODEMASK))
			{
			case SYNCFLAG_BINARY:
				m_Binary.DS_DeleteObject(&m_DelList[0], (_U32)m_DelList.size());
				break;
			case SYNCFLAG_JSON:
				m_Json.DS_DeleteObject(&m_DelList[0], (_U32)m_DelList.size());
				break;
			}
			m_DelList.clear();
		}

		LiveData::CObject* obj = m_Accesser.FindFirst();
		while(obj)
		{
			if(!obj->IsDirty())
			{
				obj = m_Accesser.FindNext(obj);
				continue;
			}

			A_LIVE_OBJECT* data = (A_LIVE_OBJECT*)obj->GetData();

			switch((m_Flag&SYNCFLAG_MODEMASK))
			{
			case SYNCFLAG_BINARY:
				{
					DDL::TMemoryWriter<1024> buf;
					if(!obj->GetStructInfo()->write_proc(buf, data))
					{
						ZION_ASSERT(0);
						return;
					}
					m_Binary.DS_UpdateObject(data->_uuid, buf.GetBuf(), buf.GetSize());
				}
				break;
			case SYNCFLAG_JSON:
				{
					String json;
					if(!DDLReflect::Struct2Json(obj->GetStructInfo(), (const _U8*)obj->GetData(), json))
					{
						ZION_ASSERT(0);
						return;
					}
					m_Json.DS_UpdateObject(data->_uuid, json.c_str());
				}
				break;
			}

			obj->Clean();
			obj = m_Accesser.FindNext(obj);
		}
	}

	bool CDataSyncClient::InProcess()
	{
		return m_Flag==0 || !m_bReady || !m_WatQ.empty();
	}

	void CDataSyncClient::DS_SetMode(_U32 mode)
	{
		m_Mode = mode;
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
		Clear();
	}

	void CDataSyncClient::DS_CreateObjectDone(const A_UUID& _uuid)
	{
		if(m_WatQ.empty())
		{
			ZION_ASSERT("invalid function call");
			return;
		}

		const DDLReflect::STRUCT_INFO* _info = m_WatQ.front().info;
		A_LIVE_OBJECT* _obj = m_WatQ.front().obj;
		_obj->_uuid = _uuid;
		m_WatQ.pop_front();
		if(!m_Accesser.Append(_info, _obj))
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

		m_Accesser.Append(_info, data);
	}

	void CDataSyncClient::DS_CreateObject(_U16 type, const _U8* data, _U32 len)
	{
		const DDLReflect::STRUCT_INFO* _info = LiveObject::GetType(type);
		if(!_info)
		{
			ZION_ASSERT(!"invalid type");
			return;
		}

		m_Accesser.Append(_info, data, len);
	}

	void CDataSyncClient::DS_UpdateObject(const A_UUID& _uuid, const char* json)
	{
		LiveData::CObject* obj = m_Accesser.Get(_uuid);
		if(!obj)
		{
			ZION_ASSERT(0);
			return;
		}
		const DDLReflect::STRUCT_INFO* info = obj->GetStructInfo();
		A_LIVE_OBJECT* data = (A_LIVE_OBJECT*)DDLReflect::CreateObject(info);
		if(!data)
		{
			ZION_ASSERT(0);
			return;
		}
		String xjson = json;
		if(!DDLReflect::Json2Struct(info, xjson, (_U8*)data))
		{
			ZION_ASSERT(0);
			return;
		}
		memcpy(obj->GetData(), data, info->size);
	}

	void CDataSyncClient::DS_UpdateObject(const A_UUID& _uuid, const _U8* buf, _U32 len)
	{
		LiveData::CObject* obj = m_Accesser.Get(_uuid);
		if(!obj)
		{
			ZION_ASSERT(0);
			return;
		}
		const DDLReflect::STRUCT_INFO* info = obj->GetStructInfo();
		A_LIVE_OBJECT* data = (A_LIVE_OBJECT*)DDLReflect::CreateObject(info);
		if(!data)
		{
			ZION_ASSERT(0);
			return;
		}
		DDL::MemoryReader reader(buf, len);
		if(!info->read_proc(reader, data))
		{
			ZION_ASSERT(0);
			return;
		}
		memcpy(obj->GetData(), data, info->size);
	}

	void CDataSyncClient::DS_DeleteObject(const A_UUID* _uuids, _U32 count)
	{
		for(_U32 i=0; i<count; i++)
		{
			m_Accesser.Remove(_uuids[i]);
		}
	}

	void CDataSyncClient::Clear()
	{
		while(!m_NewQ.empty())
		{
			DDLReflect::DestoryObject(m_NewQ.front().info, m_NewQ.front().obj);
			m_NewQ.pop_front();
		}

		while(!m_WatQ.empty())
		{
			DDLReflect::DestoryObject(m_WatQ.front().info, m_WatQ.front().obj);
			m_WatQ.pop_front();
		}

		m_DelList.clear();
		m_Accesser.Clear();
	}

}
