#ifndef	__ZION_DEFINES__
#define	__ZION_DEFINES__

typedef unsigned char			_U8;
typedef unsigned short			_U16;
typedef unsigned int			_U32;
typedef unsigned long long		_U64;
typedef char					_S8;
typedef short					_S16;
typedef long					_S32;
typedef long long				_S64;
typedef float					_F32;
typedef double					_F64;
typedef char*					_STR;

#include <assert.h>
#include <stdlib.h>
extern void zion_set_fatal(bool msgbox);
extern void zion_fatal(const char* fmt, ...);

#define ZION_ASSERT(x)			assert(x)

#ifdef	_DEBUG
#define ZION_VERIFY(x)			assert(x)
#else
#define	ZION_VERIFY(expr)		(void)(expr)
#endif

#define ZION_FATAL	zion_fatal

#define ZION_OFFSETOF(type, member) ((size_t)(&((type*)NULL)->member))

extern void* zion_malloc(size_t size);
extern void zion_free(void* mem);

#include <stdio.h>

#ifdef _WIN32
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#include <malloc.h>
#if 1
	#define ZION_ALLOC(x)			zion_malloc(x)
	#define ZION_FREE(x)			zion_free(x)
#else
	#define ZION_ALLOC(x)			malloc(x)
	#define ZION_FREE(x)			free(x)
#endif
	#define ZION_ALIGN_ALLOC(x)		_aligned_malloc(x, 16)
	#define ZION_ALIGN_FREE(x)		_aligned_free(x)
	#ifdef _DEBUG
	#define ZION_NEW				new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#else
	#define ZION_NEW				new
	#endif
	#define ZION_DELETE				delete
	#define ZION_ALLOCA(x)			_alloca(x)

	#define ZION_SLEEP(x)			Sleep(x)

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
	#define A_ATOM_XCHG(v, e, c)	InterlockedCompareExchange((LONG*)v, e, c)

	#define A_SLIST_HEADER			SLIST_HEADER
	#define A_SLIST_ENTRY			SLIST_ENTRY
	#define A_SLIST_INIT			InitializeSListHead
	#define A_SLIST_PUSH			InterlockedPushEntrySList
	#define A_SLIST_POP				InterlockedPopEntrySList
#else
	#include <stdlib.h>
	#define ZION_ALLOC(x)			malloc(x)
	#define ZION_FREE(x)			free(x)
	#define ZION_ALIGN_ALLOC(x)		malloc(x)
	#define ZION_ALIGN_FREE(x)		free(x)
	#ifdef _DEBUG
	#define ZION_NEW				new
	#else
	#define ZION_NEW				new
	#endif
	#define ZION_DELETE				delete
	#define ZION_ALLOCA(x)			_alloca(x)

	#define ZION_SLEEP(x)			sleep(x*1000)

	_U32 atom_unix_inc(_U32 volatile *p);
	_U32 atom_unix_dec(_U32 volatile *p);
	_U32 atom_unix_swap(_U32 volatile *p, _U32 v);
	_U32 atom_unix_cas(_U32 volatile *p, _U32 v, _U32 c);
	typedef struct ATOM_SLIST_ENTRY		{ struct ATOM_SLIST_ENTRY *Next; }		ATOM_SLIST_ENTRY;
	typedef struct ATOM_SLIST_HEADER	{ ATOM_SLIST_ENTRY* First; long Count;}	ATOM_SLIST_HEADER;

	void atom_slist_init(ATOM_SLIST_HEADER* header);
	ATOM_SLIST_ENTRY* atom_slist_pop(ATOM_SLIST_HEADER* head);
	ATOM_SLIST_ENTRY* atom_slist_push(ATOM_SLIST_HEADER* head, ATOM_SLIST_ENTRY* ListEntry);
	ATOM_SLIST_ENTRY* atom_slist_flush(ATOM_SLIST_HEADER* head);

	#include <pthread.h>

	#define A_MUTEX					pthread_mutex_t
	#define A_MUTEX_INIT(x)			pthread_mutex_init(x, NULL)
	#define A_MUTEX_DESTROY			pthread_mutex_destroy
	#define A_MUTEX_TRYLOCK			pthread_mutex_trylock
	#define A_MUTEX_LOCK			pthread_mutex_lock
	#define A_MUTEX_UNLOCK			pthread_mutex_unlock

	#define A_ATOM_INC(x)			atom_unix_inc(x)
	#define A_ATOM_DEC(x)			atom_unxi_dec(x)
	#define A_ATOM_XCHG(v, e, c)	atom_unix_cas(v, e, c)

	#define A_SLIST_HEADER			ATOM_SLIST_HEADER
	#define A_SLIST_ENTRY			ATOM_SLIST_ENTRY
	#define A_SLIST_INIT			atom_slist_init
	#define A_SLIST_PUSH			atom_slist_push
	#define A_SLIST_POP				atom_slist_pop
#endif

#ifdef _WIN32
#pragma warning(disable:4121)
#pragma warning(disable:4100)
#pragma warning(disable:4189)
#pragma warning(disable:4355)
#pragma warning(disable:4996)
#pragma warning(disable:4127)
#endif

#endif
