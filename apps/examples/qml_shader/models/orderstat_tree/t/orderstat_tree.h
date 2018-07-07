/*
    Shader1
    Copyright (c) 2017 Frank Richter


    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

#include <cxxtest/TestSuite.h>

#include "orderstat_tree/intrusive.h"

#include <algorithm>
#include <random>
#include <numeric>
#include <vector>

#include <boost/range/size.hpp>

class OrderStatTreeTestSuite : public CxxTest::TestSuite
{
public:
  void testIndexAddendInsertInOrder (void)
  {
    struct Node : public boost::intrusive::set_base_hook<>
    {
      size_t value = 0;
      size_t sum = 0;
    };
    typedef orderstat_tree::intrusive::rbtree<Node,
                                              orderstat_tree::intrusive::sum_member<Node, size_t, &Node::sum>,
                                              orderstat_tree::intrusive::addend_index<size_t>> tree_type;
    
    std::vector<Node> nodeStorage;
    tree_type tree;
    static const size_t nodeCount = 10;

    nodeStorage.reserve (nodeCount);
    for (size_t i = 0; i < nodeCount; i++)
    {
      nodeStorage.emplace_back ();
      auto& newNode = nodeStorage.back ();
      newNode.value = i;

      tree.push_back (newNode);
    }

    TS_ASSERT_EQUALS (tree.root_sum(), nodeCount);

    for (size_t i = 0; i < nodeCount; i++)
    {
      const auto& vecNode = nodeStorage[i];
      const auto& treeNode = *(tree.get_iterator_by_sum (i));
      TS_ASSERT_EQUALS (&vecNode, &treeNode);
    }

    {
      size_t count = 0;
      for (const auto& treeNode : tree)
      {
        TS_ASSERT_LESS_THAN(count, nodeCount);
        TS_ASSERT_EQUALS (treeNode.value, count);
        TS_ASSERT_EQUALS (tree.get_sum_for_iterator (tree.iterator_to (treeNode)), count);
        count++;
      }
      TS_ASSERT_EQUALS(count, nodeCount)
    }
  }

  void testIndexAddendInsertRandomOrder (void)
  {
    typedef orderstat_tree::intrusive::sum_base_hook<size_t> sum_hook;
    struct Node :
      public boost::intrusive::set_base_hook<>,
      public sum_hook
    {
      size_t value = 0;

      bool operator< (const Node& other) const
      { return value < other.value; }
    };
    typedef orderstat_tree::intrusive::rbtree<Node,
                                              orderstat_tree::intrusive::sum_base<sum_hook>,
                                              orderstat_tree::intrusive::addend_index<size_t>> tree_type;
    
    std::vector<Node> nodeStorage;
    tree_type tree;
    static const size_t nodeCount = 10;

    nodeStorage.reserve (nodeCount);
    for (size_t i = 0; i < nodeCount; i++)
    {
      nodeStorage.emplace_back ();
      auto& newNode = nodeStorage.back ();
      newNode.value = i;
    }

    {
      std::vector<size_t> insertOrder (nodeCount);
      std::iota (insertOrder.begin(), insertOrder.end(), 0);
      std::mt19937 rng (42);
      std::shuffle (insertOrder.begin(), insertOrder.end(), rng);

      for (auto nodeIndex : insertOrder)
      {
        tree.insert_equal (nodeStorage[nodeIndex]);
      }
    }

    TS_ASSERT_EQUALS (tree.root_sum(), nodeCount);

    for (size_t i = 0; i < nodeCount; i++)
    {
      const auto& vecNode = nodeStorage[i];
      const auto& treeNode = *(tree.get_iterator_by_sum (i));
      TS_ASSERT_EQUALS (&vecNode, &treeNode);
    }

    {
      size_t count = 0;
      for (const auto& treeNode : tree)
      {
        TS_ASSERT_LESS_THAN(count, nodeCount);
        TS_ASSERT_EQUALS (treeNode.value, count);
        TS_ASSERT_EQUALS (tree.get_sum_for_iterator (tree.iterator_to (treeNode)), count);
        count++;
      }
      TS_ASSERT_EQUALS(count, nodeCount)
    }
  }

  void testCustomAddend (void)
  {
    typedef orderstat_tree::intrusive::sum_base_hook<size_t> sum_hook;
    struct Node :
      public boost::intrusive::set_base_hook<>,
      public sum_hook
    {
      size_t value = 0;
      size_t addend = 0;

      bool operator< (const Node& other) const
      { return value < other.value; }
    };
    typedef orderstat_tree::intrusive::rbtree<Node,
                                              orderstat_tree::intrusive::sum_base<sum_hook>,
                                              orderstat_tree::intrusive::addend_member<Node, size_t, &Node::addend>> tree_type;
    
    std::vector<Node> nodeStorage;
    tree_type tree;
    static const size_t nodeCount = 5;
    static const size_t nodeAddends[nodeCount] = { 3, 1, 2, 1, 4 };

    static const size_t expectedNodeSums[nodeCount] = { 0, 3, 4, 6, 7 };
    static const size_t expectedRootSum = 11;
    static const size_t expectedValues[expectedRootSum] = { 0, 0, 0, 1, 2, 2, 3, 4, 4, 4, 4 };

    nodeStorage.reserve (nodeCount);
    for (size_t i = 0; i < nodeCount; i++)
    {
      nodeStorage.emplace_back ();
      auto& newNode = nodeStorage.back ();
      newNode.value = i;
      newNode.addend = nodeAddends[i];

      tree.push_back (newNode);
    }

    TS_ASSERT_EQUALS (tree.root_sum(), expectedRootSum);

    for (size_t i = 0; i < boost::size (expectedValues); i++)
    {
      const auto& treeNode = *(tree.get_iterator_by_sum (i));
      TS_ASSERT_EQUALS (treeNode.value, expectedValues[i]);
    }

    {
      size_t count = 0;
      for (const auto& treeNode : tree)
      {
        TS_ASSERT_LESS_THAN(count, nodeCount);
        auto nodeIterator = tree.iterator_to (treeNode);
        auto nodeSum = tree.get_sum_for_iterator (nodeIterator);
        TS_ASSERT_EQUALS (nodeSum, expectedNodeSums[count]);
        count++;
      }
      TS_ASSERT_EQUALS(count, nodeCount)
    }
  }

  void testCustomAddend2 (void)
  {
    typedef orderstat_tree::intrusive::sum_base_hook<size_t> sum_hook;
    struct Node :
      public boost::intrusive::set_base_hook<>,
      public sum_hook
    {
      size_t value = 0;
      size_t addend = 0;

      bool operator< (const Node& other) const
      { return value < other.value; }
    };
    typedef orderstat_tree::intrusive::rbtree<Node,
                                              orderstat_tree::intrusive::sum_base<sum_hook>,
                                              orderstat_tree::intrusive::addend_member<Node, size_t, &Node::addend>> tree_type;
    
    std::vector<Node> nodeStorage;
    tree_type tree;
    static const size_t nodeCount = 10;
    static const size_t nodeAddends[nodeCount] = { 3, 1, 2, 1, 4, 10, 5, 2, 2, 1 };

    static const size_t expectedNodeSums[nodeCount] = { 0, 3, 4, 6, 7, 11, 21, 26, 28, 30 };
    static const size_t expectedRootSum = 31;
    static const size_t expectedValues[expectedRootSum] =
    {
      0, 0, 0,
      1,
      2, 2,
      3,
      4, 4, 4, 4,
      5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
      6, 6, 6, 6, 6,
      7, 7,
      8, 8,
      9
    };

    nodeStorage.reserve (nodeCount);
    for (size_t i = 0; i < nodeCount; i++)
    {
      nodeStorage.emplace_back ();
      auto& newNode = nodeStorage.back ();
      newNode.value = i;
      newNode.addend = nodeAddends[i];

      tree.push_back (newNode);
    }

    TS_ASSERT_EQUALS (tree.root_sum(), expectedRootSum);

    for (size_t i = 0; i < boost::size (expectedValues); i++)
    {
      const auto& treeNode = *(tree.get_iterator_by_sum (i));
      TS_ASSERT_EQUALS (treeNode.value, expectedValues[i]);
    }

    {
      size_t count = 0;
      for (const auto& treeNode : tree)
      {
        TS_ASSERT_LESS_THAN(count, nodeCount);
        auto nodeIterator = tree.iterator_to (treeNode);
        auto nodeSum = tree.get_sum_for_iterator (nodeIterator);
        TS_ASSERT_EQUALS (nodeSum, expectedNodeSums[count]);
        count++;
      }
      TS_ASSERT_EQUALS(count, nodeCount)
    }
  }

  void testCustomAddend2_unsorted (void)
  {
    typedef orderstat_tree::intrusive::sum_base_hook<size_t> sum_hook;
    struct Node :
      public boost::intrusive::set_base_hook<>,
      public sum_hook
    {
      size_t addend = 0;
    };
    typedef orderstat_tree::intrusive::rbtree<Node,
      orderstat_tree::intrusive::sum_base<sum_hook>,
      orderstat_tree::intrusive::addend_member<Node, size_t, &Node::addend>> tree_type;

    std::vector<Node> nodeStorage;
    tree_type tree;
    static const size_t nodeCount = 10;
    static const size_t nodeAddends[nodeCount] = { 3, 1, 2, 1, 4, 10, 5, 2, 2, 1 };

    static const size_t expectedNodeSums[nodeCount] = { 0, 3, 4, 6, 7, 11, 21, 26, 28, 30 };
    static const size_t expectedRootSum = 31;

    nodeStorage.reserve (nodeCount);
    for (size_t i = 0; i < nodeCount; i++)
    {
      nodeStorage.emplace_back ();
      auto& newNode = nodeStorage.back ();
      newNode.addend = nodeAddends[i];

      tree.push_back (newNode);
    }

    TS_ASSERT_EQUALS (tree.root_sum(), expectedRootSum);

    {
      size_t count = 0;
      for (const auto& treeNode : tree)
      {
        TS_ASSERT_LESS_THAN(count, nodeCount);
        auto nodeIterator = tree.iterator_to (treeNode);
        auto nodeSum = tree.get_sum_for_iterator (nodeIterator);
        TS_ASSERT_EQUALS (nodeSum, expectedNodeSums[count]);
        count++;
      }
      TS_ASSERT_EQUALS(count, nodeCount)
    }
  }

  void testCustomAddendChanged (void)
  {
    typedef orderstat_tree::intrusive::sum_base_hook<size_t> sum_hook;
    struct Node :
      public boost::intrusive::set_base_hook<>,
      public sum_hook
    {
      size_t value = 0;
      size_t addend = 0;

      bool operator< (const Node& other) const
      { return value < other.value; }
    };
    typedef orderstat_tree::intrusive::rbtree<Node,
                                              orderstat_tree::intrusive::sum_base<sum_hook>,
                                              orderstat_tree::intrusive::addend_member<Node, size_t, &Node::addend>> tree_type;

    std::vector<Node> nodeStorage;
    tree_type tree;
    static const size_t nodeCount = 5;

    nodeStorage.reserve (nodeCount);
    for (size_t i = 0; i < nodeCount; i++)
    {
      nodeStorage.emplace_back ();
      auto& newNode = nodeStorage.back ();
      newNode.value = i ;

      tree.push_back (newNode);
    }

    TS_ASSERT_EQUALS (tree.root_sum(), 0);

    static const size_t nodeAddends[nodeCount] = { 3, 1, 2, 1, 4 };

    static const size_t expectedNodeSums[nodeCount] = { 0, 3, 4, 6, 7 };
    static const size_t expectedRootSum = 11;
    static const size_t expectedValues[expectedRootSum] = { 0, 0, 0, 1, 2, 2, 3, 4, 4, 4, 4 };

    for (size_t i = 0; i < nodeCount; i++)
    {
      auto& node = nodeStorage[i];
      node.addend = nodeAddends[i];
      tree.update_sums (tree.iterator_to (node));
    }

    TS_ASSERT_EQUALS (tree.root_sum(), expectedRootSum);

    for (size_t i = 0; i < boost::size (expectedValues); i++)
    {
      const auto& treeNode = *(tree.get_iterator_by_sum (i));
      TS_ASSERT_EQUALS (treeNode.value, expectedValues[i]);
    }

    {
      size_t count = 0;
      for (const auto& treeNode : tree)
      {
        TS_ASSERT_LESS_THAN(count, nodeCount);
        auto nodeIterator = tree.iterator_to (treeNode);
        auto nodeSum = tree.get_sum_for_iterator (nodeIterator);
        TS_ASSERT_EQUALS (nodeSum, expectedNodeSums[count]);
        count++;
      }
      TS_ASSERT_EQUALS(count, nodeCount)
    }
  }

};
