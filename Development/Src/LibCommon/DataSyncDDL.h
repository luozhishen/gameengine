////////////////////////////////////////////////
// THIS FILE IS AUTOGENERATED. DO NOT MODIFY! //
////////////////////////////////////////////////

#ifndef __DATASYNCDDL_DDL_CODEGEN__
#define __DATASYNCDDL_DDL_CODEGEN__

#pragma pack(push)
#pragma pack(4)

#include "CommonDDL.h"

const _U32 SYNCFLAG_MODEMASK = 1;

const _U32 SYNCFLAG_BINARY = 0;

const _U32 SYNCFLAG_JSON = 1;

const _U32 SYNCFLAG_CLIENT_ACTIVE = 2;

const _U32 SYNCFLAG_SERVER = 4;

class DATASYNC_JSON_C2S;

class DATASYNC_BINARY_C2S;

class DATASYNC_JSON_S2C;

class DATASYNC_BINARY_S2C;

namespace DDLStub
{

	template<typename CLASS>
	class DATASYNC_JSON_C2S : public DDLStub<CLASS>
	{
	public:
		DATASYNC_JSON_C2S(CLASS* Class) : DDLStub<CLASS>(Class)
		{
		}

		virtual const DDLReflect::CLASS_INFO* GetClassInfo()
		{
			return DDLReflect::GetClass<::DATASYNC_JSON_C2S>();
		}
		
		virtual bool Dispatcher(_U16 fid, DDL::BufferReader& Buf)
		{
			if(fid==0)
			{
				_U32 __length;
				char* _prefix_type;
				char* _prefix_data;

				// <string> <type> <> <>;
				if(!Buf.Read(__length)) return false;
				_prefix_type = (char*)alloca(sizeof(_prefix_type[0])*(__length+1));
				if(!_prefix_type) return false;
				if(!Buf.ReadBuffer(_prefix_type, (unsigned int)sizeof(_prefix_type[0])*__length)) return false;
				_prefix_type[__length] = '\0';
				// <string> <data> <> <>;
				if(!Buf.Read(__length)) return false;
				_prefix_data = (char*)alloca(sizeof(_prefix_data[0])*(__length+1));
				if(!_prefix_data) return false;
				if(!Buf.ReadBuffer(_prefix_data, (unsigned int)sizeof(_prefix_data[0])*__length)) return false;
				_prefix_data[__length] = '\0';

				// call implement
				DDLStub<CLASS>::GetClass()->DS_CreateObject(_prefix_type, _prefix_data);
				return true;
			}
			if(fid==1)
			{
				_U32 __length;
				A_UUID _prefix__uuid;
				char* _prefix_data;

				// <A_UUID> <_uuid> <> <>;
				if(!Buf.Read(_prefix__uuid)) return false;
				// <string> <data> <> <>;
				if(!Buf.Read(__length)) return false;
				_prefix_data = (char*)alloca(sizeof(_prefix_data[0])*(__length+1));
				if(!_prefix_data) return false;
				if(!Buf.ReadBuffer(_prefix_data, (unsigned int)sizeof(_prefix_data[0])*__length)) return false;
				_prefix_data[__length] = '\0';

				// call implement
				DDLStub<CLASS>::GetClass()->DS_UpdateObject(_prefix__uuid, _prefix_data);
				return true;
			}
			if(fid==2)
			{
				_U32 __length;
				A_UUID* _prefix__uuid;
				_U32 _prefix_count;

				// <A_UUID> <_uuid> <> <count>;
				if(!Buf.Read(__length)) return false;
				_prefix__uuid = (A_UUID*)alloca(sizeof(_prefix__uuid[0])*__length);
				if(!_prefix__uuid) return false;
				if(!Buf.ReadPointer(_prefix__uuid, __length)) return false;
				// <_U32> <count> <> <>;
				if(!Buf.Read(_prefix_count)) return false;

				// call implement
				DDLStub<CLASS>::GetClass()->DS_RemoveObjects(_prefix__uuid, _prefix_count);
				return true;
			}
			return false;
		}
	};

}

namespace DDLProxy
{

