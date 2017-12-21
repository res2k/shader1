/*
    Shader1
    Copyright (c) 2017 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

/**\file
 * Class to store multiple optional values
 */
#ifndef BASE_MULTIOPTIONAL_H_
#define BASE_MULTIOPTIONAL_H_

#include <bitset>

#include <boost/mpl/at.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector.hpp>

namespace s1
{
  namespace multi_optional_detail
  {
    template<typename T>
    struct Storage
    {
      uint8_t data alignas(T)[sizeof(T)];
    };
  } // multi_optional_detail

  typedef boost::bad_optional_access bad_optional_access;
  typedef boost::none_t none_t;

  const none_t none = boost::none;

  /**
   * Class to store multiple optional values.
   * This is more space-efficient than having multiple optional<> members
   * in sequence.
   */
  template<typename... T>
  class MultiOptional
  {
  public:
    typedef boost::mpl::vector<T...> types;
    MultiOptional () {}
    template<typename... U, typename = std::enable_if<sizeof...(T) == sizeof...(U)>>
    MultiOptional (const MultiOptional<U...>& other) { assign_from_after<0> (other); }
    template<typename... U, typename = std::enable_if<sizeof...(T) == sizeof...(U)>>
    MultiOptional (MultiOptional<U...>&& other) { assign_from_after<0> (std::move (other)); }
    ~MultiOptional () { destroy_all_after<0> (); }

    template<typename... U, typename = std::enable_if<sizeof...(T) == sizeof...(U)>>
    MultiOptional& operator= (const MultiOptional<U...>& other)
    {
      assign_from_after<0> (other);
      return *this;
    }
    template<typename... U, typename = std::enable_if<sizeof...(T) == sizeof...(U)>>
    MultiOptional& operator= (MultiOptional<U...>&& other)
    {
      assign_from_after<0> (std::move (other));
      return *this;
    }

    /// Check whether optional has a specific value.
    template<size_t I>
    bool has_value() const
    {
      static_assert (I < N, "index out of range");
      return constructed[I];
    }

    //@{
    /// Get value of an element
    template<size_t I> typename boost::mpl::at_c<types, I>::type& value () &
    {
      static_assert (I < N, "index out of range");
      if (!constructed[I]) throw bad_optional_access ();
      return get_ref<I> ();
    }
    template<size_t I> const typename boost::mpl::at_c<types, I>::type& value() const &
    {
      static_assert (I < N, "index out of range");
      if (!constructed[I]) throw bad_optional_access ();
      return get_ref<I> ();
    }
    template<size_t I> typename boost::mpl::at_c<types, I>::type&& value() &&
    {
      static_assert (I < N, "index out of range");
      if (!constructed[I]) throw bad_optional_access ();
      return std::move (get_ref<I> ());
    }
    template<size_t I> const typename boost::mpl::at_c<types, I>::type&& value() const &&
    {
      static_assert (I < N, "index out of range");
      if (!constructed[I]) throw bad_optional_access ();
      return std::move (get_ref<I> ());
    }
    //@}

    /// Construct an element in-place
    template<size_t I, typename... U>
    void emplace (U&&... args)
    {
      typedef typename boost::mpl::at_c<types, I>::type element_type;
      auto element_data = &(std::get<I> (storage).data);
      if (constructed[I])
      {
        reinterpret_cast<element_type*> (element_data)->~element_type ();
      }
      new (element_data) element_type (std::forward<U> (args)...);
      constructed[I] = true;
    }

    /// Reset an element
    template<size_t I>
    void reset ()
    {
      typedef typename boost::mpl::at_c<types, I>::type element_type;
      if (constructed[I])
      {
        get_ref<I>().~element_type ();
        constructed[I] = false;
      }
    }

    /// Element proxy class
    template<size_t I, typename U>
    class ElementProxy
    {
      friend class MultiOptional;
      MultiOptional& owner;

      ElementProxy (MultiOptional& owner) : owner (owner) {}
    public:
      ~ElementProxy() {}

      ElementProxy& operator= (const U& value)
      {
        owner.template assign<I> (std::forward<U> (value));
        return *this;
      }
      ElementProxy& operator= (U&& value)
      {
        owner.template assign<I> (std::forward<U> (value));
        return *this;
      }
      ElementProxy& operator= (none_t)
      {
        owner.template reset<I> ();
        return *this;
      }

