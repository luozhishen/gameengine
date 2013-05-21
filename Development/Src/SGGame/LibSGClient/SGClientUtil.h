#ifndef __SGGAME_CLIENT_UTIL__
#define __SGGAME_CLIENT_UTIL__
namespace Zion
{	
	class SGClientUtil
	{
	public:
		static void SetRightLocation(const A_CONTENT_OBJECT* content_obj, const SG_EQUIP_SLOTS& slots, const A_UUID& uuid);

		//把[lowerLevel uppperLevel]之间的小兵加到soldierVec里面
		static void GetUnlockSoldierByLevel(_U32 lowerLevel, _U32 upperLevel, Zion::Vector<_U32>& soldierVec, Zion::Vector<SG_SOLDIER>& curSoldierVec);
		static void GetUnlockTurboSkill(_U32 general_id, _U32 turbo_level, Zion::String& skill_archetype);

		static bool DiffPlayerInfo(const SG_PLAYER& player_old, const SG_PLAYER& player_new);
		static bool DiffSoldiers(const Zion::Vector<SG_SOLDIER>& soldiers_old, const Zion::Vector<SG_SOLDIER>& soldiers_new);
		static bool DiffGenerals(const Zion::Vector<SG_GENERAL>& generals_old, const Zion::Vector<SG_GENERAL>& generals_new);
		static bool DiffBag();
		static bool DiffEquipt(const Zion::Vector<SG_EQUIPT_ITEM>& equipts_old, const Zion::Vector<SG_EQUIPT_ITEM>& equipts_new);
		static bool DiffUsable(const Zion::Vector<SG_USABLE_ITEM>& usables_old, const Zion::Vector<SG_USABLE_ITEM>& usables_new);
		static bool DiffGem(const Zion::Vector<SG_GEM_ITEM>& gems_old, const Zion::Vector<SG_GEM_ITEM>& gems_new);
		static bool DiffMaterial(const Zion::Vector<SG_MATERIAL_ITEM>& material_old, const Zion::Vector<SG_MATERIAL_ITEM>& material_new);
		static bool DiffQuests(const Zion::Vector<SG_QUEST_LIVE_INFO>& quests_old, const Zion::Vector<SG_QUEST_LIVE_INFO>& quests_new);

		static bool IsEmptyPlayerInfo(const SG_PLAYER& player);
		static bool IsEmptySoldier(const Zion::Vector<SG_SOLDIER>& soldiers);
		static bool IsEmptyGeneral(const Zion::Vector<SG_GENERAL>& generals);
		static bool IsEmptyEquipt(const Zion::Vector<SG_EQUIPT_ITEM>& equipts);
		static bool IsEmptyUsable(const Zion::Vector<SG_USABLE_ITEM>& usables);
		static bool IsEmptyGem(const Zion::Vector<SG_GEM_ITEM>& gems);
		static bool IsEmptyQuests(const Zion::Vector<SG_QUEST_LIVE_INFO>& quests);

		static _U32 GetDailyActionTime(const SG_PLAYER& player_info, _U32 action_id);
		static bool SetDailyActionTimeInCache(SG_PLAYER& player_info, _U32 action_id, _U32 times);

		static bool UpdateItemCountByUUID(CSGClient* pClient, const A_UUID& uuid, _U32 count);

		template<typename T>
		static void GenerateTempNewVec(const T* t, _U32 count, Zion::Vector<T>& vec)
		{
			for(_U32 i = 0; i < count; ++i)
			{
				vec.push_back(t[i]);
			}
		}

		template<typename T1, typename T2>
		static void GenerateTempNewVec(const T1* t, _U32 count, Zion::Vector<T2>& vec);
		
		//warning !!! apple`s complier will set it as error
		//template<>
		//static void GenerateTempNewVec<SG_LEAGUE_APPLYER, _U32>(const SG_LEAGUE_APPLYER* t, _U32 count, Zion::Vector<_U32>& vec)
		//{
		//	for(_U32 i = 0; i < count; ++i)
		//	{
		//		vec.push_back(t[i].applyer_id);
		//	}
		//}


