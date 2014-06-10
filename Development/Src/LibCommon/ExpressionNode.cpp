#include <ZionDefines.h>
#include <ZionSTL.h>
#include <FastJson.h>
#include "ExpressionNode.h"

namespace Zion
{
	namespace Expression
	{

		BaseNode::BaseNode(_U32 level)
		{
			m_Level = level;
			m_pParent = NULL;
			m_Index = (_U32)-1;
		}

		BaseNode::~BaseNode()
		{
		}

		bool BaseNode::SetParent(Node* pParent, _U32 Index)
		{
			if(m_pParent)
			{
				if(!pParent->SetChild(NULL, Index)) return false;
				m_pParent = NULL;
				m_Index = (_U32)-1;
			}
			if(!pParent->SetChild(this, Index)) return false;
			m_pParent = pParent;
			m_Index = Index;
			return true;
		}

		_U32 BaseNode::GetChildrenCount()
		{
			return 0;
		}

		BaseNode* BaseNode::GetChild(_U32 index)
		{
			return NULL;
		}

		_U32 BaseNode::GetLevel()
		{
			return m_Level;
		}
		
		Node::Node(_U32 level) : BaseNode(level)
		{
		}

		Node::~Node()
		{
		}

		_U32 Node::GetChildrenCount()
		{
			return (_U32)m_Children.size();
		}

		BaseNode* Node::GetChild(_U32 index)
		{
			if(index>=(_U32)m_Children.size()) return NULL;
			return m_Children[index];
		}

		bool Node::SetChild(BaseNode* node, _U32& index)
		{
			if(index==(_U32)-1)
			{
				m_Children.push_back(node);
				index = (_U32)m_Children.size()-1;
			}
			else
			{
				if(index>=(_U32)m_Children.size()) return false;
				m_Children[index] = node;
			}
			return true;
		}

	}
}
