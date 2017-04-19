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
 * Binary tree providing in-order position of elements as well as keeping
 * track of the 'sum' of a node's children
 */
#ifndef SUMTREE_H
#define SUMTREE_H

#include <assert.h>

#include <limits>

#include <boost/call_traits.hpp>
#include <boost/compressed_pair.hpp>
#include <boost/intrusive/parent_from_member.hpp>
#include <boost/intrusive/rbtree_algorithms.hpp>

namespace sum_tree
{
  /// Node without data. All instances are equal.
  struct EmptyNodeData
  {
  };

  namespace value_traits
  {
    struct sorted
    {
      template<typename T>
      static bool is_equal (T&& a, T&& b) { return a == b; }
      template<typename T>
      static bool is_not_equal (T&& a, T&& b) { return a != b; }
      template<typename T>
      static bool is_less (T&& a, T&& b) { return a < b; }
      template<typename T>
      static bool is_less_equal (T&& a, T&& b) { return a <= b; }
      template<typename T>
      static bool is_greater (T&& a, T&& b) { return a > b; }
      template<typename T>
      static bool is_greater_equal (T&& a, T&& b) { return a >= b; }
    };

    struct unsorted
    {
      template<typename T>
      static bool is_equal (T&&, T&&) { return true; }
      template<typename T>
      static bool is_not_equal (T&&, T&&) { return false; }
      template<typename T>
      static bool is_less (T&&, T&&) { return false; }
      template<typename T>
      static bool is_less_equal (T&&, T&&) { return true; }
      template<typename T>
      static bool is_greater (T&&, T&&) { return false; }
      template<typename T>
      static bool is_greater_equal (T&&, T&&) { return true; }
    };

    template<typename T> struct default_traits : public sorted { };

    template<> struct default_traits<EmptyNodeData> : public unsorted { };
    
  } // namespace value_traits

  namespace sum_traits
  {
    /// Node sum providing an index (traversal order)
    template<typename IndexType = int>
    class node_index
    {
    public:
      typedef IndexType value_type;
      struct node_data {};

      static value_type get_node_value (const node_data&) { return 1; } 
      static void set_node_value (node_data&) {}
    };

    /// User-specified per node value
    template<typename T = int>
    class user_value
    {
    public:
      typedef T value_type;
      struct node_data
      {
        T value;

        node_data () : value () {}
        node_data (T v) : value (v) {}
      };

      static value_type get_node_value (const node_data& data) { return data.value; } 
      static void set_node_value (node_data& data, T value) { data.value = value; }
    };
  } // namespace sum_traits

  namespace impl
  {
    template<typename T, bool = std::is_class<T>::value> struct NodeDataTypeTraits;

    template<typename T>
    struct NodeDataTypeTraits<T, true>
    {
      typedef T struct_type;
      typedef T& return_type;
      typedef const T& arg_type;

      static return_type get_data (struct_type& x) { return x; }
      static const return_type get_data (const struct_type& x) { return x; }
      static void set_data (struct_type& x, arg_type value) { x = value; }
    };

    template<typename T>
    struct NodeDataTypeTraits<T, false>
    {
      struct struct_type
      {
        T v;

        struct_type () : v () {}
        struct_type (T v) : v (v) {}
      };
      typedef T return_type;
      typedef T arg_type;

      static return_type get_data (struct_type& x) { return x.v; }
      static const return_type get_data (const struct_type& x) { return x.v; }
      static void set_data (struct_type& x, arg_type value) { x.v = value; }
    };

    template<typename NodeData, typename SumTraits>
    class RBTNodeTraitsImpl
    {
      /// Container for per-node value and sum
      struct SumAndValue : public SumTraits::node_data
      {
        typename SumTraits::value_type sum_value;

        SumAndValue() : sum_value () {}
        template<typename... T>
        SumAndValue (T&&... u)
          : SumTraits::node_data (std::forward<T> (u)...), sum_value (SumTraits::get_node_value (*this)) {}
      };
    public:
      enum struct color : uint8_t { Black, Red };
      struct node_links;
      typedef node_links* node_ptr;
      typedef const node_links* const_node_ptr;

