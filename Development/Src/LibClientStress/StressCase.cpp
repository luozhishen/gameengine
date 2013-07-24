#include <ZionBase.h>
#include <ZionCommon.h>
#include <ZionClient.h>

#include "StressCase.h"
#include "StressClient.h"
#include "StressManager.h"

namespace Zion
{

	CStressCase::CStressCase(const char* name)
	{
		m_pClient = NULL;
		m_strName = name;
	}

	CStressCase::~CStressCase()
	{
		assert(m_pClient==NULL);
	}

	CClient* CStressCase::GetClient()
	{
		return m_pClient->GetClient();
	}

	const Zion::String CStressCase::GetStatusInfo()
	{
		const DDLReflect::STRUCT_INFO* pType = GetStatusType();
		if(!pType) return "";

		Zion::String json;
		DDLReflect::Struct2Json(pType, GetStatusData(), json);
		return json;
	}

	void CStressCase::Attach(CStressClient* pClient)
	{
		assert(m_pClient==NULL);
		m_pClient = pClient;
		OnAttach();
	}

	void CStressCase::Detach(CStressClient* pClient)
	{
		assert(m_pClient==pClient);
		OnDetach();
		m_pClient = NULL;
	}

}
