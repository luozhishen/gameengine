#include <AtlasBase.h>
#include <AtlasClient.h>
#include <AtlasCommon.h>

#include <algorithm>
#include <time.h>
#include "SGCommon.h"
#include "SGClient.h"
#include "SGSyncDataManager.h"
#include "SGClientUtil.h"

namespace Atlas
{
	void SGClientUtil::SetRightLocation(const A_CONTENT_OBJECT* content_obj, const SG_EQUIP_SLOTS& slots, const A_UUID& uuid)
	{
		if(((SG_EQUIPT_ITEM_CONFIG*)content_obj)->item_type == SG_EQUIPT_TYPE_WEAPON_JINZHAN
			||((SG_EQUIPT_ITEM_CONFIG*)content_obj)->item_type == SG_EQUIPT_TYPE_WEAPON_YUANCHENG
			||((SG_EQUIPT_ITEM_CONFIG*)content_obj)->item_type == SG_EQUIPT_TYPE_WEAPON_FASHU)
		{
			memcpy((void*)(&slots.weapon), &uuid, sizeof(A_UUID));
		}
		
		if(((SG_EQUIPT_ITEM_CONFIG*)content_obj)->item_type == SG_EQUIPT_TYPE_HEAD)
		{
			memcpy((void*)(&slots.head), &uuid, sizeof(A_UUID));
		}

		if(((SG_EQUIPT_ITEM_CONFIG*)content_obj)->item_type == SG_EQUIPT_TYPE_SHOULDER)
		{
			memcpy((void*)&slots.shoulder, &uuid, sizeof(A_UUID));
		}

		if(((SG_EQUIPT_ITEM_CONFIG*)content_obj)->item_type == SG_EQUIPT_TYPE_CHEST)
		{
			memcpy((void*)&slots.chest, &uuid, sizeof(A_UUID));
		}

		if(((SG_EQUIPT_ITEM_CONFIG*)content_obj)->item_type == SG_EQUIPT_TYPE_LEG)
		{
			memcpy((void*)&slots.leg, &uuid, sizeof(A_UUID));
		}

		if(((SG_EQUIPT_ITEM_CONFIG*)content_obj)->item_type == SG_EQUIPT_TYPE_DECORATION)
		{
			memcpy((void*)&slots.decoration, &uuid, sizeof(A_UUID));
		}
	}

	void SGClientUtil::GetUnlockSoldierByLevel(_U32 lowerLevel, _U32 upperLevel, Atlas::Vector<_U32>& soldierVec, Atlas::Vector<SG_SOLDIER>& curSoldierVec)
	{
		const DDLReflect::STRUCT_INFO* struct_info = DDLReflect::GetStruct< SG_SOLDIER_CONFIG >();
		
		const A_CONTENT_OBJECT* content_obj = Atlas::ContentObject::FindFirst(struct_info, true);
		while(content_obj)
		{
			SG_SOLDIER_CONFIG* config = (SG_SOLDIER_CONFIG*)content_obj;

			bool bFind = false;
			for(Atlas::Vector<SG_SOLDIER>::iterator it_loop = curSoldierVec.begin(); it_loop != curSoldierVec.end(); ++it_loop)
			{
				if(it_loop->soldier_id == config->soldier_id)
				{
					bFind = true;
					break;
				}
			}

			if(!bFind 
				&& config->unlock_level <= (_S32)upperLevel
				&& config->unlock_level > (_S32)lowerLevel)
			{
				soldierVec.push_back(config->soldier_id);
			}

			content_obj = Atlas::ContentObject::FindNext(struct_info, true, content_obj);
		}

		std::unique(soldierVec.begin(), soldierVec.end());
	}
	
	void SGClientUtil::GetUnlockTurboSkill(_U32 general_id, _U32 turbo_level, Atlas::String& skill_archetype)
	{
		if(general_id == 11002)
			general_id = 11001;

		const DDLReflect::STRUCT_INFO* struct_info = DDLReflect::GetStruct< SG_TURBO_CONFIG >();
		const A_CONTENT_OBJECT* content_obj = Atlas::ContentObject::FindFirst(struct_info, true);
		while(content_obj)
		{
			SG_TURBO_CONFIG* config = (SG_TURBO_CONFIG*)content_obj;
			if(	config->turbo_level == turbo_level
				&& config->general_id == general_id
				&& strlen(*(config->skill_archetype)) != 0)
			{
				skill_archetype += *(config->skill_archetype);
				return;
			}
		
			content_obj = Atlas::ContentObject::FindNext(struct_info, true, content_obj);
		}
	}



	bool SGClientUtil::DiffPlayerInfo(const SG_PLAYER& player_old, const SG_PLAYER& player_new)
	{
		char* pOldAddress = (char*)&player_old;
		char* pNewAddress = (char*)&player_new;
		int nOffsize = sizeof(SG_PLAYER);

		for(int n = 0; n < nOffsize; ++n)
		{
			if( *(pOldAddress + n) != *(pNewAddress + n))
			{
				return true;
			}
		}

		return false;
	}

