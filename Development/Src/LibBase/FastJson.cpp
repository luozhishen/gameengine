#include <ZionBase.h>
#include "FastJson.h"
#include "math.h"

namespace Zion
{

	class JsonReader
	{
	public:

		bool Parse(const char* begin, const char* end, JsonValue& root)
		{
			m_begin = begin;
			m_end = end;
			const char* cur = ParseElement(m_begin, root);
			if(!cur) return false;
			cur = Skip(cur);
			return cur==NULL;
		}

	protected:
		const char* Skip(const char* cur)
		{
			if(cur)
			{
				for(; cur<m_end; cur++)
				{
					switch(*cur)
					{
					case '\r':	break;
					case '\n':	break;
					case ' ':	break;
					case '\t':	break;
					default: return cur;
					}
				}
			}
			return NULL;
		}

		const char* ParseElement(const char* start, JsonValue& value)
		{
			const char* cur = Skip(start);
			if(cur==NULL) return NULL;
			switch(*cur)
			{
			case '"': return ParseString(cur, value);
			case '[': return ParseArray(cur, value);
			case '{': return ParseObject(cur, value);
			case 't':
				if(m_end-cur<4) return NULL;
				if(memcmp(cur, "true", 4)!=0) return NULL;
				value.Set(true);
				return cur + 4;
			case 'f':
				if(m_end-cur<5) return NULL;
				if(memcmp(cur, "false", 5)!=0) return NULL;
				value.Set(true);
				return cur + 5;
			case 'n':
				if(m_end-cur<4) return NULL;
				if(memcmp(cur, "null", 4)!=0) return NULL;
				value.SetType(JsonValue::TYPE_NULL);
				return cur + 4;
			case '+':
			case '-':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				return ParseNumber(cur, value);
			}
			return NULL;
		}

		const char* ParseNumber(const char* start, JsonValue& value)
		{
			const char* cur = start;

			bool is_minus = false;
			
			if(*cur=='-')
			{
				is_minus = true;
				cur += 1;
			} else if(*cur=='+')
			{
				cur += 1;
			}
			if(cur==m_end) return NULL;

			_U64 int_val = 0;
			start = cur;
			while(*cur>='0' && *cur<='9' && cur!=m_end)
			{
				int_val = int_val * 10 + (_U64)(*cur - '0');
				cur += 1;
			}
			if(start==cur) return NULL;
			if(cur==m_end || *cur!='.')
			{
				if(is_minus)
				{
					value.SetType(JsonValue::TYPE_S32);
					value.m_s32 = (_S32)int_val * -1;
				}
				else
				{
					value.SetType(JsonValue::TYPE_U32);
					value.m_u32 = (_U32)int_val;
				}
				return cur;
			}

			_F64 flt_val = 0.0f;
			_F64 flt_rat = 1.0f;
			start = cur + 1;
			while(*start>='0' && *start<='9' && start!=m_end)
			{
				flt_rat /= 10.0f;
				flt_val += (*start - '0') * flt_rat;
				start += 1;
			}
			if(start==cur) return NULL;
			cur = start;
			flt_val = (_F64)int_val + flt_val;
			if(is_minus) flt_val *= -1;

			if(cur!=m_end && (*cur=='E' || *cur=='e'))
			{
				cur += 1;
				if(cur==m_end) return NULL;
				is_minus = false;
				if(*cur=='+')
				{
					cur += 1;
				}
				else if(*cur=='-')
				{
					is_minus = true;
					cur += 1;
				}
				if(cur==m_end) return NULL;

				_U64 int_val = 0;
				start = cur;
				while(cur!=m_end && *cur>='0' && *cur<='9')
				{
					int_val = int_val * 10 + (*cur - '0');
					cur += 1;
				}
				if(start==cur) return NULL;

				value.SetType(JsonValue::TYPE_F32);
				if(is_minus)
				{
					value.m_f32 = (_F32)(flt_val / pow((double)10, (int)int_val));
				}
				else
				{
					value.m_f32 = (_F32)(flt_val * pow((double)10, (int)int_val));
				}
			}

			value.SetType(JsonValue::TYPE_F32);
			value.m_f32 = (_F32)flt_val;
			return cur;
		}

