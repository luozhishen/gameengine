#ifndef __ZION_SERVER_UTILS__
#define __ZION_SERVER_UTILS__

#include <hash_map>

namespace Zion
{

	template<class T, _U32 count>
	class CObjectManager
	{
	public:
		CObjectManager(_U64 (T::*pGetNDX)())
		{
			A_SLIST_INIT(&m_GlobalFreeObjects);
			for(_U32 i=count; i>0; i--)
			{
				m_GlobalObjects[i-1] = NULL;
				A_MUTEX_INIT(&m_csGlobalObjects[i-1]);
				A_SLIST_PUSH(&m_GlobalFreeObjects, &m_etGlobalObjects[i-1]);
			}
			m_pGetNDX = pGetNDX;
		}
		~CObjectManager()
		{
			for(_U32 i=0; i<count; i++)
			{
				ZION_ASSERT(!m_GlobalObjects[i]);
				A_MUTEX_DESTROY(&m_csGlobalObjects[i]);
			}
		}

		T* TryLock(_U64 xndx, bool bVerify=true)
		{
			_U64 ndx = (xndx&0xffffffff);
			if(ndx>=(_U64)count) return NULL;
			if(!m_GlobalObjects[ndx]) return NULL;
			if(!A_MUTEX_TRYLOCK(&m_etGlobalObjects[ndx])) return NULL;
			if(m_GlobalObjects[ndx])
			{
				T* pObj = m_GlobalObjects[ndx];
				if(!m_pGetNDX || !bVerify || (pObj->*m_pGetNDX())==xndx) return pObj;
			}
			A_MUTEX_UNLOCK(&m_csGlobalObjects[ndx]);
			return NULL;
		}

		T* Lock(_U64 xndx, bool bVerify=true)
		{
			_U64 ndx = (xndx&0xffffffff);
			if(ndx>=(_U64)count) return NULL;
			if(!m_GlobalObjects[ndx]) return NULL;
			A_MUTEX_LOCK(&m_csGlobalObjects[ndx]);
			if(m_GlobalObjects[ndx])
			{
				T* pObj = m_GlobalObjects[ndx];
				if(!m_pGetNDX || !bVerify || (pObj->*m_pGetNDX)()==xndx) return pObj;
			}
			A_MUTEX_UNLOCK(&m_csGlobalObjects[ndx]);
			return NULL;
		}

		void Unlock(_U64 ndx)
		{
			ndx = (ndx&0xffffffff);
			ZION_ASSERT(ndx<(_U64)count);
			if(!m_GlobalObjects[ndx])
			{
				A_SLIST_PUSH(&m_GlobalFreeObjects, &m_etGlobalObjects[ndx]);
			}
			A_MUTEX_UNLOCK(&m_csGlobalObjects[ndx]);
		}

		_U64 Alloc(T* pObj=NULL)
		{
			A_SLIST_ENTRY* entry = A_SLIST_POP(&m_GlobalFreeObjects);
			if(!entry) return (_U64)-1;
			_U64 ndx = (_U64)(entry - m_etGlobalObjects);
			A_MUTEX_LOCK(&m_csGlobalObjects[ndx]);
			m_GlobalObjects[ndx] = pObj;
			return ndx;
		}

		void BindObject(_U64 ndx, T* pObj)
		{
			ndx = (ndx&0xffffffff);
			ZION_ASSERT(ndx<(_U64)count);
			ZION_ASSERT(m_GlobalObjects[ndx]==NULL);
			if(ndx>=(_U64)count) return;
			m_GlobalObjects[ndx] = pObj;
		}

		void UnbindObject(_U64 ndx, T* pObj)
		{
			ndx = (ndx&0xffffffff);
			ZION_ASSERT(ndx<(_U64)count);
			ZION_ASSERT(m_GlobalObjects[ndx]==pObj);
			if(ndx>=(_U64)count) return;
			m_GlobalObjects[ndx] = NULL;
		}

		T* GetObject(_U64 ndx)
		{
			ndx = (ndx&0xffffffff);
			ZION_ASSERT(ndx<(_U64)count);
			if(ndx>=(_U64)count) return NULL;
			return m_GlobalObjects[ndx];
		}

	private:
		_U64 (T::*m_pGetNDX)();
		T*	m_GlobalObjects[count];
		A_MUTEX				m_csGlobalObjects[count];
		A_SLIST_ENTRY		m_etGlobalObjects[count];
		A_SLIST_HEADER		m_GlobalFreeObjects;
	};

	template<class T>
	class CObjectLocker
	{
	public:
		CObjectLocker(_U64 ndx)
		{
			m_NDX = ndx;
			m_pObject = Lock(ndx);
		}
		~CObjectLocker()
		{
			if(m_pObject)
			{
				Unlock(m_NDX);
			}
		}

		T* Lock(_U64 ndx);
		void Unlock(_U64 ndx);

		T* GetObject() { return m_pObject; }

	private:
		_U64 m_NDX;
		T* m_pObject;		
	};

	class CServerIndexMap
	{
	public:
		CServerIndexMap();
		~CServerIndexMap();

		_U64 Bind(_U32 key, _U64 value);
		bool Unbind(_U32 key, _U64 value=-1);
		bool GetValue(_U32 key, _U64& value);

		template<class T>
		void Enum(T* pThis, bool (T::*pFunc)(_U64 key, _U64 value))
		{
			std::hash_map<_U32, _U64>::iterator i;
			A_MUTEX_LOCK(&m_mtxMap);
			for(i=m_Map.begin(); i!=m_Map.end(); i++)
			{
				if(!(pThis->*pFunc)(i->first, i->second)) break;
			}
			A_MUTEX_UNLOCK(&m_mtxMap);
		}

	private:
		std::hash_map<_U32, _U64> m_Map;
		A_MUTEX m_mtxMap;
	};

}

#endif
