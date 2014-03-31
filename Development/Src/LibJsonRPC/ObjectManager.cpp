#include <ZionDefines.h>
#include <ZionSTL.h>
#include <uv.h>

#include "ObjectManager.h"

namespace Zion
{

	template<>
	bool TObjectMap<String>::Remove(const String& key, _U32 index)
	{
		Map<String, _U32>::iterator i;
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

	template<>
	_U32 TObjectMap<String>::Get(const String& key)
	{
		_U32 index = -1;
		Map<String, _U32>::iterator i;
		uv_rwlock_rdlock(&m_locker);
		i = m_map.find(key);
		if(i!=m_map.end()) index = i->second;
		uv_rwlock_rdunlock(&m_locker);
		return index;
	}

	template<>
	bool TObjectMap<_U32>::Remove(const _U32& key, _U32 index)
	{
		Map<_U32, _U32>::iterator i;
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

	template<>
	_U32 TObjectMap<_U32>::Get(const _U32& key)
	{
		_U32 index = -1;
		Map<_U32, _U32>::iterator i;
		uv_rwlock_rdlock(&m_locker);
		i = m_map.find(key);
		if(i!=m_map.end()) index = i->second;
		uv_rwlock_rdunlock(&m_locker);
		return index;
	}

}
