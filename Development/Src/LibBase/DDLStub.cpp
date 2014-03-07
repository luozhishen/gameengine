#include "ZionBase.h"

namespace DDLStub
{

	CDispatcher::CDispatcher()
	{
	}

	bool CDispatcher::HasStub(_U16 iid)
	{
		return m_DDLStubs.find(iid)!=m_DDLStubs.end();
	}

	bool CDispatcher::RegisterStub(_U16 iid, IStub* pStub)
	{
		ZION_ASSERT(m_DDLStubs.find(iid)==m_DDLStubs.end());
		if(m_DDLStubs.find(iid)!=m_DDLStubs.end()) return false;
		m_DDLStubs[iid] = pStub;
		return true;
	}

	bool CDispatcher::Dispatch(_U16 iid, _U16 fid, _U32 len, const _U8* data)
	{
		Zion::Map<_U16, IStub*>::iterator i;
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
