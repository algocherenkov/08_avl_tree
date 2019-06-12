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
    struct Node
    {
        AvlTree*    parent {nullptr} ;
        AvlTree*    leftSubTree {nullptr};
        AvlTree*    rightSubTree {nullptr};
        ValueType   value;
        KeyType     key;
        int         height{1};
    };

    Node* node_{nullptr};
    AvlTree() = default;

    int getHeight() const { return node_? node_->height: 0; }
    int getHeightLeft() const { return node_->leftSubTree? node_->leftSubTree->getHeight():0;}
    int getHeightRight() const { return node_->rightSubTree? node_->rightSubTree->getHeight():0;}

    int getBalance() const { return node_? getHeightLeft() - getHeightRight(): 0; }
    void recalculateHeight() { if(node_) node_->height = std::max(getHeightLeft(), getHeightRight()) + 1; }

    void rotateLeft()
    {
        auto vertex = node_->rightSubTree;
        node_->rightSubTree= vertex->leftSubTree;
        vertex->leftSubTree = this;

        node_->height--;
        vertex->height++;
    }

    void rotateRight()
    {
        auto vertex = node_->rightSubTree;
        node_->rightSubTree= vertex->leftSubTree;
        vertex->leftSubTree = this;

        node_->height--;
        vertex->height++;
    }

    void bigRotateLeft()
    {
        rotateRight(node->right);
        rotateLeft(node);
    }

    void bigRotateRight(Node* node)
    {
        rotateLeft(node->left);
        rotateRight(node);
    }


public:


    void insert(T value);
    T remove(int index);
    void rebalance();

};

BOOST_AUTO_TEST_CASE(quicksort_test)
{

}

BOOST_AUTO_TEST_SUITE_END()
