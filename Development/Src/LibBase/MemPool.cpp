#include "AtlasDefines.h"
#include "MemPool.h"

#pragma pack(push)
#pragma pack(4)
/*
namespace Atlas {

	struct MEMPOOL_CONTEXT
	{
		SLIST_ENTRY entry;
		HMEMPOOL	pool;
	};

	struct MEMORY_POOL
	{
		DWORD size, count;
		SLIST_HEADER buffers;
	};

	HMEMPOOL AllocMemoryPool(_U32 size, _U32 count)
	{
		HMEMPOOL ret = (HMEMPOOL)malloc(sizeof(MEMORY_POOL));
		if(!ret) return NULL;
		ret->size = size;
		ret->count = count;
		InitializeSListHead(&ret->buffers);

		while(ret->count<count)
		{
			MEMPOOL_CONTEXT* ctx = (MEMPOOL_CONTEXT*)malloc(ALIGN_SIZE(MEMPOOL_CONTEXT) + size);
			if(!ctx) break;
			ctx->pool = ret;
			InterlockedIncrement((LONG*)&ret->count);
		}

		return ret;
	}

	void FreeMemoryPool(HMEMPOOL pool)
	{
		_U32 deleted = 0;
	
		while(deleted<pool->count)
		{
			void* mem = LockMemoryPoolBuffer(pool);
			if(mem)
			{
				deleted++;
				free((_U8*)mem - sizeof(MEMPOOL_CONTEXT));
			}
			else
			{
				SwitchToThread();
			}
		}
	}

	void* LockMemoryPoolBuffer(HMEMPOOL pool)
	{
		MEMPOOL_CONTEXT* ctx = (MEMPOOL_CONTEXT*)InterlockedPopEntrySList(&pool->buffers);
		if(!ctx)
		{
			ctx = (MEMPOOL_CONTEXT*)malloc(ALIGN_SIZE(MEMPOOL_CONTEXT) + pool->size);
			if(!ctx) return NULL;
			ctx->pool = pool;
			InterlockedIncrement((LONG*)&pool->count);
		}
		return (_U8*)ctx + ALIGN_SIZE(MEMPOOL_CONTEXT);
	}

	void UnlockMemoryPoolBuffer(void* mem)
	{
		MEMPOOL_CONTEXT* ctx = (MEMPOOL_CONTEXT*)((_U8*)mem - ALIGN_SIZE(MEMPOOL_CONTEXT));
		InterlockedPushEntrySList(&ctx->pool->buffers, &ctx->entry);
	}

	_U32 GetMemoryPoolTotalCount(HMEMPOOL pool)
	{
		return pool->size;
	}

	_U32 GetMemoryPoolFreeCount(HMEMPOOL pool)
	{
		return pool->count;
	}

}
*/
#pragma pack(pop)
