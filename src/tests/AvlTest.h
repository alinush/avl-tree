/**
 * Author: Alin Tomescu
 * Website: http://alinush.is-great.org
 */

#include <AvlTree.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>

class AvlTreeTester : public AvlTree<long, long>
{
	public:
		AvlTreeTester()
		{
			srand(time(0));
		}
		
	public:
		void testRandom(long numbers = 3000)
		{
			long range = numbers % 32500;

			std::cout << "Inserting " << numbers << " numbers..." << std::endl;

			for(long i = 0; i < numbers; i++)
			{
				// if(i % 1000 == 0)
					// std::cout << "Insert #" << i << "\n";
					
				long num = rand() % range;
				
				this->insert(num, num);
				
				if(!testIntegrity())
					throw std::runtime_error("Integrity check failed while inserting random numbers.");
			}
		}
	
	private:
		void avlPrintInorder(Node * root, std::ostream& out)
		{
			if(root)
			{
				avlPrintInorder(root->child[0], out);
				out << root->entry.key << " (b: " << root->balance << ")\n";
				avlPrintInorder(root->child[1], out);
			}
		}
		
		unsigned int avlHeight(Node * root)
		{
			if(root)
				return 1 + std::max(avlHeight(root->getLeft()), avlHeight(root->getRight()));
			else
				return 0;
		}
		
		unsigned long avlCountNodes(Node * root)
		{
			if(root)
				return 1 + avlCountNodes(root->getLeft()) + avlCountNodes(root->getRight());
			else
				return 0;
		}
		
		bool avlTest(Node * root)
		{
			if(!root)
				return true;
				
			int balance = root->balance;
			int absBalance = balance < 0 ? -balance : balance;
			
			if(absBalance > 1)
			{
				std::cout << "Unbalanced at node: " << root->entry.key << std::endl;
				return false;
			}
			
			int diff = (int)avlHeight(root->getRight()) - (int)avlHeight(root->getLeft());
			if(diff != balance)
			{
				std::cout << "Bad balance at node: " << root->entry.key << std::endl;
				std::cout << "Real balance: " << diff << " vs. stored balance: " << balance << std::endl;
				return false;
			}
			
			if(root->getLeft() && greaterThan(root->getLeft(), root))
			{
				std::cout << "Left child of " << root->entry.key << " is "  << root->getLeft()->entry.key;
				std::cout << ", not smaller." << std::endl;
				return false;
			}
			
			if(root->getRight() && lessThan(root->getRight(), root))
			{
				std::cout << "Right child of " << root->entry.key << " is "  << root->getRight()->entry.key;
				std::cout << ", not greater." << std::endl;
				return false;
			}
			
			for(int i = 0; i < 2; i++)
				if(root->getChild(i) && root->getChild(i)->parent != root)
				{
					std::cout << "Children with bad parent at node: " << root->entry.key << std::endl;
					return false;
				}
			
			return avlTest(root->getLeft()) && avlTest(root->getRight());
		}
		
		bool testIntegrity()
		{
			unsigned long nodes = avlCountNodes(_root);
			if(_size != nodes)
			{
				std::cout << "Size: " << _size << " vs. nodes: " << nodes << std::endl;
				return false;
			}
			return avlTest(_root);
		}
		
		void printInorder(std::ostream& out)
		{
			avlPrintInorder(_root, out);
		}
};
