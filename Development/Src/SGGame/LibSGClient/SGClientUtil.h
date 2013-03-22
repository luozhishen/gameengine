#ifndef __SGGAME_CLIENT_UTIL__
#define __SGGAME_CLIENT_UTIL__
namespace Atlas
{	
	enum EActionNotifyType
	{
		ET_ActionNotify_Ready,
		ET_ActionNotify_Begin,
		ET_ActionNotify_End,
	};

	//[reset_time-none, none-ready, ready-processing processing-reset_time]
	enum EActionPeriodType
	{
		ET_ActionPeriod_HaveBegan,
		ET_ActionPeriod_Ready,
		ET_ActionPeriod_Processing,
		ET_ActionPeriod_Ending,
	};

	struct SG_DAILY_ACTION_CACHE
	{
		_U32			  action_id;
		EActionPeriodType periodType;	
	};

	class SGClientUtil
	{
	public:
		static void SetRightLocation(const A_CONTENT_OBJECT* content_obj, const SG_EQUIP_SLOTS& slots, const A_UUID& uuid);

		//把[lowerLevel uppperLevel]之间的小兵加到soldierVec里面
		static void GetUnlockSoldierByLevel(_U32 lowerLevel, _U32 upperLevel, Atlas::Vector<_U32>& soldierVec, Atlas::Vector<SG_SOLDIER>& curSoldierVec);
		static void GetUnlockTurboSkill(_U32 turbo_level, Atlas::String& skill_archetype);

		//在tick中遍历sg_daily_action_config, 根据time来判定对应的action，结果对应放入status中
		static void GetDailyActionEvent(_U32 time, const SG_PLAYER& player_info, Atlas::Vector<_U32>& actionVec, Atlas::Vector<_U8>& notifyVec);		//EActionNotifyType

		//遍历actionVec， 根据time来判定对应的action在什么状态下，结果对应放入status中
		static void GetDailActionStatus(_U32 time, Atlas::Vector<_U32>& actionVec, Atlas::Vector<_U8>& statusVec);		//EActionPeriodType



		static bool DiffPlayerInfo(const SG_PLAYER& player_old, const SG_PLAYER& player_new);
		static bool DiffSoldiers(const Atlas::Vector<SG_SOLDIER>& soldiers_old, const Atlas::Vector<SG_SOLDIER>& soldiers_new);
		static bool DiffGenerals(const Atlas::Vector<SG_GENERAL>& generals_old, const Atlas::Vector<SG_GENERAL>& generals_new);
		static bool DiffBag();
		static bool DiffEquipt(const Atlas::Vector<SG_EQUIPT_ITEM>& equipts_old, const Atlas::Vector<SG_EQUIPT_ITEM>& equipts_new);
		static bool DiffUsable(const Atlas::Vector<SG_USABLE_ITEM>& usables_old, const Atlas::Vector<SG_USABLE_ITEM>& usables_new);
		static bool DiffGem(const Atlas::Vector<SG_GEM_ITEM>& gems_old, const Atlas::Vector<SG_GEM_ITEM>& gems_new);
		static bool DiffQuests(const Atlas::Vector<SG_QUEST_LIVE_INFO>& quests_old, const Atlas::Vector<SG_QUEST_LIVE_INFO>& quests_new);

		static bool IsEmptyPlayerInfo(const SG_PLAYER& player);
		static bool IsEmptySoldier(const Atlas::Vector<SG_SOLDIER>& soldiers);
		static bool IsEmptyGeneral(const Atlas::Vector<SG_GENERAL>& generals);
		static bool IsEmptyEquipt(const Atlas::Vector<SG_EQUIPT_ITEM>& equipts);
		static bool IsEmptyUsable(const Atlas::Vector<SG_USABLE_ITEM>& usables);
		static bool IsEmptyGem(const Atlas::Vector<SG_GEM_ITEM>& gems);
		static bool IsEmptyQuests(const Atlas::Vector<SG_QUEST_LIVE_INFO>& quests);

		template<typename T>
		static void GenerateTempNewVec(const T* t, _U32 count, Atlas::Vector<T>& vec)
		{
			for(_U32 i = 0; i < count; ++i)
			{
				vec.push_back(t[i]);
			}
		}

	private:
		static Atlas::Vector<SG_DAILY_ACTION_CACHE> ms_dailyActionCacheList;
	};
}

#endif //__SGGAME_CLIENT_UTIL__