	bool SGClientUtil::DiffSoldiers(const Atlas::Vector<SG_SOLDIER>& soldiers_old, const Atlas::Vector<SG_SOLDIER>& soldiers_new)
	{
		return soldiers_old.size() != soldiers_new.size();
	}

	bool SGClientUtil::DiffGenerals(const Atlas::Vector<SG_GENERAL>& generals_old, const Atlas::Vector<SG_GENERAL>& generals_new)
	{
		return generals_old.size() != generals_new.size();
	}
	
	bool SGClientUtil::DiffBag()
	{
		return false;
	}

	template<typename T>
	bool DiffCompareTemplateFun(const T& t1, const T& t2)
	{
		return true;
	}

	template<>
	bool DiffCompareTemplateFun<SG_EQUIPT_ITEM>(const SG_EQUIPT_ITEM& t1, const SG_EQUIPT_ITEM& t2)
	{
		if(t1.item_id != t2.item_id
			||t1.count != t2.count
			||t1.turbo_level != t2.turbo_level
			||t1.expired_time != t2.expired_time
			)
		{
			return true;
		}
		
		if(t1.gems_slots._Count != t2.gems_slots._Count)
		{
			return true;
		}

		for(_U32 i = 0; i < t1.gems_slots._Count; ++i)
		{
			if(t1.gems_slots._Array[i] != t2.gems_slots._Array[i])
			{
				return true;
			}
		}

		return false;
	}

	template<>
	bool DiffCompareTemplateFun<SG_GEM_ITEM>(const SG_GEM_ITEM& t1, const SG_GEM_ITEM& t2)
	{
		if(t1.item_id != t2.item_id
			||t1.count != t2.count
			||t1.status != t2.status
			||t1.expired_time != t2.expired_time
			)
		{
			return true;
		}
		
		return false;
	}

	template<>
	bool DiffCompareTemplateFun<SG_MATERIAL_ITEM>(const SG_MATERIAL_ITEM& t1, const SG_MATERIAL_ITEM& t2)
	{
		if(t1.item_id != t2.item_id
			||t1.count != t2.count
			)
		{
			return true;
		}
		
		return false;
	}

	template<>
	bool DiffCompareTemplateFun<SG_USABLE_ITEM>(const SG_USABLE_ITEM& t1, const SG_USABLE_ITEM& t2)
	{
		if(t1.item_id != t2.item_id
			||t1.count != t2.count
			||t1.expired_time != t2.expired_time
			)
		{
			return true;
		}
		
		return false;
	}

	bool SGClientUtil::DiffEquipt(const Atlas::Vector<SG_EQUIPT_ITEM>& equipts_old, const Atlas::Vector<SG_EQUIPT_ITEM>& equipts_new)
	{
		if(equipts_old.size() != equipts_new.size())
		{
			return true;
		}

		for(size_t i = 0; i < equipts_old.size(); ++i)
		{
			if(DiffCompareTemplateFun<SG_EQUIPT_ITEM>(equipts_old[i], equipts_new[i]))
			{
				return true;
			}
		}

		return false;
	}
	
	bool SGClientUtil::DiffUsable(const Atlas::Vector<SG_USABLE_ITEM>& usables_old, const Atlas::Vector<SG_USABLE_ITEM>& usables_new)
	{
		if(usables_old.size() != usables_new.size())
		{
			return true;
		}
		
		for(size_t i = 0; i < usables_old.size(); ++i)
		{
			if(DiffCompareTemplateFun<SG_USABLE_ITEM>(usables_old[i], usables_new[i]))
			{
				return true;
			}
		}

		return false;
	}
	
	bool SGClientUtil::DiffGem(const Atlas::Vector<SG_GEM_ITEM>& gems_old, const Atlas::Vector<SG_GEM_ITEM>& gems_new)
	{
		if(gems_old.size() != gems_new.size())
		{
			return true;
		}

		for(size_t i = 0; i < gems_old.size(); ++i)
		{
			if(DiffCompareTemplateFun<SG_GEM_ITEM>(gems_old[i], gems_new[i]))
			{
				return true;
			}
		}

		return false;
	}

	bool SGClientUtil::DiffMaterial(const Atlas::Vector<SG_MATERIAL_ITEM>& material_old, const Atlas::Vector<SG_MATERIAL_ITEM>& material_new)
	{
		if(material_old.size() != material_new.size())
		{
			return true;
		}

		for(size_t i = 0; i < material_old.size(); ++i)
		{
			if(DiffCompareTemplateFun<SG_MATERIAL_ITEM>(material_old[i], material_new[i]))
			{
				return true;
			}
		}

		return false;
	}
	
