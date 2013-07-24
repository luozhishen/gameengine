#ifndef __STRESS_CASES__
#define __STRESS_CASES__

#include "StressCaseConfig.h"

namespace Zion
{
	namespace StressCases
	{

		class CLogin : public TStressCase<STRESSCASE_LOGIN_CONFIG>
		{
		public:
			CLogin();
			virtual ~CLogin();

			virtual void OnTick();

			void OnLoginFailed();
			void OnLoginDone();
			void OnDisconnected();

		protected:
			virtual CStressCase* Create();
			virtual void _GetInfo(std::string& info);

		private:
			_U32 m_retry_time;
			_U32 m_reset_time;
		};

	}
}

#endif  // __STRESS_CASES__
