#include <ZionBase.h>
#include "CommonDDL.h"
#include "LiveData.h"

namespace Zion
{
	namespace LiveData
	{

		CMonitor::CMonitor(CManagerBase* pManager, const DDLReflect::STRUCT_INFO* info)
		{
			m_pManager = pManager;
			m_pInfo = info;
			m_bDirty = false;
		}

		bool CMonitor::IsDirty()
		{
			return m_bDirty;
		}

		void CMonitor::SetDirty()
		{
			m_bDirty = true;
			m_pManager->SetLiveDataChanged();
		}

		void CMonitor::SetDirty(const char* name, _U32 offset, _U32 size)
		{
			m_bDirty = true;
			m_pManager->SetLiveDataChanged();
		}

		void CMonitor::Clean()
		{
			m_bDirty = false;
		}

		CObject::CObject(CManagerBase* pManager, const DDLReflect::STRUCT_INFO* pInfo, A_LIVE_OBJECT* pData) : m_Monitor(pManager, pInfo), DDLDataObject::CObject(&m_Monitor, pInfo, pData, "", 0)
		{
		}

		bool CObject::IsDirty()
		{
			return m_Monitor.IsDirty();
		}

		void CObject::Clean()
		{
			return m_Monitor.Clean();
		}

		CManagerBase::CManagerBase()
		{
		}

		void CManagerBase::InitRandom(_U32 seed)
		{
			m_LastNum = seed;
		}

		_F32 CManagerBase::Rand()
		{
			String val = CHashMD5::CalcToString(&m_LastNum, sizeof(m_LastNum));
			_U32 m_LastNum = CalcCRC32(val.c_str(), (_U32)val.size());
			return ((_F32)(m_LastNum & 0xffffff)) / 0xffffff;
		}

		void CManagerBase::EnableMonitor(bool bEnable)
		{
			m_bEnableMonitor = bEnable;
		}

		bool CManagerBase::IsEnableMointor()
		{
			return m_bEnableMonitor;
		}

		CObject* CManagerBase::Append(const DDLReflect::STRUCT_INFO* pInfo)
		{
			A_UUID _uuid;
			AUuidGenerate(_uuid);
			if(m_ObjMap.find(_uuid)==m_ObjMap.end())
			{
				ZION_ASSERT(!"live object generate duplicate uuid");
				return NULL;
			}

			A_LIVE_OBJECT* data = (A_LIVE_OBJECT*)DDLReflect::CreateObject(pInfo);
			data->_uuid = _uuid;
			CObject* obj = ZION_NEW CObject(this, pInfo, data);
			m_ObjMap[data->_uuid] = obj;
			return NULL;
		}

		CObject* CManagerBase::Append(const DDLReflect::STRUCT_INFO* pInfo, A_LIVE_OBJECT* data)
		{
			if(m_ObjMap.find(data->_uuid)==m_ObjMap.end())
			{
				ZION_ASSERT(!"live object alread existed");
				return NULL;
			}

			CObject* obj = ZION_NEW CObject(this, pInfo, data);
			obj->Clean();
			m_ObjMap[data->_uuid] = obj;
			return NULL;
		}

		CObject* CManagerBase::Append(const DDLReflect::STRUCT_INFO* pInfo, const _U8* buf, _U32 len)
		{
			A_LIVE_OBJECT* data = (A_LIVE_OBJECT*)DDLReflect::CreateObject(pInfo);
			DDL::MemoryReader reader(buf, len);
			if(!pInfo->read_proc(reader, data))
			{
				ZION_ASSERT(!"live object serialize binary error");
				DDLReflect::DestoryObject(pInfo, data);
				return NULL;
			}

			if(m_ObjMap.find(data->_uuid)==m_ObjMap.end())
			{
				ZION_ASSERT(!"live object alread existed");
				DDLReflect::DestoryObject(pInfo, data);
				return NULL;
			}

			CObject* obj = ZION_NEW CObject(this, pInfo, data);
			obj->Clean();
			m_ObjMap[data->_uuid] = obj;
			return NULL;
		}

		CObject* CManagerBase::Append(const DDLReflect::STRUCT_INFO* pInfo, const char* str)
		{
			A_LIVE_OBJECT* data = (A_LIVE_OBJECT*)DDLReflect::CreateObject(pInfo);
			String val(str);
			if(!DDLReflect::Struct2Json(pInfo, (_U8*)data, val))
			{
				ZION_ASSERT(!"live object serialize json error");
				DDLReflect::DestoryObject(pInfo, data);
				return NULL;
			}

			if(m_ObjMap.find(data->_uuid)==m_ObjMap.end())
			{
				ZION_ASSERT(!"live object alread existed");
				DDLReflect::DestoryObject(pInfo, data);
				return NULL;
			}

			CObject* obj = ZION_NEW CObject(this, pInfo, data);
			obj->Clean();
			m_ObjMap[data->_uuid] = obj;
			return NULL;
		}

