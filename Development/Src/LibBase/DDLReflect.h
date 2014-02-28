#ifndef __DDL_REFLECT_INCLUDE__
#define __DDL_REFLECT_INCLUDE__

#include <json/jsoncpp.h>

namespace DDLReflect
{

	const _U32 NAME_MAXLEN	= 40;

	const _U8 TYPE_VOID		= 0;
	const _U8 TYPE_U8		= 0x01;
	const _U8 TYPE_U16		= 0x02;
	const _U8 TYPE_U32		= 0x03;
	const _U8 TYPE_U64		= 0x04;
	const _U8 TYPE_S8		= 0x11;
	const _U8 TYPE_S16		= 0x12;
	const _U8 TYPE_S32		= 0x13;
	const _U8 TYPE_S64		= 0x14;
	const _U8 TYPE_F32		= 0x30;
	const _U8 TYPE_F64		= 0x31;
	const _U8 TYPE_STRING	= 0x72;
	const _U8 TYPE_UUID		= 0x73;
	const _U8 TYPE_UUID_REF	= 0x74;
	const _U8 TYPE_STRUCT	= 0x75;
	const _U8 TYPE_MASK		= 0x7f;
	const _U8 TYPE_ARRAY	= 0x80;

	const _U32 FLAG_NOTNULL	= 0x01;
	const _U32 FLAG_READONLY= 0x02;

	struct FIELD_INFO;
	struct STRUCT_INFO;
	struct FUNCTION_INFO;
	struct CLASS_INFO;

	typedef bool (*READ_PROC)(DDL::BufferReader& buf, void* data);
	typedef bool (*WRITE_PROC)(DDL::BufferWriter& buf, const void* data);

	struct FIELD_INFO
	{
		_U8					type;
		char				name[NAME_MAXLEN+1];
		_U32				flags;
		_U16				offset;
		STRUCT_INFO*		sinfo;
		_U16				slen;
		_U16				alen;
		_U16				prefix;
		_U16				elen;
		const char*			ref_type;
	};

	struct STRUCT_INFO
	{
		STRUCT_INFO*		parent;
		char				name[NAME_MAXLEN+1];
		_U32				size;
		_U16				fcount;
		const FIELD_INFO*	finfos;
		READ_PROC			read_proc;
		WRITE_PROC			write_proc;
	};

	struct FUNCTION_INFO
	{
		char				name[NAME_MAXLEN+1];
		_U16				fcount;
		FIELD_INFO*			finfos;
	};

	struct CLASS_INFO
	{
		_U16				iid;
		char				name[NAME_MAXLEN+1];
		_U16				fcount;
		FUNCTION_INFO*		finfos;
	};

	template<typename T>
	inline const STRUCT_INFO* GetStruct()
	{
		return NULL;
	}

	template<class T>
	inline const CLASS_INFO* GetClass()
	{
		return NULL;
	}

	template<typename T>
	inline _U16 GetClassID()
	{
		const CLASS_INFO* pClassInfo = GetClass<T>();
		return pClassInfo?pClassInfo->iid:(_U16)-1;
	}

	bool Call2Json(const FUNCTION_INFO* def, _U32 len, const _U8* data, Json::Value& json);
	bool Json2Call(const FUNCTION_INFO* def, const Json::Value& json, _U32& len, _U8* data);

	bool Call2Json(const FUNCTION_INFO* def, _U32 len, const _U8* data, Zion::String& json);
	bool Json2Call(const FUNCTION_INFO* def, const Zion::String& json, _U32& len, _U8* data);

	bool Struct2Json(const STRUCT_INFO* def, const _U8* data, Json::Value& Value);
	bool Json2Struct(const STRUCT_INFO* def, const Json::Value& Value, _U8* data, bool ignore=false);

	bool Struct2Json(const STRUCT_INFO* def, const _U8* data, Zion::String& json);
	bool Json2Struct(const STRUCT_INFO* def, const Zion::String& json, _U8* data, bool ignore=false);

	template<typename T>
	inline bool Struct2Json(const T& data, Zion::String& json)
	{
		return Struct2Json(GetStruct<T>(), (const _U8*)&data, json);
	}

	template<typename T>
	inline bool Json2Struct(const Zion::String& json, T& data)
	{
		return Json2Struct(GetStruct<T>(), json, (_U8*)&data);
	}

	bool IsParent(const STRUCT_INFO* child, const STRUCT_INFO* parent);
	bool IsChild(const STRUCT_INFO* parent, const STRUCT_INFO* child);

	_U32 GetStructFieldOffset(const STRUCT_INFO* info, const char* name, FIELD_INFO* finfo=NULL);
	void* GetStructFieldData(const STRUCT_INFO* info, const char* name, void* data, FIELD_INFO& finfo);
	const void* GetStructFieldData(const STRUCT_INFO* info, const char* name, const void* data, FIELD_INFO& finfo);

	bool StructParamToString(const FIELD_INFO* finfo, const void* data, Zion::String& str);
	bool StructParamFromString(const FIELD_INFO* finfo, void* data, const char* str);
	bool StructParamToString(const STRUCT_INFO* info, const char* name, const void* data, Zion::String& str, FIELD_INFO* finfo=NULL);
	bool StructParamFromString(const STRUCT_INFO* info, const char* name, void* data, const char* str);

	void* CreateObject(const STRUCT_INFO* info);
	void DestoryObject(const STRUCT_INFO* info, void* data);
	void CopyObject(const STRUCT_INFO* info, void* dst, const void* src);

}

#endif
