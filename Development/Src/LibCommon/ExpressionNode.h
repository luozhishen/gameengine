#ifndef __ZION_EXPRESSION_NODE__
#define __ZION_EXPRESSION_NODE__

namespace Zion
{
	namespace Expression
	{

		class BaseNode;
		class Node;

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
		public:
			BracketNode();
		};

		class ArrayNode : public Node
		{
		public:
			ArrayNode(const String& name);

		private:
			String m_Name;
		};

		// level 1
		class MultiplyNode : public Node
		{
		public:
			MultiplyNode();
		};

		class DivideNode : public Node
		{
		public:
			DivideNode();
		};

		class ModNode : public Node
		{
		public:
			ModNode();
		};

		// level 2
		class AddNode : public Node
		{
		public:
			AddNode();
		};

		class SubNode : public Node
		{
		public:
			SubNode();
		};

		// level 3
		class EqualNode : public Node
		{
		public:
			EqualNode();
		};

		class NotEqualNode : public Node
		{
		public:
			NotEqualNode();
		};

		class GreaterNode : public Node
		{
		public:
			GreaterNode();
		};

		class LessNode : public Node
		{
		public:
			LessNode();
		};

		class GreaterEqualNode : public Node
		{
		public:
			GreaterEqualNode();
		};

		class LessEqualNode : public Node
		{
		public:
			LessEqualNode();
		};

		// level 4
		class AndNode : public Node
		{
		public:
			AndNode();
		};

		class OrNode : public Node
		{
		public:
			OrNode();
		};

		//
		class RootNode : public BracketNode
		{
		};

	}
}

#endif
