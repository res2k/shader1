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
 * Intrusive Order Statistic Tree implementation.
 * Depending on it's configuration it can provide in-order position of elements
 * or keeping track of the 'sum' of some values associated with the children of
 * a node.
 */
#ifndef ORDER_STAT_TREE_INTRUSIVE_H_
#define ORDER_STAT_TREE_INTRUSIVE_H_

#include <assert.h>

#include <limits>

#include <boost/intrusive/intrusive_fwd.hpp>

#include <boost/intrusive/detail/hook_traits.hpp>
#include <boost/intrusive/pack_options.hpp>
#include <boost/intrusive/rbtree.hpp>

namespace orderstat_tree
{
  namespace intrusive_impl
  {
    template<typename NodeTraits, typename SumTraits, typename AddendTraits>
    class sum_tree_algo_base
    {
    public:
      typedef typename NodeTraits::node_ptr node_ptr;
      typedef typename NodeTraits::const_node_ptr const_node_ptr;
      typedef typename SumTraits::value_type sum_type;
      typedef typename AddendTraits::value_type addend_type;

      /// Update sums of node
      static void update_sums (node_ptr n)
      {
        sum_type new_sum = AddendTraits::get_value (n);

        auto left = NodeTraits::get_left (n);
        if (left) new_sum += SumTraits::get_sum (left);
        auto right = NodeTraits::get_right (n);
        if (right) new_sum += SumTraits::get_sum (right);
        SumTraits::set_sum (n, std::move (new_sum));
      }
      /// Update sums of node and ancestors
      static void update_ancestry_sums (node_ptr n)
      {
        walk_ancestors (n, [](node_ptr p){ update_sums (p); });
      }
    protected:
      /// Run a function on node \a n and each ancestor
      template<typename F>
      static void walk_ancestors (node_ptr n, F function)
      {
        while (n && !boost::intrusive::bstree_algorithms_base<NodeTraits>::is_header (n))
        {
          function (n);
          n = NodeTraits::get_parent (n);
        }
      }
    };

    // Need two classes for the same traits to 'inject' logic into bstree_algorithms<>
    template<typename N, typename S, typename A>
    class wrap_node_traits : public N
    {
    };

    /// Aggregate various traits into a single struct
    template<typename V, typename S, typename A>
    struct combined_value_traits
    {
      typedef V orig_value_traits;
      typedef S orig_sum_traits;
      typedef A orig_addend_traits;

      typedef typename V::node_traits orig_node_traits;

      typedef wrap_node_traits<orig_node_traits, S, A> node_traits;

      typedef typename orig_value_traits::pointer         pointer;
      typedef typename orig_value_traits::const_pointer   const_pointer;
      typedef typename orig_value_traits::node_ptr        node_ptr;
      typedef typename orig_value_traits::const_node_ptr  const_node_ptr;
      typedef typename orig_value_traits::reference       reference;
      typedef typename orig_value_traits::value_type      value_type;
      static const boost::intrusive::link_mode_type link_mode = orig_value_traits::link_mode;

      static node_ptr to_node_ptr (reference value) { return orig_value_traits::to_node_ptr (value); }
      static pointer to_value_ptr (const node_ptr& n) { return orig_value_traits::to_value_ptr (n); }
      static const_pointer to_value_ptr (const const_node_ptr& n) { return orig_value_traits::to_value_ptr (n); }
    };
  } // namespace intrusive_impl
} // namespace orderstat_tree