		const char* ParseString(const char* start, JsonValue& value)
		{
			value.SetType(JsonValue::TYPE_STR);
			return ParseStringValue(start, *value.m_str);
		}

		const char* ParseArray(const char* start, JsonValue& value)
		{
			const char* cur = start + 1;
			if(cur==m_end) return NULL;

			value.SetType(JsonValue::TYPE_ARRAY);
			if(*cur==']') return cur + 1;

			for(;;)
			{
				value.m_array->push_back(JsonValue());
				JsonValue& val = (*value.m_array)[value.m_array->size()-1];
				cur = ParseElement(cur, val);
				if(!cur) return NULL;

				cur = Skip(cur);
				if(!cur) return NULL;
				if(*cur!=',')
				{
					if(*cur==']') break;
					return NULL;
				}
				cur += 1;
			}
			return cur + 1;
		}

		const char* ParseObject(const char* start, JsonValue& value)
		{
			const char* cur = start + 1;
			if(cur==m_end) return NULL;

			value.SetType(JsonValue::TYPE_OBJECT);
			if(*cur=='}') return cur + 1;
			for(;;)
			{
				String name;
				cur = ParseStringValue(cur, name);
				if(!cur) return NULL;

				cur = Skip(cur);
				if(!cur || *cur!=':') return NULL;
				
				JsonValue& node = (*value.m_object)[name];
				cur = ParseElement(cur+1, node);
				if(!cur) return NULL;

				cur = Skip(cur);
				if(!cur)
					return NULL;
				if(*cur=='}')
					break;
				if(*cur!=',')
					return NULL;
				cur += 1;
				if(cur==m_end) return NULL;
			}
			return cur+1;
		}

		const char* ParseStringValue(const char* start, String& value)
		{
			start = Skip(start);
			if(!start || *start!='"') return NULL;
			const char* cur = start + 1;
			if(cur==m_end) return NULL;

			value.clear();
			while(*cur!='"')
			{
				if(*cur!='\\')
				{
					_U32 count = (_U32)(m_end - cur), i;
					for(i=0; i<count; i++)
					{
						if(cur[i]=='\\') break;
						if(cur[i]=='"') break;
						if(cur[i]<' ') return NULL;
					}
					if(i==count) return NULL;
					value.append(cur, i);
					cur += i;
					continue;
				}

				cur += 1;
				if(cur==m_end) return NULL;
				switch(*cur)
				{
				case '"':	value.append("\"", 1); cur += 1; break;
				case '\\':	value.append("\\", 1); cur += 1; break;
				case '/':	value.append("/", 1); cur += 1; break;
				case 'b':	value.append("\b", 1); cur += 1; break;
				case 'f':	value.append("\f", 1); cur += 1; break;
				case 'n':	value.append("\n", 1); cur += 1; break;
				case 'r':	value.append("\r", 1); cur += 1; break;
				case 't':	value.append("\t", 1); cur += 1; break;
				case 'u':
					{
						if(m_end-cur<6) return NULL;
						_U16 cp = 0;
						for(_U32 i=0; i<4; i++)
						{
							char v = cur[i+1];
							if(v>='0' && v<='9')
							{
								cp |= ((_U16)v - '0') << ((3-i)*4);
							}
							else if(v>='a' && v<='f')
							{
								cp |= ((_U16)v - 'a') << ((3-i)*4);
							}
							else if(v>='A' && v<='F')
							{
								cp |= ((_U16)v - 'A') << ((3-i)*4);
							}
							else
							{
								return NULL;
							}
						}
						if (cp <= 0x7f) 
						{
							value.append((char*)&cp, 1);
						} 
						else if (cp <= 0x7FF) 
						{
							char str[2];
							str[1] = (char)(0x80 | (0x3f & cp));
							str[0] = (char)(0xC0 | (0x1f & (cp >> 6)));
							value.append(str, 2);
						} 
						else if (cp <= 0xFFFF) 
						{
							char str[3];
							str[2] = (char)(0x80 | (0x3f & cp));
							str[1] = 0x80 | (char)((0x3f & (cp >> 6)));
							str[0] = 0xE0 | (char)((0xf & (cp >> 12)));
							value.append(str, 3);
						}
						cur += 5;
						break;
					}
				default: return NULL;
				}

				if(cur==m_end) return NULL;
			}
			return cur + 1;
		}

