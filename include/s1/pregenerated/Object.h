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
  T* _S1BOOSTPP_CAT(cast_to_, T)()                              \
  { return reinterpret_cast<T*> (this); }                       \
  const T* _S1BOOSTPP_CAT(cast_to_, T)() const                  \
  { return reinterpret_cast<const T*> (this); }
#define _S1TYPE_MAKE_CAST_MEMBERS(T)                            \
        _S1BOOSTPP_LIST_FOLD_RIGHT(_S1TYPE_CAST_MEMBER,         \
                                 _S1BOOSTPP_EMPTY(),            \
                                 _S1BOOSTPP_EXPAND(_S1BOOSTPP_CAT(S1TYPE_INFO_, T)))

#define _S1TYPE_DECLARE_BODY(T, Body)       \
struct T                                    \
{                                           \
  Body()                                    \
  _S1TYPE_MAKE_CAST_MEMBERS(T)              \
}

#else // defined(__cplusplus)
// Provide a pseudo-casting mechanism for public API types
#define _S1TYPE_DECLARE_BODY(T, Body)       \
struct T ## _Type_s                         \
{                                           \
  Body ()                                   \
};                                          \
typedef struct T ## _Type_s T ## _Type;     \
struct T ## _s                              \
{                                           \
  union T ## _Bases                         \
  {                                         \
    _S1TYPE_ANCESTRY(T)			    \
  } bases;                                  \
};                                          \
typedef struct T ## _s T

#define _S1TYPE_ANCESTRY_ENTRY(d, state, T)     state _S1BOOSTPP_CAT(T, _Type) T;
#define _S1TYPE_ANCESTRY(T)                                     \
        _S1BOOSTPP_LIST_FOLD_RIGHT(_S1TYPE_ANCESTRY_ENTRY,        \
                                 _S1BOOSTPP_EMPTY(),              \
                                 _S1BOOSTPP_EXPAND(_S1BOOSTPP_CAT(S1TYPE_INFO_, T)))

#define S1TYPE_CAST(x, Type)    ((Type*)(&((x)->bases.Type)))

#endif // defined(__cplusplus)

#define _S1TYPE_BODY_DUMMY()    void* reserved;
#define _S1TYPE_BODY_EMPTY()

#if defined(__cplusplus)
#define S1TYPE_DECLARE(T)       _S1TYPE_DECLARE_BODY(T, _S1TYPE_BODY_EMPTY)
#else
#define S1TYPE_DECLARE(T)       _S1TYPE_DECLARE_BODY(T, _S1TYPE_BODY_DUMMY)
#endif

#define S1TYPE_INFO_s1_Object	(s1_Object, _S1BOOSTPP_NIL)

#if defined(__cplusplus) && defined(S1_BUILD)
// Library-internal, s1_Object is declared empty
_S1TYPE_DECLARE_BODY(s1_Object, _S1TYPE_BODY_EMPTY);
#else
/* Externally, s1_Object never appears empty.
 * (This is important so the empty base class optimization can be employed
 * to get the desired memory layout for the API classes.) */
_S1TYPE_DECLARE_BODY(s1_Object, _S1TYPE_BODY_DUMMY);
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
namespace s1
{
  namespace cxxapi
  {
    class Object : public s1_Object
    {
    private:
      // Don't allow instantiation
      Object ();
      Object (const Object&);
      ~Object ();
    public:
      int AddRef()
      {
        return s1_add_ref (this);
      }
      int Release()
      {
        return s1_release (this);
      }
      int GetRefCount()
      {
        return s1_get_ref_count (this);
      }
    };
  } // namespace cxxapi

#if !defined(S1_BUILD)
  typedef cxxapi::Object Object;
#endif // !defined(S1_BUILD)
} // namespace s1

#endif // defined(__cplusplus)

#endif // __S1_OBJECT_H__
