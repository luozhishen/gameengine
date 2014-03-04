////////////////////////////////////////////////
// THIS FILE IS AUTOGENERATED. DO NOT MODIFY! //
////////////////////////////////////////////////

#ifndef __CARDGAMEDDL_DDL_CODEGEN__
#define __CARDGAMEDDL_DDL_CODEGEN__

#pragma pack(push)
#pragma pack(4)

#include "../../LibBase/ConfusedNumberDDL.h"

#include "../../LibCommon/CommonDDL.h"

#include "../../LibCommon/DataSyncDDL.h"

struct CARD_AVATAR_OPERATOR : A_CONTENT_OBJECT
{
};

namespace DDL
{
	template<>
	bool BufferReader::Read<CARD_AVATAR_OPERATOR>(CARD_AVATAR_OPERATOR& Value);
	template<>
	bool BufferWriter::Write<CARD_AVATAR_OPERATOR>(const CARD_AVATAR_OPERATOR& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<CARD_AVATAR_OPERATOR>();
	extern STRUCT_INFO _rfl_struct_CARD_AVATAR_OPERATOR_info;
}

struct CARD_AVATAR_DESC : A_LIVE_OBJECT
{
	_U32 avatar_id;
	DDL::String<100> avatar_name;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<CARD_AVATAR_DESC>(CARD_AVATAR_DESC& Value);
	template<>
	bool BufferWriter::Write<CARD_AVATAR_DESC>(const CARD_AVATAR_DESC& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<CARD_AVATAR_DESC>();
	extern STRUCT_INFO _rfl_struct_CARD_AVATAR_DESC_info;
}

struct CARD_AVATAR : A_LIVE_OBJECT
{
	DDL::String<100> avatar_name;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<CARD_AVATAR>(CARD_AVATAR& Value);
	template<>
	bool BufferWriter::Write<CARD_AVATAR>(const CARD_AVATAR& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<CARD_AVATAR>();
	extern STRUCT_INFO _rfl_struct_CARD_AVATAR_info;
}

struct CARD_AVATAR_OWNOBJ : A_LIVE_OBJECT
{
	DDL::String<100> obj_name;
	A_CONFUSED_F32 obj_value;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<CARD_AVATAR_OWNOBJ>(CARD_AVATAR_OWNOBJ& Value);
	template<>
	bool BufferWriter::Write<CARD_AVATAR_OWNOBJ>(const CARD_AVATAR_OWNOBJ& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<CARD_AVATAR_OWNOBJ>();
	extern STRUCT_INFO _rfl_struct_CARD_AVATAR_OWNOBJ_info;
}

class CARDGAME_C2S;

namespace DDLReflect
{
	template<>
	const CLASS_INFO* GetClass<CARDGAME_C2S>();
}

class CARDGAME_S2C;

namespace DDLReflect
{
	template<>
	const CLASS_INFO* GetClass<CARDGAME_S2C>();
}

namespace DDLProxy
{

	template<typename CLIENT, typename BUFFER>
	class CARDGAME_C2S : public DDLProxy<CLIENT, BUFFER>
	{
	public:
		CARDGAME_C2S(CLIENT* Client) : DDLProxy<CLIENT, BUFFER>(Client, DDLReflect::GetClassID<typename ::CARDGAME_C2S>())
		{
		}

		static CARDGAME_C2S<CLIENT, BUFFER> Get(CLIENT* Client)
		{
			CARDGAME_C2S<CLIENT, BUFFER> Proxy(Client);
			return Proxy;
		}

		bool Ping()
		{
			BUFFER Buf;

			// send
			return this->GetClient()->Send(this->GetClassID(), 0, Buf);
		}

		bool SelectServer(_U32 server_id)
		{
			BUFFER Buf;
			// <_U32> <server_id> <> <>
			if(!Buf.Write(server_id)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 1, Buf);
		}

		bool GetAvatarList()
		{
			BUFFER Buf;

			// send
			return this->GetClient()->Send(this->GetClassID(), 2, Buf);
		}

		bool CreateAvatar(const char* avatar_name, _U32 type)
		{
			BUFFER Buf;
			_U32 __length;
			// <string> <avatar_name> <> <>
			__length = DDL::StringLength(avatar_name);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WriteData(avatar_name, (unsigned int)sizeof(avatar_name[0])*__length)) return false;
			// <_U32> <type> <> <>
			if(!Buf.Write(type)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 3, Buf);
		}

		bool EnterGame(_U32 avatar_id)
		{
			BUFFER Buf;
			// <_U32> <avatar_id> <> <>
			if(!Buf.Write(avatar_id)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 4, Buf);
		}

		bool LeaveGame()
		{
			BUFFER Buf;

			// send
			return this->GetClient()->Send(this->GetClassID(), 5, Buf);
		}
	};

}

namespace DDLSigSlot
{

	class CARDGAME_S2C : public DDLStub::IStub
	{
	public:
		virtual const DDLReflect::CLASS_INFO* GetClassInfo()
		{
			return DDLReflect::GetClass<::CARDGAME_S2C>();
		}
		
