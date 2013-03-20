#include "AtlasBase.h"

namespace DDL
{

	_U16 StringLength(const char* src)
	{
		assert(src);
		_U16 length = 0;
		while(src != NULL && *src++ != '\0') ++length;
		return length;
	}

}

namespace DDLReflect
{

	static bool call_jsonread(DDL::MemoryReader& buf, _U8 type, const STRUCT_INFO* def, Json::Value& value);
	static bool call_jsonread(DDL::MemoryReader& buf, const FIELD_INFO* def, _U16 count, Json::Value& value);
	static bool call_jsonwrite(DDL::MemoryWriter& buf, _U8 type, const STRUCT_INFO* def, const Json::Value& value);
	static bool call_jsonwrite(DDL::MemoryWriter& buf, const FIELD_INFO* def, _U16 count, const Json::Value& value);

	static bool struct_jsonread(const _U8* buf, const STRUCT_INFO* def, Json::Value& value);
	static bool struct_jsonread(const _U8* buf, const FIELD_INFO* def, _U16 count, Json::Value& root);
	static bool struct_jsonread(const _U8* buf, _U8 type, _U16 slen, Json::Value& value);
	static bool struct_jsonwrite(const Json::Value& value, const STRUCT_INFO* def, _U8* data);
	static bool struct_jsonwrite(const Json::Value& value, const FIELD_INFO* def, _U16 count, _U8* data);
	static bool struct_jsonwrite(const Json::Value& value, _U8 type, _U16 slen, _U8* data);

	bool Call2Json(const FUNCTION_INFO* def, _U32 len, const _U8* data, Json::Value& json)
	{
		if(!json.isObject()) return false;
		DDL::MemoryReader buf(data, len);
		return call_jsonread(buf, def->finfos, def->fcount, json);
	}

	bool Json2Call(const FUNCTION_INFO* def, const Json::Value& json, _U32& len, _U8* data)
	{
		DDL::MemoryWriter buf(data, len);
		return call_jsonwrite(buf, def->finfos, def->fcount, json);
	}

	bool Call2Json(const FUNCTION_INFO* def, _U32 len, const _U8* data, Atlas::String& json)
	{
		Json::Value root(Json::objectValue);
		if(!Call2Json(def, len, data, root)) return false;
		Json::FastWriter writer;
		json = writer.write(root);
		return true;
	}

	bool Json2Call(const FUNCTION_INFO* def, const Atlas::String& json, _U32& len, _U8* data)
	{
		Json::Value root;
		Json::Reader reader;
		if(!reader.parse(json, root)) return false;
		return Json2Call(def, root, len, data);
	}

	bool Struct2Json(const STRUCT_INFO* def, const _U8* data, Json::Value& Value)
	{
		return struct_jsonread(data, def, Value);
	}

	bool Json2Struct(const STRUCT_INFO* def, const Json::Value& Value, _U8* data)
	{
		return struct_jsonwrite(Value, def, data);
	}

	bool Struct2Json(const STRUCT_INFO* def, const _U8* data, Atlas::String& json)
	{
		Json::Value root(Json::objectValue);
		if(!struct_jsonread(data, def, root)) return false;
		Json::FastWriter writer;
		json = writer.write(root);
		return true;
	}

	bool Json2Struct(const STRUCT_INFO* def, const Atlas::String& json, _U8* data)
	{
		Json::Value root;
		Json::Reader reader;
		if(!reader.parse(json, root)) return false;
		if(!struct_jsonwrite(root, def, data)) return false;
		return true;
	}

