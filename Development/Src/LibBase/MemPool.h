#ifndef	__MEMORY_POOL__
#define	__MEMORY_POOL__

namespace Atlas {

	typedef struct MEMORY_POOL* HMEMPOOL;

	HMEMPOOL AllocMemoryPool(_U32 size, _U32 count=0);
	void FreeMemoryPool(HMEMPOOL pool);
	void* LockMemoryPoolBuffer(HMEMPOOL pool);
	void UnlockMemoryPoolBuffer(void* mem);
	_U32 GetMemoryPoolTotalCount(HMEMPOOL pool);
	_U32 GetMemoryPoolFreeCount(HMEMPOOL pool);

};

#endif