	bool SGClientUtil::DiffQuests(const Atlas::Vector<SG_QUEST_LIVE_INFO>& quests_old, const Atlas::Vector<SG_QUEST_LIVE_INFO>& quests_new)
	{
		if(quests_old.size() != quests_new.size())
		{
			return true;
		}

		for(size_t i = 0; i < quests_old.size(); ++i)
		{
			if( quests_old[i].quest_id != quests_new[i].quest_id
				||quests_old[i].counter != quests_new[i].counter
				||quests_old[i].status != quests_new[i].status)
			{
				return true;
			}
		}

		return false;
	}

	bool SGClientUtil::IsEmptyPlayerInfo(const SG_PLAYER& player)
	{
		return player.avatar_id == 0;
	}

	bool SGClientUtil::IsEmptySoldier(const Atlas::Vector<SG_SOLDIER>& soldiers)
	{
		return soldiers.empty();
	}

	bool SGClientUtil::IsEmptyGeneral(const Atlas::Vector<SG_GENERAL>& generals)
	{
		return generals.empty();
	}

	bool SGClientUtil::IsEmptyEquipt(const Atlas::Vector<SG_EQUIPT_ITEM>& equipts)
	{
		return equipts.empty();
	}

	bool SGClientUtil::IsEmptyUsable(const Atlas::Vector<SG_USABLE_ITEM>& usables)
	{
		return usables.empty();
	}

	bool SGClientUtil::IsEmptyGem(const Atlas::Vector<SG_GEM_ITEM>& gems)
	{
		return gems.empty();
	}

	bool SGClientUtil::IsEmptyQuests(const Atlas::Vector<SG_QUEST_LIVE_INFO>& quests)
	{
		return quests.empty();
	}

	_U32 SGClientUtil::GetDailyActionTime(const SG_PLAYER& player_info, _U32 action_id)
	{
		for(_U32 i = 0; i < player_info.daily_actions._Count; ++i)
		{
			if(player_info.daily_actions._Array[i].action_id == action_id)
			{
				return player_info.daily_actions._Array[i].times;
			}
		}
		
		return 0;
	}

	bool SGClientUtil::SetDailyActionTimeInCache(SG_PLAYER& player_info, _U32 action_id, _U32 times)
	{
		for(_U32 i = 0; i < player_info.daily_actions._Count; ++i)
		{
			if(player_info.daily_actions._Array[i].action_id == action_id)
			{
				player_info.daily_actions._Array[i].times = times;
				return true;
			}
		}

		return false;
	}


	bool SGClientUtil::UpdateItemByUUID(CSGClient* pClient, const A_UUID& uuid, _U32 count)
	{
		bool bRet = false;

		Atlas::Vector<SG_EQUIPT_ITEM>& equiptVec		= (Atlas::Vector<SG_EQUIPT_ITEM>&)pClient->GetEquiptItem();
		Atlas::Vector<SG_GEM_ITEM>& gemVec				= (Atlas::Vector<SG_GEM_ITEM>&)pClient->GetGemItem();
		Atlas::Vector<SG_USABLE_ITEM>& usableVec		= (Atlas::Vector<SG_USABLE_ITEM>&)pClient->GetUsableItem();
		Atlas::Vector<SG_MATERIAL_ITEM>& materialVec	= (Atlas::Vector<SG_MATERIAL_ITEM>&)pClient->GetMaterialItem();

		//if one item has been update then do not update next vec and return true
		if(	!UpdateItemTemplateFun<SG_EQUIPT_ITEM>(equiptVec, uuid, count)
			&&!UpdateItemTemplateFun<SG_GEM_ITEM>(gemVec, uuid, count)
			&&!UpdateItemTemplateFun<SG_USABLE_ITEM>(usableVec, uuid, count)
			&&!UpdateItemTemplateFun<SG_MATERIAL_ITEM>(materialVec, uuid, count))
		{
			return bRet;
		}
		else
		{
			bRet = true;
		}


		return bRet;
	}


	void SGClientUtil::UpdateGeneralSoulBall(SG_PLAYER& player_info, Atlas::Vector<SG_GENERAL>& generals, const SG_GENERAL& new_general)
	{
		if(new_general.general_id == player_info.general_id)
		{
			player_info.minghun_slots.Resize(6);
			for(_U32 i = 0; i < 6; ++i)
			{
				player_info.minghun_slots._Array[i] = new_general.minghun_slots._Array[i];
			}
		}

		for(Atlas::Vector<SG_GENERAL>::iterator it = generals.begin(); it != generals.end(); ++it)
		{
			if((*it).general_id == new_general.general_id)
			{
				(*it).minghun_slots.Resize(6);
				for(_U32 i = 0; i < 6; ++i)
				{
					(*it).minghun_slots._Array[i] = new_general.minghun_slots._Array[i];
				}

				break;
			}
		}
	}
}