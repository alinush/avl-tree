/**
 * Author: Alin Tomescu
 * Website: http://alinush.is-great.org
 */
#include <AvlTest.hpp>

#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>

using std::endl;

/**
 *  Returns the running time in seconds.
 */
double AvlTreeTester::testRandom(long numbers)
{
    clock_t begin = clock(), end;

    for(long i = 0; i < numbers; i++)
    {

        if(i % 1000 == 0) {
            const char * optMsg = "";
            if(_checkIntegrity)
                optMsg = ", checking integrity every insert w/ O(n) overhead";
            logtrace << "Insert #" << i << optMsg << endl;

        }
            
        long num = rand();
    
        if(this->find(num) == NULL) {
            this->insert(num, num);
            if(_checkIntegrity && !testIntegrity())
                throw std::runtime_error("Integrity check failed while inserting random numbers.");

        } else
            logtrace << num << " already inserted in tree." << endl;
        
    }

    end = clock();
    double time = (double)(end - begin) / CLOCKS_PER_SEC;
    logdbg << "Done! Test succeeded in " << time << " seconds!" << std::endl;

    return time;
}
	
void AvlTreeTester::avlPrintInorder(Node * root, std::ostream& out)
{
    if(root)
    {
        avlPrintInorder(root->child[0], out);
        out << root->entry.key << " (b: " << root->balance << ")\n";
        avlPrintInorder(root->child[1], out);
    }
}

unsigned int AvlTreeTester::avlHeight(Node * root)
{
    if(root)
        return 1 + std::max(avlHeight(root->getLeft()), avlHeight(root->getRight()));
    else
        return 0;
}

bool AvlTreeTester::avlCheckHeights(Node * root)
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

bool AvlTreeTester::avlCheckBST(Node * root, Node * min, Node * max, unsigned long& currTreeSize)
{
    if(root == NULL) {
        return true;
    }

    currTreeSize++;

    // Check BST invariant left_subtree(r) < r && right_subtree(r) > r
    if(lessThan(root, min) || greaterThan(root, max)) {
        logerror << "Found node in subtree that does not respect BST property" << endl;
        return false; 
    }
    
    // Check that left child is NOT greater than the root
    if(root->getLeft() && greaterThan(root->getLeft(), root))
    {
        logerror << "Left child of " << root->entry.key << " is "  << root->getLeft()->entry.key;
        logerror << ", not smaller." << std::endl;
        return false;
    }

    // Check that right child is NOT less than the root
    if(root->getRight() && lessThan(root->getRight(), root))
    {
        logerror << "Right child of " << root->entry.key << " is "  << root->getRight()->entry.key;
        logerror << ", not greater." << std::endl;
        return false;
    }

    // Check that the children's parent pointers point to the parent node they are descended from
    for(int i = 0; i < 2; i++)
        if(root->getChild(i) && root->getChild(i)->parent != root)
        {
            logerror << "Node with inconsistent parent pointer in child # " << i << " node: " << root->entry.key << std::endl;
            return false;
        }
    
    return avlCheckBST(root->getLeft(), min, root, currTreeSize) && avlCheckBST(root->getRight(), root, max, currTreeSize);
}

bool AvlTreeTester::testIntegrity()
{
    Node min(LONG_MIN, 0);
    Node max(LONG_MAX, 0);

    unsigned long treeSize = 0;
    bool passed = avlCheckBST(_root, &min, &max, treeSize);

    if(treeSize != _size) {
        logerror << "Actual tree size of " << _size << " nodes differs from computed one of " << treeSize << " nodes." << endl;
        passed = false;
    }

    return passed;
}

void AvlTreeTester::printInorder(std::ostream& out)
{
    avlPrintInorder(_root, out);
}
