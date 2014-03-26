#include <ZionBase.h>
#include "FastJson.h"

namespace Zion
{

	static String g_NullString;
	static Array<JsonValue> g_NullArray;
	static Map<String, JsonValue> g_NullObject;
	static JsonValue g_NullValue;

	JsonValue::JsonValue()
	{
		m_type = TYPE_NONE;
	}

	JsonValue::JsonValue(const JsonValue& val)
	{
		SetType(val.m_type);
		switch(m_type)
		{
		case TYPE_STR:		m_str = val.m_str; break;
		case TYPE_U32:		m_u32 = val.m_u32; break;
		case TYPE_S32:		m_s32 = val.m_s32; break;
		case TYPE_F32:		m_f32 = val.m_f32; break;
		case TYPE_OBJECT:	m_object = val.m_object; break;
		case TYPE_ARRAY:	m_array = val.m_array; break;
		}
	}

	JsonValue::JsonValue(TYPE type)
	{
		m_type = type;
	}

	JsonValue::TYPE JsonValue::GetType()
	{
		return m_type;
	}

	void JsonValue::SetType(TYPE type)
	{
		m_object.clear();
		m_array.clear();
		m_type = type;
	}

	bool JsonValue::IsSTR() const
	{
		return m_type==TYPE_STR;
	}

	bool JsonValue::IsU32() const
	{
		return m_type==TYPE_U32 || (m_type==TYPE_F32 && m_s32>=0);
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

	const String& JsonValue::AsSTR() const
	{
		if(m_type==TYPE_STR)
		{
			return m_str;
		}
		else
		{
			return g_NullString;
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
			return m_object;
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
			return m_array;
		}
		else
		{
			return g_NullArray;
		}
	}

	const JsonValue& JsonValue::Get(_U32 index) const
	{
		if(m_type!=TYPE_ARRAY || index>(_U32)m_array.size()) return g_NullValue;
		return m_array[index];
	}

	_U32 JsonValue::GetSize() const
	{
		return (_U32)m_array.size();
	}

	const JsonValue& JsonValue::Get(const _STR name) const
	{
		if(m_type!=TYPE_OBJECT) return g_NullValue;
		Map<String, JsonValue>::const_iterator i;
		i = m_object.find((char*)name);
		if(i==m_object.end()) return g_NullValue;
		return i->second;
	}

	const JsonValue& JsonValue::Get(const String& name) const
	{
		if(m_type!=TYPE_OBJECT) return g_NullValue;
		Map<String, JsonValue>::const_iterator i;
		i = m_object.find(name);
		if(i==m_object.end()) return g_NullValue;
		return i->second;
	}

	bool JsonValue::HasMember(const _STR name) const
	{
		if(m_type!=TYPE_OBJECT) return false;
		if(m_object.find(name)==m_object.end()) return false;
		return true;
	}

	bool JsonValue::HasMember(const String& name) const
	{
		if(m_type!=TYPE_OBJECT) return false;
		if(m_object.find(name)==m_object.end()) return false;
		return true;
	}

	void JsonValue::Set(const _STR val)
	{
		m_type = TYPE_STR;
		m_str = val;
	}

	void JsonValue::Set(const String& val)
	{
		m_type = TYPE_STR;
		m_str = val;
	}

	void JsonValue::Set(const _U32 val)
	{
		m_type = TYPE_U32;
		m_u32 = val;
	}

	void JsonValue::Set(const _S32 val)
	{
		m_type = TYPE_S32;
		m_s32 = val;
	}

	void JsonValue::Set(const _F32 val)
	{
		m_type = TYPE_F32;
		m_f32 = val;
	}

	void JsonValue::Append(const JsonValue& val)
	{
		if(m_type!=TYPE_ARRAY) return;
		m_array.push_back(val);
	}

	void JsonValue::Append(const _STR name, const JsonValue& val)
	{
		if(m_type!=TYPE_OBJECT) return;
		m_object[name] = val;
	}

	void JsonValue::Append(const String& name, const JsonValue& val)
	{
		if(m_type!=TYPE_OBJECT) return;
		m_object[name] = val;
	}

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
			if(*cur=='"') return ParseString(cur, value);
			if(*cur=='[') return ParseArray(cur, value);
			if(*cur=='{') return ParseObject(cur, value);
			return ParseNumber(cur, value);
		}

		const char* ParseNumber(const char* start, JsonValue& value)
		{
			start = Skip(start);
			if(!start) return false;
			const char* cur = start;
			
			return cur;
		}

		const char* ParseString(const char* start, JsonValue& value)
		{
			start = Skip(start);
			if(!start || *start!='"') return NULL;
			const char* cur = start + 1;
			while(*cur!='"')
			{
				if((++cur)==m_end) return NULL;
			}
			value.SetType(JsonValue::TYPE_STR);
			value.m_str.clear();
			value.m_str.append(start+1, cur-start-1);
			return cur + 1;
		}

		const char* ParseArray(const char* start, JsonValue& value)
		{
			ZION_ASSERT(*start=='[');
			const char* cur = Skip(start + 1);
			if(!cur) return NULL;
			value.SetType(JsonValue::TYPE_ARRAY);
			value.m_array.clear();
			if(*cur==']') return cur + 1;

			for(;;)
			{
				value.m_array.push_back(JsonValue());
				JsonValue& val = value.m_array[value.m_array.size()-1];
				cur = ParseElement(cur, val);
				if(!cur) return false;

				cur = Skip(cur);
				if(!cur) return NULL;
				if(*cur!=',')
				{
					if(*cur==']') break;
					return false;
				}
				cur += 1;
			}
			return cur + 1;
		}

		const char* ParseObject(const char* start, JsonValue& value)
		{
			ZION_ASSERT(*start=='{');
			const char* cur = Skip(start + 1);
			if(!cur) return NULL;
			value.SetType(JsonValue::TYPE_OBJECT);
			value.m_object.clear();
			if(*cur=='}') return cur + 1;
			for(;;)
			{
				const char* name_start = cur;
				for(;;)
				{
					if(*cur<'a' || *cur>'z')
					{
						if(*cur<'A' || *cur>'Z')
						{
							if(*cur<'0' || *cur>'9')
							{
								if(*cur!='_')
								{
									break;
								}
							}
						}
					}

					cur += 1;
					if(cur==m_end)
						return NULL;
				}
				if(name_start==cur) return NULL;
				String name;
				name.append(name_start, cur-name_start);

				cur = Skip(cur);
				if(!cur || *cur!=':') return NULL;
				
				JsonValue& node = value.m_object[name];
				cur = ParseElement(cur+1, node);
				if(!cur) return NULL;

				cur = Skip(cur);
				if(!cur)
					return NULL;
				if(*cur=='}')
					break;
				if(*cur!=',')
					return NULL;
			}
			return cur+1;
		}

	private:
		const char* m_begin;
		const char* m_end;
	};

	bool JsonValue::Parse(const char* begin, const char* end, JsonValue& root)
	{
		JsonReader reader;
		return reader.Parse(begin, end, root);
	}

}
