#ifndef __ZION_DDL_DISPATCHER__
#define __ZION_DDL_DISPATCHER__

namespace Zion
{

	class CDDLDispatcher
	{
	public:
		CDDLDispatcher();

		bool HasStub(_U16 iid);
		void RegisterStub(_U16 iid, DDLStub::IStub* pStub);
		bool Dispatch(_U16 iid, _U16 fid, _U32 len, const _U8* data);

	private:
		Map<_U16, DDLStub::IStub*> m_DDLStubs;
	};

}

#endif
