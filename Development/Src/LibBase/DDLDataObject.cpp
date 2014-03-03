#include "ZionBase.h"

namespace DDLDataObject
{

	CConstObject::CConstObject()
	{
		m_pInfo = NULL;
		m_pData = NULL;
	}

	CConstObject::CConstObject(const DDLReflect::STRUCT_INFO* info, void* data, _U32 offset)
	{
		m_pInfo = info;
		m_pData = data;
		m_Offset = offset;
	}

	CConstObject::CConstObject(const CConstObject* pObject)
	{
		m_pInfo = pObject->m_pInfo;
		m_pData = pObject->m_pData;
		m_Offset = pObject->m_Offset;
	}

	CConstObject::CConstObject(const CObject* pObject)
	{
		m_pInfo = pObject->m_pInfo;
		m_pData = pObject->m_pData;
		m_Offset = pObject->m_Offset;
	}

	CConstObject CConstObject::Ref(const char* name) const
	{
		return CConstObject();
	}

	const void* CConstObject::GetData()
	{
		return (char*)m_pData + m_Offset;
	}

	const DDLReflect::STRUCT_INFO* CConstObject::GetStructInfo()
	{
		return m_pInfo;
	}

	CObject::CObject()
	{
		m_pMonitor = NULL;
		m_Offset = (_U32)-1;
	}

	CObject::CObject(IMonitor* pMonitor, const DDLReflect::STRUCT_INFO* info, void* data, const char* name, _U32 offset) : CConstObject(info, data, offset)
	{
		m_pMonitor = pMonitor;
		m_Name = name;
	}

	CObject::CObject(IMonitor* pMonitor, const DDLReflect::STRUCT_INFO* info, void* data, const Zion::String& name, _U32 offset) : CConstObject(info, data, offset)
	{
		m_pMonitor = pMonitor;
		m_Name = name;
	}

	CObject::CObject(const DDLReflect::STRUCT_INFO* info, void* data, _U32 offset) : CConstObject(info, data, offset)
	{
		m_pMonitor = NULL;
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

	void* CObject::GetData()
	{
		return (char*)m_pData + m_Offset;
	}

}
