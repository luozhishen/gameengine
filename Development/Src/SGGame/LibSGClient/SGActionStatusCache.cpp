#include <AtlasBase.h>
#include <AtlasClient.h>
#include <AtlasCommon.h>

#include <time.h>

#include "SGCommon.h"
#include "SGClient.h"

#include "SGActionStatusCache.h"

namespace Atlas
{
	Atlas::Vector<SGActionStatusCache::SG_DAILY_ACTION_CACHE> SGActionStatusCache::ms_dailyActionCacheList;

	void SGActionStatusCache::GetDailyActionEvent(_U32 time, const SG_PLAYER& player_info, Atlas::Vector<_U32>& actionVec, Atlas::Vector<_U8>& notifyVec)
	{
		const DDLReflect::STRUCT_INFO* struct_info = DDLReflect::GetStruct< SG_DAILY_ACTION_CONFIG >();
		const A_CONTENT_OBJECT* content_obj = Atlas::ContentObject::FindFirst(struct_info, true);

		for(_U32 i = 0; i < player_info.daily_actions._Count; ++i)
		{
			tm *tblock;
			time_t server_time = (time_t)time;
			tblock = localtime(&server_time);

			//convert time to the same day's
			tm today_tm = *tblock;
			today_tm.tm_sec = 0;

			while(content_obj)
			{
				SG_DAILY_ACTION_CONFIG* config = (SG_DAILY_ACTION_CONFIG*)content_obj;
				if(config->action_id != player_info.daily_actions._Array[i].action_id)
				{
					continue;
				}

				today_tm.tm_min = config->min;
				today_tm.tm_hour = config->hour;
				time_t start_time = mktime(&today_tm);
				time_t ready_time = start_time - config->prepare_min * 60;

				today_tm.tm_min = config->end_min;
				today_tm.tm_hour = config->end_hour;
				time_t end_time = mktime(&today_tm);
				
				if(server_time >= ready_time
					&& server_time < start_time)
				{
					if(!IsExistInDailyActionCache(config->action_id, ET_ActionPeriod_Ready))
					{
						actionVec.push_back(config->action_id);
						notifyVec.push_back(ET_ActionPeriod_Ready);

						UpdateDailyActionCache(config->action_id, ET_ActionPeriod_Ready);
					}
				}

				if(server_time >= start_time
					&& server_time < end_time)
				{
					if(!IsExistInDailyActionCache(config->action_id, ET_ActionPeriod_Processing))
					{
						actionVec.push_back(config->action_id);
						notifyVec.push_back(ET_ActionPeriod_Ready);

						UpdateDailyActionCache(config->action_id, ET_ActionPeriod_Processing);
					}
				}

				if(server_time >= end_time)
				{
					if(!IsExistInDailyActionCache(config->action_id, ET_ActionPeriod_Ending))
					{
						actionVec.push_back(config->action_id);
						notifyVec.push_back(ET_ActionPeriod_Ready);

						UpdateDailyActionCache(config->action_id, ET_ActionPeriod_Ending);
					}
				}

				content_obj = Atlas::ContentObject::FindNext(struct_info, true, content_obj);
			}

		}
	}

	bool SGActionStatusCache::IsExistInDailyActionCache(_U32 action_id, EActionPeriodType period_type)
	{
		for(size_t i = 0; i < ms_dailyActionCacheList.size(); ++i)
		{
			if(action_id == ms_dailyActionCacheList[i].action_id
				&&period_type ==  ms_dailyActionCacheList[i].period_type)
			{
				return true;
			}
		}

		return false;
	}

	void SGActionStatusCache::UpdateDailyActionCache(_U32 action_id, EActionPeriodType new_period_type)
	{
		for(size_t i = 0; i < ms_dailyActionCacheList.size(); ++i)
		{
			if(action_id == ms_dailyActionCacheList[i].action_id)
			{
				ms_dailyActionCacheList[i].period_type = new_period_type;
			}
		}
	}

}