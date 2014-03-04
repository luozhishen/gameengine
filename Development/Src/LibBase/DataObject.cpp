#include <ZionBase.h>
#include "CommonDDL.h"
#include "DataObject.h"
#include "ConfusedNumber.h"

namespace Zion
{
	namespace DataObject
	{

		CConstObject::CConstObject()
		{
			m_pInfo = NULL;
			m_pData = NULL;
		}

		CConstObject::CConstObject(const CConstObject* obj)
		{
			m_pInfo = obj->m_pInfo;
			m_pData = obj->m_pData;
			m_Offset = obj->m_Offset;
		}

		CConstObject::CConstObject(const CObject* obj)
		{
			m_pInfo = obj->m_pInfo;
			m_pData = obj->m_pData;
			m_Offset = obj->m_Offset;
		}

		CConstObject::CConstObject(const DDLReflect::STRUCT_INFO* info, void* data, _U32 offset)
		{
			m_pInfo = info;
			m_pData = data;
			m_Offset = offset;
		}

		CConstObject CConstObject::Ref(const char* name) const
		{
			return CConstObject();
		}

		const void* CConstObject::GetData()
		{
			return (char*)m_pData + m_Offset;
		}

		const DDLReflect::STRUCT_INFO* CConstObject::GetStructInfo()
		{
			return m_pInfo;
		}

		_U8 CConstObject::GetU8(const char* name) const
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			ZION_ASSERT(offset!=(_U32)-1);
			ZION_ASSERT(finfo.type==DDLReflect::TYPE_U8);
			return *((_U8*)((char*)m_pData + m_Offset + offset));
		}

