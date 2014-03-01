#ifndef __DDL_DATA_OBJECT_INCLUDE__
#define __DDL_DATA_OBJECT_INCLUDE__

namespace DDLDataObject
{

	class CMonitor
	{
	public:
		CMonitor(const DDLReflect::STRUCT_INFO* info, void* data);

		virtual void SetDirty(const char* name, _U32 offset, _U32 size) = 0;

	private:
		const DDLReflect::STRUCT_INFO* m_pInfo;
		void* m_pData;
	};

	class CDataObject
	{
	public:
		CDataObject(DDLReflect::STRUCT_INFO* info, void* data, CMonitor* pMonitor, _U32 offset);
		CDataObject(DDLReflect::STRUCT_INFO* info, void* data);

		template<typename T>
		const T Get(const char* name) const
		{
			DDLReflect::FIELD_INFO cinfo;
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			ZION_VERIFY(DDLReflect::GetField<T>(cinfo));
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			ZION_ASSERT(offset!=(_U32)-1);
			ZION_ASSERT(cinfo.type==finfo.type || (cinfo.type==TYPE_UUID && finfo.type==TYPE_UUID_REF));
			ZION_ASSERT(cinfo.sinfo==finfo.sinfo);
			return *((T*)((char*)data + offset));
		}

		template<char*>
		const char* Get(const char* name) const
		{
			DDLReflect::FIELD_INFO finfo;
			ZION_ASSERT(DDLReflect::GetStructFieldInfo(m_pInfo, name, finfo));
			ZION_ASSERT(finfo.type==TYPE_STRING);
			return (char*)data + offset;
		}

		template<Zion::String&>
		const Zion::String& Get(const char* name) const
		{
			DDLReflect::FIELD_INFO finfo;
			ZION_ASSERT(DDLReflect::GetStructFieldInfo(m_pInfo, name, finfo));
			ZION_ASSERT(finfo.type==TYPE_STRING);
			return Zion::String((char*)data + offset);
		}

		template<typename T>
		const T Get(const char* name, _U32 index) const
		{
			ZionString n = Zion::StringFormat("%s[%d]", name, index);
			return Get(n.c_str(), name);
		}

		template<typename T>
		void Set(const char* name, const T& value)
		{
			DDLReflect::FIELD_INFO cinfo;
			DDLReflect::FIELD_INFO finfo;
			ZION_ASSERT(DDLReflect::GenFieldInfo<T>(cinfo));
			ZION_ASSERT(DDLReflect::GetStructFieldInfo(m_pInfo, name, finfo));
			ZION_ASSERT(cinfo==finfo);
			*((T*)data) = value;
		}

		template<typename T>
		void Set(const char* name, _U32 index, const T& value)
		{
			ZionString n = Zion::StringFormat("%s[%d]", name, index);
			Set(n.c_str(), name);
		}

	private:
		CMonitor* m_pMonitor;
		_U32 m_Offset;
		void* m_pData;
		DDLReflect::STRUCT_INFO* m_pInfo;
	};

}

#endif
