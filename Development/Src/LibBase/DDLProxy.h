#ifndef __DDL_PROXY_INCLUDE__
#define __DDL_PROXY_INCLUDE__

namespace DDLProxy
{

	class IClient
	{
	public:
		virtual ~IClient() { }
		virtual bool SendData(_U16 iid, _U16 fid, _U32 len, const _U8* data) = 0;
	};

	template<_U32 BUF_SIZE>
	class DDLProxy
	{
	public:
		DDLProxy(IClient* Client, _U16 cid)
		{
			_Client = Client;
			_cid = cid;
		}

		IClient* GetClient()
		{
			return _Client;
		}

		_U16 GetClassID()
		{
			return _cid;
		}

		DDL::TMemoryWriter<BUF_SIZE> _Buf;

	private:
		IClient* _Client;
		_U16 _cid;
	};

}

#endif
