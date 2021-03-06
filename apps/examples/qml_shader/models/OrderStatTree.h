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

/**\file
 * Wrapper around orderstat_tree::intrusive::tree<> that allocates elements on the heap
 */
#ifndef ORDERSTATTREE_H_
#define ORDERSTATTREE_H_

#include "orderstat_tree/intrusive.h"

template <typename T, typename... Options>
class OrderStatTree : public orderstat_tree::intrusive::rbtree<T, Options...>
{
  typedef orderstat_tree::intrusive::rbtree<T, Options...> tree_type;
public:
  typedef typename tree_type::iterator iterator;
  typedef typename tree_type::const_iterator const_iterator;

  OrderStatTree () {}
  OrderStatTree (const OrderStatTree& other) = delete;
  OrderStatTree (OrderStatTree&& other) : tree_type (std::move (other)) {}
  ~OrderStatTree()
  {
    clear ();
  }

  void clear ()
  {
    tree_type::clear_and_dispose ([=](T* p){ delete p; });
  }

  template<typename... A>
  iterator emplace (const_iterator before, A&&... data)
  {
    auto new_node = new T (std::forward<A> (data)...);
    return tree_type::insert_before (before, *new_node);
  }
  template<typename... A>
  iterator emplace_back (A&&... data)
  {
    auto new_node = new T (std::forward<A> (data)...);
    tree_type::push_back (*new_node);
    return tree_type::iterator_to (*new_node);
  }

  static OrderStatTree& container_from_iterator (iterator it)
  { return static_cast<OrderStatTree&> (tree_type::container_from_iterator (it)); }
  static const OrderStatTree& container_from_iterator (const_iterator it)
  { return static_cast<const OrderStatTree&> (tree_type::container_from_iterator (it)); }
};

#endif // ORDERSTATTREE_H_
