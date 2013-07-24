#ifndef __DDL_INCLUDE__
#define __DDL_INCLUDE__

#include <string.h>
#include <stdlib.h>

#include "ZionUUID.h"

namespace DDL
{

	_U16 StringLength(const char* src);

	template<typename T, _U32 N>
	class Array
	{
	public:
		Array()
		{
			_Count = 0;
			memset(_Array, 0, sizeof(_Array));
		}

		void Resize(_U32 Count)
		{
			if(Count<=N)
			{
				_Count = Count;
			}
		}

		_U32 _Count;
		T _Array[N];
	};

	template<size_t N>
	class String
	{
	public:
		String()
		{
			_Value[0] = '\0';
		}

		const char* operator*() const
		{
			return _Value;
		}

		char* operator=(const char* Value)
		{
			if(!Assign(Value)) ZION_ASSERT(0);
			return _Value;
		}

		bool Assign(const char* Value)
		{
			size_t len = strlen(Value);
			if(len>=sizeof(_Value)) return false;
			memcpy(_Value, Value, len+1);
			return true;
		}

		char _Value[N+1];
	};

	const void* GetPointerData(const A_UUID& uuid);

	template<typename T>
	class Pointer
	{
	public:
		Pointer()
		{
		}

		const T* Get()
		{
			return (const T*)GetPointerData(uuid);
		}

		A_UUID uuid;
	};

	class BufferReader
	{
	public:
		virtual bool ReadBuffer(void* Buf, _U32 Size) = 0;
		virtual const void* ReadBuffer(_U32 Size) = 0;

		template<typename T>
		bool Read(T& Value)
		{
			return ReadBuffer(&Value, sizeof(Value));
		}

		template<typename T, _U32 N>
		bool ReadArray(Array<T, N>& Array)
		{
			if(!Read(Array._Count)) return false;
			if(Array._Count>N) return false;
			for(_U32 i=0; i<Array._Count; i++)
			{
				if(!Read(Array._Array[i])) return false;
			}
			return true;
		}

		template<typename T>
		bool ReadPointer(T* Ptr, _U32 Count)
		{
			for(_U32 i=0; i<Count; i++)
			{
				if(!Read(Ptr[i])) return false;
			}
			return true;
		}

		template<size_t N>
		bool ReadString(String<N>& Value)
		{
			_U32 length;
			if(!Read(length)) return false;
			if(length>N) return false;
			if(!ReadBuffer(Value._Value, sizeof(Value._Value[0])*length)) return false;
			Value._Value[length] = '\0';
			return true;
		}

		template<size_t N, size_t C>
		bool ReadStringArray(Array<String<N>, C>& Array)
		{
			if(!Read(Array._Count)) return false;
			if(Array._Count>N) return false;
			for(_U32 i=0; i<Array._Count; i++)
			{
				if(!ReadString(Array._Array[i])) return false;
			}
			return true;
		}

		template<size_t S>
		bool ReadStringPointer(String<S>* Ptr, _U32 Count)
		{
			for(_U16 i=0; i<Count; i++)
			{
				if(!ReadString(Ptr[i])) return false;
			}
			return true;
		}
	};

	class BufferWriter
	{
	public:
		virtual bool WriteData(const void* Buf, _U32 Size) = 0;

		template<typename T>
		bool Write(const T& Value)
		{
			return WriteData(&Value, sizeof(Value));
		}

		template<typename T, _U32 N>
		bool WriteArray(const Array<T, N>& Array)
		{
			if(!Write(Array._Count)) return false;
			for(_U32 i=0; i<Array._Count; i++)
			{
				if(!Write(Array._Array[i])) return false;
			}
			return true;
		}
		
		template<typename T>
		bool WritePointer(const T* Ptr, _U32 Count)
		{
			for(_U32 i=0; i<Count; i++)
			{
				if(!Write(Ptr[i])) return false;
			}
			return true;
		}

		template<size_t S>
		bool WriteString(const String<S>& Value)
		{
			_U32 length = (_U32)strlen(Value._Value);
			if(length>S) return false;
			if(!Write(length)) return false;
			if(!WriteData(Value._Value, sizeof(Value._Value[0])*length)) return false;
			return true;
		}

		template<size_t S, size_t N>
		bool WriteStringArray(const Array<String<S>, N>& Array)
		{
			if(!Write(Array._Count)) return false;
			for(_U32 i=0; i<Array._Count; i++)
			{
				if(!WriteString(Array._Array[i])) return false;
			}
			return true;
		}

		template<size_t S>
		bool WriteStringPointer(const String<S>* Ptr, _U32 Count)
		{
			for(_U32 i=0; i<Count; i++)
			{
				if(!WriteString(Ptr[i])) return false;
			}
			return true;
		}

	};

	class MemoryReader : public BufferReader
	{
	public:
		MemoryReader(const _U8* Buf=NULL, _U32 Size=0)
		{
			_Buf = Buf;
			_Size = Size;
			_Cur = 0;
		}

		virtual bool ReadBuffer(void* Buf, _U32 Size)
		{
			if(_Cur+Size>_Size) return false;
			memcpy(Buf, _Buf+_Cur, Size);
			_Cur += Size;
			return true;
		}

		virtual const void* ReadBuffer(_U32 Size)
		{
			if(_Cur+Size>_Size) return NULL;
			_Cur += Size;
			return _Buf + (_Cur-Size);
		}

	private:
		const _U8* _Buf;
		_U32 _Size, _Cur;
	};

	class MemoryWriter : public BufferWriter
	{
	public:
		MemoryWriter(_U8* Buf, _U32 Max)
		{
			m_Buf = Buf;
			m_Max = Max;
			m_Size = 0;
		}

		virtual bool WriteData(const void* Buf, _U32 Size)
		{
			if(m_Size+Size>m_Max) return false;
			memcpy(m_Buf+m_Size, Buf, Size);
			m_Size += Size;
			return true;
		}

		const _U8* GetBuf() { return m_Buf; }
		_U32 GetSize() { return m_Size; }

	private:
		_U8* m_Buf;
		_U32 m_Size;
		_U32 m_Max;
	};

	template<_U32 Max>
	class TMemoryWriter : public MemoryWriter
	{
	public:
		TMemoryWriter() : MemoryWriter(m_Buf, Max)
		{
		}
	private:
		_U8 m_Buf[Max];
	};

}

#endif
