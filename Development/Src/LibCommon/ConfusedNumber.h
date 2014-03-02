#ifndef __ZION_CONFUSED_NUMBER__
#define __ZION_CONFUSED_NUMBER__

namespace Zion
{

	_U32 CNUM_GET(const A_CONFUSED_NUMBER& num);
	void CNUM_SET(A_CONFUSED_NUMBER& num, _U32 val);
	_S32 CNUM_GET(const A_CONFUSED_INT& num);
	void CNUM_SET(A_CONFUSED_INT& num, _S32 val);
	_F32 CNUM_GET(const A_CONFUSED_FLOAT& num);
	void CNUM_SET(A_CONFUSED_FLOAT& num, _F32 val);

	class CNUM
	{
	public:
		CNUM();
		CNUM(const A_CONFUSED_NUMBER& num);
		CNUM(const CNUM& num);
		_U32 Get() const;
		void Set(_U32 val);

	protected:
		A_CONFUSED_NUMBER m_num;
	};

	class CNUM_INT : public CNUM
	{
	public:
		CNUM_INT();
		CNUM_INT(const A_CONFUSED_INT& num);
		CNUM_INT(const CNUM_INT& num);
		_S32 Get() const;
		void Set(_S32 val);
	};

	class CNUM_FLOAT : public CNUM
	{
	public:
		CNUM_FLOAT();
		CNUM_FLOAT(const A_CONFUSED_FLOAT& num);
		CNUM_FLOAT(const CNUM_FLOAT& num);
		_F32 Get() const;
		void Set(_F32 val);
	};

}

namespace DDLDataObject
{

	template<>
	const Zion::CNUM& CConstObject::Get<Zion::CNUM>(const char* name) const
	{
		DDLReflect::FIELD_INFO finfo;
		_U32 offset;
		offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
		ZION_ASSERT(offset!=(_U32)-1);
		ZION_ASSERT(finfo.type==DDLReflect::TYPE_STRUCT && finfo.sinfo==DDLReflect::GetStruct<A_CONFUSED_NUMBER>());
		return *((Zion::CNUM*)((char*)m_pData + offset));
	}

	template<>
	bool CObject::Set<Zion::CNUM>(const char* name, const Zion::CNUM value)
	{
		DDLReflect::FIELD_INFO finfo;
		_U32 offset;
		offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
		if(offset==(_U32)-1)
		{
			ZION_ASSERT(!"invalid name");
			return false;
		}
		if(finfo.type!=DDLReflect::TYPE_STRUCT && finfo.sinfo==DDLReflect::GetStruct<A_CONFUSED_NUMBER>())
		{
			ZION_ASSERT(!"type not match");
			return false;
		}
		memcpy((char*)m_pData + offset, &value, sizeof(value));
		return true;
	}

	template<>
	const Zion::CNUM_INT& CConstObject::Get<Zion::CNUM_INT>(const char* name) const
	{
		DDLReflect::FIELD_INFO finfo;
		_U32 offset;
		offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
		ZION_ASSERT(offset!=(_U32)-1);
		ZION_ASSERT(finfo.type==DDLReflect::TYPE_STRUCT && finfo.sinfo==DDLReflect::GetStruct<A_CONFUSED_INT>());
		return *((Zion::CNUM_INT*)((char*)m_pData + offset));
	}

	template<>
	bool CObject::Set<Zion::CNUM_INT>(const char* name, const Zion::CNUM_INT value)
	{
		DDLReflect::FIELD_INFO finfo;
		_U32 offset;
		offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
		if(offset==(_U32)-1)
		{
			ZION_ASSERT(!"invalid name");
			return false;
		}
		if(finfo.type!=DDLReflect::TYPE_STRUCT && finfo.sinfo==DDLReflect::GetStruct<A_CONFUSED_INT>())
		{
			ZION_ASSERT(!"type not match");
			return false;
		}
		memcpy((char*)m_pData + offset, &value, sizeof(value));
		return true;
	}

	template<>
	const Zion::CNUM_FLOAT& CConstObject::Get<Zion::CNUM_FLOAT>(const char* name) const
	{
		DDLReflect::FIELD_INFO finfo;
		_U32 offset;
		offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
		ZION_ASSERT(offset!=(_U32)-1);
		ZION_ASSERT(finfo.type==DDLReflect::TYPE_STRUCT && finfo.sinfo==DDLReflect::GetStruct<A_CONFUSED_FLOAT>());
		return *((Zion::CNUM_FLOAT*)((char*)m_pData + offset));
	}

	template<>
	bool CObject::Set<Zion::CNUM_FLOAT>(const char* name, const Zion::CNUM_FLOAT value)
	{
		DDLReflect::FIELD_INFO finfo;
		_U32 offset;
		offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
		if(offset==(_U32)-1)
		{
			ZION_ASSERT(!"invalid name");
			return false;
		}
		if(finfo.type!=DDLReflect::TYPE_STRUCT && finfo.sinfo==DDLReflect::GetStruct<A_CONFUSED_FLOAT>())
		{
			ZION_ASSERT(!"type not match");
			return false;
		}
		memcpy((char*)m_pData + offset, &value, sizeof(value));
		return true;
	}

}

#endif
