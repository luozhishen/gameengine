#ifndef __ZION_DATA_CACHE_AVATAR_DATA__
#define __ZION_DATA_CACHE_AVATAR_DATA__

namespace Zion
{
	namespace DataCache
	{

		class CAvatarDataRequest
		{
		public:
			CAvatarDataRequest();
			virtual ~CAvatarDataRequest();

			CAvatarDataRequest* m_pPrev;
			CAvatarDataRequest* m_pNext;

			virtual void Execute() = 0;
		};

		class CAvatarData
		{
		public:
			CAvatarData();
			~CAvatarData();

		private:
			CAvatarDataRequest* m_pRequestFirst;
		};

	}
}

#endif
