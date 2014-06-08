#include <ZionDefines.h>
#include <ZionSTL.h>
#include <FastJson.h>
#include "Expression.h"
#include "ExpressionParser.h"
#include "ExpressionNode.h"

namespace Zion
{
	namespace Expression
	{

		VM::VM()
		{
		}

		VM::~VM()
		{
		}

		_U32 VM::StackDepth()
		{
			return (_U32)m_Stack.size();
		}

		bool VM::StackPop(JsonValue& value)
		{
			if(m_Stack.empty()) return false;
			value = m_Stack[m_Stack.size()-1];
			m_Stack.pop_back();
			return true;
		}

		void VM::StackPush(const JsonValue& value)
		{
			m_Stack.push_back(value);
		}

		bool VM::GetVariant(const String& name)
		{
			return false;
		}

		bool VM::GetArray(const String& name)
		{
			return false;
		}

		bool VM::CallFunction(const String& name, const JsonValue* JsonValue, _U32 arg_count)
		{
			return false;
		}

		VMState::VMState()
		{
		}

		VMState::~VMState()
		{
		}

		bool VMState::Compile(const String& exp)
		{
			RootNode root;
			if(!ParseExpression(exp, root)) return false;
			return Compile(&root);
		}

		bool VMState::Compile(const RootNode* root)
		{
			return false;
		}

		bool VMState::Load(const _U8* data, _U32 len)
		{
			return false;
		}

		bool VMState::Save()
		{
			return false;
		}

		bool VMState::Execute(VM* vm, JsonValue& retval)
		{
			return true;
		}

	}
}
