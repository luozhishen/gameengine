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

			virtual void OnAttach();
			virtual void OnDetach();
			virtual void OnTick();

			void OnLoginDone();
			void OnGetServerListResult(const SG_SERVER_INFO* infos, _U32 count, _U32 last_server);
			void OnQueryAvatarFailed(_U32 errcode);
			void OnQueryAvatarResult(const SG_PLAYER& player);

		protected:
			virtual CStressCase* Create();

		private:
			_U32 m_retry_time;
			_U32 m_reset_time;
			_U32 m_disconnect_time;
		};

	}
}

#endif
