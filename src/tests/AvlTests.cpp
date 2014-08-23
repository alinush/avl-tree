/**
 * Author: Alin Tomescu
 * Website: http://alinush.is-great.org
 */
#include <AvlTests.hpp>

#include <ctime>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include <memory>

using std::endl;
using std::setw;

void AvlTests::testRandomInserts()
{
    Tree tree;

    loginfo << "Inserting " << _testSize << " random numbers ranging from 0 to " << _range - 1 << "..." << endl;

    long numCollisions = 0;

    for(unsigned long i = 0; i < _testSize; i++)
    {
        long num = rand() % _range;

        if(i == 0 || (i + 1) % 1000 == 0) {
            const char * optMsg = "";
            if(_checkIntegrity)
                optMsg = ", checking integrity every insert w/ O(n) overhead";
            logtrace << "Insert #" << i+1 << ": " << num << optMsg << endl;
        }
    
        if(tree.find(num) == NULL) {
            tree.insert(num, num);
            if(_checkIntegrity && !testIntegrity(tree))
                throw new std::runtime_error("Integrity check failed while inserting random numbers.");

        } else {
            logdbg << num << " already inserted in tree." << endl;
            numCollisions++;
        }
    }

    if(numCollisions > 0)
        logdbg << "Supposed to insert " << _testSize << " numbers, but got " << numCollisions << " collision(s)." << endl;

    if(_testSize - numCollisions != tree.size()) {
        throw new std::runtime_error("Tree size does not match expected size after insertions");
    }
}

void AvlTests::testHeight() {
    Tree tree;

    tree.insert(1, 1);
    if(tree.height() != 1)
        throw new std::runtime_error("Tree with one node should have height 1");

    tree.insert(2, 1);
    if(tree.height() != 2)
        throw new std::runtime_error("Tree with two nodes should have height 2");

}

void AvlTests::testComparator() {
    Tree tree;

    for(int i = 1; i < 1024; i++) {
        std::unique_ptr<Node> n1(new Node(i, 0));
        std::unique_ptr<Node> n2(new Node(i + 1, 0));

        if(!tree.lessThan(n1.get(), n2.get()))
            throw new std::runtime_error("lessThan is not working: smaller item reported greater");
        if(tree.lessThan(n2.get(), n1.get()))
            throw new std::runtime_error("lessThan is not working: greater item reported smaller");
        if(tree.lessThan(n1.get(), n1.get()))
            throw new std::runtime_error("lessThan is not working: equal item reported smaller");
        if(!tree.equal(n1.get(), n1.get()))
            throw new std::runtime_error("equal is not working: equal items reported different");
        if(tree.equal(n1.get(), n2.get()))
            throw new std::runtime_error("equal is not working: different items reported equal");
        if(!tree.greaterThan(n2.get(), n1.get()))
            throw new std::runtime_error("greaterThan is not working: greater item reported smaller");
        if(tree.greaterThan(n1.get(), n2.get()))
            throw new std::runtime_error("greaterThan is not working: smaller item reported greater");
        if(tree.greaterThan(n1.get(), n1.get()))
            throw new std::runtime_error("greaterThan is not working: equal item reported greater");
    }
}

void AvlTests::testRemoves() {
    // TODO: build custom trees for all test cases
}

bool AvlTests::avlCheckBST(const Tree& tree, const Node * root, const Node * min, const Node * max, long& height, unsigned long& currTreeSize) const
{
    if(root == NULL) {
        return true;
    }
    
    currTreeSize++;
    const Node * left = root->getLeft();
    const Node * right= root->getRight();

    // Check BST invariant left_subtree(r) < r && right_subtree(r) > r
    if(tree.lessThan(root, min) || tree.greaterThan(root, max)) {
        logerror << "Found node in subtree that does not respect BST property" << endl;
        return false; 
    }
    
    // Check that left child is NOT greater than the root
    if(left && tree.greaterThan(left, root))
    {
        logerror << "Left child of " << root->entry.key << " is "  << left->entry.key;
        logerror << ", not smaller." << std::endl;
        return false;
    }

    // Check that right child is NOT less than the root
    if(right && tree.lessThan(right, root))
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
            avlCheckBST(tree, left,  min, root, leftHeight,  currTreeSize) &&
            avlCheckBST(tree, right, root, max, rightHeight, currTreeSize);

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

bool AvlTests::testIntegrity(const Tree& tree) const
{
    Node min(LONG_MIN, 0);
    Node max(LONG_MAX, 0);

    unsigned long treeSize = 0;
    long h = 0;
    bool passed = avlCheckBST(tree, tree.getRoot(), &min, &max, h, treeSize);

    if(treeSize != tree.size()) {
        logerror << "Actual tree size of " <<  tree.size() << " nodes differs from computed one of " << treeSize << " nodes." << endl;
        passed = false;
    }

    if(h != tree.height()) {
        logerror << "avlCheckBST computed different height: " << h << " vs. real height of " << tree.height() << endl;
        passed = false;
    }

    return passed;
}

void AvlTests::printTree(const Tree& tree, std::ostream& out, size_t maxDigits) const
{
    maxDigits++;
    // Even max digits makes layout simpler
    if(maxDigits % 2) maxDigits++;

    std::vector<std::pair<const Node *, int> > nodes;

    // We need to know the tree's height and the max number of digits 
    // in the numbers for nice formatting.
    int height = tree.height();
    int prevLevel = 0;
    
    // We start with the root node, at level 1
    nodes.push_back(std::pair<const Node *, int>(tree.getRoot(), prevLevel));

    size_t i = 0;
    unsigned long maxNodes = pow(2.0, height) - 1;
    unsigned long numInserted = 1;

    // last level has 2^(height-1) nodes
    unsigned long lastLevelNumNodes = (maxNodes + 1) / 2;
    unsigned long nodeWidth = maxDigits * pow(2.0, height - 1);
    unsigned long spacing = (nodeWidth - maxDigits) / 2;

    logdbg << "-----------" << endl;
    logdbg << "Tree size: " <<  tree.size() << endl;
    logdbg << "Tree height: " << height << endl;
    logdbg << "Max # of nodes: "<< maxNodes << endl;
    logdbg << "Last level # of nodes: " << lastLevelNumNodes << endl;
    logdbg << "Max digits: " << maxDigits << endl;
    logdbg << "Initial node width " << nodeWidth << " and spacing " << spacing << endl;

    out << endl;
    while(numInserted < maxNodes)
    {
        std::pair<const Node *, int> p = nodes.at(i);
        const Node * next = p.first;
        int level = p.second + 1;

        // We push nodes even if they are NULL, since we need to print the whitespace
        nodes.push_back(std::pair<const Node *, int>(next ? next->getLeft()  : NULL, level));
        nodes.push_back(std::pair<const Node *, int>(next ? next->getRight() : NULL, level));

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
        std::pair<const Node *, int> p = nodes.at(i);
        const Node * next = p.first;
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

void AvlTests::avlPrintInorder(const Node * root, std::ostream& out) const
{
    if(root)
    {
        avlPrintInorder(root->child[0], out);
        out << root->entry.key << " (b: " << root->balance << ")\n";
        avlPrintInorder(root->child[1], out);
    }
}