      struct node_links
      {
        node_ptr parent = nullptr;
        node_ptr left = nullptr;
        node_ptr right = nullptr;
        color node_color = color::Black;
        bool header = false; // TODO?: could use bstree_algorithms_base<>::is_header() instead...

        node_links () {}
        node_links (bool header) : header (header) {}
      };

      typedef NodeDataTypeTraits<NodeData> node_data_type_traits;
      typedef typename node_data_type_traits::struct_type node_data_struct;
      struct node : public node_data_struct
      {
        node_links _rb_links;

        template<typename... T>
        node (T&&... data) : node_data_struct (std::forward<T> (data)...), _tree_sum_and_val (_default_sum_and_val()) {}

        typename node_data_type_traits::return_type node_data () { return node_data_type_traits::get_data (*this); }
        const typename node_data_type_traits::return_type node_data () const { return node_data_type_traits::get_data (*this); }

        typename SumTraits::value_type get_sum() const { return _tree_sum_and_val.sum_value; }
        void set_sum (typename SumTraits::value_type&& sum) { _tree_sum_and_val.sum_value = std::move (sum); }

        typename SumTraits::value_type get_node_value() const { return SumTraits::get_node_value (_tree_sum_and_val); }

        void set_node_value (typename SumTraits::value_type value)
        {
          SumTraits::set_node_value (_tree_sum_and_val, value);
        }
      private:
        SumAndValue _tree_sum_and_val;

        static SumAndValue _default_sum_and_val()
        {
          return SumAndValue (typename SumTraits::node_data ());
        }
      };

      static node* node_from_links (node_ptr links)
      {
        assert(!links->header);
        return boost::intrusive::get_parent_from_member (links, &node::_rb_links);
      }
      static const node* node_from_links (const_node_ptr links)
      {
        assert(!links->header);
        return boost::intrusive::get_parent_from_member (links, &node::_rb_links);
      }

      static node_ptr get_parent (const_node_ptr n) { return n->parent; }
      static void set_parent (node_ptr n, node_ptr parent) { n->parent = parent; }
      static node_ptr get_left (const_node_ptr n) { return n->left; }
      static void set_left (node_ptr n, node_ptr left) { n->left = left; }
      static node_ptr get_right (const_node_ptr n) { return n->right; }
      static void set_right (node_ptr n, node_ptr right) { n->right = right; }
      static color get_color (const_node_ptr n) { return n->node_color; }
      static void set_color (node_ptr n, color c) { n->node_color = c; }
      static color black () { return color::Black; }
      static color red () { return color::Red; }

      /// Update sums of node and ancestors
      static void update_sums (node_ptr n)
      {
        auto node_data = node_from_links (n);

        typename SumTraits::value_type new_sum = node_data->get_node_value ();
        if (n->left) new_sum += node_from_links (n->left)->get_sum();
        if (n->right) new_sum += node_from_links (n->right)->get_sum();
        node_data->set_sum (std::move (new_sum));
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
        while (n && !n->header)
        {
          function (n);
          n = n->parent;
        }
      }
    };

    // Need two classes for the same traits to 'inject' logic into bstree_algorithms<>
    template<typename N, typename S>
    class RBTNodeTraits : public RBTNodeTraitsImpl<N, S> {};
  } // namespace impl
} // namespace sum_tree

