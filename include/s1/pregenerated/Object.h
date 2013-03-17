/**\file
 * Object base class for Shader1 Public API objects.
 */
#ifndef __S1_OBJECT_H__
#define __S1_OBJECT_H__

#include "preprocessor.h"


#if defined(__cplusplus)
/* C++ is tricky... externally, s1_Object should not appear empty.
 * However, it's descendants are used in multiple inheritance and
 * *must* be empty (lest the dummy member would figure into 
 * C++ wrappers once for each derivation level).
 * 
 * Workaround: Keep pseudo-classes empty, and provide cast functionality
 * through a helper method.
 */
#define S1TYPE_CAST(x, Type)    (x->_S1BOOSTPP_CAT(cast_to_, Type)())

#define _S1TYPE_CAST_MEMBER(d, state, T)                        \
  state                                                         \
  T* _S1BOOSTPP_CAT(cast_to_, T)()                                \
  { return reinterpret_cast<T*> (this); }                       \
  const T* _S1BOOSTPP_CAT(cast_to_, T)() const                    \
  { return reinterpret_cast<const T*> (this); }
#define _S1TYPE_MAKE_CAST_MEMBERS(TYPEINFO)                     \
        _S1BOOSTPP_LIST_FOLD_RIGHT(_S1TYPE_CAST_MEMBER,           \
                                 _S1BOOSTPP_EMPTY(),              \
                                 TYPEINFO)

#define _S1TYPE_DECLARE_BODY1(TYPEINFO, T, Body)        \
struct T                                                \
{                                                       \
  Body()                                                \
  _S1TYPE_MAKE_CAST_MEMBERS(TYPEINFO)                   \
}

#else // defined(__cplusplus)
// Provide a pseudo-casting mechanism for public API types
#define _S1TYPE_DECLARE_BODY1(TYPEINFO, T, Body)        \
struct _S1BOOSTPP_CAT(T, _Type_s)                         \
{                                                       \
  Body ()                                               \
};                                                      \
typedef struct _S1BOOSTPP_CAT(T, _Type_s)                 \
  _S1BOOSTPP_CAT(T, _Type);                               \
struct _S1BOOSTPP_CAT(T, _s)                              \
{                                                       \
  union _S1BOOSTPP_CAT(T, _Bases)                         \
  {                                                     \
    _S1TYPE_ANCESTRY(TYPEINFO)			        \
  } bases;                                              \
};                                                      \
typedef struct _S1BOOSTPP_CAT(T, _s) T

#define _S1TYPE_ANCESTRY_ENTRY(d, state, T)     state _S1BOOSTPP_CAT(T, _Type) T;
#define _S1TYPE_ANCESTRY(TYPEINFO)                              \
        _S1BOOSTPP_LIST_FOLD_RIGHT(_S1TYPE_ANCESTRY_ENTRY,        \
                                 _S1BOOSTPP_EMPTY(),              \
                                 TYPEINFO)

#define S1TYPE_CAST(x, Type)    ((Type*)(&((x)->bases.Type)))

#endif // defined(__cplusplus)

#define _S1TYPE_DECLARE_BODY(TYPEINFO, Body)        \
  _S1TYPE_DECLARE_BODY1(TYPEINFO, _S1BOOSTPP_LIST_FIRST(TYPEINFO), Body)

#define _S1TYPE_BODY_DUMMY()    void* reserved;
#define _S1TYPE_BODY_EMPTY()

#if defined(__cplusplus)
#define S1TYPE_DECLARE(TYPEINFO)       _S1TYPE_DECLARE_BODY(TYPEINFO, _S1TYPE_BODY_EMPTY)
#else
#define S1TYPE_DECLARE(TYPEINFO)       _S1TYPE_DECLARE_BODY(TYPEINFO, _S1TYPE_BODY_DUMMY)
#endif

#define S1TYPE_INFO_s1_Object	(s1_Object, _S1BOOSTPP_NIL)

#if defined(__cplusplus) && defined(S1_BUILD)
// Library-internal, s1_Object is declared empty
_S1TYPE_DECLARE_BODY(S1TYPE_INFO_s1_Object, _S1TYPE_BODY_EMPTY);
#else
/* Externally, s1_Object never appears empty.
 * (This is important so the empty base class optimization can be employed
 * to get the desired memory layout for the API classes.) */
_S1TYPE_DECLARE_BODY(S1TYPE_INFO_s1_Object, _S1TYPE_BODY_DUMMY);
#endif

/// Add a reference to the object. Returns new reference count.
S1_API int s1_add_ref (s1_Object* obj);
/// Release a reference to the object. Returns new reference count.
S1_API int s1_release (s1_Object* obj);
/// Query reference count of an object. Useful for debugging purposes.
S1_API int s1_get_ref_count (s1_Object* obj);

static inline int _s1_add_ref_actual (s1_Object* obj)
{
  return s1_add_ref (obj);
}
static inline int _s1_release_actual (s1_Object* obj)
{
  return s1_release (obj);
}
static inline int _s1_get_ref_count_actual (s1_Object* obj)
{
  return s1_get_ref_count (obj);
}

#define s1_add_ref(Obj)         _s1_add_ref_actual (S1TYPE_CAST(Obj, s1_Object))
#define s1_release(Obj)         _s1_release_actual (S1TYPE_CAST(Obj, s1_Object))
#define s1_get_ref_count(Obj)   _s1_get_ref_count_actual (S1TYPE_CAST(Obj, s1_Object))

#if defined(__cplusplus)
#include "s1/warn_off.h"

namespace s1
{
  namespace cxxapi
  {
    class ObjectBase
    {
    private:
      // Dummy member
      void* reserved;

      // Don't allow instantiation
      ObjectBase ();
      ObjectBase (const ObjectBase&);
      ObjectBase& operator=(const ObjectBase&);
      ~ObjectBase ();
    };
    
    template<typename ExtType, typename BaseClass>
    class Rebadge : public BaseClass
    {
    public:
      ExtType* Cpointer()
      {
        return reinterpret_cast<ExtType*> (this);
      }
      const ExtType* Cpointer() const
      {
        return reinterpret_cast<const ExtType*> (this);
      }
      template<typename Derived>
      static Derived* FromC (ExtType* p)
      {
        return static_cast<Derived*> (reinterpret_cast<BaseClass*> (p));
      }
      template<typename Derived>
      static const Derived* FromC (const ExtType* p)
      {
        return static_cast<Derived*> (reinterpret_cast<const BaseClass*> (p));
      }
    };

    class Object : public Rebadge<s1_Object, ObjectBase>
    {
    public:
      int AddRef()
      {
        return s1_add_ref (Cpointer());
      }
      int Release()
      {
        return s1_release (Cpointer());
      }
      int GetRefCount()
      {
        return s1_get_ref_count (Cpointer());
      }
    };
  } // namespace cxxapi

#if !defined(S1_BUILD)
  typedef cxxapi::Object Object;
#endif // !defined(S1_BUILD)
} // namespace s1

#include "s1/warn_on.h"
#endif // defined(__cplusplus)

#endif // __S1_OBJECT_H__
