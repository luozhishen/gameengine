#include "ZionBase.h"

#define ZION_MEMPOOL_MIN_NDX	5
#define ZION_MEMPOOL_MAX_NDX	25

class ZionMempoolInit
{
public:
	ZionMempoolInit();
	~ZionMempoolInit();

	A_SLIST_HEADER Pool[ZION_MEMPOOL_MAX_NDX-ZION_MEMPOOL_MIN_NDX+1];
};

#pragma warning(disable:4324)

typedef struct _A_MEM_ENTRY {
    A_SLIST_ENTRY entry;
	_U32 ndx;
} A_MEM_ENTRY;

#define MEM_ENTRY_SIZE		(((sizeof(A_MEM_ENTRY)+15)/16)*16)
static ZionMempoolInit _GlobalZionMempoolInit;

void* zion_malloc(size_t size)
{
	if(size>(1<<ZION_MEMPOOL_MAX_NDX))
	{
		ZION_FATAL("memory size to larger");
	}

	_U32 ndx;
	for(ndx=ZION_MEMPOOL_MIN_NDX; ndx<ZION_MEMPOOL_MAX_NDX; ndx++)
	{
		if(size<=((size_t)1<<ndx)) break;
	}

	A_MEM_ENTRY* entry = (A_MEM_ENTRY*)A_SLIST_POP(&_GlobalZionMempoolInit.Pool[ndx-ZION_MEMPOOL_MIN_NDX]);
	if(!entry)
	{
		entry = (A_MEM_ENTRY*)malloc(MEM_ENTRY_SIZE+((size_t)1<<ndx));
		entry->ndx = ndx - ZION_MEMPOOL_MIN_NDX;
	}
	return (char*)entry + MEM_ENTRY_SIZE;
}

void zion_free(void* mem)
{
	if(mem)
	{
		A_MEM_ENTRY* entry = (A_MEM_ENTRY*)((char*)mem - MEM_ENTRY_SIZE);
		A_SLIST_PUSH(&_GlobalZionMempoolInit.Pool[entry->ndx], &entry->entry);
	}
}

ZionMempoolInit::ZionMempoolInit()
{
	for(_U32 i=0; i<sizeof(_GlobalZionMempoolInit.Pool)/sizeof(_GlobalZionMempoolInit.Pool[0]); i++)
	{
		A_SLIST_INIT(&_GlobalZionMempoolInit.Pool[i]);
	}
}

ZionMempoolInit::~ZionMempoolInit()
{
	for(_U32 i=0; i<sizeof(_GlobalZionMempoolInit.Pool)/sizeof(_GlobalZionMempoolInit.Pool[0]); i++)
	{
		A_SLIST_ENTRY* entry = A_SLIST_POP(&_GlobalZionMempoolInit.Pool[i]);
		while(entry)
		{
			free(entry);
			entry = A_SLIST_POP(&_GlobalZionMempoolInit.Pool[i]);
		}
	}
}

namespace Zion
{

	void ZionInit()
	{
	}

	void ZionFini()
	{
	}

}