namespace boost
{
  namespace intrusive
  {
    /* Specialize bstree_algorithms<> so we can 'inject' additional logic for specific
     * operations */
    template<typename N, typename S>
    class bstree_algorithms<sum_tree::impl::RBTNodeTraits<N, S>>
      : public bstree_algorithms<sum_tree::impl::RBTNodeTraitsImpl<N, S>>
    {
      typedef bstree_algorithms<sum_tree::impl::RBTNodeTraitsImpl<N, S>> base_algo;
      typedef typename base_algo::node_ptr node_ptr;
      typedef typename base_algo::node_traits node_traits;
    public:
      static void rotate_left_no_parent_fix(const node_ptr& p, const node_ptr& p_right)
      {
        base_algo::rotate_left_no_parent_fix (p, p_right);
        
        // Sums update
        node_traits::update_sums (p);
        node_traits::update_sums (p_right);
      }

      static void rotate_left(const node_ptr& p, const node_ptr& p_right, const node_ptr& p_parent, const node_ptr& header)
      {
        base_algo::rotate_left (p, p_right, p_parent, header);

        // Sums update
        node_traits::update_sums (p);
        node_traits::update_sums (p_right);
        node_traits::update_ancestry_sums (p_parent);
      }

      static void rotate_right_no_parent_fix(const node_ptr& p, const node_ptr& p_left)
      {
        base_algo::rotate_right_no_parent_fix (p, p_left);
        
        // Sums update
        node_traits::update_sums (p);
        node_traits::update_sums (p_left);
      }

      static void rotate_right(const node_ptr& p, const node_ptr& p_left, const node_ptr& p_parent, const node_ptr& header)
      {
        base_algo::rotate_right (p, p_left, p_parent, header);

        // Sums update
        node_traits::update_sums (p);
        node_traits::update_sums (p_left);
        node_traits::update_ancestry_sums (p_parent);
      }

      static node_ptr insert_before (const node_ptr& header, const node_ptr& pos, const node_ptr& new_node)
      {
        auto n = base_algo::insert_before (header, pos, new_node);

        node_traits::update_ancestry_sums (new_node);
        return n;
      }
      static void push_back (const node_ptr& header, const node_ptr& new_node)
      {
        base_algo::push_back (header, new_node);

        node_traits::update_ancestry_sums (new_node);
      }

      static void erase (const node_ptr& header, const node_ptr& z,
                         typename base_algo::data_for_rebalance& info)
      {
        base_algo::erase (header, z, info);

        if (info.x)
          node_traits::update_ancestry_sums (info.x);
        else if (info.x_parent)
          node_traits::update_ancestry_sums (info.x_parent);
      }
    };
  } // namespace intrusive
} // namespace boost

namespace sum_tree
{
  namespace impl
  {
    template<typename NodeData, typename SumTraits>
    class Tree_base
    {
    public:
      typedef impl::RBTNodeTraits<NodeData, SumTraits> node_traits;
      typedef boost::intrusive::rbtree_algorithms<node_traits> tree_algo;

      Tree_base() : _header (true)
      {
        tree_algo::init_header (get_header());
      }

      typename node_traits::const_node_ptr get_header() const
      {
        return reinterpret_cast<typename node_traits::const_node_ptr> (&_header);
      }
      typename node_traits::node_ptr get_header()
      {
        return reinterpret_cast<typename node_traits::node_ptr> (&_header);
      }
    private:
      // NOTE: Must be first member!
      typename node_traits::node_links _header;
    };
  } // namespace impl

  template<typename NodeData = EmptyNodeData,
           typename SumTraits = sum_traits::node_index<int>,
           typename ValueTraits = value_traits::default_traits<NodeData>>
  class Tree : protected impl::Tree_base<NodeData, SumTraits>
  {
    typedef impl::Tree_base<NodeData, SumTraits> Tree_base;
    typedef impl::RBTNodeTraits<NodeData, SumTraits> node_traits;
    typedef boost::intrusive::rbtree_algorithms<node_traits> tree_algo;

    typedef typename impl::NodeDataTypeTraits<NodeData>::return_type node_data_return_type;
  public:
    typedef typename node_traits::node_ptr node_type;
    typedef typename node_traits::const_node_ptr const_node_type;

    bool empty () const { return tree_algo::begin_node (this->get_header ()) == tree_algo::end_node (this->get_header ()); }
    typename SumTraits::value_type root_sum() const
    {
      auto root = tree_algo::root_node (this->get_header ());
      if (root == this->get_header()) return 0;
      return tree_sum (root);
    }

