#ifndef __ZION_EXPRESSION__
#define __ZION_EXPRESSION__

namespace Zion
{
	namespace Expression
	{

		class RootNode;

		class VM
		{
		public:
			VM();
			virtual ~VM();

			_U32 StackDepth();
			bool StackPop(JsonValue& value);
			void StackPush(const JsonValue& value);

			bool GetVariant(const String& name);
			bool GetArray(const String& name);
			bool CallFunction(const String& name, const JsonValue* JsonValue, _U32 arg_count);

			virtual bool GetVariant(const String& name, JsonValue& value) = 0;
			virtual bool GetArray(const String& name, _U32 index, JsonValue& value) = 0;
			virtual bool CallFunction(const String& name, const Array<JsonValue>& params, JsonValue& value) = 0;

		private:
			Array<JsonValue> m_Stack;
		};

		class VMState
		{
		public:
			VMState();
			~VMState();

			bool Compile(const String& exp);
			bool Compile(const RootNode* root);
			bool Load(const _U8* data, _U32 len);
			bool Save();
			bool Execute(VM* vm, JsonValue& retval);
		};

	}
}

#endif
