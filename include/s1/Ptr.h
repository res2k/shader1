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
  /** 
   * Pointer cleanup for TransferRefPtr<>: Type-safe.
   * Will not compile if type is not derived from s1::Object.
   */
  struct PtrCleanupDefault
  {
    template<typename T>
    static void Release (T* p) { s1_release (p); }
  };

  /** 
   * Pointer cleanup for TransferRefPtr<>: Type-unssafe.
   * Assumes type is derived from s1::Object, will not trigger compile-time checks.
   */
  struct PtrCleanupUnsafe
  {
    template<typename T>
    static void Release (T* p) { s1_release (reinterpret_cast<cxxapi::Object*> (p)); }
  };

  /**
   * Smart pointer for Shader1 C++ public API objects.
   * This smart pointer \em always assumes ownership.
   * \tparam Cleanup Trait to handle pointer cleanup.
   */
  template<typename T, typename Cleanup = PtrCleanupDefault>
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
      if (obj) Cleanup::Release (obj);
    }
    
    /**
     * Assume ownership of a pointer.
     * Replaces the currently stored pointer. Properly releases reference of the
     * old pointer, but does not add a reference to the new pointer!
     */
    void take (T* p)
    {
      if (obj) s1_release (obj);
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
      s1_add_ref (p);
    }
    /// Move reference from \a other
    template<typename T2, typename C>
    Ptr (const TransferRefPtr<T2, C>& other) : obj (const_cast<TransferRefPtr<T2, C>&> (other).detach()) { }
    /// Copy from \a other, adding a reference
    Ptr (const Ptr& other) : obj (0) { reset (other.obj); }
    /// Releases reference
    ~Ptr()
    {
      if (obj) s1_release (obj);
    }
    
    /**
     * Replace the currently stored pointer.
     * To the new pointer a reference is added, and from the old
     * pointer a reference is removed.
     */
    void reset (T* p = 0)
    {
      if (p) s1_add_ref (p);
      if (obj) s1_release (obj);
      obj = p;
    }
    /**
     * Assume ownership of a pointer.
     * Replaces the currently stored pointer. Properly releases reference of the
     * old pointer, but does not add a reference to the new pointer!
     */
    void take (T* p)
    {
      if (obj) s1_release (obj);
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
    /// Move reference from \a other
    template<typename T2, typename C>
    Ptr& operator= (const TransferRefPtr<T2, C>& other)
    {
      take (const_cast<TransferRefPtr<T2, C>&> (other).detach());
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
} // namespace s1
#endif

#endif // __S1_PTR_H__
