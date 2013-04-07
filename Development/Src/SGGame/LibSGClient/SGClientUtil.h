#ifndef __SGGAME_CLIENT_UTIL__
#define __SGGAME_CLIENT_UTIL__
namespace Atlas
{	
	class SGClientUtil
	{
	public:
		static void SetRightLocation(const A_CONTENT_OBJECT* content_obj, const SG_EQUIP_SLOTS& slots, const A_UUID& uuid);

		//��[lowerLevel uppperLevel]֮���С���ӵ�soldierVec����
		static void GetUnlockSoldierByLevel(_U32 lowerLevel, _U32 upperLevel, Atlas::Vector<_U32>& soldierVec, Atlas::Vector<SG_SOLDIER>& curSoldierVec);
		static void GetUnlockTurboSkill(_U32 general_id, _U32 turbo_level, Atlas::String& skill_archetype);

		static bool DiffPlayerInfo(const SG_PLAYER& player_old, const SG_PLAYER& player_new);
		static bool DiffSoldiers(const Atlas::Vector<SG_SOLDIER>& soldiers_old, const Atlas::Vector<SG_SOLDIER>& soldiers_new);
		static bool DiffGenerals(const Atlas::Vector<SG_GENERAL>& generals_old, const Atlas::Vector<SG_GENERAL>& generals_new);
		static bool DiffBag();
		static bool DiffEquipt(const Atlas::Vector<SG_EQUIPT_ITEM>& equipts_old, const Atlas::Vector<SG_EQUIPT_ITEM>& equipts_new);
		static bool DiffUsable(const Atlas::Vector<SG_USABLE_ITEM>& usables_old, const Atlas::Vector<SG_USABLE_ITEM>& usables_new);
		static bool DiffGem(const Atlas::Vector<SG_GEM_ITEM>& gems_old, const Atlas::Vector<SG_GEM_ITEM>& gems_new);
		static bool DiffMaterial(const Atlas::Vector<SG_MATERIAL_ITEM>& material_old, const Atlas::Vector<SG_MATERIAL_ITEM>& material_new);
		static bool DiffQuests(const Atlas::Vector<SG_QUEST_LIVE_INFO>& quests_old, const Atlas::Vector<SG_QUEST_LIVE_INFO>& quests_new);

		static bool IsEmptyPlayerInfo(const SG_PLAYER& player);
		static bool IsEmptySoldier(const Atlas::Vector<SG_SOLDIER>& soldiers);
		static bool IsEmptyGeneral(const Atlas::Vector<SG_GENERAL>& generals);
		static bool IsEmptyEquipt(const Atlas::Vector<SG_EQUIPT_ITEM>& equipts);
		static bool IsEmptyUsable(const Atlas::Vector<SG_USABLE_ITEM>& usables);
		static bool IsEmptyGem(const Atlas::Vector<SG_GEM_ITEM>& gems);
		static bool IsEmptyQuests(const Atlas::Vector<SG_QUEST_LIVE_INFO>& quests);

		static _U32 GetDailyActionTime(const SG_PLAYER& player_info, _U32 action_id);
		static bool SetDailyActionTimeInCache(SG_PLAYER& player_info, _U32 action_id, _U32 times);

		static bool UpdateItemByUUID(CSGClient* pClient, const A_UUID& uuid, _U32 count);

		template<typename T>
		static void GenerateTempNewVec(const T* t, _U32 count, Atlas::Vector<T>& vec)
		{
			for(_U32 i = 0; i < count; ++i)
			{
				vec.push_back(t[i]);
			}
		}

		template<typename T1, typename T2>
		static void GenerateTempNewVec(const T1* t, _U32 count, Atlas::Vector<T2>& vec);
		
		//warning !!! apple`s complier will set it as error
		//template<>
		//static void GenerateTempNewVec<SG_LEAGUE_APPLYER, _U32>(const SG_LEAGUE_APPLYER* t, _U32 count, Atlas::Vector<_U32>& vec)
		//{
		//	for(_U32 i = 0; i < count; ++i)
		//	{
		//		vec.push_back(t[i].applyer_id);
		//	}
		//}

		
		template<typename T>
		static bool UpdateItemTemplateFun(Atlas::Vector<T>& vec, const A_UUID& uuid, _U32 count)
		{
			for(typename Atlas::Vector<T>::iterator it = vec.begin(); it != vec.end(); ++it)
			{
				if((*it).uuid == uuid)
				{
					if(count == 0)
					{
						vec.erase(it);
					}
					else
					{
						(*it).count = count;
					}

					return true;
				}
			}

			return false;
		}

	};
}

#endif //__SGGAME_CLIENT_UTIL__