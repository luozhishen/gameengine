#ifndef __FAST_JSON__
#define __FAST_JSON__

namespace Zion
{

	class JsonValue
	{
	public:
		static bool Parse(const char* begin, const char* end, JsonValue& root);

		enum TYPE
		{
			TYPE_NONE,
			TYPE_STR,
			TYPE_U32,
			TYPE_S32,
			TYPE_F32,
			TYPE_OBJECT,
			TYPE_ARRAY,
		};
		JsonValue();
		JsonValue(const JsonValue& val);
		JsonValue(TYPE typ);

		TYPE GetType();
		void SetType(TYPE type);

		bool IsSTR() const;
		bool IsU32() const;
		bool IsS32() const;
		bool IsF32() const;
		bool IsObject() const;
		bool IsArray() const;

		const String& AsSTR() const;
		const _U32 AsU32() const;
		const _S32 AsS32() const;
		const _F32 AsF32() const;
		const Map<String, JsonValue>& AsObject() const;
		const Array<JsonValue>& AsArray() const;

		const JsonValue& Get(_U32 index) const;
		_U32 GetSize() const;

		const JsonValue& Get(const _STR name) const;
		const JsonValue& Get(const String& name) const;
		bool HasMember(const _STR name) const;
		bool HasMember(const String& name) const;

		void Set(const _STR val);
		void Set(const String& val);
		void Set(const _U32 val);
		void Set(const _S32 val);
		void Set(const _F32 val);

		void Append(const JsonValue& val);
		void Append(const _STR name, const JsonValue& val);
		void Append(const String& name, const JsonValue& val);
		
	private:
		TYPE	m_type;
		String	m_str;
		_U32	m_u32;
		_S32	m_s32;
		_F32	m_f32;
		Map<String, JsonValue>	m_object;
		Array<JsonValue>		m_array;
	};


}

#endif