		_U16 CConstObject::GetU16(const char* name) const
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			ZION_ASSERT(offset!=(_U32)-1);
			ZION_ASSERT(finfo.type==DDLReflect::TYPE_U16);
			return *((_U16*)((char*)m_pData + m_Offset + offset));
		}

		_U32 CConstObject::GetU32(const char* name) const
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			ZION_ASSERT(offset!=(_U32)-1);
			if(finfo.type==DDLReflect::TYPE_STRUCT && finfo.sinfo==DDLReflect::GetStruct<A_CONFUSED_U32>())
			{
				return CNUM_GET(*((A_CONFUSED_U32*)((char*)m_pData + m_Offset + offset)));
			}
			ZION_ASSERT(finfo.type==DDLReflect::TYPE_U32);
			return *((_U32*)((char*)m_pData + m_Offset + offset));
		}

		_U64 CConstObject::GetU64(const char* name) const
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			ZION_ASSERT(offset!=(_U32)-1);
			ZION_ASSERT(finfo.type==DDLReflect::TYPE_U64);
			return *((_U64*)((char*)m_pData + m_Offset + offset));
		}

		_S8 CConstObject::GetS8(const char* name) const
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			ZION_ASSERT(offset!=(_U32)-1);
			ZION_ASSERT(finfo.type==DDLReflect::TYPE_S8);
			return *((_S8*)((char*)m_pData + m_Offset + offset));
		}

		_S16 CConstObject::GetS16(const char* name) const
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			ZION_ASSERT(offset!=(_U32)-1);
			ZION_ASSERT(finfo.type==DDLReflect::TYPE_S16);
			return *((_S16*)((char*)m_pData + m_Offset + offset));
		}

		_S32 CConstObject::GetS32(const char* name) const
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			ZION_ASSERT(offset!=(_U32)-1);
			if(finfo.type==DDLReflect::TYPE_STRUCT && finfo.sinfo==DDLReflect::GetStruct<A_CONFUSED_S32>())
			{
				return CNUM_GET(*((A_CONFUSED_S32*)((char*)m_pData + m_Offset + offset)));
			}
			ZION_ASSERT(finfo.type==DDLReflect::TYPE_S32);
			return *((_S32*)((char*)m_pData + m_Offset + offset));
		}

		_S64 CConstObject::GetS64(const char* name) const
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			ZION_ASSERT(offset!=(_U32)-1);
			ZION_ASSERT(finfo.type==DDLReflect::TYPE_U64);
			return *((_U64*)((char*)m_pData + m_Offset + offset));
		}

		_F32 CConstObject::GetF32(const char* name) const
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			ZION_ASSERT(offset!=(_U32)-1);
			if(finfo.type==DDLReflect::TYPE_STRUCT && finfo.sinfo==DDLReflect::GetStruct<A_CONFUSED_S32>())
			{
				return CNUM_GET(*((A_CONFUSED_F32*)((char*)m_pData + m_Offset + offset)));
			}
			ZION_ASSERT(finfo.type==DDLReflect::TYPE_F32);
			return *((_F32*)((char*)m_pData + m_Offset + offset));
		}

		_F64 CConstObject::GetF64(const char* name) const
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			ZION_ASSERT(offset!=(_U32)-1);
			ZION_ASSERT(finfo.type==DDLReflect::TYPE_F32);
			return *((_F32*)((char*)m_pData + m_Offset + offset));
		}

		const _STR CConstObject::GetSTR(const char* name) const
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			ZION_ASSERT(offset!=(_U32)-1);
			ZION_ASSERT(finfo.type==DDLReflect::TYPE_STRING);
			return (const _STR)m_pData + m_Offset + offset;
		}

		const A_UUID& CConstObject::GetUUID(const char* name) const
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			ZION_ASSERT(offset!=(_U32)-1);
			ZION_ASSERT(finfo.type==DDLReflect::TYPE_UUID || finfo.type==DDLReflect::TYPE_UUID_REF);
			return *((A_UUID*)((char*)m_pData + m_Offset + offset));
		}

		const void* CConstObject::GetObjPtr(const char* name, const DDLReflect::STRUCT_INFO* type) const
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			ZION_ASSERT(offset!=(_U32)-1);
			ZION_ASSERT(finfo.type==DDLReflect::TYPE_STRUCT && finfo.sinfo==type);
			return (char*)m_pData + m_Offset + offset;
		}

		_U8 CConstObject::GetU8(const char* name, _U32 index) const
		{
			return GetU8(StringFormat("%s[%d]", name, index).c_str());
		}

		_U16 CConstObject::GetU16(const char* name, _U32 index) const
		{
			return GetU16(StringFormat("%s[%d]", name, index).c_str());
		}

		_U32 CConstObject::GetU32(const char* name, _U32 index) const
		{
			return GetU32(StringFormat("%s[%d]", name, index).c_str());
		}

		_U64 CConstObject::GetU64(const char* name, _U32 index) const
		{
			return GetU64(StringFormat("%s[%d]", name, index).c_str());
		}

		_S8 CConstObject::GetS8(const char* name, _U32 index) const
		{
			return GetS8(StringFormat("%s[%d]", name, index).c_str());
		}

		_S16 CConstObject::GetS16(const char* name, _U32 index) const
		{
			return GetS16(StringFormat("%s[%d]", name, index).c_str());
		}

		_S32 CConstObject::GetS32(const char* name, _U32 index) const
		{
			return GetS32(StringFormat("%s[%d]", name, index).c_str());
		}

		_S64 CConstObject::GetS64(const char* name, _U32 index) const
		{
			return GetS64(StringFormat("%s[%d]", name, index).c_str());
		}

		_F32 CConstObject::GetF32(const char* name, _U32 index) const
		{
			return GetF32(StringFormat("%s[%d]", name, index).c_str());
		}

		_F64 CConstObject::GetF64(const char* name, _U32 index) const
		{
			return GetF64(StringFormat("%s[%d]", name, index).c_str());
		}

		const _STR CConstObject::GetSTR(const char* name, _U32 index) const
		{
			return GetSTR(StringFormat("%s[%d]", name, index).c_str());
		}

		const A_UUID& CConstObject::GetUUID(const char* name, _U32 index) const
		{
			return GetUUID(StringFormat("%s[%d]", name, index).c_str());
		}

		const void* CConstObject::GetObjPtr(const char* name, _U32 index, const DDLReflect::STRUCT_INFO* type) const
		{
			return GetObjPtr(StringFormat("%s[%d]", name, type).c_str(), type);
		}

		CObject::CObject()
		{
			m_pMonitor = NULL;
			m_Offset = (_U32)-1;
		}

		CObject::CObject(IMonitor* pMonitor, const DDLReflect::STRUCT_INFO* info, void* data, const char* name, _U32 offset) : CConstObject(info, data, offset)
		{
			m_pMonitor = pMonitor;
			m_Name = name;
		}

		CObject::CObject(IMonitor* pMonitor, const DDLReflect::STRUCT_INFO* info, void* data, const String& name, _U32 offset) : CConstObject(info, data, offset)
		{
			m_pMonitor = pMonitor;
			m_Name = name;
		}

		CObject::CObject(const DDLReflect::STRUCT_INFO* info, void* data, _U32 offset) : CConstObject(info, data, offset)
		{
			m_pMonitor = NULL;
		}

		CObject::CObject(const CObject* pObject) :  CConstObject(pObject)
		{
			m_pMonitor = pObject->m_pMonitor;
			m_Name = pObject->m_Name;
			m_Offset = pObject->m_Offset;
		}

		const CObject CObject::Ref(const char* name)
		{
			return CObject();
		}

		void* CObject::GetData()
		{
			return (char*)m_pData + m_Offset;
		}


		bool CObject::SetU8(const char* name, _U8 val)
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			if(offset==(_U32)-1 || finfo.type!=DDLReflect::TYPE_U8)
			{
				ZION_ASSERT(!"invalid name or invalid type");
				return false;
			}

			*((_U8*)((char*)m_pData + m_Offset + offset)) = val;

			if(m_pMonitor)
			{
				m_pMonitor->SetDirty(StringFormat("%s%s%s", m_Name.c_str(), m_Name.empty()?"":".", name).c_str(), m_Offset+offset, sizeof(val));
			}
			return true;
		}

		bool CObject::SetU16(const char* name, _U16 val)
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			if(offset==(_U32)-1 || finfo.type!=DDLReflect::TYPE_U16)
			{
				ZION_ASSERT(!"invalid name or invalid type");
				return false;
			}

			*((_U16*)((char*)m_pData + m_Offset + offset)) = val;

			if(m_pMonitor)
			{
				m_pMonitor->SetDirty(StringFormat("%s%s%s", m_Name.c_str(), m_Name.empty()?"":".", name).c_str(), m_Offset+offset, sizeof(val));
			}
			return true;
		}

		bool CObject::SetU32(const char* name, _U32 val)
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			if(offset==(_U32)-1)
			{
				ZION_ASSERT(!"invalid name");
				return false;
			}

			if(finfo.type==DDLReflect::TYPE_STRUCT && finfo.sinfo==DDLReflect::GetStruct<A_CONFUSED_U32>())
			{
				CNUM_SET(*((A_CONFUSED_U32*)((char*)m_pData + m_Offset + offset)), val);
				if(m_pMonitor)
				{
					m_pMonitor->SetDirty(StringFormat("%s%s%s", m_Name.c_str(), m_Name.empty()?"":".", name).c_str(), m_Offset+offset, sizeof(A_CONFUSED_U32));
				}
				return true;
			}
			if(finfo.type!=DDLReflect::TYPE_U32)
			{
				ZION_ASSERT(!"invalid type");
				return false;
			}

			*((_U32*)((char*)m_pData + m_Offset + offset)) = val;

			if(m_pMonitor)
			{
				m_pMonitor->SetDirty(StringFormat("%s%s%s", m_Name.c_str(), m_Name.empty()?"":".", name).c_str(), m_Offset+offset, sizeof(val));
			}
			return true;
		}

		bool CObject::SetU64(const char* name, _U64 val)
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			if(offset==(_U32)-1 || finfo.type!=DDLReflect::TYPE_U64)
			{
				ZION_ASSERT(!"invalid name or invalid type");
				return false;
			}

			*((_U64*)((char*)m_pData + m_Offset + offset)) = val;

			if(m_pMonitor)
			{
				m_pMonitor->SetDirty(StringFormat("%s%s%s", m_Name.c_str(), m_Name.empty()?"":".", name).c_str(), m_Offset+offset, sizeof(val));
			}
			return true;
		}

		bool CObject::SetS8(const char* name, _S8 val)
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			if(offset==(_U32)-1 || finfo.type!=DDLReflect::TYPE_S8)
			{
				ZION_ASSERT(!"invalid name or invalid type");
				return false;
			}

			*((_S8*)((char*)m_pData + m_Offset + offset)) = val;

			if(m_pMonitor)
			{
				m_pMonitor->SetDirty(StringFormat("%s%s%s", m_Name.c_str(), m_Name.empty()?"":".", name).c_str(), m_Offset+offset, sizeof(val));
			}
			return true;
		}

		bool CObject::SetS16(const char* name, _S16 val)
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			if(offset==(_U32)-1 || finfo.type!=DDLReflect::TYPE_S16)
			{
				ZION_ASSERT(!"invalid name or invalid type");
				return false;
			}

			*((_S16*)((char*)m_pData + m_Offset + offset)) = val;

			if(m_pMonitor)
			{
				m_pMonitor->SetDirty(StringFormat("%s%s%s", m_Name.c_str(), m_Name.empty()?"":".", name).c_str(), m_Offset+offset, sizeof(val));
			}
			return true;
		}

		bool CObject::SetS32(const char* name, _S32 val)
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			if(offset==(_U32)-1)
			{
				ZION_ASSERT(!"invalid name");
				return false;
			}

			if(finfo.type==DDLReflect::TYPE_STRUCT && finfo.sinfo==DDLReflect::GetStruct<A_CONFUSED_S32>())
			{
				CNUM_SET(*((A_CONFUSED_S32*)((char*)m_pData + m_Offset + offset)), val);
				if(m_pMonitor)
				{
					m_pMonitor->SetDirty(StringFormat("%s%s%s", m_Name.c_str(), m_Name.empty()?"":".", name).c_str(), m_Offset+offset, sizeof(A_CONFUSED_S32));
				}
				return true;
			}
			if(finfo.type!=DDLReflect::TYPE_S32)
			{
				ZION_ASSERT(!"invalid type");
				return false;
			}

			*((_S32*)((char*)m_pData + m_Offset + offset)) = val;

			if(m_pMonitor)
			{
				m_pMonitor->SetDirty(StringFormat("%s%s%s", m_Name.c_str(), m_Name.empty()?"":".", name).c_str(), m_Offset+offset, sizeof(val));
			}
			return true;
		}

		bool CObject::SetS64(const char* name, _S64 val)
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			if(offset==(_U32)-1 || finfo.type!=DDLReflect::TYPE_S64)
			{
				ZION_ASSERT(!"invalid name or invalid type");
				return false;
			}

			*((_S64*)((char*)m_pData + m_Offset + offset)) = val;

			if(m_pMonitor)
			{
				m_pMonitor->SetDirty(StringFormat("%s%s%s", m_Name.c_str(), m_Name.empty()?"":".", name).c_str(), m_Offset+offset, sizeof(val));
			}
			return true;
		}

		bool CObject::SetF32(const char* name, _F32 val)
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			if(offset==(_U32)-1)
			{
				ZION_ASSERT(!"invalid name");
				return false;
			}

			if(finfo.type==DDLReflect::TYPE_STRUCT && finfo.sinfo==DDLReflect::GetStruct<A_CONFUSED_F32>())
			{
				CNUM_SET(*((A_CONFUSED_F32*)((char*)m_pData + m_Offset + offset)), val);
				if(m_pMonitor)
				{
					m_pMonitor->SetDirty(StringFormat("%s%s%s", m_Name.c_str(), m_Name.empty()?"":".", name).c_str(), m_Offset+offset, sizeof(A_CONFUSED_F32));
				}
				return true;
			}
			if(finfo.type!=DDLReflect::TYPE_F32)
			{
				ZION_ASSERT(!"invalid type");
				return false;
			}

			*((_F32*)((char*)m_pData + m_Offset + offset)) = val;

			if(m_pMonitor)
			{
				m_pMonitor->SetDirty(StringFormat("%s%s%s", m_Name.c_str(), m_Name.empty()?"":".", name).c_str(), m_Offset+offset, sizeof(val));
			}
			return true;
		}

		bool CObject::SetF64(const char* name, _F64 val)
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			if(offset==(_U32)-1 || finfo.type!=DDLReflect::TYPE_F64)
			{
				ZION_ASSERT(!"invalid name or invalid type");
				return false;
			}

			*((_F64*)((char*)m_pData + m_Offset + offset)) = val;

			if(m_pMonitor)
			{
				m_pMonitor->SetDirty(StringFormat("%s%s%s", m_Name.c_str(), m_Name.empty()?"":".", name).c_str(), m_Offset+offset, sizeof(val));
			}
			return true;
		}

		bool CObject::SetSTR(const char* name, const _STR val)
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset, len;
			len = DDL::StringLength(val);
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			if(offset==(_U32)-1 || finfo.type!=DDLReflect::TYPE_STRING || len>finfo.slen)
			{
				ZION_ASSERT(!"invalid name or invalid type");
				return false;
			}

			memcpy((char*)m_pData + m_Offset + offset, val, len+1);

			if(m_pMonitor)
			{
				m_pMonitor->SetDirty(StringFormat("%s%s%s", m_Name.c_str(), m_Name.empty()?"":".", name).c_str(), m_Offset+offset, len+1);
			}
			return true;
		}

		bool CObject::SetUUID(const char* name, const A_UUID& val)
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			if(offset==(_U32)-1 || (finfo.type!=DDLReflect::TYPE_UUID && finfo.type!=DDLReflect::TYPE_UUID_REF))
			{
				ZION_ASSERT(!"invalid name or invalid type");
				return false;
			}

			*((A_UUID*)((char*)m_pData + m_Offset + offset)) = val;

			if(m_pMonitor)
			{
				m_pMonitor->SetDirty(StringFormat("%s%s%s", m_Name.c_str(), m_Name.empty()?"":".", name).c_str(), m_Offset+offset, sizeof(val));
			}
			return true;
		}

		bool CObject::SetObjPtr(const char* name, const DDLReflect::STRUCT_INFO* type, const void* val)
		{
			DDLReflect::FIELD_INFO finfo;
			_U32 offset;
			offset = DDLReflect::GetStructFieldOffset(m_pInfo, name, &finfo);
			if(offset==(_U32)-1 || (finfo.type!=DDLReflect::TYPE_UUID && finfo.type!=DDLReflect::TYPE_UUID_REF))
			{
				ZION_ASSERT(!"invalid name or invalid type");
				return false;
			}

			memcpy((char*)m_pData + m_Offset + offset, val, type->size);

			if(m_pMonitor)
			{
				m_pMonitor->SetDirty(StringFormat("%s%s%s", m_Name.c_str(), m_Name.empty()?"":".", name).c_str(), m_Offset+offset, type->size);
			}
			return true;
		}

		bool CObject::SetU8(const char* name, _U32 index, _U8 val)
		{
			return SetU8(StringFormat("%s[%d]", name, index).c_str(), val);
		}

		bool CObject::SetU16(const char* name, _U32 index, _U16 val)
		{
			return SetU16(StringFormat("%s[%d]", name, index).c_str(), val);
		}

		bool CObject::SetU32(const char* name, _U32 index, _U32 val)
		{
			return SetU32(StringFormat("%s[%d]", name, index).c_str(), val);
		}

		bool CObject::SetU64(const char* name, _U32 index, _U64 val)
		{
			return SetU64(StringFormat("%s[%d]", name, index).c_str(), val);
		}

		bool CObject::SetS8(const char* name, _U32 index, _S8 val)
		{
			return SetS8(StringFormat("%s[%d]", name, index).c_str(), val);
		}

		bool CObject::SetS16(const char* name, _U32 index, _S16 val)
		{
			return SetS16(StringFormat("%s[%d]", name, index).c_str(), val);
		}

		bool CObject::SetS32(const char* name, _U32 index, _S32 val)
		{
			return SetS32(StringFormat("%s[%d]", name, index).c_str(), val);
		}

		bool CObject::SetS64(const char* name, _U32 index, _S64 val)
		{
			return SetS64(StringFormat("%s[%d]", name, index).c_str(), val);
		}

		bool CObject::SetF32(const char* name, _U32 index, _F32 val)
		{
			return SetF32(StringFormat("%s[%d]", name, index).c_str(), val);
		}

		bool CObject::SetF64(const char* name, _U32 index, _F64 val)
		{
			return SetF64(StringFormat("%s[%d]", name, index).c_str(), val);
		}

		bool CObject::SetSTR(const char* name, _U32 index, const _STR val)
		{
			return SetSTR(StringFormat("%s[%d]", name, index).c_str(), val);
		}

		bool CObject::SetUUID(const char* name, _U32 index, const A_UUID& val)
		{
			return SetUUID(StringFormat("%s[%d]", name, index).c_str(), val);
		}

		bool CObject::SetObjPtr(const char* name, _U32 index, const DDLReflect::STRUCT_INFO* type, const void* val)
		{
			return SetObjPtr(StringFormat("%s[%d]", name, index).c_str(), type, val);
		}

	}
}
