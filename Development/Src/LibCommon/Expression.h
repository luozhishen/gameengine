#ifndef __ZION_EXPRESSION__
#define __ZION_EXPRESSION__

namespace Zion
{
	namespace Expression
	{

		class BaseNode;
		class Node;
		class VM;

		class BaseNode
		{
		public:
			BaseNode(_U32 level);
			virtual ~BaseNode();

			bool SetParent(Node* pParent, _U32 Index);
			virtual _U32 GetChildrenCount();
			virtual BaseNode* GetChild(_U32 index);

			_U32 GetLevel();

		private:
			_U32 m_Level;
			Node* m_pParent;
			_U32 m_Index;
		};

		class Node : public BaseNode
		{
			friend class BaseNode;
		public:
			Node(_U32 level);
			virtual ~Node();

			virtual _U32 GetChildrenCount();
			virtual BaseNode* GetChild(_U32 index);

		protected:
			bool SetChild(BaseNode* node, _U32& index);

		private:
			Array<BaseNode*> m_Children;
		};

		class VM
		{
		public:
			VM();
			virtual ~VM();

			_U32 StackDepth();
			void StackPop(JsonValue& value);
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

		// level 0
		class ConstantNode : public BaseNode
		{
		public:
			ConstantNode(const JsonValue& value);

			const JsonValue& GetValue();

		private:
			JsonValue m_Value;
		};

		class VariantNode : public BaseNode
		{
		public:
			VariantNode(const String& name);

			const String& GetName();

		private:
			String m_Name;
		};

		class FunctionNode : public Node
		{
		public:
			FunctionNode(const String& name) : Node(0) {}

		private:
			String m_Name;
		};

		class NotNode : public Node
		{
		public:
			NotNode();
		};

		class BracketNode : public Node
		{
		};

		class ArrayNode : public Node
		{
		public:
		private:
			String m_Name;
		};

		// level 1
		class MultiplyNode : public Node
		{
		};

		class DivideNode : public Node
		{
		};

		class ModNode : public Node
		{
		};

		// level 2
		class AddNode : public Node
		{
		};

		class SubNode : public Node
		{
		};

		// level 3
		class EqualNode : public Node
		{
		};

		class NotEqualNode : public Node
		{
		};

		class GreaterNode : public Node
		{
		};

		class LessNode : public Node
		{
		};

		class GreaterEqualNode : public Node
		{
		};

		class LessEqualNode : public Node
		{
		};

		// level 4
		class AndNode : public Node
		{
		};

		class OrNode : public Node
		{
		};

		//

		bool ParseExpression(const String& exp, BracketNode& node);
		bool ExecuteExpression(VM* vm, const BraketNode& node, JsonValue& value);
*/

	}
}

#endif