namespace boost
{
  namespace intrusive
  {
    /* Specialize bstree_algorithms<> so we can 'inject' additional logic for specific
     * operations */
    template<typename N, typename S, typename A>
    class bstree_algorithms<orderstat_tree::intrusive_impl::wrap_node_traits<N, S, A>>
      : public bstree_algorithms<N>
    {
      typedef bstree_algorithms<N> base_algo;
      typedef typename base_algo::node_ptr node_ptr;
      typedef typename base_algo::node_traits node_traits;
      typedef orderstat_tree::intrusive_impl::sum_tree_algo_base<node_traits, S, A> sum_tree_algo_base;
    public:
      typedef typename base_algo::insert_commit_data insert_commit_data;

      static void rotate_left_no_parent_fix(const node_ptr& p, const node_ptr& p_right)
      {
        base_algo::rotate_left_no_parent_fix (p, p_right);
        
        // Sums update
        sum_tree_algo_base::update_sums (p);
        sum_tree_algo_base::update_sums (p_right);
      }

      static void rotate_left(const node_ptr& p, const node_ptr& p_right, const node_ptr& p_parent, const node_ptr& header)
      {
        base_algo::rotate_left (p, p_right, p_parent, header);

        // Sums update
        sum_tree_algo_base::update_sums (p);
        sum_tree_algo_base::update_sums (p_right);
        sum_tree_algo_base::update_ancestry_sums (p_parent);
      }

      static void rotate_right_no_parent_fix(const node_ptr& p, const node_ptr& p_left)
      {
        base_algo::rotate_right_no_parent_fix (p, p_left);
        
        // Sums update
        sum_tree_algo_base::update_sums (p);
        sum_tree_algo_base::update_sums (p_left);
      }

      static void rotate_right(const node_ptr& p, const node_ptr& p_left, const node_ptr& p_parent, const node_ptr& header)
      {
        base_algo::rotate_right (p, p_left, p_parent, header);

        // Sums update
        sum_tree_algo_base::update_sums (p);
        sum_tree_algo_base::update_sums (p_left);
        sum_tree_algo_base::update_ancestry_sums (p_parent);
      }

      static void insert_unique_commit (const node_ptr& header, const node_ptr& new_value, const insert_commit_data& commit_data)
      {
        base_algo::insert_unique_commit (header, new_value, commit_data);
        sum_tree_algo_base::update_ancestry_sums (new_value);
      }
      template<class NodePtrCompare>
      static node_ptr insert_equal (const node_ptr& h, const node_ptr& hint, const node_ptr& new_node, NodePtrCompare comp)
      {
        auto n = base_algo::insert_equal (h, hint, new_node, comp);
        sum_tree_algo_base::update_ancestry_sums (new_node);
        return n;
      }
      template<class NodePtrCompare>
      static node_ptr insert_equal_upper_bound (const node_ptr& h, const node_ptr& new_node, NodePtrCompare comp)
      {
        auto n = base_algo::insert_equal_upper_bound (h, new_node, comp);
        sum_tree_algo_base::update_ancestry_sums (new_node);
        return n;
      }
      template<class NodePtrCompare>
      static node_ptr insert_equal_lower_bound (const node_ptr& h, const node_ptr& new_node, NodePtrCompare comp)
      {
        auto n = base_algo::insert_equal_lower_bound (h, new_node, comp);
        sum_tree_algo_base::update_ancestry_sums (new_node);
        return n;
      }
      static node_ptr insert_before (const node_ptr& header, const node_ptr& pos, const node_ptr& new_node)
      {
        auto n = base_algo::insert_before (header, pos, new_node);
        sum_tree_algo_base::update_ancestry_sums (new_node);
        return n;
      }
      static void push_back (const node_ptr& header, const node_ptr& new_node)
      {
        base_algo::push_back (header, new_node);
        sum_tree_algo_base::update_ancestry_sums (new_node);
      }

      static void erase (const node_ptr& header, const node_ptr& z,
                         typename base_algo::data_for_rebalance& info)
      {
        base_algo::erase (header, z, info);

        if (info.x)
          sum_tree_algo_base::update_ancestry_sums (info.x);
        else if (info.x_parent)
          sum_tree_algo_base::update_ancestry_sums (info.x_parent);
      }
    };
  } // namespace intrusive
} // namespace boost

