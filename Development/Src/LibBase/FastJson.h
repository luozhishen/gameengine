#ifndef __FAST_JSON__
#define __FAST_JSON__

namespace Zion
{

	class JsonReader;

	class JsonValue
	{
		friend 	class JsonReader;
	public:
		static bool Parse(const char* begin, const char* end, JsonValue& root);

		enum TYPE
		{
			TYPE_NULL,
			TYPE_BOOL,
			TYPE_STR,
			TYPE_U32,
			TYPE_S32,
			TYPE_F32,
			TYPE_OBJECT,
			TYPE_ARRAY,
		};
		JsonValue();
		JsonValue(const JsonValue& val);
		JsonValue(TYPE type);
		~JsonValue();

		TYPE GetType();
		void SetType(TYPE type);

		bool IsNull() const;
		bool IsBool() const;
		bool IsSTR() const;
		bool IsU32() const;
		bool IsS32() const;
		bool IsF32() const;
		bool IsObject() const;
		bool IsArray() const;

		bool AsBool() const;
		const String& AsSTR() const;
		const char* AsCSTR() const;
		const _U32 AsU32() const;
		const _S32 AsS32() const;
		const _F32 AsF32() const;
		const Map<String, JsonValue>& AsObject() const;
		const Array<JsonValue>& AsArray() const;

		const JsonValue& Get(_U32 index) const;
		_U32 GetSize() const;

		const JsonValue& Get(const char* name) const;
		const JsonValue& Get(const String& name) const;
		bool HasMember(const char* name) const;
		bool HasMember(const String& name) const;

		void Set(bool val);
		void Set(const char* val);
		void Set(const String& val);
		void Set(const _U32 val);
		void Set(const _S32 val);
		void Set(const _F32 val);

		void Append(const JsonValue& val);
		void Append(const _STR name, const JsonValue& val);
		void Append(const String& name, const JsonValue& val);
		
	private:
		TYPE	m_type;
		union {
			bool					m_bool;
			String*					m_str;
			_U32					m_u32;
			_S32					m_s32;
			_F32					m_f32;
			Map<String, JsonValue>*	m_object;
			Array<JsonValue>*		m_array;
		};
	};


}

#endif
