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
 * Object base class. Reference-countable with boost::intrusive_ptr<>.
 */
#ifndef __BASE_OBJECT_H__
#define __BASE_OBJECT_H__

#include "s1/Object.h"

#include <boost/atomic.hpp>

namespace s1
{
  /**
   * Reference-counted base class for all objects exposed to the public API.
   * \remarks New instances are initialized with a zero reference count.
   */
  class Object : public s1_Object
  {
  public:
    Object() : refCount (0) {}
    virtual ~Object () {}

    /// Add a reference to the object. Returns new reference count.
    int AddRef ()
    {
      return refCount.fetch_add (1, boost::memory_order_relaxed)+1;
    }
    /// Release a reference to the object. Returns new reference count.
    int Release ()
    {
      int newRC (refCount.fetch_sub (1, boost::memory_order_release)-1);
      if (newRC == 0)
      {
        boost::atomic_thread_fence(boost::memory_order_acquire);
        delete this;
      }
      return newRC;
    }
    /// Query reference count of an object. Useful for debugging purposes.
    int GetRefCount() const
    {
      return refCount;
    }

    /**\name Casts to externally visible instance layout
     * Helper methods to cast instances of Object and descendants to the
     * externally visible instance layout (i.e. the public API opaque
     * objects).
     * \tparam ExtType External type to cast to.
     * @{ */
    template<typename ExtType>
    ExtType* DowncastEvil ()
    {
      return reinterpret_cast<ExtType*> (static_cast<s1_Object*> (this));
    }
    template<typename ExtType>
    const ExtType* DowncastEvil () const
    {
      return reinterpret_cast<const ExtType*> (static_cast<const s1_Object*> (this));
    }
    /** @} */
  private:
    boost::atomic<int32_t> refCount;
  };

  /**\name Casts to externally visible instance layout
   * Helper methods to cast objects in the externally visible instance layout
   * (i.e. the public API opaque representation) to actually useable
   * instances of Object or an descendant.
   * \tparam IntType External type to cast to.
   * \tparam ExtType External type to cast from. (Don't specify, let the compiler
   *   deduce it.)
   * @{ */
  template<typename IntType, typename ExtType>
  static inline IntType* EvilUpcast (ExtType* obj)
  {
    return static_cast<IntType*> (reinterpret_cast<s1_Object*> (obj));
  }
  template<typename IntType, typename ExtType>
  static inline const IntType* EvilUpcast (const ExtType* obj)
  {
    return static_cast<const IntType*> (reinterpret_cast<const s1_Object*> (obj));
  }
  /** @} */

  static inline void intrusive_ptr_add_ref (Object* p)
  {
    p->AddRef();
  }

  static inline void intrusive_ptr_release (Object* p)
  {
    p->Release();
  }
} // namespace s1

#endif // __BASE_OBJECT_H__
