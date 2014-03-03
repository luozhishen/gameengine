#include <ZionBase.h>
#include "DDLDispatcher.h"

namespace Zion
{

	CDDLDispatcher::CDDLDispatcher()
	{
	}

	bool CDDLDispatcher::HasStub(_U16 iid)
	{
		return m_DDLStubs.find(iid)!=m_DDLStubs.end();
	}

	void CDDLDispatcher::RegisterStub(_U16 iid, DDLStub::IStub* pStub)
	{
		ZION_ASSERT(m_DDLStubs.find(iid)==m_DDLStubs.end());
		m_DDLStubs[iid] = pStub;
	}

	bool CDDLDispatcher::Dispatch(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		Zion::Map<_U16, DDLStub::IStub*>::iterator i;
		i = m_DDLStubs.find(iid);
		if(i==m_DDLStubs.end())
		{
			ZION_ASSERT(!"invalid iid");
			return false;
		}

		DDL::MemoryReader buf(data, len);
		if(!i->second->Dispatcher(fid, buf))
		{
			ZION_ASSERT(!"invalid data");
			return false;
		}

		return true;
	}

}