namespace orderstat_tree
{
  namespace intrusive
  {
    template<typename NodeTraits,
             typename SumTraits,
             typename AddendTraits,
             template<typename> class BSTreeAlgos>
    class sum_tree_algos : public BSTreeAlgos<intrusive_impl::wrap_node_traits<NodeTraits, SumTraits, AddendTraits>>,
                           public intrusive_impl::sum_tree_algo_base<NodeTraits, SumTraits, AddendTraits>
    {
      typedef typename NodeTraits::node_ptr node_ptr;
      typedef typename NodeTraits::const_node_ptr const_node_ptr;
    public:
      static node_ptr get_node_by_sum (const_node_ptr node, typename SumTraits::value_type value)
      {
        assert (!boost::intrusive::bstree_algorithms_base<NodeTraits>::is_header (node));
        if (auto left = NodeTraits::get_left (node))
        {
          auto left_sum = SumTraits::get_sum (left);
          if (value < left_sum)
          {
            return get_node_by_sum (left, value);
          }
          value -= left_sum;
        }
        auto node_value = AddendTraits::get_value (node);
        if (value < node_value) return const_cast<node_ptr> (node);
        if (auto right = NodeTraits::get_right (node))
        {
          return get_node_by_sum (right, value - node_value);
        }
        assert (false);
        return nullptr;
      }

      /// Get sum of all predecessor nodes for a node (doesn't include node!)
      static typename SumTraits::value_type get_sum_for_node (const_node_ptr node)
      {
        assert (!boost::intrusive::bstree_algorithms_base<NodeTraits>::is_header (node));
        auto value = typename SumTraits::value_type ();

        if (auto left = NodeTraits::get_left (node))
        {
          value = SumTraits::get_sum (left);
        }

        auto parent = NodeTraits::get_parent (node);
        while (!boost::intrusive::bstree_algorithms_base<NodeTraits>::is_header (parent))
        {
          auto p_right = NodeTraits::get_right (parent);
          if (node == p_right)
          {
            if (auto p_left = NodeTraits::get_left (parent))
            {
              value += SumTraits::get_sum (p_left);
            }
            value += AddendTraits::get_value (parent);
          }

          node = parent;
          parent = NodeTraits::get_parent (node);
        }

        return value;
      }
      //@}
    };

    namespace impl
    {
      template<typename T, typename... Tag>
      struct base_hook
      {
        typedef T value_type;
        value_type value_;

        static value_type get (const base_hook& data) { return data.value_; }
        static void set (base_hook& data, value_type&& x) { data.value_ = std::move (x); }
      };

      struct sum_tag;
      struct addend_tag;

      template<typename Parent, typename MemberType, MemberType Parent::* PtrToMember>
      struct member_sum_traits
      {
        typedef MemberType    value_type;

        static value_type get_sum (const Parent& data)
        { return (&data)->*PtrToMember; }
        static void set_sum (Parent& data, value_type&& x)
        { (&data)->*PtrToMember = std::move (x); }
      };

      template<typename Parent, typename MemberType, MemberType Parent::* PtrToMember>
      struct member_addend_traits
      {
        typedef MemberType    value_type;

        static value_type get_value (const Parent& data)
        { return (&data)->*PtrToMember; }
        static void set_value (Parent& data, value_type&& x)
        { (&data)->*PtrToMember = std::move (x); }
      };

      template<typename T, T AddendValue>
      struct const_addend_traits
      {
        typedef T value_type;

        template<typename X>
        static value_type get_value (const X&) { return AddendValue; }
      };

      struct default_options : public boost::intrusive::rbtree_defaults { };

    } // namespace impl

    template<typename T>
    struct sum_base_hook : protected impl::base_hook<T, impl::sum_tag>
    {
      typedef impl::base_hook<T, impl::sum_tag> base_hook_type;
      typedef T value_type;

      template<typename X>
      static value_type get_sum (const X& data) { return base_hook_type::get (static_cast<const base_hook_type&> (data)); }
      template<typename X>
      static void set_sum (X& data, value_type&& x) { base_hook_type::set (static_cast<base_hook_type&> (data), std::move (x)); }
    };

    template<typename T>
    struct addend_base_hook : protected impl::base_hook<T, impl::addend_tag>
    {
      typedef impl::base_hook<T, impl::sum_tag> base_hook_type;
      typedef T value_type;