	bool call_jsonread(DDL::MemoryReader& buf, _U8 type, const STRUCT_INFO* def, Json::Value& value)
	{
		if(type&TYPE_ARRAY)
		{
			_U32 count;
			if(!buf.Read(count)) return false;
			value = Json::Value(Json::arrayValue);
			for(_U32 i=0; i<count; i++)
			{
				Json::Value svalue(Json::nullValue);
				if(!call_jsonread(buf, type&TYPE_MASK, def, svalue)) return false;
				value.append(svalue);
			}
		}
		else
		{
			switch(type&TYPE_MASK)
			{
			case TYPE_U8:
				{
					_U8 v;
					if(!buf.Read(v)) return false;
					value = (Json::UInt)v;
					break;
				}
			case TYPE_U16:
				{
					_U16 v;
					if(!buf.Read(v)) return false;
					value = (Json::UInt)v;
					break;
				}
			case TYPE_U32:
				{
					_U32 v;
					if(!buf.Read(v)) return false;
					value = (Json::UInt)v;
					break;
				}
			case TYPE_U64:
				{
					_U64 v;
					if(!buf.Read(v)) return false;
					value = (Json::UInt)v;
					break;
				}
			case TYPE_S8:
				{
					_S8 v;
					if(!buf.Read(v)) return false;
					value = (Json::Int)v;
					break;
				}
			case TYPE_S16:
				{
					_S16 v;
					if(!buf.Read(v)) return false;
					value = (Json::Int)v;
					break;
				}
			case TYPE_S32:
				{
					_S32 v;
					if(!buf.Read(v)) return false;
					value = (Json::Int)v;
					break;
				}
			case TYPE_S64:
				{
					_S64 v;
					if(!buf.Read(v)) return false;
					value = (Json::Int)v;
					break;
				}
			case TYPE_F32:
				{
					_F32 v;
					if(!buf.Read(v)) return false;
					value = (double)v;
					break;
				}
			case TYPE_F64:
				{
					_F64 v;
					if(!buf.Read(v)) return false;
					value = (double)v;
					break;
				}
			case TYPE_STRING:
				{
					_U32 len;
					if(!buf.Read(len)) return false;
					const char* str = (const char*)buf.ReadBuffer(len);
					if(!str) return false;
					value = Json::Value(str, str+len);
					break;
				}
			case TYPE_UUID:
			case TYPE_UUID_REF:
				{
					A_UUID uuid;
					if(!buf.Read(uuid)) return false;
					char str[100];
					AUuidToString(uuid, str);
					value = Json::Value(str);
					break;
				}
				break;
			case TYPE_STRUCT:
				{
					if(def->parent)
					{
						if(!call_jsonread(buf, type, def->parent, value)) return false;
					}
					else
					{
						value = Json::Value(Json::objectValue);
					}

					if(!call_jsonread(buf, def->finfos, def->fcount, value)) return false;
					break;
				}
			default:
				return false;
			}
		}
		return true;
	}

	bool call_jsonread(DDL::MemoryReader& buf, const FIELD_INFO* def, _U16 count, Json::Value& root)
	{
		for(_U16 i=0; i<count; i++)
		{
			Json::Value svalue(Json::nullValue);
			if(!call_jsonread(buf, def[i].type, def[i].sinfo, svalue)) return false;
			root[def[i].name] = svalue;
		}
		return true;
	}

