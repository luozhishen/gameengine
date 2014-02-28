#ifndef __DDL_STUB_INCLUDE__
#define __DDL_STUB_INCLUDE__

namespace DDLStub
{

	class IStub
	{
	public:
		IStub()
		{
		}

		virtual ~IStub()
		{
		}

		
		virtual const ::DDLReflect::CLASS_INFO* GetClassInfo() = 0;
		virtual bool Dispatcher(_U16 fid, DDL::BufferReader& Buf) = 0;
	};

	template<class CLASS>
	class DDLStub : public IStub
	{
	public:
		DDLStub(CLASS* Class)
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
