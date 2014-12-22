/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


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
 * Smart pointer for Shader1 public API objects.
 */
#ifndef __S1_PTR_H__
#define __S1_PTR_H__

#include "s1/Object.h"

#ifdef __cplusplus
namespace s1
{
#if !defined(DOXYGEN_RUN)
  namespace cxxapi
  {
    namespace
    {
      typedef int Yes;
      typedef char No;

      template<typename T>
      struct IsDefinedHelper
      {
        static No tester(...);

        template<typename U>
        static Yes tester(U* p, typename U::Sentinel = typename U::Sentinel());

        enum { Result = sizeof(tester((T*)0)) != sizeof(No) };
      };

      // Call s1_release() for 'unknown' types - let compiler sort it out
      template<size_t S>
      struct PtrRefHelper
      {
        template<typename Ptr>
        static inline void AddRef (Ptr* p)
        {
          s1_add_ref (p);
        }
        template<typename Ptr>
        static inline void Release (Ptr* p)
        {
          s1_release (p);
        }
      };
      // Call s1_release() for a 'well-known' type - reinterpret to Object pointer
      template<>
      struct PtrRefHelper<0> /* Note: if we have a well-known forward decl,
                                WellKnownForwardDecl<> is _not_ defined! */
      {
        template<typename Ptr>
        static inline void AddRef (Ptr* p)
        {
          s1_add_ref (reinterpret_cast<Object*> (p));
        }
        template<typename Ptr>
        static inline void Release (Ptr* p)
        {
          s1_release (reinterpret_cast<Object*> (p));
        }
      };
    }

    template<typename T>
    inline void AddRefPtr (T* ptr)
    {
      PtrRefHelper<IsDefinedHelper<WellKnownForwardDecl<T> >::Result>::AddRef (ptr);
    }
    template<typename T>
    inline void ReleasePtr (T* ptr)
    {
      PtrRefHelper<IsDefinedHelper<WellKnownForwardDecl<T> >::Result>::Release (ptr);
    }
  } // namespace cxxapi

#ifndef S1_HAVE_RVALUES
  /**
   * Smart pointer for Shader1 C++ public API objects.
   * This smart pointer \em always assumes ownership.
   */
  template<typename T>
  class TransferRefPtr
  {
  private:
    T* obj;
  public:
    /// Construct with a \NULL pointer.
    TransferRefPtr () : obj (0) {}
    /// Construct from \a p, never adding a reference
    TransferRefPtr (T* p) : obj (p) { }
    /// Move reference from \a other
    template<typename T2>
    TransferRefPtr (const TransferRefPtr<T2>& other) : obj (const_cast<TransferRefPtr&> (other).detach()) { }
    /// Releases reference
    ~TransferRefPtr ()
    {
      if (obj) cxxapi::ReleasePtr (obj);
    }
    
    /**
     * Assume ownership of a pointer.
     * Replaces the currently stored pointer. Properly releases reference of the
     * old pointer, but does not add a reference to the new pointer!
     */
    void take (T* p)
    {
      if (obj) cxxapi::ReleasePtr (obj);
      obj = p;
    }
    /// Return the stored pointer.
    T* get() const
    {
      return obj;
    }
    /// Return the stored pointer.
    operator T* () const
    {
      return obj;
    }
    /**
     * Hand over ownership of a pointer.
     * Returns the stored pointer and clears it internally.
     * Does not release a reference to the pointer!
     */
    T* detach()
    {
      T* p (obj);
      obj = 0;
      return p;
    }
    
    /// Move reference from \a other
    TransferRefPtr& operator= (const TransferRefPtr& other)
    {
      take (const_cast<TransferRefPtr&> (other).detach());
      return *this;
    }

    /// Dereference stored pointer.
    T& operator* () const
    {
      return *obj;
    }
    /// Dereference stored pointer.
    T* operator-> () const
    {
      return obj;
    }
  };
#endif
#endif

