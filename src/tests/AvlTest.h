/**
 * Author: Alin Tomescu
 * Website: http://alinush.is-great.org
 */

#include <Core.h>

#include <AvlTree.h>

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
		double testRandom(long numbers = 16384)
		{
			clock_t begin = clock(), end;

            long range = numbers % 32500;


			for(long i = 0; i < numbers; i++)
			{

				if(i % 1000 == 0)
				    logtrace << "Insert #" << i << "\n";
					
				long num = rand() % range;
				
				this->insert(num, num);
				
				if(_checkIntegrity && !testIntegrity())
					throw std::runtime_error("Integrity check failed while inserting random numbers.");
			}

            end = clock();
            double time = (double)(end - begin) / CLOCKS_PER_SEC;
			logdbg << "Done! Test succeeded in " << time << " seconds!" << std::endl;

            return time;
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
		
        bool avlCheckHeights(Node * root)
		{
			if(!root) {
				return true;
			}
            	
			int balance = root->balance;
			int absBalance = balance < 0 ? -balance : balance;
			
			if(absBalance > 1)
			{
				logerror << "Unbalanced at node: " << root->entry.key << std::endl;
				return false;
			}
			
			int diff = (int)avlHeight(root->getRight()) - (int)avlHeight(root->getLeft());
			if(diff != balance)
			{
				logerror << "Bad balance at node: " << root->entry.key << std::endl;
				logerror << "Real balance: " << diff << " vs. stored balance: " << balance << std::endl;
				return false;
			}
			
			return avlCheckHeights(root->getLeft()) && avlCheckHeights(root->getRight());
		}
		
        bool avlCheckBST(Node * root, Node * min, Node * max)
		{
			if(!root) {
				return true;
			} else if(lessThan(root, min) || greaterThan(root, max)) {
                logerror << "Found node in subtree that does not respect BST property" << endl;
                return false; 
            }
				
			if(root->getLeft() && greaterThan(root->getLeft(), root))
			{
				logerror << "Left child of " << root->entry.key << " is "  << root->getLeft()->entry.key;
				logerror << ", not smaller." << std::endl;
				return false;
			}
			
			if(root->getRight() && lessThan(root->getRight(), root))
			{
				logerror << "Right child of " << root->entry.key << " is "  << root->getRight()->entry.key;
				logerror << ", not greater." << std::endl;
				return false;
			}
			
			for(int i = 0; i < 2; i++)
				if(root->getChild(i) && root->getChild(i)->parent != root)
				{
					logerror << "Children with bad parent at node: " << root->entry.key << std::endl;
					return false;
				}
			
			return avlCheckBST(root->getLeft(), min, root) && avlCheckBST(root->getRight(), root, max);
		}
		
		bool testIntegrity()
		{
			unsigned long nodes = avlCountNodes(_root);
			if(_size != nodes)
			{
				logerror << "Size: " << _size << " vs. nodes: " << nodes << std::endl;
				return false;
			}

            Node min(LONG_MIN, 0);
            Node max(LONG_MAX, 0);

			return avlCheckBST(_root, &min, &max);
		}
		
		void printInorder(std::ostream& out)
		{
			avlPrintInorder(_root, out);
		}
};
