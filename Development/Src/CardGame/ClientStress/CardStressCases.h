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

		class CEnterGame : public TCardStressCase<STRESSCASE_ENTERGAME_CONFIG, STRESSCASE_ENTERGAME_STATUS>
		{
		public:
			CEnterGame();
			virtual ~CEnterGame();

			virtual void OnAttach();
			virtual void OnDetach();
			virtual void OnTick();

			void OnLoginDone();
			void OnGetAvatarListCallback(const CG_AVATAR_DESC* desc, _U32 count);
			void OnCreateAvatarCallback(_U32 errcode);
			void OnEnterGameCallback(_U32 errcode);

		protected:
			virtual CStressCase* Create();
		};

	}
}

#endif
