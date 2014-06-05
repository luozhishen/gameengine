#ifndef __ZION_GAME_OBJECT__
#define __ZION_GAME_OBJECT__

namespace Zion
{

	class CGameObject
	{
	public:
		CGameObject();
		virtual ~CGameObject();
	};

/*
	template<typename LIVE, typename CONTENT>
	class TGameObject
	{
	public:
		TGameObject();
		virtual ~TGameObject();

		LIVE* m_pLive;
		CONTENT* m_pContent;
	};
*/

}

#endif