	template<typename CLIENT, typename BUFFER>
	class DATASYNC_JSON_C2S : public DDLProxy<CLIENT, BUFFER>
	{
	public:
		DATASYNC_JSON_C2S(CLIENT* Client) : DDLProxy<CLIENT, BUFFER>(Client, DDLReflect::GetClassID<typename ::DATASYNC_JSON_C2S>())
		{
		}

		static DATASYNC_JSON_C2S<CLIENT, BUFFER> Get(CLIENT* Client)
		{
			DATASYNC_JSON_C2S<CLIENT, BUFFER> Proxy(Client);
			return Proxy;
		}

		bool DS_CreateObject(const char* type, const char* data)
		{
			BUFFER Buf;
			_U32 __length;
			// <string> <type> <> <>
			__length = DDL::StringLength(type);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WriteData(type, (unsigned int)sizeof(type[0])*__length)) return false;
			// <string> <data> <> <>
			__length = DDL::StringLength(data);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WriteData(data, (unsigned int)sizeof(data[0])*__length)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 0, Buf);
		}

		bool DS_UpdateObject(const A_UUID& _uuid, const char* data)
		{
			BUFFER Buf;
			_U32 __length;
			// <A_UUID> <_uuid> <> <>
			if(!Buf.Write(_uuid)) return false;
			// <string> <data> <> <>
			__length = DDL::StringLength(data);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WriteData(data, (unsigned int)sizeof(data[0])*__length)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 1, Buf);
		}

		bool DS_RemoveObjects(const A_UUID* _uuid, _U32 count)
		{
			BUFFER Buf;
			_U32 __length;
			// <A_UUID> <_uuid> <> <count>
			__length = (_U16)(count);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WritePointer(_uuid, __length)) return false;
			// <_U32> <count> <> <>
			if(!Buf.Write(count)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 2, Buf);
		}
	};

}

namespace DDLReflect
{
	template<>
	const CLASS_INFO* GetClass<DATASYNC_JSON_C2S>();
}

namespace DDLStub
{

	template<typename CLASS>
	class DATASYNC_JSON_S2C : public DDLStub<CLASS>
	{
	public:
		DATASYNC_JSON_S2C(CLASS* Class) : DDLStub<CLASS>(Class)
		{
		}

		virtual const DDLReflect::CLASS_INFO* GetClassInfo()
		{
			return DDLReflect::GetClass<::DATASYNC_JSON_S2C>();
		}
		
