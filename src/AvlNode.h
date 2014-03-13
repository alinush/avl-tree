/**
 * Author: Alin Tomescu
 * Website: http://alinush.is-great.org
 */

#pragma once

#include <Core.h>

#include <stdexcept>

template<class Key, class Value>
class AvlEntry
{
	public:
		AvlEntry() {}
		AvlEntry(const Key& k, const Value& v)
			:	key(k), value(v)
		{}
		
	public:
		Key key;
		Value value;
};

template<class Key, class Value>
class AvlNode
{
	private:
		typedef AvlNode<Key, Value> Node;
		typedef AvlEntry<Key, Value> Entry;
		
	public:
		AvlNode(Node * parentNode = 0)
			:	parent(parentNode), balance(0)
		{
			child[0] = 0; child[1] = 0;	
		}
		
		AvlNode(const Key& k, const Value& v, Node * parentNode = 0)
			:	entry(k, v), parent(parentNode), balance(0)
		{
			child[0] = 0; child[1] = 0;	
		}
	
	public:
		void setLeft(Node * node) { setChild(node, 0); }
		void setRight(Node * node) { setChild(node, 1); }
		void setChild(Node * node, unsigned int index)
		{
			child[index] = node;
			if(node)
				node->parent = this;
		}
		void replaceChild(Node * oldChild, Node * newChild)
		{
			setChild(newChild, getChildIndex(oldChild));
		}
		
		Node * getLeft() { return getChild(0); }
		Node * getRight() { return getChild(1); }
		Node * getChild(unsigned int index)
		{
			return child[index];
		}
	
		bool isLeftChild(const Node * node) const { return getChildIndex(node) == 0; }
		bool isRightChild(const Node * node) const { return getChildIndex(node) == 1; }
		unsigned int getChildIndex(const Node * node) const
		{
			for(unsigned int i = 0; i < 2; i++)
				if(node == child[i])
					return i;
			
			throw std::runtime_error("AvlNode::getChildIndex(Node *) could not find specified node.");
		}
		
	public:
		Entry entry;
		
		Node * child[2];
		Node * parent;
		
		/**
		 *	The balance factor of a node is defined as the difference in height of 
		 *	its right subtree and leftsubtree.
		 *
		 *		balance(node) = height(node->right) - height(node->left);
		 */
		int balance;
};
