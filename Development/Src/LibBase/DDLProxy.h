#ifndef __DDL_PROXY_INCLUDE__
#define __DDL_PROXY_INCLUDE__

namespace DDLProxy
{

	template<typename CLIENT, typename BUFFER>
	class DDLProxy
	{
	public:
		DDLProxy(CLIENT* Client, _U16 cid)
		{
			_Client = Client;
			_cid = cid;
		}

		CLIENT* GetClient()
		{
			return _Client;
		}

		_U16 GetClassID()
		{
			return _cid;
		}

	private:
		CLIENT* _Client;
		_U16 _cid;
	};

}

#endif
