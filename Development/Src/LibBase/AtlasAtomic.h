#ifndef	__ATLAS_ATOMIC__
#define	__ATLAS_ATOMIC__

#ifdef _WIN32
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#include <malloc.h>
	#define ATLAS_ALLOC(x)			malloc(x)
	#define ATLAS_FREE(x)			free(x)
	#define ATLAS_ALIGN_ALLOC(x)	_aligned_malloc(x, 16)
	#define ATLAS_ALIGN_FREE(x)		_aligned_free(x)
	#ifdef _DEBUG
	#define ATLAS_NEW				new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#else
	#define ATLAS_NEW				new
	#endif
	#define ATLAS_DELETE			delete
	#define ATLAS_ALLOCA(x)			_alloca(x)

	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#define A_MUTEX					CRITICAL_SECTION
	#define A_MUTEX_INIT			InitializeCriticalSection
	#define A_MUTEX_DESTROY			DeleteCriticalSection
	#define A_MUTEX_TRYLOCK			TryEnterCriticalSection
	#define A_MUTEX_LOCK			EnterCriticalSection
	#define A_MUTEX_UNLOCK			LeaveCriticalSection

	#define A_ATOM_INC(v)			InterlockedIncrement((LONG*)v)
	#define A_ATOM_DEC(v)			InterlockedDecrement((LONG*)v)
	#define A_ATOM_XCHG(v, e, c)	InterlockedCompareExchange((LONG*)v, e, c);

	#define A_SLIST_HEADER			SLIST_HEADER
	#define A_SLIST_ENTRY			SLIST_ENTRY
	#define A_SLIST_INIT			InitializeSListHead
	#define A_SLIST_PUSH			InterlockedPushEntrySList
	#define A_SLIST_POP				InterlockedPopEntrySList

	#define ATLAS_SLEEP(x)			Sleep(x)
#else
	#include <stdlib.h>
	#include <pthread.h>
	#define ATLAS_ALLOC(x)			malloc(x)
	#define ATLAS_FREE(x)			free(x)
	#define ATLAS_ALIGN_ALLOC(x)	malloc(x)
	#define ATLAS_ALIGN_FREE(x)		free(x)
	#ifdef _DEBUG
	#define ATLAS_NEW				new
	#else
	#define ATLAS_NEW				new
	#endif
	#define ATLAS_DELETE			delete
	#define ATLAS_ALLOCA(x)			_alloca(x)

	#include <pthread.h>
	#define A_MUTEX					pthread_mutex_t
	#define A_MUTEX_INIT(x)			pthread_mutex_init(x, NULL)
	#define A_MUTEX_DESTROY			pthread_mutex_destroy
	#define A_MUTEX_TRYLOCK			pthread_mutex_trylock
	#define A_MUTEX_LOCK			pthread_mutex_lock
	#define A_MUTEX_UNLOCK			pthread_mutex_unlock
	#define A_ATOM_INC(x)			InterlockedIncrement((LONG*)x)
	#define A_ATOM_DEC(x)			InterlockedDecrement((LONG*)x)
	#define A_ATOM_XCHG(v, e, c)	InterlockedCompareExchange((LONG*)v, e, c);

	#define A_SLIST_HEADER			SLIST_HEADER
	#define A_SLIST_ENTRY			SLIST_ENTRY
	#define A_SLIST_INIT			InitializeSListHead
	#define A_SLIST_PUSH			InterlockedPushEntrySList
	#define A_SLIST_POP				InterlockedPopEntrySList

	#define ATLAS_SLEEP(x)			sleep(x*1000)
#endif

#endif
