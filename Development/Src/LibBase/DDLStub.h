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

	class CDispatcher
	{
	public:
		CDispatcher();

		bool HasStub(_U16 iid);
		bool RegisterStub(_U16 iid, IStub* pStub);
		bool Dispatch(_U16 iid, _U16 fid, _U32 len, const _U8* data);

	private:
		Zion::Map<_U16, IStub*> m_DDLStubs;
	};

}

#endif
