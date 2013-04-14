/**\file
 * Smart pointer for Shader1 public API objects.
 */
#ifndef __S1_PTR_H__
#define __S1_PTR_H__

#include "s1/Object.h"

#ifdef __cplusplus
namespace s1
{
  /// Smart pointer for Shader1 C public API objects.
  template<typename T>
  class CPtr
  {
  private:
    T* obj;
  public:
    struct TakeReference {};
    
    CPtr () : obj (0) {}
    CPtr (T* p) : obj (p)
    {
      s1_add_ref (S1TYPE_CAST (p, s1_Object));
    }
    CPtr (T* p, TakeReference) : obj (p) { }
    CPtr (const CPtr& other) : obj (0) { reset (other.obj); }
    ~CPtr()
    {
      if (obj) s1_release (S1TYPE_CAST (obj, s1_Object));
    }
    
    void reset (T* p = 0)
    {
      if (p) s1_add_ref (S1TYPE_CAST (p, s1_Object));
      if (obj) s1_release (S1TYPE_CAST (obj, s1_Object));
      obj = p;
    }
    /**
     * Assume ownership of a pointer.
     * Replaces the currently stored pointer. Properly releases reference of the
     * old pointer, but does not add a reference to the new pointer!
     */
    void take (T* p)
    {
      if (obj) s1_release (S1TYPE_CAST (obj, s1_Object));
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
    
    CPtr& operator= (const CPtr& other)
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

  /// Smart pointer for Shader1 C++ public API objects.
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
      s1_add_ref (p->Cpointer());
    }
    Ptr (T* p, TakeReference) : obj (p) { }
    Ptr (typename T::CType* p) : obj (T::FromC (p))
    {
      s1_add_ref (p);
    }
    Ptr (typename T::CType* p, TakeReference) : obj (T::FromC (p)) { }
    Ptr (const Ptr& other) : obj (0) { reset (other.obj); }
    template<typename T2>
    Ptr (const CPtr<T2>& other) : obj (0) { reset (T::FromC (other)); }
    ~Ptr()
    {
      if (obj) s1_release (obj->Cpointer());
    }
    
    void reset (T* p = 0)
    {
      if (p) s1_add_ref (p->Cpointer());
      if (obj) s1_release (obj->Cpointer());
      obj = p;
    }
    void reset (typename T::CType* p)
    {
      if (p) s1_add_ref (p);
      if (obj) s1_release (obj->Cpointer());
      obj = T::FromC (p);
    }
    /**
     * Assume ownership of a pointer.
     * Replaces the currently stored pointer. Properly releases reference of the
     * old pointer, but does not add a reference to the new pointer!
     */
    void take (T* p)
    {
      if (obj) s1_release (obj->Cpointer());
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
    template<typename T2>
    Ptr& operator= (const CPtr<T2>& other)
    {
      reset (T::FromC (other));
      return *this;
    }
    Ptr& operator= (typename T::CType* other)
    {
      reset (other);
      return *this;
    }

    operator CPtr<typename T::CType> () const
    {
      return CPtr<typename T::CType> (obj->Cpointer());
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
