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
			virtual void GetValue(VM* vm, JsonValue& value) = 0;
			virtual String GetString() = 0;

			void SetParent(Node* pParent, _U32 Index);

			virtual _U32 GetNodeCount();
			virtual BaseNode* GetNode(_U32 index);

		private:
			Node* m_pParent;
			_U32 m_Index;
		};

		class ConstantNode : public BaseNode
		{
		};

		class VariantNode : public BaseNode
		{
		};

		class Node : public CBaseNode
		{
		public:
			Node(_U32 level);

			virtual _U32 GetNodeCount();
			virtual BaseNode* GetNode(_U32 index);

			void Apend(BaseNode* node);
			void SetNode(_U32 index, BaseNode* node);

		private:
			Array<BaseNode*> m_Children;
		};

		// level 0
		class FunctionNode : public Node
		{
		public:
			FunctionNode(const String& name) : Node(0) {}

		private:
			String m_Name;
		};

		class NotNode : public Node
		{
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
		class VM
		{
		public:
			virtual bool GetVariant(const String& name, JsonValue& value) = 0;
			virtual bool GetArray(const String& name, _U32 index, JsonValue& value) = 0;
			virtual bool CallFunction(const String& name, const Array<JsonValue>& params, JsonValue& value) = 0;
		};

		bool ParseExpression(const String& exp, BracketNode& node);
		bool ExecuteExpression(VM* vm, const BraketNode& node, JsonValue& value);

	}
}

#endif