	private:
		const char* m_begin;
		const char* m_end;
	};

	static String g_NullString;
	static Array<JsonValue> g_NullArray;
	static Map<String, JsonValue> g_NullObject;
	static JsonValue g_NullValue;

	JsonValue::JsonValue()
	{
		m_type = TYPE_NULL;
	}

	JsonValue::JsonValue(const JsonValue& val)
	{
		m_type = TYPE_NULL;
		Set(val);
	}

	JsonValue::JsonValue(TYPE type)
	{
		m_type = TYPE_NULL;
		SetType(type);
	}

	JsonValue::JsonValue(bool val)
	{
		m_type = TYPE_BOOL;
		m_bool = val;
	}

	JsonValue::JsonValue(const char* val)
	{
		m_type = TYPE_STR;
		m_str = ZION_NEW String();
		*m_str = val;
	}

	JsonValue::JsonValue(const String& val)
	{
		m_type = TYPE_STR;
		m_str = ZION_NEW String();
		*m_str = val;
	}

	JsonValue::JsonValue(_U32 val)
	{
		m_type = TYPE_U32;
		m_u32 = val;
	}

	JsonValue::JsonValue(_S32 val)
	{
		m_type = TYPE_S32;
		m_s32 = val;
	}

	JsonValue::JsonValue(_F32 val)
	{
		m_type = TYPE_F32;
		m_f32 = val;
	}

	JsonValue::~JsonValue()
	{
		SetType(TYPE_NULL);
	}

	bool JsonValue::Parse(const char* begin, const char* end)
	{
		if(!end) end = begin + strlen(begin);
		JsonReader reader;
		return reader.Parse(begin, end, *this);
	}

	String JsonValue::Stringify() const
	{
		String ret;
		Stringify(ret);
		return ret;
	}

	void JsonValue::Stringify(String& json) const
	{
		OutputStringStream sstream;
		Stringify(sstream);
		json = sstream.str();
	}

	static void StringifyString(OutputStringStream& sstream, const String& str)
	{
		sstream<<"\"";

		for(size_t i=0; i<str.size(); i++)
		{
			char chr = *(str.c_str()+i);

			if (chr == '"' || chr == '\\' || chr == '/')
			{
				sstream << '\\' << chr;
			}
			else if (chr == '\b')
			{
				sstream << "\\b";
			}
			else if (chr == '\f')
			{
				sstream<<"\\f";
			}
			else if (chr == '\n')
			{
				sstream<<"\\n";
			}
			else if (chr == '\r')
			{
				sstream<<"\\r";
			}
			else if (chr == '\t')
			{
				sstream<<"\\t";
			}
			else if (chr < ' ')
			{
				sstream<<"\\u";
				for (int i = 0; i < 4; i++)
				{
					int value = (chr >> 12) & 0xf;
					if (value >= 0 && value <= 9)
						sstream<<(wchar_t)('0' + value);
					else if (value >= 10 && value <= 15)
						sstream<<(wchar_t)('A' + (value - 10));
					chr <<= 4;
				}
			}
			else
			{
				sstream<<chr;
			}
		}

		sstream<<"\"";
	}

	void JsonValue::Stringify(OutputStringStream& sstream) const
	{
		switch(m_type)
		{
		case TYPE_NULL: sstream << "null"; break;
		case TYPE_BOOL: sstream << m_bool?"true":"false"; break;
		case TYPE_U32: sstream << m_u32; break;
		case TYPE_S32: sstream << m_s32; break;
		case TYPE_F32: sstream << m_f32; break;
		case TYPE_STR: StringifyString(sstream, *m_str); break;
		case TYPE_OBJECT:
			{
				Map<String, JsonValue>::const_iterator i;
				sstream<<"{";
				i=m_object->begin();
				for(;;)
				{
					StringifyString(sstream, i->first);
					sstream<<":";
					i->second.Stringify(sstream);

					if((++i)==m_object->end()) break;
					sstream<<",";
				}
				sstream<<"}";
			}
			break;
		case TYPE_ARRAY:
			{
				sstream<<"[";
				if(m_array->size())
				{
					Array<JsonValue>::const_iterator i;
					i = m_array->begin();
					for(;;)
					{
						(*i).Stringify(sstream);
						if((++i)==m_array->end()) break;
						sstream<<",";
					}
				}
				sstream<<"]";
			}
			break;
		}
	}

