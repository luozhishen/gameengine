#include "AtlasBase.h"
#include "ServerUtils.h"

namespace Atlas
{

	CServerIndexMap::CServerIndexMap()
	{
		A_MUTEX_INIT(&m_mtxMap);
	}

	CServerIndexMap::~CServerIndexMap()
	{
		A_MUTEX_DESTROY(&m_mtxMap);
	}

	_U64 CServerIndexMap::Bind(_U32 key, _U64 value)
	{
		_U64 ret;
		std::hash_map<_U32, _U64>::iterator i;
		A_MUTEX_LOCK(&m_mtxMap);
		i = m_Map.find(key);
		if(i==m_Map.end())
		{
			m_Map[key] = value;
			ret = (_U64)-1;
		}
		else
		{
			ret = i->second;
		}
		A_MUTEX_UNLOCK(&m_mtxMap);
		return ret;
	}

	bool CServerIndexMap::Unbind(_U32 key, _U64 value)
	{
		bool ret;
		std::hash_map<_U32, _U64>::iterator i;
		A_MUTEX_LOCK(&m_mtxMap);
		i = m_Map.find(key);
		if(i!=m_Map.end())
		{
			if(value!=-1 && i->second!=value)
			{
				ret = false;
			}
			else
			{
				m_Map.erase(i);
				ret = true;
			}
		}
		else
		{
			ret = false;
		}
		A_MUTEX_UNLOCK(&m_mtxMap);
		return ret;
	}

	bool CServerIndexMap::GetValue(_U32 key, _U64& value)
	{
		bool ret;
		std::hash_map<_U32, _U64>::iterator i;
		A_MUTEX_LOCK(&m_mtxMap);
		i = m_Map.find(key);
		if(i==m_Map.end())
		{
			ret = false;
		}
		else
		{
			value = i->second;
			ret = true;
		}
		A_MUTEX_UNLOCK(&m_mtxMap);
		return ret;
	}

}
