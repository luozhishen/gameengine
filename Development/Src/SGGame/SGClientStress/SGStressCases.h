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

			void AwardLeagueBattleResult(_U8 ret, _U32 type, _U32 level, _U32 exp_addition, _U32 exp, _U32 gold, _U32 rmb, _U32 reputation, _U32 contribution, const SG_DROP_ITEM_CONFIG* items, _U32 count)
			{
			}

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
