/**\file
 * Smart pointer for Shader1 public API objects.
 */
#ifndef __S1_PTR_H__
#define __S1_PTR_H__

#include "s1/Object.h"

#ifdef __cplusplus
namespace s1
{
  template<typename T>
  class Ptr
  {
  private:
    T* obj;
  public:
    struct TakeReference {};
    
    Ptr () : obj (0) {}
    Ptr (T* p) : obj (p)
    {
      s1_add_ref (static_cast<s1_Object*> (p));
    }
    Ptr (T* p, TakeReference) : obj (p) { }
    ~Ptr()
    {
      if (obj) s1_release (static_cast<s1_Object*> (obj));
    }
    
    void reset (T* p = 0)
    {
      if (p) s1_add_ref (static_cast<s1_Object*> (p));
      if (obj) s1_release (static_cast<s1_Object*> (obj));
      obj = p;
    }
    /**
     * Assume ownership of a pointer.
     * Replaces the currently stored pointer. Properly releases reference of the
     * old pointer, but does not add a reference to the new pointer!
     */
    void take (T* p)
    {
      if (obj) s1_release (static_cast<s1_Object*> (obj));
      obj = p;
    }
    T* get() const
    {
      return obj;
    }
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
    
    Ptr& operator= (const Ptr& other)
    {
      reset (other.obj);
      return *this;
    }

    T& operator* () const
    {
      return *obj;
    }
    T* operator-> () const
    {
      return obj;
    }
  };
} // namespace s1
#endif

#endif // __S1_PTR_H__