		bool CManagerBase::Remove(const A_UUID& _uuid)
		{
			Map<A_UUID, CObject*>::iterator o = m_ObjMap.find(_uuid);
			if(o==m_ObjMap.end())
			{
				ZION_ASSERT(!"object not found");
				return false;
			}
			else
			{
				m_ObjMap.erase(o);
				return true;
			}
		}

		void CManagerBase::Clear()
		{
			Map<A_UUID, CObject*>::iterator i;
			for(i=m_ObjMap.begin(); i!=m_ObjMap.end(); i++)
			{
				ZION_DELETE i->second;
			}
			m_ObjMap.clear();
		}

		CObject* CManagerBase::Get(const A_UUID& _uuid)
		{
			Map<A_UUID, CObject*>::iterator o = m_ObjMap.find(_uuid);
			if(o==m_ObjMap.end()) return NULL;
			return o->second;
		}

		CObject* CManagerBase::FindFirst()
		{
			return m_ObjMap.begin()->second;
		}

		CObject* CManagerBase::FindNext(CObject* obj)
		{
			A_LIVE_OBJECT* data = (A_LIVE_OBJECT*)obj->GetData();
			Map<A_UUID, CObject*>::iterator i = m_ObjMap.find(data->_uuid);
			if(i==m_ObjMap.end() || (++i)==m_ObjMap.end())
			{
				ZION_ASSERT(!"error");
				return NULL;
			}
			return i->second;
		}

		bool CManagerBase::IsLiveDataChanged()
		{
			return m_bIsLiveDataChanged;
		}

		void CManagerBase::SetLiveDataChanged()
		{
			m_bIsLiveDataChanged = true;
		}

		void CManagerBase::ClearLiveDataChanged()
		{
			m_bIsLiveDataChanged = false;
		}

		CRequestClient::CRequestClient(CManager* pManager)
		{
			m_pManager = pManager;
		}

		CRequestClient::~CRequestClient()
		{

		}

		bool CRequestClient::SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
		{
			if(!m_pManager) return false;
			return m_pManager->SendRequestData(iid, fid, len, data);
		}

		CResponseClient::CResponseClient(CManager* pManager)
		{
			m_pManager = pManager;
		}

		CResponseClient::~CResponseClient()
		{

		}

		bool CResponseClient::SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
		{
			if(!m_pManager) return false;
			return m_pManager->SendResponseData(iid, fid, len, data);
		}

		CManager::CManager()
			: m_RequestClient(this), m_ResponseClient(this)
		{
			m_pRequestClientImpl = NULL;
			m_pResponseStubImpl = NULL;
		}

		bool CManager::RegisterRequestStub(_U16 iid, DDLStub::IStub* pStub)
		{
			return m_RequestClient.RegisterStub(iid, pStub);
		}

		bool CManager::SendRequestData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
		{
			ClearLiveDataChanged();

			if(!m_RequestClient.Dispatch(iid, fid, len, data))
			{
				return false;
			}

			if(!IsLiveDataChanged())
			{
				return true;
			}

			if(!m_pRequestClientImpl) return true;
			return m_pRequestClientImpl->SendData(iid, fid, len, data);
		}

		bool CManager::SendResponseData(_U16 iid, _U16 fid, _U32 len, const _U8* data)
		{
			if(!m_pResponseStubImpl) return true;
			return m_pResponseStubImpl->Dispatch(iid, fid, len, data);
		}

		bool CManager::HasRequestClient()
		{
			return m_pRequestClientImpl!=NULL;
		}

		bool CManager::HasResponseClient()
		{
			return m_pResponseClientImpl!=NULL;
		}

		bool CManager::HasResponseStub()
		{
			return m_pResponseStubImpl!=NULL;
		}

		void CManager::SetRequestClient(DDLProxy::IClient* pRquestClientImpl)
		{
			m_pRequestClientImpl = pRquestClientImpl;
		}

		void CManager::SetResponseClient(DDLProxy::IClient* pResponseClientImpl)
		{
			m_pResponseClientImpl = pResponseClientImpl;
		}

		void CManager::SetResponseStub(DDLStub::CDispatcher* pResponseStubImpl)
		{
			m_pResponseStubImpl = pResponseStubImpl;
		}

		DDLProxy::IClient* CManager::GetRequestClient()
		{
			return m_pRequestClientImpl;
		}

		DDLProxy::IClient* CManager::GetResponseClient()
		{
			return m_pResponseClientImpl;
		}

		DDLStub::CDispatcher* CManager::GetResponseStub()
		{
			return m_pResponseStubImpl;
		}

	}
}
