#ifndef __CARDSTRESS_CASES__
#define __CARDSTRESS_CASES__

#include "CardStressCaseConfig.h"

namespace Zion
{

	class CCardClient;

	namespace StressCases
	{

		template <typename T, typename S>
		class TCardStressCase : public TStressCase<T, S>
		{
		public:
			TCardStressCase(const char* Name) : TStressCase<T, S>(Name)
			{
			}

			~TCardStressCase()
			{
			}

			Zion::CCardClient* GetClient()
			{
				return (Zion::CCardClient*)TStressCase<T, S>::GetClient();
			}
		};


		class CEcho : public TCardStressCase<STRESSCASE_ECHO_CONFIG, STRESSCASE_ECHO_STATUS>
		{
		public:
			CEcho();
			virtual ~CEcho();

			virtual void OnTick();

		protected:
			virtual CStressCase* Create();
		};

		class CEnterServer : public TCardStressCase<STRESSCASE_ENTERSERVER_CONFIG, STRESSCASE_ENTERSERVER_STATUS>
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
