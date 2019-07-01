#include <fstream>
#include <math.h>
#include <algorithm>

#define BOOST_TEST_MODULE test_main

#include <boost/test/included/unit_test.hpp>

using namespace boost::unit_test;
BOOST_AUTO_TEST_SUITE(test_suite_main)

template <typename KeyType, typename ValueType>
class AvlTree
{
public:
    struct Node
    {
        Node(KeyType key, ValueType value):
            key(key),
            value(value)
        {}
        AvlTree*    parent {nullptr} ;
        AvlTree*    leftSubTree {nullptr};
        AvlTree*    rightSubTree {nullptr};
        ValueType   value;
        KeyType     key;
        int         height{1};
    };

private:
    Node* node_{nullptr};



    int getHeight() const { return node_? node_->height: 0; }
    int getHeightLeft(Node* node) const { return node->leftSubTree? node->leftSubTree->getHeight():0;}
    int getHeightRight(Node* node) const { return node->rightSubTree? node->rightSubTree->getHeight():0;}

    int getBalance(Node* node) const { return node? getHeightLeft(node) - getHeightRight(node): 0; }
    void recalculateHeight(Node* node) { node->height = std::max(getHeightLeft(node), getHeightRight(node)) + 1; }

    Node* rotateLeft(Node* pivotNode)
    {
        if(!pivotNode)
            return nullptr;

        auto rightSubtree = pivotNode->rightSubTree;
        if(!rightSubtree)
            return nullptr;

        pivotNode->rightSubTree = rightSubtree->node_->leftSubTree;
        rightSubtree->node_->leftSubTree = new AvlTree(pivotNode);

        recalculateHeight(pivotNode);
        recalculateHeight(rightSubtree->node_);

        return rightSubtree->node_;
    }

    Node* rotateRight(Node* pivotNode)
    {
        if(!pivotNode)
            return nullptr;

        auto leftSubtree = pivotNode->leftSubTree;
        if(!leftSubtree)
            return nullptr;

        pivotNode->leftSubTree= leftSubtree->node_->rightSubTree;
        leftSubtree->node_->rightSubTree = new AvlTree(pivotNode);

        recalculateHeight(pivotNode);
        recalculateHeight(leftSubtree->node_);

        return leftSubtree->node_;
    }

    Node* bigRotateLeft(Node* node)
    {
        node->rightSubTree->node_ = rotateRight(node->rightSubTree->node_);
        return rotateLeft(node);
    }

    Node* bigRotateRight(Node* node)
    {
        node->leftSubTree->node_ = rotateLeft(node->leftSubTree->node_);
        return rotateRight(node);
    }

public:

    AvlTree(KeyType key, ValueType value)
    {
        node_ = new Node(key, value);
    }

    AvlTree(Node* node)
    {
        node_ = node;
    }

    ~AvlTree()
    {
        delete node_;
    }

    Node* getNode() const { return node_; }

    Node* insert(KeyType key, ValueType value)
    {        
        if(key <= node_->key)
        {
            if(node_->leftSubTree)
            {
                node_->leftSubTree->node_ = node_->leftSubTree->insert(key, value);
            }
            else
            {
                node_->leftSubTree = new AvlTree(key, value);                
            }
        }
        else
        {
            if(node_->rightSubTree)
            {
                node_->rightSubTree->node_ = node_->rightSubTree->insert(key, value);
            }
            else
            {
                node_->rightSubTree = new AvlTree(key, value);                
            }
        }

        recalculateHeight(node_);
        auto balance = getBalance(node_);

        if (balance > 1 && node_->leftSubTree && key <= node_->leftSubTree->node_->key)
            return rotateRight(node_);

        if (balance < -1 && node_->rightSubTree && key > node_->rightSubTree->node_->key)
            return rotateLeft(node_);

        if (balance > 1 && node_->leftSubTree && key > node_->leftSubTree->node_->key)
            return bigRotateRight(node_);

        if (balance < -1 && node_->rightSubTree && key < node_->rightSubTree->node_->key)
            return bigRotateLeft(node_);

        return node_;
    }

