/**
 * Author: Alin Tomescu
 * Website: http://alinush.is-great.org
 */

#pragma once

#include <Core.hpp>

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

        enum { LEFT = 0, RIGHT = 1 };
		
	public:
		AvlNode(Node * parentNode = NULL)
			:	parent(parentNode), balance(0)
		{
			child[LEFT] = NULL; child[RIGHT] = NULL;
		}
		
		AvlNode(const Key& k, const Value& v, Node * parentNode = NULL)
			:	entry(k, v), parent(parentNode), balance(0)
		{
			child[LEFT] = NULL; child[RIGHT] = NULL;	
		}
	
	public:
		void setLeft(Node * node) { setChild(node, LEFT); }
		void setRight(Node * node) { setChild(node, RIGHT); }
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

        bool hasChildren() { return child[LEFT] != NULL || child[RIGHT] != NULL; }
        bool hasLeftChild() { return child[LEFT] != NULL; }
        bool hasRightChild() { return child[RIGHT] != NULL; }
		
		Node * getLeft() { return getChild(LEFT); }
		const Node * getLeft() const { return getChild(LEFT); }
		Node * getRight() { return getChild(RIGHT); }
		const Node * getRight() const { return getChild(RIGHT); }
		Node * getChild(unsigned int index) { return child[index]; }
		const Node * getChild(unsigned int index) const { return child[index]; }
	
		bool isLeftChild(const Node * node) const { return getChildIndex(node) == LEFT; }
		bool isRightChild(const Node * node) const { return getChildIndex(node) == RIGHT; }
		unsigned int getChildIndex(const Node * node) const
		{
			for(unsigned int i = 0; i < 2; i++)
				if(node == child[i])
					return i;
			
			throw new std::runtime_error("AvlNode::getChildIndex(Node *) could not find specified node.");
		}

        Value getValue() const { return entry.value; }
        Value * getValuePtr() { return &entry.value; }
        Value& getValueRef() { return entry.value; }
        const Value& getValueRef() const { return entry.value; }
        Key getKey() { return entry.key; }
		
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
