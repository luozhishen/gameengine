#ifndef __DDL_DATA_OBJECT_INCLUDE__
#define __DDL_DATA_OBJECT_INCLUDE__

namespace DDLDataObject
{

	class IMonitor
	{
	public:
		virtual ~IMonitor() {}

		virtual void SetDirty(const char* name, _U32 offset, _U32 size) = 0;
	};

	class CConstObject;
	class CObject;

	class CConstObject
	{
	public:
		CConstObject();
		CConstObject(const CConstObject& obj);
		CConstObject(const CObject& obj);
		CConstObject(const DDLReflect::STRUCT_INFO* info, void* data, _U32 offset);
		CConstObject Ref(const char* name) const;

		template<typename T>
		const T& Get(const char* name) const
		{
			DDLReflect::FIELD_INFO cinfo;
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			ZION_VERIFY(DDLReflect::GetPtrType<T*>(cinfo));
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			ZION_ASSERT(offset!=(_U32)-1);
			ZION_ASSERT(cinfo.type==finfo.type || (cinfo.type==DDLReflect::TYPE_UUID && finfo.type==DDLReflect::TYPE_UUID_REF));
			ZION_ASSERT(cinfo.sinfo==finfo.sinfo);
			return *((T*)((char*)m_pData + m_Offset + offset));
		}

		template<typename T>
		const T& Get(const char* name, _U32 index) const
		{
			Zion::String n = Zion::StringFormat("%s[%d]", name, index);
			return Get<T>(n.c_str());
		}

		const void* GetData();
		const DDLReflect::STRUCT_INFO* GetStructInfo();

	protected:
		_U32 m_Offset;
		void* m_pData;
		const DDLReflect::STRUCT_INFO* m_pInfo;
	};

	template<>
	const _STR CConstObject::Get<_STR>(const char* name) const
	{
		DDLReflect::FIELD_INFO finfo;
		_U32 offset;
		offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
		ZION_ASSERT(offset!=(_U32)-1);
		ZION_ASSERT(finfo.type==DDLReflect::TYPE_STRING);
		return (char*)m_pData + m_Offset + offset;
	}

	class CObject : public CConstObject
	{
	public:
		CObject();
		CObject(IMonitor* pMonitor, const DDLReflect::STRUCT_INFO* info, void* data, const char* name, _U32 offset);
		CObject(IMonitor* pMonitor, const DDLReflect::STRUCT_INFO* info, void* data, const Zion::String& name, _U32 offset);
		CObject(const DDLReflect::STRUCT_INFO* info, void* data, _U32 offset);
		CObject(const CObject* pObject);
		const CObject Ref(const char* name);

		template<typename T>
		bool Set(const char* name, const T value)
		{
			DDLReflect::FIELD_INFO cinfo;
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			if(!DDLReflect::GetPtrType<T*>(cinfo))
			{
				ZION_ASSERT(!"type not found");
				return false;
			}
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			if(offset==(_U32)-1)
			{
				ZION_ASSERT(!"invalid name");
				return false;
			}

			if((cinfo.type!=finfo.type || cinfo.sinfo!=finfo.sinfo) && (cinfo.type!=DDLReflect::TYPE_UUID || finfo.type!=DDLReflect::TYPE_UUID_REF))
			{
				ZION_ASSERT(!"type not match");
				return false;
			}

			*((T*)((char*)m_pData + offset)) = value;
			if(m_pMonitor)
			{
				Zion::String oname;
				if(!m_Name.empty() && m_Name[m_Name.size()-1]==']')
				{
					oname = Zion::StringFormat("%s%s", m_Name.c_str(), name);
				}
				else
				{
					oname = Zion::StringFormat("%s.%s", m_Name.c_str(), name);
				}
				m_pMonitor->SetDirty(oname.c_str(), m_Offset+offset, sizeof(T));
			}
			return true;
		}

		template<typename T>
		bool Set(const char* name, _U32 index, const T& value)
		{
			Zion::String n = Zion::StringFormat("%s[%d]", name, index);
			return Set<T>(n.c_str(), value);
		}

		void* GetData();

	private:
		IMonitor* m_pMonitor;
		Zion::String m_Name;
	};

	template<>
	bool CObject::Set<_STR>(const char* name, const _STR value)
	{
		DDLReflect::FIELD_INFO finfo;
		_U32 offset;
		offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
		if(offset==(_U32)-1)
		{
			ZION_ASSERT(!"invalid name");
			return false;
		}
		if(finfo.type!=DDLReflect::TYPE_STRING)
		{
			ZION_ASSERT(!"type not match");
			return false;
		}
		_U32 len = DDL::StringLength(value);
		if(len>finfo.slen) return false;
		memcpy((char*)m_pData + offset, value, len+1);
		if(m_pMonitor)
		{
			Zion::String oname;
			if(m_Name.empty() || m_Name[m_Name.size()-1]==']')
			{
				oname = Zion::StringFormat("%s%s", m_Name.c_str(), name);
			}
			else
			{
				oname = Zion::StringFormat("%s.%s", m_Name.c_str(), name);
			}
			m_pMonitor->SetDirty(oname.c_str(), m_Offset+offset, len+1);
		}
		return true;
	}

}

#endif
