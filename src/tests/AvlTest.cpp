/**
 * Author: Alin Tomescu
 * Website: http://alinush.is-great.org
 */
#include <AvlTest.hpp>

#include <ctime>
#include <vector>
#include <iomanip>
#include <stdexcept>

using namespace std;
using std::endl;

/**
 *  Returns the running time in seconds.
 */
double AvlTreeTester::testRandom(long numbers, long range)
{
    clock_t begin = clock(), end;
    long numCollisions = 0;

    for(long i = 0; i < numbers; i++)
    {
        long num = rand() % range;

        if(i == 0 || (i + 1) % 1000 == 0) {
        	const char * optMsg = "";
            if(_checkIntegrity)
                optMsg = ", checking integrity every insert w/ O(n) overhead";
            logtrace << "Insert #" << i+1 << ": " << num << optMsg << endl;
        }
    
        if(this->find(num) == NULL) {
            this->insert(num, num);
            if(_checkIntegrity && !testIntegrity())
                throw new std::runtime_error("Integrity check failed while inserting random numbers.");

        } else {
            logdbg << num << " already inserted in tree." << endl;
            numCollisions++;
        }
    }

    end = clock();
    double time = (double)(end - begin) / CLOCKS_PER_SEC;
    logdbg << "Done! Inserted " << numbers - numCollisions << " items"
    	<< " (supposed to insert " << numbers << ", but got " << numCollisions << " collisions)" << endl;
    logdbg << "Test succeeded in " << time << " seconds!" << std::endl;

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

bool AvlTreeTester::avlCheckBST(Node * root, Node * min, Node * max, long& height, unsigned long& currTreeSize)
{
    if(root == NULL) {
        return true;
    }
    
    currTreeSize++;
    Node * left = root->getLeft();
    Node * right= root->getRight();

    // Check BST invariant left_subtree(r) < r && right_subtree(r) > r
    if(lessThan(root, min) || greaterThan(root, max)) {
        logerror << "Found node in subtree that does not respect BST property" << endl;
        return false; 
    }
    
    // Check that left child is NOT greater than the root
    if(left && greaterThan(left, root))
    {
        logerror << "Left child of " << root->entry.key << " is "  << left->entry.key;
        logerror << ", not smaller." << std::endl;
        return false;
    }

    // Check that right child is NOT less than the root
    if(right && lessThan(right, root))
    {
        logerror << "Right child of " << root->entry.key << " is "  << right->entry.key;
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
    
    // Check balance factors (stored vs. calculated) 
    int balance = root->balance;
    int absBalance = balance < 0 ? -balance : balance;
    
    if(absBalance > 1)
    {
        logerror << "Unbalanced at node: " << root->entry.key << std::endl;
        return false;
    }
 
    // Since the root node is not NULL, increase the height of the subtrees
    long leftHeight = height + 1, rightHeight = height + 1;
    bool passed =
    		avlCheckBST(left,  min, root, leftHeight,  currTreeSize) &&
    		avlCheckBST(right, root, max, rightHeight, currTreeSize);

    height = std::max(leftHeight, rightHeight);

    if((rightHeight - leftHeight) != balance)
    {
        logerror
            << "Bad balance at node '" << root->entry.key
            << "'. Real balance is "  << leftHeight - rightHeight
            << " != stored balance of " << balance << std::endl;
        return false;
    }

    return passed;
}

bool AvlTreeTester::testIntegrity()
{
    Node min(LONG_MIN, 0);
    Node max(LONG_MAX, 0);

    unsigned long treeSize = 0;
    long h = 0;
    bool passed = avlCheckBST(_root, &min, &max, h, treeSize);

    if(treeSize != _size) {
        logerror << "Actual tree size of " << _size << " nodes differs from computed one of " << treeSize << " nodes." << endl;
        passed = false;
    }

    if(h != height()) {
    	logerror << "avlCheckBST computed different height: " << h << " vs. real height of " << height() << endl;
    	passed = false;
    }

    return passed;
}

void AvlTreeTester::printInorder(std::ostream& out)
{
    avlPrintInorder(_root, out);
}

void AvlTreeTester::printTree(std::ostream& out, size_t maxDigits)
{
    maxDigits++;
    if(maxDigits % 2) maxDigits++;

    std::vector<std::pair<Node *, int> > nodes;

    // We need to know the tree's height and the max number of digits 
    // in the numbers for nice formatting.
    int height = avlHeight(_root);
    int prevLevel = 0;
    
    // We start with the root node, at level 1
    nodes.push_back(std::pair<Node *, int>(_root, prevLevel));

    size_t i = 0;
    unsigned long maxNodes = pow(2.0, height) - 1;
    unsigned long numInserted = 1;

    // last level has 2^(height-1) nodes
    unsigned long lastLevelNumNodes = (maxNodes + 1) / 2;
    unsigned long nodeWidth = maxDigits * pow(2.0, height - 1);
    unsigned long spacing = (nodeWidth - maxDigits) / 2;

    logdbg << "-----------" << endl;
    logdbg << "Tree size: " << size() << endl;
    logdbg << "Tree height: " << height << endl;
    logdbg << "Max # of nodes: "<< maxNodes << endl;
    logdbg << "Last level # of nodes: " << lastLevelNumNodes << endl;
    logdbg << "Max digits: " << maxDigits << endl;
    logdbg << "Initial node width " << nodeWidth << " and spacing " << spacing << endl;

    out << endl;
    while(numInserted < maxNodes)
    {
        std::pair<Node *, int> p = nodes.at(i);
        Node * next = p.first;
        int level = p.second + 1;

        // We push nodes even if they are NULL, since we need to print the whitespace
        nodes.push_back(std::pair<Node *, int>(next ? next->getLeft()  : NULL, level));
        nodes.push_back(std::pair<Node *, int>(next ? next->getRight() : NULL, level));

        if(level != 1 && level != prevLevel) {
            prevLevel = level;
            out << endl;
            nodeWidth /= 2;
            spacing = (nodeWidth - maxDigits) / 2;
        }

        if(next) {
            out << setw(spacing) << "";
        	out << setw(maxDigits) << next->getValue();
        	out << setw(spacing) << "";
        } else {
            out << setw(spacing) << "";
        	out << setw(maxDigits) << ".";
        	out << setw(spacing) << "";
        }

        i++;
        numInserted += 2;
    }

    // Print the last level
    while(i < numInserted) {
    	std::pair<Node *, int> p = nodes.at(i);
		Node * next = p.first;
		int level = p.second + 1;

		if(level != prevLevel) {
			prevLevel = level;
			out << endl;
			nodeWidth /= 2;
			spacing = (nodeWidth - maxDigits) / 2;
		}

		if(next) {
			out << setw(spacing) << "";
			out << setw(maxDigits) << next->getValue();
			out << setw(spacing) << "";
		} else {
			out << setw(spacing) << "";
			out << setw(maxDigits) << ".";
			out << setw(spacing) << "";
		}
    	i++;
    }
    out << endl << endl;

    out << "All done! " << nodes.size() << " nodes inserted" << endl;
}
