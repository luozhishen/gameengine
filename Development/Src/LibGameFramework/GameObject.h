#ifndef __ZION_GAME_OBJECT__
#define __ZION_GAME_OBJECT__

namespace Zion
{

	class IGameObjectContainer
	{
	public:
		virtual ~IGameObjectContainer() {}
		virtual bool GetGameObject(const A_UUID& _uuid, const DDLReflect::STRUCT_INFO*& pType, const A_LIVE_OBJECT*& pData) = 0;
	};

	class CGameObject
	{
	public:
		CGameObject(const DDLReflect::STRUCT_INFO* pLiveType, const DDLReflect::STRUCT_INFO* pContentType);
		CGameObject(const DDLReflect::STRUCT_INFO* pLiveType, const A_LIVE_OBJECT* pLiveData, const DDLReflect::STRUCT_INFO* pContentType, const A_CONTENT_OBJECT* pContentData);
		virtual ~CGameObject();

		bool IsValid();
		void Init(IGameObjectContainer* pContainer, const A_UUID& _uuid);

		const DDLReflect::STRUCT_INFO* GetLiveType();
		const A_LIVE_OBJECT* GetLiveData();
		const DDLReflect::STRUCT_INFO* GetContentType();
		const A_CONTENT_OBJECT* GetContentData();

	private:
		const DDLReflect::STRUCT_INFO* m_pLiveType;
		const A_LIVE_OBJECT* m_pLiveData;
		const DDLReflect::STRUCT_INFO* m_pContentType;
		const A_CONTENT_OBJECT* m_pContentData;
	};

	template<typename LIVE>
	class TGameObjectWithoutContent : public CGameObject
	{
	public:
		TGameObjectWithoutContent(LIVE* pLiveData) : CGameObject(DDLReflect::GetStruct<LIVE>(), pLiveData, NULL, NULL)
		{
		}

		TGameObjectWithoutContent(IGameObjectContainer* pContainer, const A_UUID& _uuid) : CGameObject(DDLReflect::GetStruct<LIVE>(), NULL)
		{
			Init(pContainer, _uuid);
		}

		virtual ~TGameObjectWithoutContent()
		{
		}

		const LIVE& Live()
		{
			return *((const LIVE*)GetLiveData());
		}
	};

	template<typename LIVE, typename CONTENT>
	class TGameObject : public CGameObject
	{
	public:
		TGameObject(const LIVE* pLiveData, const CONTENT* pContentData) : CGameObject(DDLReflect::GetStruct<LIVE>(), pLiveData, DDLReflect::GetStruct<CONTENT>(), pContentData)
		{
		}

		TGameObject(IGameObjectContainer* pContainer, const A_UUID& _uuid) : CGameObject(DDLReflect::GetStruct<LIVE>(), DDLReflect::GetStruct<CONTENT>())
		{
			Init(pContainer, _uuid);
		}

		virtual ~TGameObject()
		{
		}

		const LIVE& Live()
		{
			return *((const LIVE*)GetLiveData());
		}

		const CONTENT& Content()
		{
			return *((const CONTENT*)GetContentData());
		}
	};

}

#endif