    //@{
    /// Extract node data from a node
    static const node_data_return_type get_node_data (const_node_type node)
    {
      return node_traits::node_from_links (node)->node_data();
    }
    static node_data_return_type get_node_data (node_type node)
    {
      return node_traits::node_from_links (node)->node_data();
    }
    //@}

    //@{
    /// Get a node from its data
    static const_node_type get_data_node (const NodeData& data)
    {
      return &(static_cast<const typename node_traits::node&> (data)._rb_links);
    }
    static node_type get_data_node (NodeData& data)
    {
      return &(static_cast<typename node_traits::node&> (data)._rb_links);
    }
    //@}

    //@{
    /// Get reference to tree instance from a contained node
    static const Tree& get_tree (const_node_type node)
    {
      auto tree_header = tree_algo::get_header (node);
      auto tree_base = reinterpret_cast<const Tree_base*> (tree_header);
      return *(static_cast<const Tree*> (tree_base));
    }
    static Tree& get_tree (node_type node)
    {
      auto tree_header = tree_algo::get_header (node);
      auto tree_base = reinterpret_cast<Tree_base*> (tree_header);
      return *(static_cast<Tree*> (tree_base));
    }
    //@}

    /// Set the value used for summing associated with a node.
    template<typename... T>
    void set_node_value (node_type node, T&&... value)
    {
      node_traits::node_from_links (node)->set_node_value (std::forward<T> (value)...);
      node_traits::update_ancestry_sums (node);
    }

    //@{
    /// Get a node by querying against sum
    node_type get_node_by_sum (typename SumTraits::value_type value)
    {
      auto root = tree_algo::root_node (this->get_header ());
      if (root == this->get_header()) return nullptr;
      if (value >= tree_sum (root)) return nullptr;
      return get_node_by_sum (root, value);
    }
    const_node_type get_node_by_sum (typename SumTraits::value_type value) const
    {
      auto root = tree_algo::root_node (this->get_header ());
      if (root == this->get_header()) return nullptr;
      if (value >= tree_sum (root)) return nullptr;
      return get_node_by_sum (root, value);
    }
    //@}

    //@{
    /// Get sum of all predecessor nodes for a node (doesn't include node!)
    typename SumTraits::value_type get_sum_for_node (const_node_type node) const
    {
      auto value = typename SumTraits::value_type();

      if (auto left = node_traits::get_left (node))
      {
        value = tree_sum (left);
      }

      auto parent = node_traits::get_parent (node);
      while (parent != this->get_header())
      {
        auto p_right = node_traits::get_right (parent);
        if (node == p_right)
        {
          if (auto p_left = node_traits::get_left (parent))
          {
            value += tree_sum (p_left);
          }
          value += node_traits::node_from_links (node)->get_node_value ();
        }

        node = parent;
        parent = node_traits::get_parent (node);
      }

      return value;
    }
    //@}

    void clear ()
    {
      tree_algo::clear_and_dispose (this->get_header (), [=](node_type p){ delete node_traits::node_from_links (p); });
    }

    /**
     * Add a node before \a before.
     * \a data must not be smaller than 'before'.
     */
    template<typename... T>
    node_type emplace (node_type before, T&&... data)
    {
      auto new_node = new typename node_traits::node (std::forward<T> (data)...);
      tree_algo::init (&new_node->_rb_links);
      tree_algo::insert_before (this->get_header(), before, &new_node->_rb_links);
      auto new_node_parent = node_traits::get_parent (&new_node->_rb_links);
      if (!tree_algo::is_header (new_node_parent))
      {
        assert (!ValueTraits::is_less (new_node->node_data(), get_node_data (new_node_parent)));
      }
      return &new_node->_rb_links;
    }
    /**
     * Add a node to the "end" (rightmost child).
     * \a data must not be smaller than the current rightmost child.
     */
    template<typename... T>
    node_type emplace_back (T&&... data)
    {
      auto new_node = new typename node_traits::node (std::forward<T> (data)...);
      tree_algo::init (&new_node->_rb_links);
      tree_algo::push_back (this->get_header(), &new_node->_rb_links);
      auto new_node_parent = node_traits::get_parent (&new_node->_rb_links);
      if (!tree_algo::is_header (new_node_parent))
      {
        assert (!ValueTraits::is_less (new_node->node_data(), get_node_data (new_node_parent)));
      }
      return &new_node->_rb_links;
    }

