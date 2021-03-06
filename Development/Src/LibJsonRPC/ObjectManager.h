#ifndef	__ZION_OBJECT_MANAGER__
#define	__ZION_OBJECT_MANAGER__

namespace Zion
{

	template<typename T, _U32 COUNT>
	class TObjectManager
	{
	public:
		TObjectManager()
		{
			_mutexs = (A_MUTEX*)malloc(sizeof(A_MUTEX)*COUNT);
			_objects = (T**)malloc(sizeof(T*)*COUNT);
			_entrys = (A_SLIST_ENTRY*)malloc(sizeof(A_SLIST_ENTRY)*COUNT);
			_freelist = (A_SLIST_HEADER*)malloc(sizeof(*_freelist));

			memset(this->_mutexs, 0, sizeof(A_MUTEX)*COUNT);
			memset(this->_objects, 0, sizeof(T*)*COUNT);
			memset(this->_entrys, 0, sizeof(A_SLIST_ENTRY)*COUNT);
			memset(this->_freelist, 0, sizeof(*_freelist));

			A_SLIST_INIT(_freelist);
			for(_U32 i=0; i<COUNT; i++)
			{
				A_MUTEX_INIT(&_mutexs[i]);
				_objects[i] = NULL;
				A_SLIST_PUSH(_freelist, &_entrys[i]);
			}
		}
		~TObjectManager()
		{
			free(_mutexs);
			free(_objects);
			free(_entrys);
			free(_freelist);
		}

		_U32 Insert(T* object)
		{
			A_SLIST_ENTRY* entry = A_SLIST_POP(_freelist);
			if(!entry) return (_U32)-1;
			_U32 index = entry - _entrys;
			A_MUTEX_LOCK(&_mutexs[index]);
			ZION_ASSERT(_objects[index]==NULL);
			_objects[index] = object;
			return index;
		}

		bool Remove(_U32 index, T* object)
		{
			ZION_ASSERT(_objects[index]==object);
			if(_objects[index]!=object) return false;
			_objects[index] = NULL;
			A_MUTEX_UNLOCK(&_mutexs[index]);
			A_SLIST_PUSH(_freelist, &_entrys[index]);
			return true;
		}

		T* Lock(_U32 index)
		{
			A_MUTEX_LOCK(&_mutexs[index]);
			if(_objects[index]==NULL)
			{
				A_MUTEX_UNLOCK(&_mutexs[index]);
				return NULL;
			}
			return _objects[index];
		}

		void Unlock(_U32 index)
		{
			A_MUTEX_UNLOCK(&_mutexs[index]);
		}

		_U32 GetCount()
		{
			return COUNT;
		}

	private:
		A_SLIST_HEADER*	_freelist;
		A_SLIST_ENTRY*	_entrys;
		A_MUTEX*		_mutexs;
		T**				_objects;
		_U32			_count;
	};

	template<typename KEY>
	class TObjectMap
	{
	public:
		TObjectMap()
		{
			uv_rwlock_init(&m_locker);
		}

		~TObjectMap()
		{
			m_map.clear();
			uv_rwlock_destroy(&m_locker);
		}

		bool Insert(const KEY& key, _U32 index)
		{
			uv_rwlock_wrlock(&m_locker);
			if(m_map.find(key)!=m_map.end())
			{
				index = (_U32)-1;
			}
			else
			{
				m_map[key] = index;
			}
			uv_rwlock_wrunlock(&m_locker);
			return index!=(_U32)-1;
		}

		bool Remove(const KEY& key, _U32 index);
/*
		{
			Zion::Map<_U32, _U32>::iterator i;
			uv_rwlock_wrlock(&m_locker);
			i = m_map.find(key);
			if(i==m_map.end() || i->second!=index)
			{
				index = (_U32)-1;
			}
			else
			{
				m_map.erase(i);
			}
			uv_rwlock_wrunlock(&m_locker);
			return index!=(_U32)-1;
		}
*/
		_U32 Get(const KEY& key);
/*
		{
			_U32 index = -1;
			Map<KEY, _U32>::iterator i;
			uv_rwlock_rdlock(&m_locker);
			i = m_map.find(key);
			if(i!=m_map.end()) index = i->second;
			uv_rwlock_rdunlock(&m_locker);
			return index;
		}
*/

	private:
		uv_rwlock_t m_locker;
		Map<KEY, _U32> m_map;
	};

	template<>
	bool TObjectMap<String>::Remove(const String& key, _U32 index);
	template<>
	_U32 TObjectMap<String>::Get(const String& key);
	template<>
	bool TObjectMap<_U32>::Remove(const _U32& key, _U32 index);
	template<>
	_U32 TObjectMap<_U32>::Get(const _U32& key);
	template<>
	bool TObjectMap<_U64>::Remove(const _U64& key, _U32 index);
	template<>
	_U32 TObjectMap<_U64>::Get(const _U64& key);

}

#endif

