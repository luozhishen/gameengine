#include <ZionBase.h>
#include <ZionCommon.h>
#include <ZionClient.h>

#include <StressCase.h>
#include <StressClient.h>

#include <CardClient.h>
#include "CardStressCases.h"

namespace Zion
{
	namespace StressCases
	{

		CEnterGame::CEnterGame() : TCardStressCase<STRESSCASE_ENTERGAME_CONFIG, STRESSCASE_ENTERGAME_STATUS>("EnterGame")
		{
			m_Config.server_id = 1;
			m_Config.avatar_name = "\0";
		}

		CEnterGame::~CEnterGame()
		{
		}

		void CEnterGame::OnAttach()
		{
			GetClient()->_OnLoginDone.connect(this, &CEnterGame::OnLoginDone);
			GetClient()->s2c_base._GetAvatarListCallback.connect(this, &CEnterGame::OnGetAvatarListCallback);
			GetClient()->s2c_base._EnterGameCallback.connect(this, &CEnterGame::OnEnterGameCallback);
		}

		void CEnterGame::OnDetach()
		{
		}

		void CEnterGame::OnTick()
		{
		}

		void CEnterGame::OnLoginDone()
		{
			GetClient()->c2s_base.SelectServer(m_Config.server_id);
			GetClient()->c2s_base.GetAvatarList();
		}

		void CEnterGame::OnGetAvatarListCallback(const CG_AVATAR_DESC* desc, _U32 count)
		{
			if(count!=0)
			{
				GetClient()->c2s_base.EnterGame(desc->avatar_id);
				return;
			}

			String avatar_name;
			if(strcmp(m_Config.avatar_name._Value, "")==0)
			{
				avatar_name = StringFormat("AV%p", this);
			}
			GetClient()->c2s_base.CreateAvatar(avatar_name.c_str());
		}

		void CEnterGame::OnCreateAvatarCallback(_U32 errcode)
		{
			GetClient()->c2s_base.GetAvatarList();
		}

		void CEnterGame::OnEnterGameCallback(_U32 errcode)
		{
			if(errcode==0)
			{
				m_Status.sucess_times += 1;
			}
			else
			{
				m_Status.failed_times += 1;
				GetClient()->Logout();
			}
		}

		CStressCase* CEnterGame::Create()
		{
			return ZION_NEW CEnterGame();
		}

	}
}
