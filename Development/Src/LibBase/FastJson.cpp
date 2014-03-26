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
			m_current = begin;
			if(!ParseElement(root)) return false;
			m_current = Skip(m_current);
			return m_current==end;
		}

	protected:
		const char* Skip(const char* cur)
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
			return cur;
		}

		bool ParseElement(JsonValue& value)
		{
			if(ParseNumber(value)) return true;
			if(ParseString(value)) return true;
			if(ParseObject(value)) return true;
			if(ParseArray(value)) return true;
			return false;
		}

		bool ParseNumber(JsonValue& value)
		{
			return true;
		}

		bool ParseString(JsonValue& value)
		{
			m_current = Skip(m_current);
			if(*m_current!='"') return false;
			const char* start = (++m_current);
			for(; m_current<m_end; m_current++)
			{
				if(*m_current=='"')
				{
					String val;
					val.append(start, m_current-start);
					m_current += 1;
					return true;
				}
			}
			return false;
		}

		bool ParseArray(JsonValue& value)
		{
			m_current = Skip(m_current);
			if(*m_current!='[') return false;
			const char* cur = m_current + 1;

			value = JsonValue(JsonValue::TYPE_ARRAY);

			Skip();
			if(*cur!=']')
			{
				for(;;)
				{

					JsonValue val;
					if(!ParseElement(val)) return false;

					cur = Skip(cur);
					if(*cur!=',')
					{
						if(*cur==']') break;
						return false;
					}
					cur += 1;
				}
			}
			m_current = cur + 1;
			return true;
		}

		bool ParseObject(JsonValue& value)
		{
			m_current = Skip(m_current);
			if(*m_current!='{') return false;
			m_current += 1;
			return true;
		}

	private:
		const char* m_begin;
		const char* m_end;
		const char* m_current;
	};

	bool JsonValue::Parse(const char* begin, const char* end, JsonValue& root)
	{
		JsonReader reader;
		return reader.Parse(begin, end, root);
	}

}
