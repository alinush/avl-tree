/**
 * Author: Alin Tomescu
 * Website: http://alinush.is-great.org
 */

#pragma once

#include <Core.hpp>

#include <AvlNode.hpp>

#include <functional>
#include <iosfwd>

/**
 *	This class declares and implements an AVL tree, a balanced binary tree
 *	that provides logarithmic insertion, deletion and lookup time.
 */
template<class Key, class Value, class Compare = std::less<Key> >
class AvlTree
{
	protected:
		typedef AvlTree<Key, Value, Compare> Tree;
		typedef AvlNode<Key, Value> Node;
		
	public:
		AvlTree() : _root(NULL), _size(0) {}
	
	protected:
		/**
		 *	Returns true if the key in the new node is strictly less than the
		 *	key in the specified subtree's root, and false otherwise.
		 */
		bool lessThan(const Node * newNode, const Node * subtree) const
		{
			return _compare(newNode->entry.key, subtree->entry.key);
		}
		
		/**
		 *	Returns true if the key in the first node is equal to the
		 *	key in the second node, and false otherwise.
		 */
		bool equal(const Node * first, const Node * second)
		{
			return !lessThan(first, second) && !lessThan(second, first);
		}
		
		/**
		 *	Returns true if the key in the first node is greatert than the
		 *	key in the second node, and false otherwise.
		 */
		bool greaterThan(const Node * first, const Node * second)
		{
			return !equal(first, second) && !lessThan(first, second);
		}
		
		/**
		 *	Inserts the new node into the AVL tree. First, it finds the spot in 
		 *	the tree for the new node. Second, it updates the balance factors
		 *	of all the nodes down the path from the root to the new node. Third,
		 *	it calls avlBalance to check if the tree become unbalanced and
		 *	rebalance it.
		 */
		void avlInsert(Node * newNode)
		{
			/**
			 *	Go down through the tree and insert the new node as a leaf.
			 */
			Node * it = _root, * parent;
			int idx;
			
			do
			{
				parent = it;
                // TODO: duplicate keys handle
				idx = lessThan(newNode, it) ? 0 : 1;
				
				it = it->child[idx];
			} while(it);
			
			/**
			 *	We found the place for the new node in the tree.
			 */
			parent->setChild(newNode, idx);
			
			/**
			 *	Now we must update the balances of all the ancestors of our new node,
			 *	until one of the new balance factors becomes zero or we reach the root node.
			 *
			 *	We also stop when we update a balance to 2 or -2 because this balance will
			 *	become 0 after rebalancing, which implies higher ancestors will keep their
			 *	current balance factors.
			 */
			Node * ancestor = newNode;
			Node * child;
			
			do {
				child = ancestor;
				ancestor = ancestor->parent;
				
				ancestor->balance += ancestor->isLeftChild(child) ? -1 : 1;
				
			} while((ancestor->balance == 1 || ancestor->balance == -1) && ancestor != _root);
			
			/**
			 *	Balance the tree, unless the newly inserted node does not have a ancestor,
			 *	in which case the tree is too small to be rebalanced.
			 */
			if(newNode->parent != _root)
				avlBalance(ancestor);
		}
		
		/**
		 *	Checks if there are any nodes with balance factors of 2 or -2, in which case
		 *	the subtree needs to be rotated at that node.
		 *
		 *	The rotation and balance factor "magic" is explained in the documentation thoroughly.
		 */
		void avlBalance(Node * ancestor)
		{
			if(ancestor->balance == -2)
			{
				if(ancestor->getLeft()->balance == -1)
				{
					avlSingleRotation(ancestor, 0);
				}
				else if(ancestor->getLeft()->balance == 1)
				{
					avlDoubleRotation(ancestor, 0);
				}
				else
				{
					throw new std::runtime_error("AvlTree::avlBalance(Node *) inconsistency detected.");
				}
			}
			else if(ancestor->balance == 2)
			{
				if(ancestor->getRight()->balance == 1)
				{
					avlSingleRotation(ancestor, 1);
				}
				else if(ancestor->getRight()->balance == -1)
				{
					avlDoubleRotation(ancestor, 1);
				}
				else
				{
					throw new std::runtime_error("AvlTree::avlBalance(Node *) inconsistency detected.");
				}
			}
		}
		
