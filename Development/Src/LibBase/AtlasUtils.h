#ifndef	__ATLAS_UTILS__
#define	__ATLAS_UTILS__

#include <string>
#include <vector>
#include <map>
#include <functional>

#ifdef WIN32
	#define ATLAS_PATH_SLASH		'\\'
#else
	#define ATLAS_PATH_SLASH		'/'
#endif

namespace Atlas
{

	const char* GetExeDir();

	void StringSplit(const std::string& value, char splitchar, std::vector<std::string>& result);
	std::string StringFormat(const char* fmt, ...);

	class CNoCopy
	{
	private:
		CNoCopy(const CNoCopy&) { }
		CNoCopy& operator=(const CNoCopy&) { return *this; }
	public:
		CNoCopy() { }
	};

	template <typename pool_t>
	pool_t AllocPool(_U32, _U32)
	{
		return(NULL);
	}

	template <typename pool_t>
	void FreePool(pool_t)
	{
	}

	template <typename pool_t>
	_U8* LockBuffer(pool_t)
	{
		return(NULL);
	}

	template <typename pool_t>
	void UnlockBuffer(pool_t, _U8*)
	{
	}

	template <typename pool_t, _U32 LVL_MIN=8, _U32 LVL_MAX=10>
	struct TURBO_ALLOCATOR {
		TURBO_ALLOCATOR() {
			for(_U32 ndx=0; ndx<=LVL_MAX-LVL_MIN; ++ndx) {
				htpools[ndx] = AllocPool<pool_t>(1<<(ndx+LVL_MIN), 0);
			}
		}
		~TURBO_ALLOCATOR() {
			for(_U32 ndx=0; ndx<=LVL_MAX-LVL_MIN; ++ndx) {
				FreePool(htpools[ndx]);
			}
		}
		void *Alloc(size_t size) {
			if(size>(1<<LVL_MAX)) {
				return(NULL);
			} else {
				_U32 ndx;
				for(ndx=0; ndx<=LVL_MAX-LVL_MIN; ++ndx) {
					if((size_t)1<<(ndx+LVL_MIN)>=size) break;
				}
				return(LockBuffer(htpools[ndx]));
			}
		}
		void Free(void *buf) {
			UnlockBuffer(htpools[0], (_U8*)buf);
		}
		pool_t GetMaxPool() {
			return htpools[LVL_MAX-LVL_MIN];
		}
		pool_t htpools[LVL_MAX-LVL_MIN+1];
	};

	template <>
	inline
	HMEMPOOL AllocPool<HMEMPOOL>(_U32 size, _U32 count)
	{
		return AllocMemoryPool(size, count);
	}

	template <>
	inline
	void FreePool(HMEMPOOL pool)
	{
		FreeMemoryPool(pool);
	}

	template <>
	inline
	_U8* LockBuffer(HMEMPOOL pool)
	{
		return (_U8*)LockMemoryPoolBuffer(pool);
	}

	template <>
	inline
	void UnlockBuffer(HMEMPOOL, _U8* buf)
	{
		UnlockMemoryPoolBuffer(buf);
	}

	template <>
	inline
	HIOPOOL AllocPool<HIOPOOL>(_U32 size, _U32 count)
	{
		return AllocIoBufferPool(size, size, 0, count);
	}

	template <>
	inline
	void FreePool(HIOPOOL pool)
	{
		FreeIoBufferPool(pool);
	}

	template <>
	inline
	_U8* LockBuffer(HIOPOOL pool)
	{
		return LockIoBuffer(pool);
	}

	template <>
	inline
	void UnlockBuffer(HIOPOOL, _U8* buf)
	{
		UnlockIoBuffer(buf);
	}

	template<_U32 bcount>
	class IOBUFFER_STREAM : public DDL::BufferWriter
	{
	public:
		IOBUFFER_STREAM(HIOPOOL hPool, _U32 nSkip=4)
		{
			m_hPool = hPool;
			m_nCurLen = nSkip;
			m_nSkip = nSkip;
			m_nBufSize = GetIoBufferSize(hPool);
			memset(m_pBufs, 0, sizeof(m_pBufs));
			m_pBufs[0] = LockIoBuffer(hPool);
		}

		~IOBUFFER_STREAM()
		{
			for(_U32 i=0; i<bcount; i++)
			{
				if(m_pBufs[i])
				{
					UnlockIoBuffer(m_pBufs[i]);
				}
			}
		}

		void Reset()
		{
			for(_U32 i=1; i<bcount; i++)
			{
				if(m_pBufs[i])
				{
					UnlockIoBuffer(m_pBufs[i]);
				}
			}
			if(!m_pBufs[0]) m_pBufs[0] = LockIoBuffer(m_hPool);
			m_nCurLen = m_nSkip;
		}

		virtual bool WriteData(const void* data, _U32 len)
		{
			if(len==0) return false;
			if(m_nCurLen+len>m_nBufSize*bcount) return false;
			_U32 nCurLen = m_nCurLen;
			m_nCurLen += len;
			_U32 start = (nCurLen==0 ? 0 : (nCurLen-1) / m_nBufSize);
			_U32 end = (m_nCurLen-1) / m_nBufSize;
			for(_U32 i=start; i<end; i++)
			{
				m_pBufs[i] = LockIoBuffer(m_hPool);
			}
			if(!data) return true;
			return WriteData(nCurLen, data, len);
		}

		bool WriteData(_U32 pos, const void* data, _U32 len)
		{
			if(pos+len>m_nCurLen) return false;
			_U32 page, unuse, start;
			page = pos / m_nBufSize;
			start = pos % m_nBufSize;
			while(len>0)
			{
				unuse = m_nBufSize - start;
				if(unuse>len) unuse = len;
				memcpy(m_pBufs[page]+start, data, unuse);
				pos += unuse;
				len -= unuse;
				data = (const void*)((const _U8*)data + unuse);
				start = 0;
			}
			return true;
		}

		void Send(HCONNECT hConn)
		{
			if(m_nCurLen>0)
			{
				_U32 c = (m_nCurLen-1) / m_nBufSize + 1, i;
				for(i=1; i<c; i++)
				{
					Atlas::Send(hConn, m_nBufSize, m_pBufs[i-1]);
					m_pBufs[i-1] = NULL;
				}
				c = m_nCurLen % m_nBufSize;
				if(c>0)
				{
					Atlas::Send(hConn, c, m_pBufs[i-1]);
					m_pBufs[i-1] = NULL;
				}
			}
		}

		HIOPOOL m_hPool;
		_U8* m_pBufs[bcount];
		_U32 m_nCurLen, m_nBufSize, m_nSkip;
	};

}

#endif
