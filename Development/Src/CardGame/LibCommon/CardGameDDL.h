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

struct CG_AVATAR_DESC : A_LIVE_OBJECT
{
	_U32 avatar_id;
	DDL::String<100> avatar_name;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<CG_AVATAR_DESC>(CG_AVATAR_DESC& Value);
	template<>
	bool BufferWriter::Write<CG_AVATAR_DESC>(const CG_AVATAR_DESC& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<CG_AVATAR_DESC>();
	extern STRUCT_INFO _rfl_struct_CG_AVATAR_DESC_info;
}

struct CG_AVATAR : A_LIVE_OBJECT
{
	DDL::String<100> avatar_name;
	A_CONFUSED_F32 money;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<CG_AVATAR>(CG_AVATAR& Value);
	template<>
	bool BufferWriter::Write<CG_AVATAR>(const CG_AVATAR& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<CG_AVATAR>();
	extern STRUCT_INFO _rfl_struct_CG_AVATAR_info;
}

struct CG_CARD : A_LIVE_OBJECT
{
	A_UUID content_uuid;
	_U32 win_count;
	_U32 lost_count;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<CG_CARD>(CG_CARD& Value);
	template<>
	bool BufferWriter::Write<CG_CARD>(const CG_CARD& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<CG_CARD>();
	extern STRUCT_INFO _rfl_struct_CG_CARD_info;
}

struct CG_SHOPITEM : A_CONTENT_OBJECT
{
	DDL::String<100> shopitem_id;
	_U32 price;
	A_UUID content_uuid;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<CG_SHOPITEM>(CG_SHOPITEM& Value);
	template<>
	bool BufferWriter::Write<CG_SHOPITEM>(const CG_SHOPITEM& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<CG_SHOPITEM>();
	extern STRUCT_INFO _rfl_struct_CG_SHOPITEM_info;
}

struct CG_CARD_CONFIG : A_CONTENT_OBJECT
{
	DDL::String<100> disp_name;
	_U32 point;
};

namespace DDL
{
	template<>
	bool BufferReader::Read<CG_CARD_CONFIG>(CG_CARD_CONFIG& Value);
	template<>
	bool BufferWriter::Write<CG_CARD_CONFIG>(const CG_CARD_CONFIG& Value);
}

namespace DDLReflect
{
	template<>
	const STRUCT_INFO* GetStruct<CG_CARD_CONFIG>();
	extern STRUCT_INFO _rfl_struct_CG_CARD_CONFIG_info;
}

class CGSERVER_BASE;

namespace DDLReflect
{
	template<>
	const CLASS_INFO* GetClass<CGSERVER_BASE>();
}

class CGCALLBACK_BASE;

namespace DDLReflect
{
	template<>
	const CLASS_INFO* GetClass<CGCALLBACK_BASE>();
}

class CGSERVER_GAME;

namespace DDLReflect
{
	template<>
	const CLASS_INFO* GetClass<CGSERVER_GAME>();
}

class CGCALLBACK_GAME;

namespace DDLReflect
{
	template<>
	const CLASS_INFO* GetClass<CGCALLBACK_GAME>();
}

namespace DDLProxy
{

	template<_U32 BUF_SIZE>
	class CGSERVER_BASE : public DDLProxy<BUF_SIZE>
	{
	public:
		CGSERVER_BASE(IClient* Client) : DDLProxy<BUF_SIZE>(Client, DDLReflect::GetClassID<typename ::CGSERVER_BASE>())
		{
		}

		bool Ping()
		{
			_Buf.Reset();

			// send
			return GetClient()->SendData(this->GetClassID(), 0, _Buf.GetSize(), _Buf.GetBuf());
		}

		bool SelectServer(_U32 server_id)
		{
			_Buf.Reset();

			// <_U32> <server_id> <> <>
			if(!_Buf.Write(server_id)) return false;

			// send
			return GetClient()->SendData(this->GetClassID(), 1, _Buf.GetSize(), _Buf.GetBuf());
		}

		bool GetAvatarList()
		{
			_Buf.Reset();

			// send
			return GetClient()->SendData(this->GetClassID(), 2, _Buf.GetSize(), _Buf.GetBuf());
		}

		bool CreateAvatar(const char* avatar_name)
		{
			_Buf.Reset();

			_U32 __length;
			// <string> <avatar_name> <> <>
			__length = DDL::StringLength(avatar_name);
			if(!_Buf.Write(__length)) return false;
			if(!_Buf.WriteData(avatar_name, (unsigned int)sizeof(avatar_name[0])*__length)) return false;

			// send
			return GetClient()->SendData(this->GetClassID(), 3, _Buf.GetSize(), _Buf.GetBuf());
		}

