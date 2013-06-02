#ifndef __ZION_SERVER_TASK__
#define __ZION_SERVER_TASK__

#include <functional>

namespace Zion
{

	// 1. 可以被异步处理的任务对象
	class CServerTask
	{
		friend class CServerApp;
	public:
		CServerTask(bool bEmit);

		virtual bool SerializeRequest(bool bRead) { return true; }
		virtual bool SerializeResponse(bool bRead) { return true; }

		bool IsEmit() { return m_bEmit; }
		virtual void OnEmit() { }

		template<typename T>
		bool Serialize(bool bRead, T& Val)
		{
			if(m_nDataLen+sizeof(T)>m_nDataMax) return false;
			if(bRead)
			{
				memcpy(&Val, m_pData+m_nDataLen, sizeof(T));
			}
			else
			{
				memcpy(m_pData+m_nDataLen, &Val, sizeof(T));
			}
			m_nDataLen += sizeof(T);
			return true;
		}

		template<typename T>
		bool Serialize(bool bRead, T* Val, _U32 Count, _U32 Max=-1)
		{
			if(Count>Max) return false;
			if(m_nDataLen+sizeof(T)*Count>m_nDataMax) return false;
			if(bRead)
			{
				memcpy(&Val, m_pData+m_nDataLen, sizeof(T)*Count);
			}
			else
			{
				memcpy(m_pData+m_nDataLen, &Val, sizeof(T)*Count);
			}
			m_nDataLen += sizeof(T)*Count;
			return true;			
		}

	private:
		bool m_bEmit;
		_U8* m_pData;
		_U32 m_nDataLen, m_nDataMax;
	};

	template <typename TThisClass, typename TBaseClass>
	class CServerTaskTpl : public TBaseClass
	{
	public:
		CServerTaskTpl() : CServerTask(false)
		{
		}

		CServerTaskTpl(std::tr1::function<void(TThisClass&)>& CallbackPtr) : CServerTask(true)
		{
			m_CallbackPtr = CallbackPtr;
		}

		virtual void OnEmit()
		{
			m_CallbackPtr(*this);
		}

	private:
		std::tr1::function<void(TThisClass&)> m_ResultCallbackPtr;
	};

};

#endif