	bool call_jsonwrite(DDL::MemoryWriter& buf, _U8 type, const STRUCT_INFO* def, const Json::Value& value)
	{
		if(type&TYPE_ARRAY)
		{
			if(!value.isArray()) return false;
			_U32 count = (_U32)value.size();
			if(!buf.Write(count)) return false;
			for(_U32 i=0; i<count; i++)
			{
				if(!call_jsonwrite(buf, type&TYPE_MASK, def, value[i])) return false;
			}
		}
		else
		{
			switch(type&TYPE_MASK)
			{
			case TYPE_U8:
				{
					if(!value.isUInt() && !value.isInt()) return false;
					_U8 v = (_U8)value.asUInt();
					if(!buf.Write(v)) return false;
					break;
				}
			case TYPE_U16:
				{
					if(!value.isUInt() && !value.isInt()) return false;
					_U16 v = (_U16)value.asUInt();
					if(!buf.Write(v)) return false;
					break;
				}
			case TYPE_U32:
				{
					if(!value.isUInt() && !value.isInt()) return false;
					_U32 v = (_U32)value.asUInt();
					if(!buf.Write(v)) return false;
					break;
				}
			case TYPE_U64:
				{
					if(!value.isUInt() && !value.isInt()) return false;
					_U64 v = (_U64)value.asUInt();
					if(!buf.Write(v)) return false;
					break;
				}
			case TYPE_S8:
				{
					if(!value.isInt()) return false;
					_S8 v = (_S8)value.asInt();
					if(!buf.Write(v)) return false;
					break;
				}
			case TYPE_S16:
				{
					if(!value.isInt()) return false;
					_S16 v = (_S16)value.asInt();
					if(!buf.Write(v)) return false;
					break;
				}
			case TYPE_S32:
				{
					if(!value.isInt()) return false;
					_S32 v = (_S32)value.asInt();
					if(!buf.Write(v)) return false;
					break;
				}
			case TYPE_S64:
				{
					if(!value.isInt()) return false;
					_S64 v = (_S64)value.asInt();
					if(!buf.Write(v)) return false;
					break;
				}
			case TYPE_F32:
				{
					if(!value.isNumeric()) return false;
					_F32 v = (_F32)value.asDouble();
					if(!buf.Write(v)) return false;
					break;
				}
			case TYPE_F64:
				{
					if(!value.isNumeric()) return false;
					_F64 v = (_F64)value.asDouble();
					if(!buf.Write(v)) return false;
					break;
				}
			case TYPE_STRING:
				{
					if(!value.isString()) return false;
					const char* str = value.asCString();
					_U32 len = (_U32)strlen(str);
					if(!buf.Write(len)) return false;
					if(!buf.WriteData(str, len)) return false;
					break;
				}
			case TYPE_UUID:
			case TYPE_UUID_REF:
				{
					if(!value.isString()) return false;
					A_UUID uuid;
					const char* str = value.asCString();
					if(!AUuidFromString(str, uuid)) return false;
					if(!buf.Write(uuid)) return false;
					break;
				}
				break;
			case TYPE_STRUCT:
				{
					if(!value.isObject()) return false;

					if(def->parent)
					{
						if(!call_jsonwrite(buf, type, def->parent, value)) return false;
					}

					if(!call_jsonwrite(buf, def->finfos, def->fcount, value)) return false;
					break;
				}
			default:
				return false;
			}
		}
		return true;
	}

	bool call_jsonwrite(DDL::MemoryWriter& buf, const FIELD_INFO* def, _U16 count, const Json::Value& root)
	{
		if(!root.isObject()) return false;
		for(_U16 i=0; i<count; i++)
		{
			if(!root.isMember(def[i].name)) return false;
			if(!call_jsonwrite(buf, def[i].type, def[i].sinfo, root[def[i].name])) return false;
		}
		return true;
	}

	bool struct_jsonread(const _U8* buf, const STRUCT_INFO* def, Json::Value& root)
	{
		if(def->parent)
		{
			if(!struct_jsonread(buf, def->parent, root)) return false;
		}
		return struct_jsonread(buf, def->finfos, def->fcount, root);
	}

	bool struct_jsonread(const _U8* buf, const FIELD_INFO* def, _U16 count, Json::Value& root)
	{
		for(_U16 i=0; i<count; i++)
		{
			Json::ValueType type = Json::nullValue;
			switch(def[i].type&TYPE_MASK)
			{
			case TYPE_U8:
			case TYPE_U16:
			case TYPE_U32:
			case TYPE_U64:
				type = Json::uintValue;
				break;
			case TYPE_S8:
			case TYPE_S16:
			case TYPE_S32:
			case TYPE_S64:
				type = Json::intValue;
				break;
			case TYPE_F32:
			case TYPE_F64:
				type = Json::realValue;
				break;
			case TYPE_STRING:
				type = Json::stringValue;
				break;
			case TYPE_UUID:
			case TYPE_UUID_REF:
				type = Json::stringValue;
				break;
			case TYPE_STRUCT:
				type = Json::objectValue;
				break;
			default:
				return false;
			}

			if(def[i].type&TYPE_ARRAY)
			{
				Json::Value avalue(Json::arrayValue);
				_U32 len = *(_U32*)((const _U8*)buf + def[i].offset);
				if(len>def[i].alen) return false;

				for(_U32 a=0; a<len; a++)
				{
					Json::Value svalue(type);
					if((def[i].type&TYPE_MASK)==TYPE_STRUCT)
					{
						if(!struct_jsonread(buf+def[i].offset+def[i].prefix+def[i].elen*a, def[i].sinfo, svalue))
						{
							return false;
						}
					}
					else
					{
						if(!struct_jsonread(buf+def[i].offset+def[i].prefix+def[i].elen*a, def[i].type&TYPE_MASK, def[i].alen, svalue))
						{
							return false;
						}
					}
					avalue.append(svalue);
				}
				root[def[i].name] = avalue;
			}
			else
			{
				Json::Value svalue(type);
				if((def[i].type&TYPE_MASK)==TYPE_STRUCT)
				{
					if(!struct_jsonread(buf+def[i].offset, def[i].sinfo, svalue)) return false;
				}
				else 
				{
					if(!struct_jsonread(buf+def[i].offset, def[i].type, def[i].slen, svalue)) return false;
				}
				root[def[i].name] = svalue;
			}
		}
		return true;
	}