    /// Erase a node from the tree. Deletes node object as well!
    void erase_node (node_type node)
    {
      tree_algo::erase (this->get_header (), node);
      delete node;
    }

    /**\name Node-based iteration
     * @{ */
    node_type begin_node () const { return tree_algo::begin_node (this->get_header ()); }
    node_type end_node () const { return tree_algo::end_node (this->get_header ()); }

    static node_type next_node (node_type node) { return tree_algo::next_node (node); }
    /** @} */

    /**
     * Transfer a node in a tree or between trees.
     * \returns The node that used to follow \a source before the transfer.
     */
    node_type transfer (node_type before, node_type source)
    {
      auto source_next = next_node (source);

      tree_algo::unlink (source);
      tree_algo::insert_before (this->get_header(), before, source);
      assert (!ValueTraits::is_less (get_node_data (source), get_node_data (before)));

      return source_next;
    }

    class iterator
    {
    protected:
      node_type current_node;
    public:
      iterator (node_type node) : current_node (node) {}
      iterator (const iterator& other) : current_node (other.current_node) {}

      iterator& operator= (const iterator& other)
      {
        current_node = other.node;
        return *this;
      }

      NodeData& operator*() const { return get_node_data (current_node); }
      NodeData* operator->() const { return &(get_node_data (current_node)); }

      node_type node () const { return current_node; }

      bool operator== (const iterator& other) const { return current_node == other.current_node; }
      bool operator!= (const iterator& other) const { return current_node != other.current_node; }
      iterator& operator++()
      {
        current_node = next_node (current_node);
        return *this;
      }
    };
    class const_iterator
    {
    protected:
      node_type current_node;
    public:
      const_iterator (node_type node) : current_node (node) {}
      const_iterator (const const_iterator& other) : current_node (other.current_node) {}
      const_iterator (const iterator& other) : current_node (other.current_node) {}

      const_iterator& operator= (const const_iterator& other)
      {
        current_node = other.node;
        return *this;
      }
      const_iterator& operator= (const iterator& other)
      {
        current_node = other.node;
        return *this;
      }

      const NodeData& operator*() const { return get_node_data (current_node); }
      const NodeData* operator->() const { return &(get_node_data (current_node)); }

      const_node_type node() const { return current_node; }

      bool operator== (const const_iterator& other) const { return current_node == other.current_node; }
      bool operator!= (const const_iterator& other) const { return current_node != other.current_node; }
      const_iterator& operator++()
      {
        current_node = next_node (current_node);
        return *this;
      }
    };

    iterator begin () { return iterator (begin_node()); }
    iterator end() { return iterator (end_node()); }

    const_iterator cbegin() const { return const_iterator (begin_node()); }
    const_iterator cend() const { return const_iterator (end_node()); }

    const_iterator begin() const { return const_iterator (begin_node()); }
    const_iterator end() const { return const_iterator (end_node()); }
  private:
    static typename SumTraits::value_type tree_sum (const_node_type node)
    { return node_traits::node_from_links (node)->get_sum(); }

    static node_type get_node_by_sum (const_node_type node, typename SumTraits::value_type value)
    {
      if (auto left = node_traits::get_left (node))
      { 
        auto left_sum = tree_sum (left);
        if (value < left_sum)
        {
          return get_node_by_sum (left, value);
        }
        value -= left_sum;
      }
      auto node_value = node_traits::node_from_links (node)->get_node_value ();
      if (value < node_value) return const_cast<node_type> (node);
      if (auto right = node_traits::get_right (node))
      {
        return get_node_by_sum (right, value - node_value);
      }
      assert (false);
      return nullptr;
    }
  };
} // namespace sum_tree

#endif // SUMTREE_H