      template<typename X>
      static value_type get_value (const X& data) { return base_hook_type::get (static_cast<const base_hook_type&> (data)); }
      template<typename X>
      static void set_value (X& data, value_type&& x) { base_hook_type::set (static_cast<base_hook_type&> (data), std::move (x)); }
    };
    
    /// Option: Store per-node sum in a member
    template<typename Parent, typename MemberType, MemberType Parent::* PtrToMember>
    struct sum_member
    {
      typedef impl::member_sum_traits<Parent, MemberType, PtrToMember> member_value_traits;
      template<class Base>
      struct pack : Base
      {
          typedef member_value_traits sum_traits;
      };
    };
    /// Option: Store per-node sum in a base class
    BOOST_INTRUSIVE_OPTION_TYPE(sum_base, SumTraits, SumTraits, sum_traits)
    /// Option: Custom per-sum traits
    //BOOST_INTRUSIVE_OPTION_TYPE(sum_traits, SumTraits, SumTraits, sum_traits)

    /// Option: Store per-node addend in a member
    template<typename Parent, typename MemberType, MemberType Parent::* PtrToMember>
    struct addend_member
    {
      typedef impl::member_addend_traits<Parent, MemberType, PtrToMember> member_addend_traits;
      template<class Base>
      struct pack : Base
      {
          typedef member_addend_traits addend_traits;
      };
    };
    /// Option: Store per-node addend in a base class
    BOOST_INTRUSIVE_OPTION_TYPE(addend_base, AddendTraits, AddendTraits, addend_traits)
    /// Option: Custom addend traits
    //BOOST_INTRUSIVE_OPTION_TYPE(addend_traits, AddendTraits, AddendTraits, addend_traits)
    /// Option: Constant addend
    template<typename T, T AddendValue>
    struct addend_const
    {
      template<class Base>
      struct pack : Base
      {
          typedef impl::const_addend_traits<T, AddendValue> addend_traits;
      };
    };

    template<typename IndexType = int>
    struct addend_index : public addend_const<IndexType, 1> {};

    template<class CombinedValueTraits, class VoidOrKeyOfValue, class VoidOrKeyComp, class SizeType, bool ConstantTimeSize, typename HeaderHolder>
    class rbtree_impl
      : public boost::intrusive::rbtree_impl<CombinedValueTraits, VoidOrKeyOfValue, VoidOrKeyComp, SizeType, ConstantTimeSize, HeaderHolder>
    {
      typedef typename CombinedValueTraits::orig_value_traits value_traits;
      typedef typename CombinedValueTraits::orig_sum_traits sum_traits;
      typedef intrusive::sum_tree_algos<typename CombinedValueTraits::orig_node_traits,
                                        sum_traits,
                                        typename CombinedValueTraits::orig_addend_traits,
                                        boost::intrusive::rbtree_algorithms> tree_algo;
      typedef typename boost::intrusive::rbtree_impl<CombinedValueTraits, VoidOrKeyOfValue, VoidOrKeyComp, SizeType, ConstantTimeSize, HeaderHolder> base_tree_impl;
    public:
      typedef typename base_tree_impl::iterator iterator;
      typedef typename base_tree_impl::const_iterator const_iterator;

      typedef typename sum_traits::value_type sum_type;

      rbtree_impl () {}
      rbtree_impl (const rbtree_impl& other) = delete;
      rbtree_impl (rbtree_impl&& other) : base_tree_impl (std::move (other)) {}

      sum_type root_sum() const
      {
        auto root_it = this->root ();
        assert (root_it != this->end());
        return sum_traits::get_sum (value_traits::to_node_ptr (*root_it));
      }
      iterator get_iterator_by_sum (sum_type value)
      {
        auto root_it = this->root ();
        if (root_it == this->end ()) return this->end ();
        auto root_node = value_traits::to_node_ptr (*root_it);
        if (value >= sum_traits::get_sum (root_node)) return this->end ();
        auto node = tree_algo::get_node_by_sum (root_node, value);
        return this->iterator_to (*(value_traits::to_value_ptr (node)));
      }
      const_iterator get_iterator_by_sum (sum_type value) const
      {
        auto root_it = this->root ();
        if (root_it == this->end ()) return this->end ();
        auto root_node = value_traits::to_node_ptr (*root_it);
        if (value >= sum_traits::get_sum (root_node)) return this->end ();
        auto node = tree_algo::get_node_by_sum (root_node, value);
        return this->iterator_to (*(value_traits::to_value_ptr (node)));
      }
      /// Get sum of all predecessors for an element (doesn't include element!)
      sum_type get_sum_for_iterator (const_iterator it) const
      {
        auto node = value_traits::to_node_ptr (*it);
        return tree_algo::get_sum_for_node (node);
      }

      /// Update sums after an addend was changed
      void update_sums (iterator it)
      {
        auto node = value_traits::to_node_ptr (*it);
        tree_algo::update_ancestry_sums (node);
      }

      /**
       * Transfer a node in a tree or between trees.
       * \returns The node that used to follow \a source before the transfer.
       */
      iterator transfer (iterator before, iterator source)
      {
        auto& source_tree = this->container_from_iterator (source);

        auto& source_data = *source;
        auto source_next = source_tree.erase (source);

        auto& dest_tree = this->container_from_iterator (before);
        dest_tree.insert_before (before, source_data);

        return source_next;
      }
    };