	bool struct_jsonread(const _U8* buf, _U8 type, _U16 slen, Json::Value& value)
	{
		switch(type)
		{
		case TYPE_U8:
			value = (Json::UInt)(*((const _U8*)buf));
			break;
		case TYPE_U16:
			value = (Json::UInt)(*((const _U16*)buf));
			break;
		case TYPE_U32:
			value = (Json::UInt)(*((const _U32*)buf));
			break;
		case TYPE_U64:
			value = (Json::UInt)(*((const _U64*)buf));
			break;
		case TYPE_S8:
			value = (Json::Int)(*((const _S8*)buf));
			break;
		case TYPE_S16:
			value = (Json::Int)(*((const _S16*)buf));
			break;
		case TYPE_S32:
			value = (Json::Int)(*((const _S32*)buf));
			break;
		case TYPE_S64:
			value = (Json::Int)(*((const _S64*)buf));
			break;
		case TYPE_F32:
			value = (double)(*((const _F32*)buf));
			break;
		case TYPE_F64:
			value = (double)(*((const _F64*)buf));
			break;
		case TYPE_STRING:
			if(strlen((const char*)buf)>slen) return false;
			value = (const char*)buf;
			break;
		case TYPE_UUID:
		case TYPE_UUID_REF:
			{
				char str[100];
				AUuidToString(*((const A_UUID*)buf), str);
				value = str;
				break;
			}
		default:
			return false;
		}
		return true;
	}

	bool struct_jsonwrite(const Json::Value& value, const STRUCT_INFO* def, _U8* data)
	{
		if(!value.isObject()) return false;
		if(def->parent)
		{
			if(!struct_jsonwrite(value, def->parent, data)) return false;
		}
		return struct_jsonwrite(value, def->finfos, def->fcount, data);
	}

	bool struct_jsonwrite(const Json::Value& value, const FIELD_INFO* def, _U16 count, _U8* data)
	{
		for(_U16 i=0; i<count; i++)
		{
			if(!value.isMember(def[i].name)) return false;
			const Json::Value& svalue = value[def[i].name];

			if(def[i].type&TYPE_ARRAY)
			{
				if(!svalue.isArray()) return false;

				Json::UInt a;

				//write length of array
				if(!struct_jsonwrite(svalue.size(), TYPE_U32, 0, data+def[i].offset)) return false;

				for(a=0; a<svalue.size(); a++)
				{
					if((def[i].type&TYPE_MASK)==TYPE_STRUCT)
					{
						if(!struct_jsonwrite(svalue[a], def[i].sinfo, data+def[i].offset+def[i].prefix+def[i].elen*a)) return false;
					}
					else
					{
						if(!struct_jsonwrite(svalue[a], def[i].type&TYPE_MASK, def[i].alen, data+def[i].offset+def[i].prefix+def[i].elen*a)) return false;
					}
				}
			}
			else
			{
				if(def[i].type==TYPE_STRUCT)
				{
					if(!struct_jsonwrite(svalue, def[i].sinfo, data+def[i].offset)) return false;
				}
				else 
				{
					if(!struct_jsonwrite(svalue, def[i].type, def[i].slen, data+def[i].offset)) return false;
				}
			}
		}
		return true;
	}

