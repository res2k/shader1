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
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/none.hpp>
#include <boost/optional/bad_optional_access.hpp>

namespace s1
{
  namespace multi_optional_detail
  {
    template<typename T, bool IsTrivial = std::is_trivial<T>::value>
    class Storage
    {
      uint8_t data alignas(T)[sizeof(T)];
    public:
      /// Construct (unconditionally)
      template<typename... U>
      void construct (U&&... args)
      {
        new (data) T (std::forward<U> (args)...);
      }
      /// Destroy (unconditionally)
      void destroy ()
      {
        uint8_t* p = &(data[0]);
        reinterpret_cast<T*> (p)->~T();
      }

      //@{
      /// Obtain reference to contained data
      T& get_ref ()
      {
        uint8_t* p = &(data[0]);
        return *(reinterpret_cast<T*> (p));
      }
      const T& get_ref () const
      {
        const uint8_t* p = &(data[0]);
        return *(reinterpret_cast<const T*> (p));
      }
      //@}
    };

    template<typename T>
    class Storage<T, true>
    {
      T data;
    public:
      //@{
      /// Construct (unconditionally)
      void construct ()
      {
        data = T ();
      }
      template<typename U>
      void construct (U&& arg)
      {
        data = std::forward<U> (arg);
      }
      //@}
      /// Destroy (unconditionally)
      void destroy ()
      {
        data.~T();
      }

      //@{
      /// Obtain reference to contained data
      T& get_ref ()
      {
        return data;
      }
      const T& get_ref () const
      {
        return data;
      }
      //@}
    };
  } // multi_optional_detail

  typedef boost::bad_optional_access bad_optional_access;
  typedef boost::none_t nullopt_t;

  const nullopt_t nullopt = boost::none;

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
    MultiOptional (const MultiOptional<U...>& other) { construct_from_after<0> (other); }
    template<typename... U, typename = std::enable_if<sizeof...(T) == sizeof...(U)>>
    MultiOptional (MultiOptional<U...>&& other) { construct_from_after<0> (std::move (other)); }
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
      if (constructed[I])
      {
        std::get<I> (storage).destroy ();
      }
      std::get<I> (storage).construct (std::forward<U> (args)...);
      constructed[I] = true;
    }

    /// Reset an element
    template<size_t I>
    void reset ()
    {
      if (constructed[I])
      {
        std::get<I> (storage).destroy ();
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
      ElementProxy& operator= (nullopt_t)
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

    /// Swap contained values with another instance
    void swap (MultiOptional& right)
    {
      using std::swap;
      swap_all_after<0> (right);
      // Swap bits last
      swap (constructed, right.constructed);
    }

    friend void swap (MultiOptional& left, MultiOptional& right)
    {
      left.swap (right);
    }
  protected:
    template<typename... U> friend class MultiOptional;
    enum { N = boost::mpl::size<types>::value };

    std::bitset<N> constructed;
    std::tuple<multi_optional_detail::Storage<T>...> storage;

    //@{
    /// Obtain reference to an element
    template<size_t I>
    typename boost::mpl::at_c<types, I>::type& get_ref ()
    {
      return std::get<I> (storage).get_ref ();
    }

    template<size_t I>
    const typename boost::mpl::at_c<types, I>::type& get_ref () const
    {
      return std::get<I> (storage).get_ref ();
    }
    //@}

    /// Assign value to an element
    template<size_t I, typename U>
    void assign (U&& value)
    {
      if (constructed[I])
      {
        std::get<I> (storage).get_ref() = std::forward<U> (value);
      }
      else
      {
        std::get<I> (storage).construct (std::forward<U> (value));
        constructed[I] = true;
      }
    }

    //@{
    /// Construct from other MultiOptional
    template<size_t I, typename... U>
    typename std::enable_if <I < N>::type construct_from_after (const MultiOptional<U...>& other)
    {
      assert (!constructed[I]);
      if (other.template has_value<I> ())
      {
        std::get<I> (storage).construct (other.template get_ref<I> ());
        constructed[I] = true;
      }
      construct_from_after<I + 1> (other);
    }
    template<size_t I, typename... U>
    typename std::enable_if<I == N>::type construct_from_after (const MultiOptional<U...>& other)
    {
    }

    template<size_t I, typename... U>
    typename std::enable_if <I < N>::type construct_from_after (MultiOptional<U...>&& other)
    {
      assert (!constructed[I]);
      if (other.template has_value<I> ())
      {
        std::get<I> (storage).construct (std::move (other.template get_ref<I> ()));
        constructed[I] = true;
      }
      construct_from_after<I + 1> (std::move (other));
    }
    template<size_t I, typename... U>
    typename std::enable_if<I == N>::type construct_from_after (MultiOptional<U...>&& other)
    {
    }
    //@}

    //@{
    /// Copy from other MultiOptional
    template<size_t I, typename... U>
    typename std::enable_if <I < N>::type assign_from_after (const MultiOptional<U...>& other)
    {
      if (other.template has_value<I> ())
        assign<I> (other.template get_ref<I> ());
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
      if (other.template has_value<I> ())
        assign<I> (std::move (other.template get_ref<I> ()));
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
    /// Swap contained values with another instance
    template<size_t I>
    typename std::enable_if <I < N>::type swap_all_after (MultiOptional& right)
    {
      using std::swap;
      if (has_value<I> ())
      {
        if (right.template has_value<I> ())
        {
          swap (get_ref<I> (), right.template get_ref<I> ());
        }
        else
        {
          std::get<I> (right.storage).construct (std::move (get_ref<I> ()));
          std::get<I> (storage).destroy ();
        }
      }
      else
      {
        if (right.template has_value<I> ())
        {
          std::get<I> (storage).construct (std::move (right.template get_ref<I> ()));
          std::get<I> (right.storage).destroy ();
        }
        else
        {
          // Nothing to do
        }
      }
      swap_all_after<I + 1> (right);
    }
    template<size_t I>
    typename std::enable_if<I == N>::type swap_all_after (MultiOptional& right)
    {
    }
    //@}

    //@{
    /// Destroy multiple elements
    template<size_t I>
    typename std::enable_if<I < N>::type destroy_all_after ()
    {
      if (constructed[I]) std::get<I> (storage).destroy ();
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
