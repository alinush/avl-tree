/**
 * Author: Alin Tomescu
 * Website: http://alinush.is-great.org
 */
#pragma once

#include <Core.hpp>

#include <AvlTree.hpp>
#include <AvlNode.hpp>

#include <climits>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <stdexcept>

using std::endl;

typedef AvlNode<long, long> Node;
typedef AvlTree<long, long> Tree;

class AvlTreeTester
{
    private:
        bool _checkIntegrity;
        unsigned long _testSize, _range;

	public:
		AvlTreeTester(bool checkIntegrity, unsigned long testSize)
            : _checkIntegrity(checkIntegrity), _testSize(testSize), _range(testSize*testSize)
		{
			srand(time(0));
		}
		
	public:
		void testComparator();
		void testHeight();
		void testRandom();

	
	private:
		void printTree(const Tree& tree, std::ostream& out, size_t maxDigits) const;
		void printInorder(const Tree& tree, std::ostream& out) const { avlPrintInorder(tree.getRoot(), out); }
        bool testIntegrity(const Tree& tree) const;

		void avlPrintInorder(const Node * root, std::ostream& out) const;
        bool avlCheckBST(const Tree& tree, const Node * root, const Node * min, const Node * max, long& height, unsigned long& currTreeSize) const;
};
