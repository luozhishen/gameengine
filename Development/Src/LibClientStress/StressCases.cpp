#include <ZionBase.h>
#include <ZionCommon.h>
#include <ZionClient.h>

#include "StressCase.h"
#include "StressClient.h"
#include "StressCases.h"

#include <time.h>

namespace Zion
{
	namespace StressCases
	{

		CLogin::CLogin() : TStressCase<STRESSCASE_LOGIN_CONFIG, STRESSCASE_LOGIN_STATUS>("Login")
		{
			m_retry_time = 0;
			m_disconnect_time = 0;
		}

		CLogin::~CLogin()
		{
		}

		void CLogin::OnAttach()
		{
			GetClient()->_OnLoginFailed.connect(this, &CLogin::OnLoginFailed);
			GetClient()->_OnLoginDone.connect(this, &CLogin::OnLoginDone);
			GetClient()->_OnDisconnected.connect(this, &CLogin::OnDisconnected);
		}

		void CLogin::OnDetach()
		{
			GetClient()->_OnLoginFailed.disconnect(this);
			GetClient()->_OnLoginDone.disconnect(this);
			GetClient()->_OnDisconnected.disconnect(this);
		}

		void CLogin::OnTick()
		{
			if(GetClient()->GetState()==CClient::STATE_NA || GetClient()->GetState()==CClient::STATE_FAILED)
			{
				if(m_retry_time==0)
				{
					m_retry_time = (_U32)time(NULL) + m_Config.retry_time;
				}
				if(m_retry_time<=(_U32)time(NULL))
				{
					GetStressClient()->Login();
					m_retry_time = 0;
				}
			}
			if(GetClient()->GetState()==CClient::STATE_LOGINED)
			{
				if(m_Config.disconnect_time!=0)
				{
					if(m_disconnect_time==0)
					{
						m_disconnect_time = (_U32)time(NULL) + m_Config.disconnect_time;
					}
					if(m_disconnect_time<=(_U32)time(NULL))
					{
						GetClient()->Logout();
						m_disconnect_time = 0;
					}
				}
			}
		}

		void CLogin::OnLoginFailed()
		{
			m_Status.failed_times++;
		}

		void CLogin::OnLoginDone()
		{
			if(m_Config.disconnect_time)
			{
				m_disconnect_time = (_U32)time(NULL) + m_Config.disconnect_time;
			}
			m_Status.sucess_time++;
		}

		void CLogin::OnDisconnected()
		{
		}

		CStressCase* CLogin::Create()
		{
			return ZION_NEW CLogin();
		}

	}
}