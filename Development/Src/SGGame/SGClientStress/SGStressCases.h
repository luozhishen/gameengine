#ifndef __SGSTRESS_CASES__
#define __SGSTRESS_CASES__

namespace Zion
{
	namespace StressCases
	{
		class CEcho : public Zion::CStressCase
		{
		public:
			CEcho();
			virtual ~CEcho();

			virtual void OnTick();

		protected:
			virtual CStressCase* Create();
		};
	}
}

#endif
