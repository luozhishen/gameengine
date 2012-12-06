////////////////////////////////////////////////
// THIS FILE IS AUTOGENERATED. DO NOT MODIFY! //
////////////////////////////////////////////////

#ifndef __COMMONDDL_DDL_CODEGEN__
#define __COMMONDDL_DDL_CODEGEN__

#include <AtlasDefines.h>
#include <AtlasUUID.h>
#include <DDL.h>
#include <DDLProxy.h>
#include <DDLStub.h>
#include <DDLReflect.h>

#pragma pack(push)
#pragma pack(4)

struct A_CONTENT_OBJECT
{
	A_UUID uuid;
	DDL::String<100> name;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<A_CONTENT_OBJECT>(A_CONTENT_OBJECT& Value);
	template<>
	bool BufferWriter::Write<A_CONTENT_OBJECT>(const A_CONTENT_OBJECT& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<A_CONTENT_OBJECT>();
	extern STRUCT_INFO _rfl_struct_A_CONTENT_OBJECT_info;
}

struct A_LIVE_OBJECT
{
	A_UUID uuid;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<A_LIVE_OBJECT>(A_LIVE_OBJECT& Value);
	template<>
	bool BufferWriter::Write<A_LIVE_OBJECT>(const A_LIVE_OBJECT& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<A_LIVE_OBJECT>();
	extern STRUCT_INFO _rfl_struct_A_LIVE_OBJECT_info;
}

struct A_SYNC_OBJECT
{
};

namespace DDL
{
	template<>
	bool BufferReader::Read<A_SYNC_OBJECT>(A_SYNC_OBJECT& Value);
	template<>
	bool BufferWriter::Write<A_SYNC_OBJECT>(const A_SYNC_OBJECT& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<A_SYNC_OBJECT>();
	extern STRUCT_INFO _rfl_struct_A_SYNC_OBJECT_info;
}

class SyncObjectProtocol;

namespace DDLReflect
{
	template<>
	const CLASS_INFO* GetClass<SyncObjectProtocol>();
}

namespace DDLStub
{

	template<typename CALLER, typename CLASS>
	class SyncObjectProtocol : public DDLStub<CALLER, CLASS>
	{
	public:
		SyncObjectProtocol(CLASS* Class) : DDLStub<CALLER, CLASS>(Class)
		{
		}

		virtual bool Dispatcher(CALLER* Caller, _U16 fid, DDL::BufferReader& Buf)
		{
			if(fid==0)
			{
				_U16 _prefix_domain;

				// <_U16> <domain> <> <>;
				if(!Buf.Read(_prefix_domain)) return false;

				// call implement
				DDLStub<CALLER, CLASS>::GetClass()->CreateDomain(Caller, _prefix_domain);
				return true;
			}
			if(fid==1)
			{
				_U16 _prefix_domain;

				// <_U16> <domain> <> <>;
				if(!Buf.Read(_prefix_domain)) return false;

				// call implement
				DDLStub<CALLER, CLASS>::GetClass()->DeleteDomain(Caller, _prefix_domain);
				return true;
			}
			if(fid==2)
			{
				_U16 __length;
				_U16 _prefix_domain;
				_U32 _prefix_id;
				_U16 _prefix_index;
				_U16 _prefix_type;
				_U32 _prefix_len;
				_U8* _prefix_data;

				// <_U16> <domain> <> <>;
				if(!Buf.Read(_prefix_domain)) return false;
				// <_U32> <id> <> <>;
				if(!Buf.Read(_prefix_id)) return false;
				// <_U16> <index> <> <>;
				if(!Buf.Read(_prefix_index)) return false;
				// <_U16> <type> <> <>;
				if(!Buf.Read(_prefix_type)) return false;
				// <_U32> <len> <> <>;
				if(!Buf.Read(_prefix_len)) return false;
				// <_U8> <data> <> <len>;
				if(!Buf.Read(__length)) return false;
				_prefix_data = (_U8*)alloca(sizeof(_prefix_data[0])*__length);
				if(!_prefix_data) return false;
				if(!Buf.ReadPointer(_prefix_data, __length)) return false;

				// call implement
				DDLStub<CALLER, CLASS>::GetClass()->CreateObject(Caller, _prefix_domain, _prefix_id, _prefix_index, _prefix_type, _prefix_len, _prefix_data);
				return true;
			}
			if(fid==3)
			{
				_U16 __length;
				_U16 _prefix_domain;
				_U32 _prefix_id;
				_U16 _prefix_index;
				_U16 _prefix_type;
				_U32 _prefix_len;
				_U8* _prefix_data;

				// <_U16> <domain> <> <>;
				if(!Buf.Read(_prefix_domain)) return false;
				// <_U32> <id> <> <>;
				if(!Buf.Read(_prefix_id)) return false;
				// <_U16> <index> <> <>;
				if(!Buf.Read(_prefix_index)) return false;
				// <_U16> <type> <> <>;
				if(!Buf.Read(_prefix_type)) return false;
				// <_U32> <len> <> <>;
				if(!Buf.Read(_prefix_len)) return false;
				// <_U8> <data> <> <len>;
				if(!Buf.Read(__length)) return false;
				_prefix_data = (_U8*)alloca(sizeof(_prefix_data[0])*__length);
				if(!_prefix_data) return false;
				if(!Buf.ReadPointer(_prefix_data, __length)) return false;

				// call implement
				DDLStub<CALLER, CLASS>::GetClass()->UpdateObject(Caller, _prefix_domain, _prefix_id, _prefix_index, _prefix_type, _prefix_len, _prefix_data);
				return true;
			}
			if(fid==4)
			{
				_U32 _prefix_domain;
				_U32 _prefix_id;
				_U16 _prefix_index;

				// <_U32> <domain> <> <>;
				if(!Buf.Read(_prefix_domain)) return false;
				// <_U32> <id> <> <>;
				if(!Buf.Read(_prefix_id)) return false;
				// <_U16> <index> <> <>;
				if(!Buf.Read(_prefix_index)) return false;

				// call implement
				DDLStub<CALLER, CLASS>::GetClass()->DeleteObject(Caller, _prefix_domain, _prefix_id, _prefix_index);
				return true;
			}
			return false;
		}
	};

}

namespace DDLProxy
{

