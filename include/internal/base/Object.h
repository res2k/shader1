/**\file
 * Object base class. Reference-countable with boost::intrusive_ptr<>.
 */
#ifndef __BASE_OBJECT_H__
#define __BASE_OBJECT_H__

#include "s1/Object.h"

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
      return ++refCount;
    }
    /// Release a reference to the object. Returns new reference count.
    int Release ()
    {
      int newRC (--refCount);
      if (newRC == 0)
        delete this;
      return newRC;
    }
    /// Query reference count of an object. Useful for debugging purposes.
    int GetRefCount() const
    {
      return refCount;
    }
  private:
    int refCount;
  };


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