		virtual bool Dispatcher(_U16 fid, DDL::BufferReader& Buf)
		{
			if(fid==0)
			{
				_U32 _prefix_flag;

				// <_U32> <flag> <> <>;
				if(!Buf.Read(_prefix_flag)) return false;

				// call implement
				DDLStub<CLASS>::GetClass()->DS_SyncOpen(_prefix_flag);
				return true;
			}
			if(fid==1)
			{


				// call implement
				DDLStub<CLASS>::GetClass()->DS_SyncReady();
				return true;
			}
			if(fid==2)
			{


				// call implement
				DDLStub<CLASS>::GetClass()->DS_SyncClose();
				return true;
			}
			if(fid==3)
			{
				A_UUID _prefix__uuid;

				// <A_UUID> <_uuid> <> <>;
				if(!Buf.Read(_prefix__uuid)) return false;

				// call implement
				DDLStub<CLASS>::GetClass()->DS_CreateObjectDone(_prefix__uuid);
				return true;
			}
			if(fid==4)
			{
				_U32 __length;
				char* _prefix_type;
				char* _prefix_json;

				// <string> <type> <> <>;
				if(!Buf.Read(__length)) return false;
				_prefix_type = (char*)alloca(sizeof(_prefix_type[0])*(__length+1));
				if(!_prefix_type) return false;
				if(!Buf.ReadBuffer(_prefix_type, (unsigned int)sizeof(_prefix_type[0])*__length)) return false;
				_prefix_type[__length] = '\0';
				// <string> <json> <> <>;
				if(!Buf.Read(__length)) return false;
				_prefix_json = (char*)alloca(sizeof(_prefix_json[0])*(__length+1));
				if(!_prefix_json) return false;
				if(!Buf.ReadBuffer(_prefix_json, (unsigned int)sizeof(_prefix_json[0])*__length)) return false;
				_prefix_json[__length] = '\0';

				// call implement
				DDLStub<CLASS>::GetClass()->DS_CreateObject(_prefix_type, _prefix_json);
				return true;
			}
			if(fid==5)
			{
				_U32 __length;
				A_UUID _prefix__uuid;
				char* _prefix_json;

				// <A_UUID> <_uuid> <> <>;
				if(!Buf.Read(_prefix__uuid)) return false;
				// <string> <json> <> <>;
				if(!Buf.Read(__length)) return false;
				_prefix_json = (char*)alloca(sizeof(_prefix_json[0])*(__length+1));
				if(!_prefix_json) return false;
				if(!Buf.ReadBuffer(_prefix_json, (unsigned int)sizeof(_prefix_json[0])*__length)) return false;
				_prefix_json[__length] = '\0';

				// call implement
				DDLStub<CLASS>::GetClass()->DS_UpdateObject(_prefix__uuid, _prefix_json);
				return true;
			}
			if(fid==6)
			{
				_U32 __length;
				A_UUID* _prefix__uuid;
				_U32 _prefix_count;

				// <A_UUID> <_uuid> <> <count>;
				if(!Buf.Read(__length)) return false;
				_prefix__uuid = (A_UUID*)alloca(sizeof(_prefix__uuid[0])*__length);
				if(!_prefix__uuid) return false;
				if(!Buf.ReadPointer(_prefix__uuid, __length)) return false;
				// <_U32> <count> <> <>;
				if(!Buf.Read(_prefix_count)) return false;

				// call implement
				DDLStub<CLASS>::GetClass()->DS_RemoveObjects(_prefix__uuid, _prefix_count);
				return true;
			}
			return false;
		}
	};

}

namespace DDLProxy
{

	template<typename CLIENT, typename BUFFER>
	class DATASYNC_JSON_S2C : public DDLProxy<CLIENT, BUFFER>
	{
	public:
		DATASYNC_JSON_S2C(CLIENT* Client) : DDLProxy<CLIENT, BUFFER>(Client, DDLReflect::GetClassID<typename ::DATASYNC_JSON_S2C>())
		{
		}

		static DATASYNC_JSON_S2C<CLIENT, BUFFER> Get(CLIENT* Client)
		{
			DATASYNC_JSON_S2C<CLIENT, BUFFER> Proxy(Client);
			return Proxy;
		}

		bool DS_SyncOpen(_U32 flag)
		{
			BUFFER Buf;
			// <_U32> <flag> <> <>
			if(!Buf.Write(flag)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 0, Buf);
		}

		bool DS_SyncReady()
		{
			BUFFER Buf;

			// send
			return this->GetClient()->Send(this->GetClassID(), 1, Buf);
		}

		bool DS_SyncClose()
		{
			BUFFER Buf;

			// send
			return this->GetClient()->Send(this->GetClassID(), 2, Buf);
		}

		bool DS_CreateObjectDone(const A_UUID& _uuid)
		{
			BUFFER Buf;
			// <A_UUID> <_uuid> <> <>
			if(!Buf.Write(_uuid)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 3, Buf);
		}

		bool DS_CreateObject(const char* type, const char* json)
		{
			BUFFER Buf;
			_U32 __length;
			// <string> <type> <> <>
			__length = DDL::StringLength(type);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WriteData(type, (unsigned int)sizeof(type[0])*__length)) return false;
			// <string> <json> <> <>
			__length = DDL::StringLength(json);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WriteData(json, (unsigned int)sizeof(json[0])*__length)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 4, Buf);
		}

		bool DS_UpdateObject(const A_UUID& _uuid, const char* json)
		{
			BUFFER Buf;
			_U32 __length;
			// <A_UUID> <_uuid> <> <>
			if(!Buf.Write(_uuid)) return false;
			// <string> <json> <> <>
			__length = DDL::StringLength(json);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WriteData(json, (unsigned int)sizeof(json[0])*__length)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 5, Buf);
		}

		bool DS_RemoveObjects(const A_UUID* _uuid, _U32 count)
		{
			BUFFER Buf;
			_U32 __length;
			// <A_UUID> <_uuid> <> <count>
			__length = (_U16)(count);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WritePointer(_uuid, __length)) return false;
			// <_U32> <count> <> <>
			if(!Buf.Write(count)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 6, Buf);
		}
	};

}

