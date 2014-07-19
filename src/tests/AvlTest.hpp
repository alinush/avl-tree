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

	public:
		AvlTreeTester(bool checkIntegrity = false)
            : _checkIntegrity(checkIntegrity)
		{
			srand(time(0));
		}
		
	public:

		void testHeight();

        /**
         *  Returns the running time in seconds.
         */
		double testRandom(long numbers, long range);

	
	private:
		void printTree(const Tree& tree, std::ostream& out, size_t maxDigits) const;
		void printInorder(const Tree& tree, std::ostream& out) const { avlPrintInorder(tree.getRoot(), out); }
        bool testIntegrity(const Tree& tree) const;

		void avlPrintInorder(const Node * root, std::ostream& out) const;
        bool avlCheckBST(const Tree& tree, const Node * root, const Node * min, const Node * max, long& height, unsigned long& currTreeSize) const;
};
