#include <fstream>
#include <math.h>

#define BOOST_TEST_MODULE test_main

#include <boost/test/included/unit_test.hpp>

using namespace boost::unit_test;
BOOST_AUTO_TEST_SUITE(test_suite_main)

template <typename T>
class AvlTree
{
    struct Node
    {
        Node* left {nullptr};
        Node* right {nullptr};
        T value;
        int height{0};
    };

    AvlTree() = default;
    Node root;

public:
    void insert(T value);
    T remove(int index);
    void rebalance();

};

BOOST_AUTO_TEST_CASE(quicksort_test)
{

}

BOOST_AUTO_TEST_SUITE_END()