namespace DDLReflect
{
	template<>
	const CLASS_INFO* GetClass<DATASYNC_JSON_S2C>();
}

namespace DDLStub
{

	template<typename CLASS>
	class DATASYNC_BINARY_C2S : public DDLStub<CLASS>
	{
	public:
		DATASYNC_BINARY_C2S(CLASS* Class) : DDLStub<CLASS>(Class)
		{
		}

		virtual const DDLReflect::CLASS_INFO* GetClassInfo()
		{
			return DDLReflect::GetClass<::DATASYNC_BINARY_C2S>();
		}
		
		virtual bool Dispatcher(_U16 fid, DDL::BufferReader& Buf)
		{
			if(fid==0)
			{
				_U32 __length;
				_U16 _prefix_type;
				_U8* _prefix_buf;
				_U32 _prefix_len;

				// <_U16> <type> <> <>;
				if(!Buf.Read(_prefix_type)) return false;
				// <_U8> <buf> <> <len>;
				if(!Buf.Read(__length)) return false;
				_prefix_buf = (_U8*)alloca(sizeof(_prefix_buf[0])*__length);
				if(!_prefix_buf) return false;
				if(!Buf.ReadPointer(_prefix_buf, __length)) return false;
				// <_U32> <len> <> <>;
				if(!Buf.Read(_prefix_len)) return false;

				// call implement
				DDLStub<CLASS>::GetClass()->DS_CreateObject(_prefix_type, _prefix_buf, _prefix_len);
				return true;
			}
			if(fid==1)
			{
				_U32 __length;
				A_UUID _prefix__uuid;
				_U8* _prefix_buf;
				_U32 _prefix_len;

				// <A_UUID> <_uuid> <> <>;
				if(!Buf.Read(_prefix__uuid)) return false;
				// <_U8> <buf> <> <len>;
				if(!Buf.Read(__length)) return false;
				_prefix_buf = (_U8*)alloca(sizeof(_prefix_buf[0])*__length);
				if(!_prefix_buf) return false;
				if(!Buf.ReadPointer(_prefix_buf, __length)) return false;
				// <_U32> <len> <> <>;
				if(!Buf.Read(_prefix_len)) return false;

				// call implement
				DDLStub<CLASS>::GetClass()->DS_UpdateObject(_prefix__uuid, _prefix_buf, _prefix_len);
				return true;
			}
			if(fid==2)
			{
				_U32 __length;
				A_UUID* _prefix__uuid;
				_U32 _prefix_count;

				// <A_UUID> <_uuid> <> <count>;
				if(!Buf.Read(__length)) return false;
				_prefix__uuid = (A_UUID*)alloca(sizeof(_prefix__uuid[0])*__length);
				if(!_prefix__uuid) return false;
				if(!Buf.ReadPointer(_prefix__uuid, __length)) return false;
				// <_U32> <count> <> <>;
				if(!Buf.Read(_prefix_count)) return false;

				// call implement
				DDLStub<CLASS>::GetClass()->DS_RemoveObjects(_prefix__uuid, _prefix_count);
				return true;
			}
			return false;
		}
	};

}

namespace DDLProxy
{

	template<typename CLIENT, typename BUFFER>
	class DATASYNC_BINARY_C2S : public DDLProxy<CLIENT, BUFFER>
	{
	public:
		DATASYNC_BINARY_C2S(CLIENT* Client) : DDLProxy<CLIENT, BUFFER>(Client, DDLReflect::GetClassID<typename ::DATASYNC_BINARY_C2S>())
		{
		}

		static DATASYNC_BINARY_C2S<CLIENT, BUFFER> Get(CLIENT* Client)
		{
			DATASYNC_BINARY_C2S<CLIENT, BUFFER> Proxy(Client);
			return Proxy;
		}

