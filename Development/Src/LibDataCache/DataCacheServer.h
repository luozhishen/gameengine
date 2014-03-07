#ifndef __ZION_DATA_CACHE_SERVER__
#define __ZION_DATA_CACHE_SERVER__

namespace Zion
{
	namespace DataCache
	{

		class CServer : public IServer
		{
		public:
			CServer();
			virtual ~CServer();

			virtual void GetAvatar(const JSONRPC_RESPONSE& res, _U32 avatar_id);
			virtual void KeepAlive(const JSONRPC_RESPONSE& res, _U32 avatar_id);
			virtual void ExecuteDDL(const JSONRPC_RESPONSE& res, _U32 avatar_id, const char* method, const char* data);
		};

	}
}

#endif