  /// Smart pointer for Shader1 C++ public API objects.
  template<typename T>
  class Ptr
  {
  private:
    T* obj;
  public:
    /// Construct with a \NULL pointer.
    Ptr () : obj (0) {}
    /// Construct from \a p, adding a reference
    Ptr (T* p) : obj (p)
    {
      if (obj) cxxapi::AddRefPtr (obj);
    }
    struct TakeTag {};
    /// Construct from \a p, \em not adding a reference
    Ptr (T* p, const TakeTag&) : obj (p) { }
#if !defined(DOXYGEN_RUN) && !defined(S1_HAVE_RVALUES)
    /// Move reference from \a other
    template<typename T2>
    Ptr (const TransferRefPtr<T2>& other) : obj (const_cast<TransferRefPtr<T2>&> (other).detach()) { }
#endif
    /// Copy from \a other, adding a reference
    Ptr (const Ptr& other) : obj (0) { reset (other.obj); }
#ifdef S1_HAVE_RVALUES
    /// Move reference from \a other
    Ptr (Ptr&& other) : obj (other.detach()) { }
#if !defined(DOXYGEN_RUN) && !defined(S1_HAVE_RVALUES)
    /// Move reference from \a other
    template<typename T2>
    Ptr (TransferRefPtr<T2>&& other) : obj (other.detach()) { }
#endif
#endif
    /// Releases reference
    ~Ptr()
    {
      if (obj) cxxapi::ReleasePtr (obj);
    }
    
    //@{
    /**
     * Replace the currently stored pointer.
     * To the new pointer a reference is added, and from the old
     * pointer a reference is removed.
     */
    void reset (T* p = 0)
    {
      if (p) s1_add_ref (p);
      if (obj) cxxapi::ReleasePtr (obj);
      obj = p;
    }
#ifdef S1_HAVE_RVALUES
    void reset (Ptr&& p)
    {
      if (obj) cxxapi::ReleasePtr (obj);
      obj = p.detach();
    }
#endif
    //@}
    /**
     * Assume ownership of a pointer.
     * Replaces the currently stored pointer. Properly releases reference of the
     * old pointer, but does not add a reference to the new pointer!
     */
    void take (T* p)
    {
      if (obj) cxxapi::ReleasePtr (obj);
      obj = p;
    }
    /// Return the stored pointer.
    T* get() const
    {
      return obj;
    }
    /// Return the stored pointer.
    operator T* () const
    {
      return obj;
    }
    /**
     * Hand over ownership of a pointer.
     * Returns the stored pointer and clears it internally.
     * Does not release a reference to the pointer!
     */
    T* detach()
    {
      T* p (obj);
      obj = 0;
      return p;
    }
    
    /// Copy from \a other, adding a reference
    Ptr& operator= (const Ptr& other)
    {
      reset (other.obj);
      return *this;
    }
#if !defined(DOXYGEN_RUN) && !defined(S1_HAVE_RVALUES)
    /// Move reference from \a other
    template<typename T2>
    Ptr& operator= (const TransferRefPtr<T2>& other)
    {
      take (const_cast<TransferRefPtr<T2>&> (other).detach());
      return *this;
    }
#endif
#ifdef S1_HAVE_RVALUES
    /// Move reference from \a other
    Ptr& operator= (Ptr&& other)
    {
      take (other.detach());
      return *this;
    }
#endif

    /// Dereference stored pointer.
    T& operator* () const
    {
      return *obj;
    }
    /// Dereference stored pointer.
    T* operator-> () const
    {
      return obj;
    }
  };
} // namespace s1

/* Helper macro resolving to return type for pointers from API that already
 * have a reference.
 * Note: S1_RETURN_TRANSFER_REF() is a macro so it gets inlined on MSVC */
#ifdef S1_HAVE_RVALUES
  // Prefer Ptr<> if we have rvalues. Also plays nicer with 'auto'
  #define S1_RETURN_TRANSFER_REF_TYPE(Type)                            \
      ::s1::Ptr< Type >
  #define S1_RETURN_TRANSFER_REF(Type, p)                              \
      ::s1::Ptr<Type> ((p), ::s1::Ptr< Type >::TakeTag ())
#else
  #define S1_RETURN_TRANSFER_REF_TYPE(Type)                            \
      ::s1::TransferRefPtr< Type >
  #define S1_RETURN_TRANSFER_REF(Type, p)                              \
      (p)
#endif

#endif

#endif // __S1_PTR_H__
