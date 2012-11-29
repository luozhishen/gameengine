#ifndef __ATLAS_SYNC_MONITOR__
#define __ATLAS_SYNC_MONITOR__

namespace Atlas
{
	namespace Sync
	{
		
		class CSyncObject;

		class CSyncMonitor
		{
		public:
			CSyncMonitor();
			~CSyncMonitor();

			bool Attach(CSyncObject* pObject);
			bool Detach(CSyncObject* pObject);
			bool SetDirty(CSyncObject* pObject);

		private:
			std::map<CSyncObject*, bool> m_mapObjects;
		};

	}
}

#endif
