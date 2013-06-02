#ifndef _JSON_H_
#define _JSON_H_

// Win32 incompatibilities
#if defined(WIN32) && !defined(__GNUC__)
	#define wcsncasecmp _strnicmp
	static inline bool isnan(double x) { return x != x; }
	static inline bool isinf(double x) { return !isnan(x) && isnan(x - x); }
#endif

// Linux compile fix - from quaker66
#ifdef __GNUC__
	#include <cstring>
	#include <cstdlib>
#endif

// Mac compile fixes - from quaker66, Lion fix by dabrahams
#if defined(__APPLE__) && __DARWIN_C_LEVEL < 200809L || (defined(WIN32) && defined(__GNUC__))
	#include <wctype.h>
	#include <wchar.h>
	
	static inline int wcsncasecmp(const char *s1, const char *s2, size_t n)
	{
		int lc1  = 0;
		int lc2  = 0;

		while (n--)
		{
			lc1 = towlower (*s1);
			lc2 = towlower (*s2);

			if (lc1 != lc2)
				return (lc1 - lc2);

			if (!lc1)
				return 0;

			++s1;
			++s2;
		}

		return 0;
	}
#endif

// Simple function to check a string 's' has at least 'n' characters
static inline bool simplejson_wcsnlen(const char *s, size_t n) {
	if (s == 0)
		return false;

	const char *save = s;
	while (n-- > 0)
	{
		if (*(save++) == 0) return false;
	}

	return true;
}

namespace Zion
{

	class JSON;
	class JSONValue;

	// Custom types
	typedef Array<JSONValue*> JSONArray;
	typedef Map<String, JSONValue*> JSONObject;

	enum JSONType { JSONType_Null, JSONType_String, JSONType_Bool, JSONType_Number, JSONType_Array, JSONType_Object };

	class JSONValue
	{
		friend class JSON;
	
		public:
			JSONValue(JSONType type=JSONType_Null);
			JSONValue(const char *m_char_value);
			JSONValue(const String &m_string_value);
			JSONValue(bool m_bool_value);
			JSONValue(double m_number_value);
			JSONValue(const JSONArray &m_array_value);
			JSONValue(const JSONObject &m_object_value);
			JSONValue(const JSONValue& node);
			~JSONValue();

			bool IsNull() const;
			bool IsString() const;
			bool IsBool() const;
			bool IsNumber() const;
			bool IsArray() const;
			bool IsObject() const;
		
			const String &AsString() const;
			bool AsBool() const;
			double AsNumber() const;
			const JSONArray &AsArray() const;
			const JSONObject &AsObject() const;

			std::size_t CountChildren() const;
			bool HasChild(std::size_t index) const;
			JSONValue *Child(std::size_t index);
			bool HasChild(const char* name) const;
			JSONValue *Child(const char* name);

			String Stringify() const;

		protected:
			static bool Parse(JSONValue& value, const char **data);

		private:
			static String StringifyString(const String &str);
	
			JSONType type;
			String string_value;
			bool bool_value;
			double number_value;
			JSONArray array_value;
			JSONObject object_value;
	};

	class JSON
	{
		friend class JSONValue;
	
		public:
			static JSONValue* Parse(const char *data);
			static String Stringify(const JSONValue *value);
		protected:
			static bool SkipWhitespace(const char **data);
			static bool ExtractString(const char **data, String &str);
			static double ParseInt(const char **data);
			static double ParseDecimal(const char **data);
		private:
			JSON();
	};

}

#endif
