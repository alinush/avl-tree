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
#include <iostream>
#include <stdexcept>

using std::endl;

class AvlTreeTester : public AvlTree<long, long>
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
        /**
         *  Returns the running time in seconds.
         */
		double testRandom(long numbers = 16384);
	
	private:
		void avlPrintInorder(Node * root, std::ostream& out);
		
		unsigned int avlHeight(Node * root);
		
        bool avlCheckHeights(Node * root);
		
        bool avlCheckBST(Node * root, Node * min, Node * max, unsigned long& currTreeSize);
		
		bool testIntegrity();
		
		void printInorder(std::ostream& out);
};