		virtual bool Dispatcher(_U16 fid, DDL::BufferReader& Buf)
		{
			if(fid==0)
			{
				// call implement
				_Pong();
				return true;
			}
			if(fid==1)
			{
				_U32 __length;
				_U32 _prefix_errcode;
				CARD_AVATAR_DESC* _prefix_arr;
				_U32 _prefix_count;

				// <_U32> <errcode> <> <>;
				if(!Buf.Read(_prefix_errcode)) return false;
				// <CARD_AVATAR_DESC> <arr> <> <100>;
				if(!Buf.Read(__length)) return false;
				_prefix_arr = (CARD_AVATAR_DESC*)alloca(sizeof(_prefix_arr[0])*__length);
				if(!_prefix_arr) return false;
				if(!Buf.ReadPointer(_prefix_arr, __length)) return false;
				// <_U32> <count> <> <>;
				if(!Buf.Read(_prefix_count)) return false;

				// call implement
				_GetAvatarListCallback(_prefix_errcode, _prefix_arr, _prefix_count);
				return true;
			}
			if(fid==2)
			{
				_U32 _prefix_errcode;

				// <_U32> <errcode> <> <>;
				if(!Buf.Read(_prefix_errcode)) return false;

				// call implement
				_CreateAvatarCallback(_prefix_errcode);
				return true;
			}
			if(fid==3)
			{
				_U32 _prefix_errcode;

				// <_U32> <errcode> <> <>;
				if(!Buf.Read(_prefix_errcode)) return false;

				// call implement
				_EnterGameCallback(_prefix_errcode);
				return true;
			}
			if(fid==4)
			{
				_U32 _prefix_errcode;

				// <_U32> <errcode> <> <>;
				if(!Buf.Read(_prefix_errcode)) return false;

				// call implement
				_LeaveGameCallback(_prefix_errcode);
				return true;
			}
			return false;
		}

		sigslot::signal0<> _Pong;
		sigslot::signal3<_U32, CARD_AVATAR_DESC*, _U32> _GetAvatarListCallback;
		sigslot::signal1<_U32> _CreateAvatarCallback;
		sigslot::signal1<_U32> _EnterGameCallback;
		sigslot::signal1<_U32> _LeaveGameCallback;
	};

}

class CARDGAME_OP;

namespace DDLReflect
{
	template<>
	const CLASS_INFO* GetClass<CARDGAME_OP>();
}

namespace DDLStub
{

	template<typename CLASS>
	class CARDGAME_OP : public DDLStub<CLASS>
	{
	public:
		CARDGAME_OP(CLASS* Class) : DDLStub<CLASS>(Class)
		{
		}

		virtual const DDLReflect::CLASS_INFO* GetClassInfo()
		{
			return DDLReflect::GetClass<::CARDGAME_OP>();
		}
		
		virtual bool Dispatcher(_U16 fid, DDL::BufferReader& Buf)
		{
			if(fid==0)
			{
				_U32 __length;
				char* _prefix_name;
				_S32 _prefix_value;

				// <string> <name> <> <>;
				if(!Buf.Read(__length)) return false;
				_prefix_name = (char*)alloca(sizeof(_prefix_name[0])*(__length+1));
				if(!_prefix_name) return false;
				if(!Buf.ReadBuffer(_prefix_name, (unsigned int)sizeof(_prefix_name[0])*__length)) return false;
				_prefix_name[__length] = '\0';
				// <_S32> <value> <> <>;
				if(!Buf.Read(_prefix_value)) return false;

				// call implement
				DDLStub<CLASS>::GetClass()->AddOwnObj(_prefix_name, _prefix_value);
				return true;
			}
			if(fid==1)
			{
				A_UUID _prefix__uuid;

				// <A_UUID> <_uuid> <> <>;
				if(!Buf.Read(_prefix__uuid)) return false;

				// call implement
				DDLStub<CLASS>::GetClass()->DelOwnObj(_prefix__uuid);
				return true;
			}
			if(fid==2)
			{
				A_UUID _prefix__uuid;
				_S32 _prefix_value;

				// <A_UUID> <_uuid> <> <>;
				if(!Buf.Read(_prefix__uuid)) return false;
				// <_S32> <value> <> <>;
				if(!Buf.Read(_prefix_value)) return false;

				// call implement
				DDLStub<CLASS>::GetClass()->AddOwnObjValue(_prefix__uuid, _prefix_value);
				return true;
			}
			return false;
		}
	};

}

namespace DDLProxy
{

	template<typename CLIENT, typename BUFFER>
	class CARDGAME_OP : public DDLProxy<CLIENT, BUFFER>
	{
	public:
		CARDGAME_OP(CLIENT* Client) : DDLProxy<CLIENT, BUFFER>(Client, DDLReflect::GetClassID<typename ::CARDGAME_OP>())
		{
		}

		static CARDGAME_OP<CLIENT, BUFFER> Get(CLIENT* Client)
		{
			CARDGAME_OP<CLIENT, BUFFER> Proxy(Client);
			return Proxy;
		}

		bool AddOwnObj(const char* name, _S32 value)
		{
			BUFFER Buf;
			_U32 __length;
			// <string> <name> <> <>
			__length = DDL::StringLength(name);
			if(!Buf.Write(__length)) return false;
			if(!Buf.WriteData(name, (unsigned int)sizeof(name[0])*__length)) return false;
			// <_S32> <value> <> <>
			if(!Buf.Write(value)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 0, Buf);
		}

		bool DelOwnObj(const A_UUID& _uuid)
		{
			BUFFER Buf;
			// <A_UUID> <_uuid> <> <>
			if(!Buf.Write(_uuid)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 1, Buf);
		}

		bool AddOwnObjValue(const A_UUID& _uuid, _S32 value)
		{
			BUFFER Buf;
			// <A_UUID> <_uuid> <> <>
			if(!Buf.Write(_uuid)) return false;
			// <_S32> <value> <> <>
			if(!Buf.Write(value)) return false;

			// send
			return this->GetClient()->Send(this->GetClassID(), 2, Buf);
		}
	};

}


#pragma pack(pop)

#endif