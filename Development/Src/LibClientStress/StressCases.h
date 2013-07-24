#ifndef __STRESS_CASES__
#define __STRESS_CASES__

#include "StressCaseConfig.h"

namespace Zion
{
	namespace StressCases
	{

		class CLogin : public TStressCase<STRESSCASE_LOGIN_CONFIG, STRESSCASE_LOGIN_STATUS>
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

		private:
			_U32 m_retry_time;
			_U32 m_reset_time;
			_U32 m_disconnect_time;
		};

	}
}

#endif  // __STRESS_CASES__
