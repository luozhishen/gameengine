#pragma once

namespace DDLStorage
{

	class Tag
	{
	public:
		Tag(DWORD Type)
		{
			m_Type = Type;
		}

		template<class TagClass>
		TagClass* Get();

	private:
		DWORD m_Type;
	};

	class StorageEngine
	{
	public:
		virtual ~StorageEngine() { }
	};

	class DDLStorage
	{
	public:
		DDLStorage(StorageEngine* pEngine)
		{
			m_pEngine = pEngine;
		}
	
		~DDLStorage()
		{
		}

		template<class TAG, class T>
		bool ObjectWrite(const TAG& Tag, const T& Object);

		template<class TAG, class T>
		bool ObjectRead(const TAG& Tag, T& Object);

		template<class TAG, class T, DWORD COUNT>
		bool ArrayWrite(const TAG& Tag, const T& Array[COUNT], DWORD Count);

		template<class TAG, class T, DWORD COUNT>
		bool ArrayRead(const TAG& Tag, T& T[COUNT], DWORD& Count);

	private:
		StorageEngine* m_pEngine;
	};

}