	bool struct_jsonwrite(const Json::Value& value, _U8 type, _U16 slen, _U8* data)
	{
		switch(type)
		{
		case TYPE_U8:
			if(!value.isUInt() && !value.isInt()) return false;
			*((_U8*)data) = (_U8)value.asUInt();
			break;
		case TYPE_U16:
			if(!value.isUInt() && !value.isInt()) return false;
			*((_U16*)data) = (_U16)value.asUInt();
			break;
		case TYPE_U32:
			if(!value.isUInt() && !value.isInt()) return false;
			*((_U32*)data) = (_U32)value.asUInt();
			break;
		case TYPE_U64:
			if(!value.isUInt() && !value.isInt()) return false;
			*((_U64*)data) = (_U64)value.asUInt();
			break;
		case TYPE_S8:
			if(!value.isUInt() && !value.isInt()) return false;
			*((_S8*)data) = (_S8)value.asInt();
			break;
		case TYPE_S16:
			if(!value.isUInt() && !value.isInt()) return false;
			*((_S16*)data) = (_S16)value.asInt();
			break;
		case TYPE_S32:
			if(!value.isUInt() && !value.isInt()) return false;
			*((_S32*)data) = (_S32)value.asInt();
			break;
		case TYPE_S64:
			if(!value.isUInt() && !value.isInt()) return false;
			*((_S64*)data) = (_S64)value.asInt();
			break;
		case TYPE_F32:
			if(!value.isNumeric()) return false;
			*((_F32*)data) = (_F32)value.asDouble();
			break;
		case TYPE_F64:
			if(!value.isNumeric()) return false;
			*((_F64*)data) = (_F64)value.asDouble();
			break;
		case TYPE_STRING:
			{
				Atlas::String v;
				if(value.isString())
				{
					v = value.asString();
				}
				else if(value.isIntegral())
				{
					v = Atlas::StringFormat("%d", value.asInt());
				}
				else if(value.isDouble())
				{
					v = Atlas::StringFormat("%f", value.asDouble());
				}
				else
				{
					return false;
				}
				if(v.size()>slen) return false;
				memcpy(data, v.c_str(), v.size()+1);
			}
			break;
		case TYPE_UUID:
		case TYPE_UUID_REF:
			{
				if(!value.isString()) return false;
				Atlas::String v = value.asString();
				if(!AUuidFromString(v.c_str(), *((A_UUID*)data))) return false;
			}
			break;
		default:
			return false;
		}
		return true;
	}

	bool IsParent(const STRUCT_INFO* child, const STRUCT_INFO* parent)
	{
		child = child->parent;
		while(child)
		{
			if(child==parent) return true;
			child = child->parent;
		}
		return false;
	}

	bool IsChild(const STRUCT_INFO* parent, const STRUCT_INFO* child)
	{
		return IsParent(child, parent);
	}

	bool GetStructFieldInfo(const STRUCT_INFO* info, const char* name, void* data, FIELD_INFO& finfo, void*& fdata)
	{
		Atlas::Vector<Atlas::String> ns;
		Atlas::StringSplit(name, '.', ns);
		if(ns.empty()) return false;
		Atlas::String fname;
		int findex = -1;
		size_t i=0, old_i=ns.size();

		for(;;)
		{
			if(old_i!=i)
			{
				size_t pos = ns[i].find_first_of('[');
				if(pos==(size_t)-1)
				{
					fname = ns[i];
					findex = -1;
				}
				else
				{
					if(pos<1) return false;
					if(ns[i].length()<pos+3) return false;
					if(*(ns[i].c_str()+ns[i].length()-1)!=']') return false;
					fname = ns[i].substr(0, pos);
					findex = atoi(ns[i].substr(pos+1, ns[i].length()-pos-2).c_str());
				}
				old_i = i;
			}

			_U16 f;
			for(f=0; f<info->fcount; f++)
			{
				if(strcmp(info->finfos[f].name, fname.c_str())==0) break;
			}
			if(f==info->fcount)
			{
				if(!info->parent) return false;
				info = info->parent;
				continue;
			}

			if(info->finfos[f].type&TYPE_ARRAY)
			{
				if(findex<0)
				{
					if(i+1==ns.size())
					{
						finfo = info->finfos[f];
						fdata = (void*)((char*)data + info->finfos[f].offset);
						break;
					}
					if(i+2==ns.size())
					{
						if(ns[i+1]=="length")
						{
							finfo.type = TYPE_U32;
							strcpy(finfo.name, "length");
							finfo.flags = 0;
							finfo.offset = 0;
							finfo.sinfo = NULL;
							finfo.slen = 0;
							finfo.alen = 0;
							finfo.prefix = 0;
							finfo.elen = 0;
							finfo.ref_type = 0;
							fdata = (void*)((char*)data + info->finfos[f].offset);
							break;
						}
					}
					return false;
				}

				if((_U32)findex>=info->finfos[f].alen) return false;

				if((info->finfos[f].type&TYPE_MASK)==TYPE_STRUCT)
				{
					if(i+1<ns.size())
					{
						data = (void*)((char*)data + info->finfos[f].offset + info->finfos[f].prefix + info->finfos[f].sinfo->size * findex);
						info = info->finfos[f].sinfo;
						i++;
						continue;
					}
				}
				else
				{
					if(i+1!=ns.size()) return false;
				}

				finfo = info->finfos[f];
				finfo.type &= TYPE_MASK;
				fdata = (void*)((char*)data + info->finfos[f].offset + info->finfos[f].prefix + info->finfos[f].elen * findex);
				break;
			}
			else
			{
				if(findex>=0) return false;
				if(info->finfos[f].type==TYPE_STRUCT)
				{
					if(i+1<ns.size())
					{
						data = (void*)((char*)data + info->finfos[f].offset);
						info = info->finfos[f].sinfo;
						i++;
						continue;
					}
				}
				else
				{
					if(i+1<ns.size()) return false;
				}
				finfo = info->finfos[f];
				fdata = (void*)((char*)data + info->finfos[f].offset);
				return true;
			}
		}

		return true;
	}