	JsonValue::TYPE JsonValue::GetType()
	{
		return m_type;
	}

	void JsonValue::SetType(TYPE type)
	{
		if(type==m_type)
		{
			switch(m_type)
			{
			case TYPE_OBJECT: m_object->clear(); break;
			case TYPE_ARRAY: m_array->clear(); break;
			case TYPE_STR: m_str->clear(); break;
			default: break;
			};
			return;
		}
		switch(m_type)
		{
		case TYPE_OBJECT: m_object->~Map<String, JsonValue>(); ZION_FREE(m_object); break;
		case TYPE_ARRAY: m_array->~Array<JsonValue>(); ZION_FREE(m_array); break;
		case TYPE_STR: m_str->~String(); ZION_FREE(m_str); break;
		default: break;
		};
		m_type = type;
		switch(m_type)
		{
		case TYPE_OBJECT: m_object = new (ZION_ALLOC(sizeof(Map<String, JsonValue>))) Map<String, JsonValue>(); break;
		case TYPE_ARRAY: m_array = new (ZION_ALLOC(sizeof(Array<JsonValue>))) Array<JsonValue>(); break;
		case TYPE_STR: m_str = new (ZION_ALLOC(sizeof(String))) String(); break;
		default: break;
		};
	}
	
	bool JsonValue::IsNull() const
	{
		return m_type==TYPE_NULL;
	}

	bool JsonValue::IsBool() const
	{
		return m_type==TYPE_BOOL;
	}

	bool JsonValue::IsSTR() const
	{
		return m_type==TYPE_STR;
	}

	bool JsonValue::IsU32() const
	{
		return m_type==TYPE_U32 || (m_type==TYPE_F32 && m_s32>=-1);
	}

	bool JsonValue::IsS32() const
	{
		return m_type==TYPE_S32 || (m_type==TYPE_U32 && (m_u32&0x80000000)!=0);
	}

	bool JsonValue::IsF32() const
	{
		return m_type==TYPE_F32 || m_type==TYPE_U32 || m_type==TYPE_S32;
	}

	bool JsonValue::IsObject() const
	{
		return m_type==TYPE_OBJECT;
	}

	bool JsonValue::IsArray() const
	{
		return m_type==TYPE_ARRAY;
	}

	bool JsonValue::AsBool() const
	{
		if(m_type!=TYPE_BOOL) return false;
		return m_bool;
	}

	const String& JsonValue::AsSTR() const
	{
		if(m_type==TYPE_STR)
		{
			return *m_str;
		}
		else
		{
			return g_NullString;
		}
	}

	const char* JsonValue::AsCSTR() const
	{
		if(m_type==TYPE_STR)
		{
			return m_str->c_str();
		}
		else
		{
			return g_NullString.c_str();
		}
	}

	const _U32 JsonValue::AsU32() const
	{
		switch(m_type)
		{
		case TYPE_U32:
			return m_u32;
		case TYPE_S32:
			return (_U32)m_s32;
		default:
			return 0;
		}
	}

	const _S32 JsonValue::AsS32() const
	{
		switch(m_type)
		{
		case TYPE_U32:
			return (_S32)m_u32;
		case TYPE_S32:
			return m_s32;
		default:
			return 0;
		}
	}

	const _F32 JsonValue::AsF32() const
	{
		switch(m_type)
		{
		case TYPE_U32:
			return (_F32)m_u32;
		case TYPE_S32:
			return (_F32)m_s32;
		case TYPE_F32:
			return m_f32;
		default:
			return 0;
		}
	}

	const Map<String, JsonValue>& JsonValue::AsObject() const
	{
		if(m_type==TYPE_OBJECT)
		{
			return *m_object;
		}
		else
		{
			return g_NullObject;
		}
	}

	const Array<JsonValue>& JsonValue::AsArray() const
	{
		if(m_type==TYPE_ARRAY)
		{
			return *m_array;
		}
		else
		{
			return g_NullArray;
		}
	}

