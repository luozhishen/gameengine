#include <ZionBase.h>
#include <ZionBase.h>
#include <ZionCommon.h>
#include "GameDDL.h"
#include "GameObject.h"

namespace Zion
{

	CGameObject::CGameObject(const DDLReflect::STRUCT_INFO* pLiveType, const DDLReflect::STRUCT_INFO* pContentType)
	{
		if(pContentType)
		{
			ZION_ASSERT(DDLReflect::IsParent(pLiveType, DDLReflect::GetStruct<A_GAME_OBJECT>()));
			ZION_ASSERT(DDLReflect::IsParent(pContentType, DDLReflect::GetStruct<A_CONTENT_OBJECT>()));
		}
		else
		{
			ZION_ASSERT(DDLReflect::IsParent(pLiveType, DDLReflect::GetStruct<A_LIVE_OBJECT>()));
		}
		m_pLiveType = pLiveType;
		m_pContentType = pContentType;
		m_pLiveData = NULL;
		m_pContentData = NULL;
	}

	CGameObject::CGameObject(const DDLReflect::STRUCT_INFO* pLiveType, const A_LIVE_OBJECT* pLiveData, const DDLReflect::STRUCT_INFO* pContentType, const A_CONTENT_OBJECT* pContentData)
	{
		if(pContentType)
		{
			ZION_ASSERT(DDLReflect::IsParent(pLiveType, DDLReflect::GetStruct<A_GAME_OBJECT>()));
			ZION_ASSERT(DDLReflect::IsParent(pContentType, DDLReflect::GetStruct<A_CONTENT_OBJECT>()));
		}
		else
		{
			ZION_ASSERT(DDLReflect::IsParent(pLiveType, DDLReflect::GetStruct<A_LIVE_OBJECT>()));
		}
		m_pLiveType = pLiveType;
		m_pLiveData = pLiveData;
		m_pContentType = pContentType;
		m_pContentData = pContentData;
	}

	CGameObject::~CGameObject()
	{
	}

	bool CGameObject::IsValid()
	{
		if(!m_pLiveType || !m_pLiveData) return false;
		if((m_pContentType==NULL)!=(m_pContentData==NULL)) return false;
		return true;
	}

	void CGameObject::Init(IGameObjectContainer* pContainer, const A_UUID& _uuid)
	{
		m_pLiveData = NULL;
		m_pContentData = NULL;

		const DDLReflect::STRUCT_INFO* pType;
		const A_LIVE_OBJECT* pLiveData;
		ZION_ASSERT(pContainer->GetGameObject(_uuid, pType, pLiveData));
		if(!pContainer->GetGameObject(_uuid, pType, pLiveData)) return;
		ZION_ASSERT(DDLReflect::IsParent(pType, GetLiveType()));
		if(!DDLReflect::IsParent(pType, GetLiveType())) return;

		if(m_pContentType)
		{
			m_pContentData = ContentObject::QueryByUUID(((const A_GAME_OBJECT*)pLiveData)->content_uuid, m_pContentType);
			ZION_ASSERT(m_pContentData);
			if(!m_pContentData) return;
		}
		m_pLiveData = pLiveData;
	}

	const DDLReflect::STRUCT_INFO* CGameObject::GetLiveType()
	{
		return m_pLiveType;
	}

	const A_LIVE_OBJECT* CGameObject::GetLiveData()
	{
		return m_pLiveData;
	}

	const DDLReflect::STRUCT_INFO* CGameObject::GetContentType()
	{
		return m_pContentType;
	}

	const A_CONTENT_OBJECT* CGameObject::GetContentData()
	{
		return m_pContentData;
	}

}