	bool GetStructFieldInfo(const STRUCT_INFO* info, const char* name, const void* data, FIELD_INFO& finfo, const void*& fdata)
	{
		void* _fdata;
		if(!GetStructFieldInfo(info, name, (void*)data, finfo, _fdata)) return false;
		fdata = _fdata;
		return true;
	}

	bool StructParamToString(const FIELD_INFO* finfo, const void* data, Atlas::String& str)
	{
		if(finfo->type&TYPE_ARRAY)
		{
			str = "[";
			Atlas::String item;
			FIELD_INFO _finfo = *finfo;
			_finfo.type &= TYPE_MASK;
			for(_U32 i=0; i<*(_U32*)data; i++)
			{
				if(i>0) str += ",";
				if(!StructParamToString(finfo, (const void*)((char*)data+finfo->prefix+finfo->elen*i), item)) return false;
				str += item;
			}
			str += "]";
		}
		else
		{
			switch(finfo->type)
			{
			case TYPE_U8:
				str = Atlas::StringFormat("%u", (unsigned int)*((const _U8*)data));
				break;
			case TYPE_U16:
				str = Atlas::StringFormat("%u", (unsigned int)*((const _U16*)data));
				break;
			case TYPE_U32:
				str = Atlas::StringFormat("%u", *((const _U32*)data));
				break;
			case TYPE_U64:
#ifdef WIN32
				str = Atlas::StringFormat("%I64u", *((const _U64*)data));
#else
				str = Atlas::StringFormat("%llu", *((const _U64*)data));
#endif
				break;
			case TYPE_S8:
				str = Atlas::StringFormat("%d", (int)*((const _S8*)data));
				break;
			case TYPE_S16:
				str = Atlas::StringFormat("%d", (int)*((const _S16*)data));
				break;
			case TYPE_S32:
				str = Atlas::StringFormat("%d", *((const _S32*)data));
				break;
			case TYPE_S64:
#ifdef WIN32
				str = Atlas::StringFormat("%I64d", *((const _S64*)data));
#else
				str = Atlas::StringFormat("%lld", *((const _S64*)data));
#endif
				break;
			case TYPE_F32:
				str = Atlas::StringFormat("%G", (double)*((const _F32*)data));
				break;
			case TYPE_F64:
				str = Atlas::StringFormat("%G", *((const _F64*)data));
				break;
			case TYPE_STRING:
				str = (const char*)data;
				break;
			case TYPE_UUID:
			case TYPE_UUID_REF:
				{
					char uuid[100];
					AUuidToString(*((const A_UUID*)data), uuid);
					str = uuid;
				}
				break;
			case TYPE_STRUCT:
				if(!Struct2Json(finfo->sinfo, str)) return false;
				break;
			}
		}

		return true;
	}