		bool DS_CreateObject(_U16 type, const _U8* buf, _U32 len)
		{
			BUFFER Buf;
			_U32 __length;
			// <_U16> <type> <> <>
			if(!Buf.Write(type)) return false;
			// <_U8> <buf> <> <len>
			__length = (_U16)(len);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WritePointer(buf, __length)) return false;
			// <_U32> <len> <> <>
			if(!Buf.Write(len)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 0, Buf);
		}

		bool DS_UpdateObject(const A_UUID& _uuid, const _U8* buf, _U32 len)
		{
			BUFFER Buf;
			_U32 __length;
			// <A_UUID> <_uuid> <> <>
			if(!Buf.Write(_uuid)) return false;
			// <_U8> <buf> <> <len>
			__length = (_U16)(len);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WritePointer(buf, __length)) return false;
			// <_U32> <len> <> <>
			if(!Buf.Write(len)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 1, Buf);
		}

		bool DS_RemoveObjects(const A_UUID* _uuid, _U32 count)
		{
			BUFFER Buf;
			_U32 __length;
			// <A_UUID> <_uuid> <> <count>
			__length = (_U16)(count);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WritePointer(_uuid, __length)) return false;
			// <_U32> <count> <> <>
			if(!Buf.Write(count)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 2, Buf);
		}
	};

}

namespace DDLReflect
{
	template<>
	const CLASS_INFO* GetClass<DATASYNC_BINARY_C2S>();
}

namespace DDLStub
{

	template<typename CLASS>
	class DATASYNC_BINARY_S2C : public DDLStub<CLASS>
	{
	public:
		DATASYNC_BINARY_S2C(CLASS* Class) : DDLStub<CLASS>(Class)
		{
		}

		virtual const DDLReflect::CLASS_INFO* GetClassInfo()
		{
			return DDLReflect::GetClass<::DATASYNC_BINARY_S2C>();
		}
		
		virtual bool Dispatcher(_U16 fid, DDL::BufferReader& Buf)
		{
			if(fid==0)
			{
				_U32 _prefix_flag;

				// <_U32> <flag> <> <>;
				if(!Buf.Read(_prefix_flag)) return false;

				// call implement
				DDLStub<CLASS>::GetClass()->DS_SyncOpen(_prefix_flag);
				return true;
			}
			if(fid==1)
			{


				// call implement
				DDLStub<CLASS>::GetClass()->DS_SyncReady();
				return true;
			}
			if(fid==2)
			{


				// call implement
				DDLStub<CLASS>::GetClass()->DS_SyncClose();
				return true;
			}
			if(fid==3)
			{
				A_UUID _prefix__uuid;

				// <A_UUID> <_uuid> <> <>;
				if(!Buf.Read(_prefix__uuid)) return false;

				// call implement
				DDLStub<CLASS>::GetClass()->DS_CreateObjectDone(_prefix__uuid);
				return true;
			}
			if(fid==4)
			{
				_U32 __length;
				_U16 _prefix_type;
				_U8* _prefix_buf;
				_U32 _prefix_len;

				// <_U16> <type> <> <>;
				if(!Buf.Read(_prefix_type)) return false;
				// <_U8> <buf> <> <len>;
				if(!Buf.Read(__length)) return false;
				_prefix_buf = (_U8*)alloca(sizeof(_prefix_buf[0])*__length);
				if(!_prefix_buf) return false;
				if(!Buf.ReadPointer(_prefix_buf, __length)) return false;
				// <_U32> <len> <> <>;
				if(!Buf.Read(_prefix_len)) return false;

				// call implement
				DDLStub<CLASS>::GetClass()->DS_CreateObject(_prefix_type, _prefix_buf, _prefix_len);
				return true;
			}
			if(fid==5)
			{
				_U32 __length;
				A_UUID _prefix__uuid;
				_U8* _prefix_buf;
				_U32 _prefix_len;

				// <A_UUID> <_uuid> <> <>;
				if(!Buf.Read(_prefix__uuid)) return false;
				// <_U8> <buf> <> <len>;
				if(!Buf.Read(__length)) return false;
				_prefix_buf = (_U8*)alloca(sizeof(_prefix_buf[0])*__length);
				if(!_prefix_buf) return false;
				if(!Buf.ReadPointer(_prefix_buf, __length)) return false;
				// <_U32> <len> <> <>;
				if(!Buf.Read(_prefix_len)) return false;

				// call implement
				DDLStub<CLASS>::GetClass()->DS_UpdateObject(_prefix__uuid, _prefix_buf, _prefix_len);
				return true;
			}
			if(fid==6)
			{
				_U32 __length;
				A_UUID* _prefix__uuid;
				_U32 _prefix_count;

				// <A_UUID> <_uuid> <> <count>;
				if(!Buf.Read(__length)) return false;
				_prefix__uuid = (A_UUID*)alloca(sizeof(_prefix__uuid[0])*__length);
				if(!_prefix__uuid) return false;
				if(!Buf.ReadPointer(_prefix__uuid, __length)) return false;
				// <_U32> <count> <> <>;
				if(!Buf.Read(_prefix_count)) return false;

				// call implement
				DDLStub<CLASS>::GetClass()->DS_RemoveObjects(_prefix__uuid, _prefix_count);
				return true;
			}
			return false;
		}
	};

}

