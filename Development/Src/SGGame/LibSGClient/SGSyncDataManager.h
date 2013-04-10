#ifndef _SG_SYNC_DATA_MANAGER_H__
#define _SG_SYNC_DATA_MANAGER_H__

#include "SGClient.h"

namespace Atlas
{
	#define SYNC_SEQ_MAX 9

	class CSGSyncDataManager
	{
	public:
		enum eSyncProtocol
		{
			eSyncNone = 0x0,
			//eSyncAvatar = 0x0,
			eSyncPlayer,
			eSyncGenerals,
			eSyncSoldiers,
			eSyncBagBegin,
			eSyncEquipt,
			eSyncUsable,
			eSyncGem,
			eSyncMaterial,
			eSyncBagEnd,
			eSyncPlayerQuest,
			eSyncServerTime,

			eSyncMax,
		};

		CSGSyncDataManager(CSGClient* pClient = NULL);
		~CSGSyncDataManager();

		void SetSGClient(CSGClient* pClient);

		void AddAllRequest();

		bool AddRequest(int nSeq, bool bStatus = false);
		bool ReceiveRequest(int nSeq);
		void ReceiveProcess();
		void SendSyncRequest();
		bool IsEmpty();

	private:
		std::map<int, bool> m_nSeqContainer;
		Atlas::CSGClient* m_pClient;
		_U8	m_nStatus;
		_U8 m_nLastSendReq;
	};

}

#endif	//_SG_SYNC_DATA_MANAGER_H__