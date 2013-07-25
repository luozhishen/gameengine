#ifndef __SGSTRESS_CASES__
#define __SGSTRESS_CASES__

#include "SGStressCaseConfig.h"

namespace Zion
{

	class CSGClient;

	namespace StressCases
	{

		template <typename T, typename S>
		class TSGStressCase : public TStressCase<T, S>
		{
		public:
			TSGStressCase(const char* Name) : TStressCase<T, S>(Name)
			{
			}

			~TSGStressCase()
			{
			}

			Zion::CSGClient* GetClient()
			{
				return (Zion::CSGClient*)TStressCase<T, S>::GetClient();
			}
		};


		class CEcho : public TSGStressCase<STRESSCASE_ECHO_CONFIG, STRESSCASE_ECHO_STATUS>
		{
		public:
			CEcho();
			virtual ~CEcho();

			virtual void OnTick();

		protected:
			virtual CStressCase* Create();
		};

		class CEnterServer : public TSGStressCase<STRESSCASE_ENTERSERVER_CONFIG, STRESSCASE_ENTERSERVER_STATUS>
		{
		public:
			CEnterServer();
			virtual ~CEnterServer();

			virtual void OnTick();

		protected:
			virtual CStressCase* Create();
		};

	}
}

#endif