	bool StructParamFromString(const FIELD_INFO* finfo, void* data, const char* str)
	{
		FIELD_INFO _finfo = *finfo;
		STRUCT_INFO _sinfo = { NULL, "_sinfo", 0, 1, &_finfo };
		_finfo.offset = 0;
		Atlas::String json;
		if(finfo->type==TYPE_STRING || finfo->type==TYPE_UUID || finfo->type==TYPE_UUID_REF)
		{
			json = Atlas::StringFormat("{\"%s\":\"%s\"}", finfo->name, str);
		}
		else
		{
			json = Atlas::StringFormat("{\"%s\":%s}", finfo->name, str);
		}
		return Json2Struct(&_sinfo, json, (_U8*)data);
	}

	bool StructParamToString(const STRUCT_INFO* info, const char* name, const void* data, Atlas::String& str)
	{
		FIELD_INFO finfo;
		const void* fdata;
		if(!GetStructFieldInfo(info, name, data, finfo, fdata)) return false;
		if(!StructParamToString(&finfo, fdata, str)) return false;
		return true;
	}

	bool StructParamFromString(const STRUCT_INFO* info, const char* name, void* data, const char* str)
	{
		FIELD_INFO finfo;
		void* fdata;
		if(!GetStructFieldInfo(info, name, data, finfo, fdata)) return false;
		if(!StructParamFromString(&finfo, fdata, str)) return false;
		return true;
	}

	bool StructParamType(const FIELD_INFO* finfo, Atlas::String& type)
	{
		char name[100];
		switch(finfo->type&TYPE_MASK)
		{
		case DDLReflect::TYPE_U8:		sprintf(name, "%s", "_U8"); break;
		case DDLReflect::TYPE_U16:		sprintf(name, "%s", "_U16"); break;
		case DDLReflect::TYPE_U32:		sprintf(name, "%s", "_U32"); break;
		case DDLReflect::TYPE_U64:		sprintf(name, "%s", "_U64"); break;
		case DDLReflect::TYPE_S8:		sprintf(name, "%s", "_S8"); break;
		case DDLReflect::TYPE_S16:		sprintf(name, "%s", "_S16"); break;
		case DDLReflect::TYPE_S32:		sprintf(name, "%s", "_S32"); break;
		case DDLReflect::TYPE_S64:		sprintf(name, "%s", "_S64"); break;
		case DDLReflect::TYPE_F32:		sprintf(name, "%s", "_F32"); break;
		case DDLReflect::TYPE_F64:		sprintf(name, "%s", "_F64"); break;
		case DDLReflect::TYPE_STRING:	sprintf(name, "string<%d>", finfo->slen); break;
		case DDLReflect::TYPE_UUID:		sprintf(name, "%s", "A_UUID"); break;
		case DDLReflect::TYPE_UUID_REF:	sprintf(name, "content_ref<%s>", finfo->ref_type); break;
		case DDLReflect::TYPE_STRUCT:	sprintf(name, "%s", finfo->sinfo->name); break;
		default: return false;
		}
		if((finfo->type&DDLReflect::TYPE_ARRAY)==0)
		{
			type = name;
		}
		else
		{
			type = Atlas::StringFormat("array<%s, %d>", name, finfo->alen);
		}
		return true;
	}

	bool StructParamType(const STRUCT_INFO* info, _U16 index, Atlas::String& type)
	{
		if(index>=info->fcount) return false;
		return StructParamType(&info->finfos[index], type);
	}

	bool StructParamType(const STRUCT_INFO* info, const char* name, Atlas::String& type)
	{
		void* data = NULL;
		void* fdata = NULL;
		FIELD_INFO finfo;
		if(!GetStructFieldInfo(info, name, data, finfo, fdata)) return false;
		if(!StructParamType(&finfo, type)) return false;
		return true;
	}

	void* CreateObject(const STRUCT_INFO* info)
	{
		void* data = malloc(info->size);
		memset(data, 0, (size_t)info->size);
		return data;
	}

	void DestoryObject(void* data)
	{
		free(data);
	}

}
