#ifndef	__ATLAS_DEFINES__
#define	__ATLAS_DEFINES__

typedef unsigned char			_U8;
typedef unsigned short			_U16;
typedef unsigned long			_U32;
typedef unsigned long long		_U64;
typedef char					_S8;
typedef short					_S16;
typedef long					_S32;
typedef long long				_S64;
typedef float					_F32;
typedef double					_F64;
typedef char*					_STR;

#define ALIGN_SIZE(x)			((sizeof(x)+7) / 8 * 8)

#include <assert.h>
#define ATLAS_ASSERT(x)			assert(x)

#ifdef	_DEBUG
#define ATLAS_VERIFY(x)			assert(x)
#else
#define	ATLAS_VERIFY(expr)		(void)(expr)
#endif

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
	#define ATLAS_ALLOCA(x)			_alloca(x)

	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#define A_MUTEX					CRITICAL_SECTION
	#define A_MUTEX_INIT			InitializeCriticalSection
	#define A_MUTEX_DESTROY			DeleteCriticalSection
	#define A_MUTEX_TRYLOCK			TryEnterCriticalSection
	#define A_MUTEX_LOCK			EnterCriticalSection
	#define A_MUTEX_UNLOCK			LeaveCriticalSection
	#define A_ATOM_INC(x)			InterlockedIncrement((LONG*)x)
	#define A_ATOM_DEC(x)			InterlockedDecrement((LONG*)x)

	#define A_SLIST_HEADER			SLIST_HEADER
	#define A_SLIST_ENTRY			SLIST_ENTRY
	#define A_SLIST_INIT			InitializeSListHead
	#define A_SLIST_PUSH			InterlockedPushEntrySList
	#define A_SLIST_POP				InterlockedPopEntrySList

	#define ATLAS_SLEEP(x)			Sleep(x)
#else
	#include <stdlib.h>
	#define ATLAS_ALLOC(x)			malloc(x)
	#define ATLAS_FREE(x)			free(x)
	#define ATLAS_ALIGN_ALLOC(x)	malloc(x)
	#define ATLAS_ALIGN_FREE(x)		free(x)
	#ifdef _DEBUG
	#define ATLAS_NEW				new
	#else
	#define ATLAS_NEW				new
	#endif
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

	#define A_SLIST_HEADER			SLIST_HEADER
	#define A_SLIST_ENTRY			SLIST_ENTRY
	#define A_SLIST_INIT			InitializeSListHead
	#define A_SLIST_PUSH			InterlockedPushEntrySList
	#define A_SLIST_POP				InterlockedPopEntrySList

	#define ATLAS_SLEEP(x)			sleep(x*1000)
#endif

//#pragma warning(disable:4366)
#pragma warning(disable:4121)
#pragma warning(disable:4100)
#pragma warning(disable:4189)
#pragma warning(disable:4355)

#define ATLAS_OFFSETOF(type, member) ((size_t)(&((type*)NULL)->member))

#endif