		bool EnterGame(_U32 avatar_id)
		{
			_Buf.Reset();

			// <_U32> <avatar_id> <> <>
			if(!_Buf.Write(avatar_id)) return false;

			// send
			return GetClient()->SendData(this->GetClassID(), 4, _Buf.GetSize(), _Buf.GetBuf());
		}

		bool LeaveGame()
		{
			_Buf.Reset();

			// send
			return GetClient()->SendData(this->GetClassID(), 5, _Buf.GetSize(), _Buf.GetBuf());
		}
	};

}

namespace DDLProxy
{

	template<_U32 BUF_SIZE>
	class CGSERVER_GAME : public DDLProxy<BUF_SIZE>
	{
	public:
		CGSERVER_GAME(IClient* Client) : DDLProxy<BUF_SIZE>(Client, DDLReflect::GetClassID<typename ::CGSERVER_GAME>())
		{
		}

		bool Buy(const char* shopitem_id)
		{
			_Buf.Reset();

			_U32 __length;
			// <string> <shopitem_id> <> <>
			__length = DDL::StringLength(shopitem_id);
			if(!_Buf.Write(__length)) return false;
			if(!_Buf.WriteData(shopitem_id, (unsigned int)sizeof(shopitem_id[0])*__length)) return false;

			// send
			return GetClient()->SendData(this->GetClassID(), 0, _Buf.GetSize(), _Buf.GetBuf());
		}

		bool Discard(const A_UUID& uuid)
		{
			_Buf.Reset();

			// <A_UUID> <uuid> <> <>
			if(!_Buf.Write(uuid)) return false;

			// send
			return GetClient()->SendData(this->GetClassID(), 1, _Buf.GetSize(), _Buf.GetBuf());
		}

		bool Beg(_U32 money)
		{
			_Buf.Reset();

			// <_U32> <money> <> <>
			if(!_Buf.Write(money)) return false;

			// send
			return GetClient()->SendData(this->GetClassID(), 2, _Buf.GetSize(), _Buf.GetBuf());
		}

		bool Gamble(const A_UUID& card, _S32 mode)
		{
			_Buf.Reset();

			// <A_UUID> <card> <> <>
			if(!_Buf.Write(card)) return false;
			// <_S32> <mode> <> <>
			if(!_Buf.Write(mode)) return false;

			// send
			return GetClient()->SendData(this->GetClassID(), 3, _Buf.GetSize(), _Buf.GetBuf());
		}
	};

}

namespace DDLSigSlot
{

	class CGCALLBACK_BASE : public DDLStub::IStub
	{
	public:
		virtual const DDLReflect::CLASS_INFO* GetClassInfo()
		{
			return DDLReflect::GetClass<::CGCALLBACK_BASE>();
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
				CG_AVATAR_DESC* _prefix_arr;
				_U32 _prefix_count;

				// <CG_AVATAR_DESC> <arr> <> <count>;
				if(!Buf.Read(__length)) return false;
				_prefix_arr = (CG_AVATAR_DESC*)alloca(sizeof(_prefix_arr[0])*__length);
				if(!_prefix_arr) return false;
				if(!Buf.ReadPointer(_prefix_arr, __length)) return false;
				// <_U32> <count> <> <>;
				if(!Buf.Read(_prefix_count)) return false;

				// call implement
				_GetAvatarListCallback(_prefix_arr, _prefix_count);
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
		sigslot::signal2<CG_AVATAR_DESC*, _U32> _GetAvatarListCallback;
		sigslot::signal1<_U32> _CreateAvatarCallback;
		sigslot::signal1<_U32> _EnterGameCallback;
		sigslot::signal1<_U32> _LeaveGameCallback;
	};

}

namespace DDLSigSlot
{

	class CGCALLBACK_GAME : public DDLStub::IStub
	{
	public:
		virtual const DDLReflect::CLASS_INFO* GetClassInfo()
		{
			return DDLReflect::GetClass<::CGCALLBACK_GAME>();
		}
		
		virtual bool Dispatcher(_U16 fid, DDL::BufferReader& Buf)
		{
			if(fid==0)
			{
				// call implement
				_Ready();
				return true;
			}
			if(fid==1)
			{
				_U32 _prefix_money;

				// <_U32> <money> <> <>;
				if(!Buf.Read(_prefix_money)) return false;

				// call implement
				_BegResult(_prefix_money);
				return true;
			}
			if(fid==2)
			{
				_U32 _prefix_point;

				// <_U32> <point> <> <>;
				if(!Buf.Read(_prefix_point)) return false;

				// call implement
				_GambleResult(_prefix_point);
				return true;
			}
			return false;
		}

		sigslot::signal0<> _Ready;
		sigslot::signal1<_U32> _BegResult;
		sigslot::signal1<_U32> _GambleResult;
	};

}


#pragma pack(pop)

#endif