    Node* getMinFromRightSubTree(Node* node)
    {
        Node* currentNode = node;

        while(currentNode->leftSubTree != nullptr)
            currentNode = currentNode->leftSubTree->node_;

        return currentNode;
    }

    void removeNode(KeyType key)
    {
        removeNode(key, node_);
    }

    Node* removeNode(KeyType key, Node* node)
    {
        if(node == nullptr)
            return node;

        if(key < node->key)
            node->leftSubTree->node_ = removeNode(key, node->leftSubTree? node->leftSubTree->node_: nullptr);
        else if(key > node->key)
            node->rightSubTree->node_ = removeNode(key, node->rightSubTree? node->rightSubTree->node_: nullptr);
        else
        {
            if(node->leftSubTree == nullptr || node->rightSubTree == nullptr)
            {
                Node* temp = node->leftSubTree ? node->leftSubTree->node_ : node->rightSubTree? node->rightSubTree->node_: nullptr;

                if (temp == nullptr)
                {
                    temp = node;
                    node = nullptr;
                }
                else
                {
                    node->key = temp->key;
                    node->value = temp->value;
                }

                delete temp;
            }
            else
            {
                Node* temp = getMinFromRightSubTree(node->rightSubTree->node_);

                node->key = temp->key;
                node->value = temp->value;

                node->rightSubTree->node_ = removeNode(temp->key, node->rightSubTree->node_);
            }
        }

        if(node == nullptr)
            return node;

        recalculateHeight(node);
        auto balance = getBalance(node);

        if (balance > 1 && node->leftSubTree && key <= node->leftSubTree->node_->key)
            return rotateRight(node_);

        if (balance < -1 && node->rightSubTree && key > node->rightSubTree->node_->key)
            return rotateLeft(node_);

        if (balance > 1 && node->leftSubTree && key > node->leftSubTree->node_->key)
            return bigRotateRight(node_);

        if (balance < -1 && node->rightSubTree && key < node->rightSubTree->node_->key)
            return bigRotateLeft(node_);

        return node;
    }

    void rebalance();


};
void fillVectorFromTree(std::vector<int>& vector, AvlTree<int, std::string>::Node* root)
{
    if(root)
    {
        if(root->leftSubTree)
            fillVectorFromTree(vector, root->leftSubTree->getNode());

        vector.push_back(root->key);

        if(root->rightSubTree)
            fillVectorFromTree(vector, root->rightSubTree->getNode());
    }
}

BOOST_AUTO_TEST_CASE(quicksort_test)
{
    AvlTree<int, std::string> tree(5, "root");
    tree.insert(7, "alala");
    tree.insert(3, "ololo");
    tree.insert(10, "atata");
    tree.insert(1, "azaza");
    tree.insert(12, "ululu");
    tree.insert(15, "ahaha");
    tree.insert(19, "tilitili");
    tree.insert(4, "bububu");
    tree.insert(-1, "gagaga");
    tree.insert(-3, "fififi");
    tree.insert(21, "tratata");
    tree.insert(20, "wiwiwi");

    std::vector<int> keyOrder;
    fillVectorFromTree(keyOrder, tree.getNode());

    int prev = keyOrder[0];
    for(const auto& key: keyOrder)
    {
        BOOST_CHECK_MESSAGE(prev <= key, "tree was formed wrong - key: " << key);
        prev = key;
    }

    tree.removeNode(12);
    tree.removeNode(-1);
    tree.removeNode(21);
    tree.removeNode(10);
    tree.removeNode(5);
    keyOrder.clear();

    fillVectorFromTree(keyOrder, tree.getNode());

    prev = keyOrder[0];
    for(const auto& key: keyOrder)
    {
        BOOST_CHECK_MESSAGE(prev <= key, "tree was formed wrong - key: " << key);
        prev = key;
    }

    BOOST_TEST_MESSAGE("end of test");
}

BOOST_AUTO_TEST_SUITE_END()

