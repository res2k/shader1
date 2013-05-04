/**\file
 * Smart pointer for Shader1 public API objects.
 */
#ifndef __S1_PTR_H__
#define __S1_PTR_H__

#include "s1/Object.h"

#ifdef __cplusplus
namespace s1
{
  namespace detail
  {
    /// CPtr<> reference handling trait: Add/release references
    struct Counted
    {
      static void Ref (s1_Object* obj) { s1_add_ref (obj); }
      static void Release (s1_Object* obj) { s1_release (obj); }
    };
    /**
     * CPtr<> reference handling trait: Ignore references.
     * \remarks This type is intended for optimization purposes, i.e.
     *  in method arguments to avoid unnecessary s1_add_ref/s1_release calls.
     *  Do not use this type to permanently store an object pointer!
     */
    struct Uncounted
    {
      static void Ref (s1_Object* obj) { }
      static void Release (s1_Object* obj) { }
    };
  } // namespace detail

  /**
   * Smart pointer for Shader1 C public API objects.
   * \tparam T C API object type (e.g. s1_Object).
   * \tparam Ref Reference handling traits.
   *  This can either be s1::detail::Counted or s1::detail::Uncounted.
   *  However, be careful with s1::detail::Uncounted; it is intended
   *  for method arguments only. Do not use when permanently storing an object pointer!
   */
  template<typename T, typename Ref = detail::Counted>
  class CPtr
  {
  private:
    T* obj;
  public:
    struct TakeReference {};
    
    CPtr () : obj (0) {}
    CPtr (T* p) : obj (p)
    {
      if (p) Ref::Ref (S1TYPE_CAST (p, s1_Object));
    }
    CPtr (T* p, TakeReference) : obj (p) { }
    template<typename OtherRef>
    CPtr (const CPtr<T, OtherRef>& other) : obj (0) { reset (other.get()); }
    ~CPtr()
    {
      if (obj) Ref::Release (S1TYPE_CAST (obj, s1_Object));
    }
    
    void reset (T* p = 0)
    {
      if (p) Ref::Ref (S1TYPE_CAST (p, s1_Object));
      if (obj) Ref::Release (S1TYPE_CAST (obj, s1_Object));
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
    
    template<typename OtherRef>
    CPtr& operator= (const CPtr<T, OtherRef>& other)
    {
      reset (other.get());
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
    template<typename T2, typename CPtrRef>
    Ptr (const CPtr<T2, CPtrRef>& other) : obj (0) { reset (T::FromC (other)); }
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
    template<typename T2, typename CPtrRef>
    Ptr& operator= (const CPtr<T2, CPtrRef>& other)
    {
      reset (T::FromC (other));
      return *this;
    }
    Ptr& operator= (typename T::CType* other)
    {
      reset (other);
      return *this;
    }

    template<typename CPtrRef>
    operator CPtr<typename T::CType, CPtrRef> () const
    {
      return CPtr<typename T::CType, CPtrRef> (obj->Cpointer());
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
