#ifndef __ZION_LIVE_ACCESSER__
#define __ZION_LIVE_ACCESSER__

namespace Zion
{
	namespace LiveData
	{

		class CAccesser : public CStubDispatcher
		{
		public:
			CAccesser(CManager* pManager);

			void InitRandom(_U32 seed);
			_F32 Rand(); // return 0~1.0

		private:
			_U32 m_LastNum;
			CManager* m_pManager;
		};

	}
}

#endif
