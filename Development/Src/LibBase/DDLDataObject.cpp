#include "ZionBase.h"

namespace DDLDataObject
{

	CConstObject::CConstObject()
	{
		m_pInfo = NULL;
		m_pData = NULL;
	}

	CConstObject::CConstObject(const DDLReflect::STRUCT_INFO* info, void* data)
	{
		m_pInfo = info;
		m_pData = data;
	}

	CConstObject::CConstObject(const CConstObject* pObject)
	{
		m_pInfo = pObject->m_pInfo;
		m_pData = pObject->m_pData;
	}

	CConstObject::CConstObject(const CObject* pObject)
	{
		m_pInfo = pObject->m_pInfo;
		m_pData = pObject->m_pData;
	}

	CConstObject CConstObject::Ref(const char* name) const
	{
		return CConstObject();
	}

	CObject::CObject()
	{
		m_pMonitor = NULL;
		m_Offset = (_U32)-1;
	}

	CObject::CObject(const DDLReflect::STRUCT_INFO* info, void* data, IMonitor* pMonitor, const char* name, _U32 offset) : CConstObject(info, data)
	{
		m_pMonitor = pMonitor;
		m_Name = name;
		m_Offset = offset;
	}

	CObject::CObject(const DDLReflect::STRUCT_INFO* info, void* data, IMonitor* pMonitor, const Zion::String& name, _U32 offset) : CConstObject(info, data)
	{
		m_pMonitor = pMonitor;
		m_Name = name;
		m_Offset = offset;
	}

	CObject::CObject(const DDLReflect::STRUCT_INFO* info, void* data) : CConstObject(info, data)
	{
		m_pMonitor = NULL;
		m_Offset = (_U32)-1;
	}

	CObject::CObject(const CObject* pObject) :  CConstObject(pObject)
	{
		m_pMonitor = pObject->m_pMonitor;
		m_Name = pObject->m_Name;
		m_Offset = pObject->m_Offset;
	}

	const CObject CObject::Ref(const char* name)
	{
		return CObject();
	}

}