		/**
		 *	Direction is 0 for right-right rotation and 1 for left-left.
		 */
		void avlSingleRotation(Node * p, unsigned int dir)
		{
			/**
			 *	If we're rotating at the root of the tree then we need to reset 
			 *	the _root pointer once we're done.
			 */
			unsigned int opposed = dir ? 0 : 1;
			
			bool setNewRoot = (p == _root);
			Node * q = p->child[dir];
			
			/**
			 *	"Rewire" the tree accordingly.
			 */
			p->setChild(q->child[opposed], dir);
			
			Node * oldRootParent = p->parent;
			if(oldRootParent)
				oldRootParent->replaceChild(p, q);

			q->setChild(p, opposed);
			
			if(setNewRoot)
			{
				_root = q;
				_root->parent = 0;
			}
			
			p->balance = 0;
			q->balance = 0;
		}
		
		/**
		 *	Direction is 0 for left-right rotation and 1 for right-left rotation.
		 */
		void avlDoubleRotation(Node * p, unsigned int dir)
		{
			unsigned int opposed = dir ? 0 : 1;
			bool setNewRoot = (p == _root);
			
			/**
			 *	The new subtree after the rotation will be rooted at a different
			 *	node and will have a left and a right subtree.
			 */
			Node * q = p->getChild(dir);
			Node * r = q->getChild(opposed);
			
			/**
			 *	We are now ready to "rewire" the subtree.
			 */
			Node * pParent = p->parent;
			if(pParent)
				pParent->replaceChild(p, r);
			
			p->setChild(r->getChild(opposed), dir);
			q->setChild(r->getChild(dir), opposed);
			
			r->setChild(q, dir);
			r->setChild(p, opposed);
			
			if(setNewRoot)
			{
				_root = r;
				_root->parent = 0;
			}
			
			/**
			 *	Recompute the new balance factors. There are a few cases depending
			 *	on the old balance factor of R.
			 */
			if(r->balance == 0)
			{
				r->getLeft()->balance = 0;
				r->getRight()->balance = 0;
			}
			else if(r->balance == -1)
			{
				r->getLeft()->balance = 0;
				r->getRight()->balance = 1;
			}
			else /* if(r->balance == 1) */
			{
				r->getLeft()->balance = -1;
				r->getRight()->balance = 0;
			}
			
			r->balance = 0;
		}
		
		unsigned int avlHeight(Node * root) {
			if(root)
				return 1 + std::max(avlHeight(root->getLeft()), avlHeight(root->getRight()));
			else
				return 0;
		}

	public:
		/**
		 *	Looks for the value associated with the specified key in the tree
		 *	and returns a pointer to the value or null if such a (key, value) 
		 *	pair was not inserted in the tree.
		 */
		Value * find(const Key& key)
		{
            assert((_root != NULL && _size != 0) || (_size == 0 && _root == NULL));
			Node * it = _root;

            if(_root == NULL)
                return NULL;
			
			do
			{
				if(_compare(key, it->entry.key))
					it = it->child[0];
				else if(_compare(it->entry.key, key))
					it = it->child[1];
				else
					return &(it->entry.value);
			} while(it);
			
			return NULL;
		}
		
		const Value * find(const Key& key) const
		{
			return find(key);
		}
		
		/**
		 *	Inserts the specified (key, value) pair into the tree.
		 */
		void insert(const Key& key, const Value& value)
		{ 
			_size++;
			
			/**
			 *	The basic case arises when the tree is empty. In this case
			 *	we'll create a new node and set it as the root of the tree.
			 */
			if(_root == NULL)
				_root = new Node(key, value);
			else
			{
				/**
				 *	Create an isolated node and insert it into the tree.
				 */
				avlInsert(new Node(key, value));
			}
		}
		
		/**
		 *	Returns the number of (key, value) pairs stored into the tree.
		 */
		unsigned long size() const { return _size; }
		
		unsigned int height()
		{
		    return avlHeight(_root);
		}

	protected:
		/**
		 *	The comparator object takes in two keys as parameters and returns
		 *	true if the first one is less than the second one and false otherwise.
		 */
		Compare _compare;
		
		/**
		 *	Pointer to the root of the tree.
		 */
		Node * _root;
		
		/**
		 *	The size of the tree -- the number of nodes.
		 */
		unsigned long _size;
};