	JsonValue& JsonValue::Get(_U32 index)
	{
		if(m_type!=TYPE_ARRAY || index>=(_U32)m_array->size()) return g_NullValue;
		return (*m_array)[index];
	}

	const JsonValue& JsonValue::Get(_U32 index) const
	{
		if(m_type!=TYPE_ARRAY || index>=(_U32)m_array->size()) return g_NullValue;
		return (*m_array)[index];
	}

	_U32 JsonValue::GetSize() const
	{
		if(m_type!=TYPE_ARRAY) return 0;
		return (_U32)m_array->size();
	}

	const JsonValue& JsonValue::Get(const char* name) const
	{
		if(m_type!=TYPE_OBJECT) return g_NullValue;
		Map<String, JsonValue>::const_iterator i;
		i = m_object->find((char*)name);
		if(i==m_object->end()) return g_NullValue;
		return i->second;
	}

	const JsonValue& JsonValue::Get(const String& name) const
	{
		if(m_type!=TYPE_OBJECT) return g_NullValue;
		Map<String, JsonValue>::const_iterator i;
		i = m_object->find(name);
		if(i==m_object->end()) return g_NullValue;
		return i->second;
	}

	bool JsonValue::HasMember(const char* name) const
	{
		if(m_type!=TYPE_OBJECT) return false;
		if(m_object->find(name)==m_object->end()) return false;
		return true;
	}

	bool JsonValue::HasMember(const String& name) const
	{
		if(m_type!=TYPE_OBJECT) return false;
		if(m_object->find(name)==m_object->end()) return false;
		return true;
	}

	void JsonValue::Set(const JsonValue& val)
	{
		SetType(val.m_type);
		switch(m_type)
		{
		case TYPE_STR:		*m_str = *val.m_str;		break;
		case TYPE_U32:		m_u32 = val.m_u32;			break;
		case TYPE_S32:		m_s32 = val.m_s32;			break;
		case TYPE_F32:		m_f32 = val.m_f32;			break;
		case TYPE_OBJECT:	*m_object = *val.m_object;	break;
		case TYPE_ARRAY:	*m_array = *val.m_array;	break;
		default: break;
		}
	}

	void JsonValue::Set(bool val)
	{
		SetType(TYPE_BOOL);
		m_bool = val;
	}

	void JsonValue::Set(const char* val)
	{
		SetType(TYPE_STR);
		*m_str = val;
	}

	void JsonValue::Set(const String& val)
	{
		SetType(TYPE_STR);
		*m_str = val;
	}

	void JsonValue::Set(const _U32 val)
	{
		SetType(TYPE_U32);
		m_u32 = val;
	}

	void JsonValue::Set(const _S32 val)
	{
		SetType(TYPE_S32);
		m_s32 = val;
	}

	void JsonValue::Set(const _F32 val)
	{
		SetType(TYPE_F32);
		m_f32 = val;
	}

	const JsonValue& JsonValue::operator=(const JsonValue& val)
	{
		Set(val);
		return *this;
	}

	const JsonValue& JsonValue::operator=(bool val)
	{
		Set(val);
		return *this;
	}

	const JsonValue& JsonValue::operator=(const char* val)
	{
		Set(val);
		return *this;
	}

	const JsonValue& JsonValue::operator=(const String& val)
	{
		Set(val);
		return *this;
	}

	const JsonValue& JsonValue::operator=(_U32 val)
	{
		Set(val);
		return *this;
	}

	const JsonValue& JsonValue::operator=(_S32 val)
	{
		Set(val);
		return *this;
	}

	const JsonValue& JsonValue::operator=(_F32 val)
	{
		Set(val);
		return *this;
	}

	JsonValue* JsonValue::Append(const JsonValue& val)
	{
		if(m_type!=TYPE_ARRAY) return NULL;
		m_array->push_back(val);
		return &(*m_array)[m_array->size()-1];
	}

	JsonValue* JsonValue::Append(const _STR name, const JsonValue& val)
	{
		return Append(String(name), val);
	}

	JsonValue* JsonValue::Append(const String& name, const JsonValue& val)
	{
		if(m_type!=TYPE_OBJECT) return NULL;
		(*m_object)[name] = val;
		return &(*m_object)[name];
	}

}