namespace DDLProxy
{

	template<typename CLIENT, typename BUFFER>
	class DATASYNC_BINARY_S2C : public DDLProxy<CLIENT, BUFFER>
	{
	public:
		DATASYNC_BINARY_S2C(CLIENT* Client) : DDLProxy<CLIENT, BUFFER>(Client, DDLReflect::GetClassID<typename ::DATASYNC_BINARY_S2C>())
		{
		}

		static DATASYNC_BINARY_S2C<CLIENT, BUFFER> Get(CLIENT* Client)
		{
			DATASYNC_BINARY_S2C<CLIENT, BUFFER> Proxy(Client);
			return Proxy;
		}

		bool DS_SyncOpen(_U32 flag)
		{
			BUFFER Buf;
			// <_U32> <flag> <> <>
			if(!Buf.Write(flag)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 0, Buf);
		}

		bool DS_SyncReady()
		{
			BUFFER Buf;

			// send
			return this->GetClient()->Send(this->GetClassID(), 1, Buf);
		}

		bool DS_SyncClose()
		{
			BUFFER Buf;

			// send
			return this->GetClient()->Send(this->GetClassID(), 2, Buf);
		}

		bool DS_CreateObjectDone(const A_UUID& _uuid)
		{
			BUFFER Buf;
			// <A_UUID> <_uuid> <> <>
			if(!Buf.Write(_uuid)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 3, Buf);
		}

		bool DS_CreateObject(_U16 type, const _U8* buf, _U32 len)
		{
			BUFFER Buf;
			_U32 __length;
			// <_U16> <type> <> <>
			if(!Buf.Write(type)) return false;
			// <_U8> <buf> <> <len>
			__length = (_U16)(len);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WritePointer(buf, __length)) return false;
			// <_U32> <len> <> <>
			if(!Buf.Write(len)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 4, Buf);
		}

		bool DS_UpdateObject(const A_UUID& _uuid, const _U8* buf, _U32 len)
		{
			BUFFER Buf;
			_U32 __length;
			// <A_UUID> <_uuid> <> <>
			if(!Buf.Write(_uuid)) return false;
			// <_U8> <buf> <> <len>
			__length = (_U16)(len);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WritePointer(buf, __length)) return false;
			// <_U32> <len> <> <>
			if(!Buf.Write(len)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 5, Buf);
		}

		bool DS_RemoveObjects(const A_UUID* _uuid, _U32 count)
		{
			BUFFER Buf;
			_U32 __length;
			// <A_UUID> <_uuid> <> <count>
			__length = (_U16)(count);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WritePointer(_uuid, __length)) return false;
			// <_U32> <count> <> <>
			if(!Buf.Write(count)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 6, Buf);
		}
	};

}

namespace DDLReflect
{
	template<>
	const CLASS_INFO* GetClass<DATASYNC_BINARY_S2C>();
}


#pragma pack(pop)

#endif