    template<typename T,
             typename ...Options>
    struct make_rbtree
    {
      // 'Pack' options
      typedef typename boost::intrusive::pack_options<impl::default_options, Options...>::type packed_options;

      typedef typename boost::intrusive::detail::get_value_traits<T, typename packed_options::proto_value_traits>::type value_traits;
      typedef typename packed_options::sum_traits sum_traits;
      typedef typename packed_options::addend_traits addend_traits;

      /// Wrapper class that calls sum_traits with node value instead of data
      struct WrapSumTraits
      {
        typedef typename sum_traits::value_type value_type;

        static value_type get_sum (typename value_traits::const_node_ptr n)
        { return sum_traits::get_sum (*(value_traits::to_value_ptr (n))); }
        static void set_sum (typename value_traits::node_ptr n, value_type&& x)
        { return sum_traits::set_sum (*(value_traits::to_value_ptr (n)), std::move (x)); }
      };
      /// Wrapper class that calls addend_traits with node value instead of data
      struct WrapAddendTraits
      {
        typedef typename addend_traits::value_type value_type;

        static value_type get_value (typename value_traits::const_node_ptr n)
        { return addend_traits::get_value (*(value_traits::to_value_ptr (n))); }
      };

      typedef intrusive_impl::combined_value_traits<value_traits, WrapSumTraits, WrapAddendTraits> combined_value_traits;

      typedef rbtree_impl<combined_value_traits,
                          typename packed_options::key_of_value,
                          typename packed_options::compare,
                          typename packed_options::size_type,
                          packed_options::constant_time_size,
                          typename packed_options::header_holder_type> type;
    };

    template<typename T,
             typename ...Options>
    class rbtree : public make_rbtree<T, Options...>::type
    {
      typedef typename make_rbtree<T, Options...>::type Base;
    public:
      typedef typename Base::iterator           iterator;
      typedef typename Base::const_iterator     const_iterator;

      rbtree () {}
      rbtree (const rbtree& other) = delete;
      rbtree (rbtree&& other) : Base (std::move (other)) {}

      static rbtree& container_from_end_iterator (iterator it)
      { return static_cast<rbtree&> (Base::container_from_end_iterator (it)); }
      static const rbtree& container_from_end_iterator (const_iterator it)
      { return static_cast<const rbtree&> (Base::container_from_end_iterator (it)); }
      static rbtree& container_from_iterator (iterator it)
      { return static_cast<rbtree&> (Base::container_from_iterator (it)); }
      static const rbtree& container_from_iterator (const_iterator it)
      { return static_cast<const rbtree&> (Base::container_from_iterator (it)); }
    };
  } // namespace intrusive
} // namespace orderstat_tree

#endif // ORDER_STAT_TREE_INTRUSIVE_H_