	template<typename CLIENT, typename BUFFER>
	class SyncObjectProtocol : public DDLProxy<CLIENT, BUFFER>
	{
	public:
		SyncObjectProtocol(CLIENT* Client) : DDLProxy<CLIENT, BUFFER>(Client, DDLReflect::GetClassID<typename ::SyncObjectProtocol>())
		{
		}

		static SyncObjectProtocol<CLIENT, BUFFER> Get(CLIENT* Client)
		{
			SyncObjectProtocol<CLIENT, BUFFER> Proxy(Client);
			return Proxy;
		}

		bool CreateDomain(_U16 domain)
		{
			BUFFER Buf;
			// <_U16> <domain> <> <>
			if(!Buf.Write(domain)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 0, Buf);
		}

		bool DeleteDomain(_U16 domain)
		{
			BUFFER Buf;
			// <_U16> <domain> <> <>
			if(!Buf.Write(domain)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 1, Buf);
		}

		bool CreateObject(_U16 domain, _U32 id, _U16 index, _U16 type, _U32 len, _U8* data)
		{
			BUFFER Buf;
			_U16 __length;
			// <_U16> <domain> <> <>
			if(!Buf.Write(domain)) return false;
			// <_U32> <id> <> <>
			if(!Buf.Write(id)) return false;
			// <_U16> <index> <> <>
			if(!Buf.Write(index)) return false;
			// <_U16> <type> <> <>
			if(!Buf.Write(type)) return false;
			// <_U32> <len> <> <>
			if(!Buf.Write(len)) return false;
			// <_U8> <data> <> <len>
			__length = (_U16)(len);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WritePointer(data, __length)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 2, Buf);
		}

		bool UpdateObject(_U16 domain, _U32 id, _U16 index, _U16 type, _U32 len, _U8* data)
		{
			BUFFER Buf;
			_U16 __length;
			// <_U16> <domain> <> <>
			if(!Buf.Write(domain)) return false;
			// <_U32> <id> <> <>
			if(!Buf.Write(id)) return false;
			// <_U16> <index> <> <>
			if(!Buf.Write(index)) return false;
			// <_U16> <type> <> <>
			if(!Buf.Write(type)) return false;
			// <_U32> <len> <> <>
			if(!Buf.Write(len)) return false;
			// <_U8> <data> <> <len>
			__length = (_U16)(len);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WritePointer(data, __length)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 3, Buf);
		}

		bool DeleteObject(_U32 domain, _U32 id, _U16 index)
		{
			BUFFER Buf;
			// <_U32> <domain> <> <>
			if(!Buf.Write(domain)) return false;
			// <_U32> <id> <> <>
			if(!Buf.Write(id)) return false;
			// <_U16> <index> <> <>
			if(!Buf.Write(index)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 4, Buf);
		}
	};

}

struct SG_ITEM
{
	A_UUID fref;
	A_UUID fuuid;
	_U8 u8;
	_U16 u16;
	_U32 u32;
	_U64 u64;
	_S8 s8;
	_S16 s16;
	_S32 s32;
	_S64 s64;
	_F32 f32;
	_F64 f64;
	DDL::String<100> str;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<SG_ITEM>(SG_ITEM& Value);
	template<>
	bool BufferWriter::Write<SG_ITEM>(const SG_ITEM& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<SG_ITEM>();
	extern STRUCT_INFO _rfl_struct_SG_ITEM_info;
}


#pragma pack(pop)

#endif