		template<typename T>
		static bool DiffData(const T& old_data, const T& new_data)
		{
			char* pOldAddress = (char*)&old_data;
			char* pNewAddress = (char*)&new_data;
			int nOffsize = sizeof(T);

			for(int n = 0; n < nOffsize; ++n)
			{
				if( *(pOldAddress + n) != *(pNewAddress + n))
				{
					return true;
				}
			}

			return false;
		}

		template<typename T>
		static bool DiffDataVec(const Zion::Vector<T>& old_data, const Zion::Vector<T>& new_data)
		{
			if(old_data.size() != new_data.size())
				return true;

			for(size_t i = 0; i < old_data.size(); ++i)
			{
				if(DiffData<T>(old_data[i], new_data[i]))
				{
					return true;	
				}
			}

			return false;
		}


		static void UpdateGeneralSoulBall(SG_PLAYER& player_info, Zion::Vector<SG_GENERAL>& generals, const SG_GENERAL& new_general);

		template<typename T, typename CT>
		static void AddItem(Zion::Vector<T>& cacheVec, const SG_ITEM& item)
		{
			T t;
			t.item_id = item.item_id;

			CT* config = NULL;
			_U32 stack_max = 1;
			_U32 count = item.count;

			const DDLReflect::STRUCT_INFO* struct_info = DDLReflect::GetStruct< CT >();
			const A_CONTENT_OBJECT* content_obj = Zion::ContentObject::FindFirst(struct_info, true);
			bool bFind = false;
			while(content_obj)
			{
				config = (CT*)content_obj;
				if(config->item_id == item.item_id)
				{
					bFind = true;
					break;
				}

				content_obj = Zion::ContentObject::FindNext(struct_info, true, content_obj);
			}

			if(bFind)
			{
				stack_max = config->stack_max;
			}
			
			for(typename Zion::Vector<T>::iterator it = cacheVec.begin(); it != cacheVec.end(); ++it)
			{
				if((*it).item_id == item.item_id)
				{
					const T& t_item = *it;
					if(t_item.count < stack_max)
					{
						if(t_item.count + count <= stack_max)
						{
							UpdateItemCountTemplateFun<T>(cacheVec, t_item.uuid, t_item.count + count);
						}
						else
						{
							count -= (stack_max - t_item.count);
						}
					}
				}
			}

			if(count)
			{
				t.uuid = item.uuid;
				t.count = count;
				t.expired_time = item.expired_time;

				cacheVec.push_back(t);
			}
		}

		template<typename T>
		static bool UpdateItemCountTemplateFun(Zion::Vector<T>& vec, const A_UUID& uuid, _U32 count)
		{
			for(typename Zion::Vector<T>::iterator it = vec.begin(); it != vec.end(); ++it)
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

		/*template<typename T>
		static bool UpdateItemTemplateFun(Zion::Vector<T>& vec, const A_UUID& uuid, const T& newT)
		{
			return false;
		}*/


		static _U32 GetHorseXpAdd(_U32 cur_level, _U32 new_level, _U32 cur_exp, _U32 exp);
		
		static bool UpdateEquipt(Zion::Vector<SG_EQUIPT_ITEM>& cache_equipt, const SG_EQUIPT_ITEM& new_equipt);
		static bool UpdateEquiptTurboLevel(Zion::Vector<SG_EQUIPT_ITEM>& cache_equipt, const A_UUID& uuid, const _U8 turbo_level);

		static bool UnlockSoldier(SG_PLAYER& player_info, Zion::Vector<SG_SOLDIER>& cache_soldiers, _U32 soldier_id);

		static bool EnhanceSoldier(SG_PLAYER& player_info, Zion::Vector<SG_SOLDIER>& cache_soldiers, _U32 soldier_id);

		static bool UpdateGemItemCount(Zion::Vector<SG_GEM_ITEM>& cache_gems, _U32 gem_id, _U32 count);
		static _U32 GetGemItemCount(const Zion::Vector<SG_GEM_ITEM>& cache_gems, _U32 gem_id);

	};
}

#endif //__SGGAME_CLIENT_UTIL__