      U& operator*() const { return owner.template get_ref<I> (); }

      bool has_value() const { return owner.template has_value<I> (); }
      U& value() const { return owner.template value<I> (); }

      template<typename... V>
      void emplace (V&&... args) { owner.template emplace<I> (std::forward<V> (args)...); }

      void reset () { owner.template reset<I> (); }
    };

    /// Const element proxy class
    template<size_t I, typename U>
    class ElementConstProxy
    {
      friend class MultiOptional;
      const MultiOptional& owner;

      ElementConstProxy (const MultiOptional& owner) : owner (owner) {}
    public:
      ~ElementConstProxy() {}

      const U& operator*() const { return owner.template get_ref<I> (); }

      bool has_value() const { return owner.template has_value<I> (); }
      const U& value() const { return owner.template value<I> (); }
    };

    //@{
    /// Get proxy for an element
    template<size_t I>
    ElementProxy<I, typename boost::mpl::at_c<types, I>::type> get ()
    {
      static_assert (I < N, "index out of range");
      typedef ElementProxy<I, typename boost::mpl::at_c<types, I>::type> proxy_type;
      return proxy_type (*this);
    }
    template<size_t I>
    ElementConstProxy<I, typename boost::mpl::at_c<types, I>::type> get () const
    {
      static_assert (I < N, "index out of range");
      typedef ElementConstProxy<I, typename boost::mpl::at_c<types, I>::type> proxy_type;
      return proxy_type (*this);
    }
    //@}
  protected:
    enum { N = boost::mpl::size<types>::value };
    typedef typename boost::mpl::range_c<size_t, 0, N>::type indices_range_type;

    std::bitset<N> constructed;
    std::tuple<multi_optional_detail::Storage<T>...> storage;

    //@{
    /// Obtain reference to an element
    template<size_t I>
    typename boost::mpl::at_c<types, I>::type& get_ref ()
    {
      uint8_t* p = &(std::get<I> (storage).data[0]);
      return *(reinterpret_cast<typename boost::mpl::at_c<types, I>::type *> (p));
    }

    template<size_t I>
    const typename boost::mpl::at_c<types, I>::type& get_ref () const
    {
      const uint8_t* p = &(std::get<I> (storage).data[0]);
      return *(reinterpret_cast<const typename boost::mpl::at_c<types, I>::type *> (p));
    }
    //@}

    /// Assign value to an element
    template<size_t I, typename U>
    void assign (U&& value)
    {
      typedef typename boost::mpl::at_c<types, I>::type element_type;
      auto element_data = &(std::get<I> (storage).data);
      if (constructed[I])
      {
        *(reinterpret_cast<element_type*> (element_data)) = std::forward<U> (value);
      }
      else
      {
        new (element_data) element_type (std::forward<U> (value));
        constructed[I] = true;
      }
    }

    //@{
    /// Copy from other MultiOptional
    template<size_t I, typename... U>
    typename std::enable_if <I < N>::type assign_from_after (const MultiOptional<U...>& other)
    {
      if (other.has_value<I> ())
        assign<I> (other.value<I> ());
      else
        reset<I> ();
      assign_from_after<I + 1> (other);
    }
    template<size_t I, typename... U>
    typename std::enable_if<I == N>::type assign_from_after (const MultiOptional<U...>& other)
    {
    }

    template<size_t I, typename... U>
    typename std::enable_if <I < N>::type assign_from_after (MultiOptional<U...>&& other)
    {
      if (other.has_value<I> ())
        assign<I> (std::move (other.value<I> ()));
      else
        reset<I> ();
      assign_from_after<I + 1> (std::move (other));
    }
    template<size_t I, typename... U>
    typename std::enable_if<I == N>::type assign_from_after (MultiOptional<U...>&& other)
    {
    }
    //@}

    //@{
    /// Destroy multiple elements
    template<size_t I>
    typename std::enable_if<I < N>::type destroy_all_after ()
    {
      typedef typename boost::mpl::at_c<types, I>::type element_type;
      if (constructed[I])
      {
        get_ref<I>().~element_type ();
      }
      destroy_all_after<I + 1> ();
    }
    template<size_t I>
    typename std::enable_if<I == N>::type destroy_all_after ()
    {
    }
    //@}
  };
} // namespace s1

#endif // BASE_MULTIOPTIONAL_H_
