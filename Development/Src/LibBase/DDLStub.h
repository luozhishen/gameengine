#ifndef __DDL_STUB_INCLUDE__
#define __DDL_STUB_INCLUDE__

#include "DDL.h"

namespace DDLStub
{

	template<typename CALLER>
	class DDLStubBase;
	template<typename CALLER, class CLASS>
	class DDLStub;

	template<typename CALLER>
	class DDLStubBase
	{
	public:
		DDLStubBase()
		{
		}

		~DDLStubBase()
		{
		}

		virtual bool Dispatcher(CALLER* Caller, _U16 fid, DDL::BufferReader& Buf) = 0;
	};

	template<typename CALLER, class CLASS>
	class DDLStub : public DDLStubBase<CALLER>
	{
	public:
		DDLStub(CLASS* Class) : DDLStubBase<CALLER>()
		{
			_Class = Class;
		}

		CLASS* GetClass()
		{
			return _Class;
		}

	private:
		CLASS* _Class;
	};

}

#endif
