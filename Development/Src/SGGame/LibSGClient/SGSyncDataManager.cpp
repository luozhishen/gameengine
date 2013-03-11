#include <AtlasBase.h>
#include <AtlasClient.h>
#include <AtlasCommon.h>
#include "SGCommon.h"
#include "SGClient.h"
#include "SGSyncDataManager.h"

namespace Atlas
{
	#define SYNC_SEQ_MAX 9

	CSGSyncDataManager::CSGSyncDataManager(CSGClient* pClient) 
	{
		m_pClient = pClient;
	}

	CSGSyncDataManager::~CSGSyncDataManager() {}

	void CSGSyncDataManager::AddAllRequest()
	{
		for(int i = eSyncPlayer; i < eSyncMax; ++i)
		{
			AddRequest(i);
		}
	}

	bool CSGSyncDataManager::AddRequest(int nSeq, bool bStatus)
	{
		m_nSeqContainer[nSeq] = bStatus;
		return true;
	}

	bool CSGSyncDataManager::ReceiveRequest(int nSeq)
	{
		std::map<int, bool>::iterator it = m_nSeqContainer.find(nSeq);
		if(it != m_nSeqContainer.end())
		{
			it->second = true;

			if(IsEmpty())
			{
				return false;								//no msg send any more
			}
			else
			{
				SendSyncRequest();							//send continuely
				return true;
			}
		}

		return false;
	}

	void CSGSyncDataManager::ReceiveProcess()
	{
		//m_receive receive data
	}

	void CSGSyncDataManager::SendSyncRequest()
	{
		for(std::map<int, bool>::iterator it = m_nSeqContainer.begin(); it != m_nSeqContainer.end(); ++it)
		{
			if(it->second != true)
			{
				switch(it->first)
				{
				case eSyncPlayer:
					{
						m_pClient->QueryPlayer(1);
					}
					break;
				case eSyncGenerals:
					{
						m_pClient->QueryGenerals(1);
					}
					break;
				case eSyncSoldiers:
					{
						m_pClient->QuerySoldiers(1);
					}
					break;
				case eSyncBagBegin:
					{
						m_pClient->QueryBag(1);
					}
					break;
				case eSyncEquipt:
				case eSyncUsable:
				case eSyncGem:
				case eSyncBagEnd:
					break;
				case eSyncPlayerQuest:
					{
						m_pClient->QueryPlayerQuest(1);
					}
					break;

				case eSyncServerTime:
					{
						m_pClient->QueryServerTime();
					}
					break;
				}

				return;
			}
		}

		//finish
		m_pClient->SyncSuccNotify();
	}

	bool CSGSyncDataManager::IsEmpty()
	{
		for(std::map<int, bool>::iterator it = m_nSeqContainer.begin(); it != m_nSeqContainer.end(); ++it)
		{
			if(it->second == false)
			{
				return false;
			}
		}

		return true;
	}
}