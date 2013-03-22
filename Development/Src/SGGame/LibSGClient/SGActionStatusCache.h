#ifndef __SGGAME_ACTION_STATUS_CACHE_H__
#define __SGGAME_ACTION_STATUS_CACHE_H__

namespace Atlas
{
	class SGActionStatusCache
	{
	public:
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
			EActionPeriodType period_type;	
		};

		//在tick中遍历sg_daily_action_config, 根据time来判定对应的action，结果对应放入status中
		static void GetDailyActionEvent(_U32 time, const SG_PLAYER& player_info, Atlas::Vector<_U32>& actionVec, Atlas::Vector<_U8>& notifyVec);		//EActionNotifyType

		static bool IsExistInDailyActionCache(_U32 action_id, EActionPeriodType period_type);
		static void UpdateDailyActionCache(_U32 action_id, EActionPeriodType new_period_type);

		//遍历actionVec， 根据time来判定对应的action在什么状态下，结果对应放入status中
		static void GetDailActionStatus(_U32 time, Atlas::Vector<_U32>& actionVec, Atlas::Vector<_U8>& statusVec);		//EActionPeriodType

	private:
		static Atlas::Vector<SG_DAILY_ACTION_CACHE> ms_dailyActionCacheList;
	};
}

#endif //__SGGAME_ACTION_STATUS_CACHE_KEEPER_H__
