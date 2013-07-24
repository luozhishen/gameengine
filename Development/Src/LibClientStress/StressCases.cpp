#include <ZionBase.h>
#include <ZionCommon.h>
#include <ZionClient.h>

#include "StressCase.h"
#include "StressClient.h"
#include "StressCases.h"

namespace Zion
{
	namespace StressCases
	{

		CLogin::CLogin() : TStressCase<STRESSCASE_LOGIN_CONFIG>("Login")
		{
		}

		CLogin::~CLogin()
		{
		}
		
		void CLogin::OnTick()
		{
		}

		void CLogin::OnLoginFailed()
		{
		}

		void CLogin::OnLoginDone()
		{
		}

		void CLogin::OnDisconnected()
		{
		}

		CStressCase* CLogin::Create()
		{
			return ZION_NEW CLogin();
		}

		void CLogin::_GetInfo(std::string& info)
		{
		}

	}
}