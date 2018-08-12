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
 * Object base class for Shader1 Public API objects.
 */
#ifndef __S1_OBJECT_H__
#define __S1_OBJECT_H__

/* MSVC prerequisites */

#include "preprocessor.h"

#include "s1/defs.h"

#if !defined(DOXYGEN_RUN)
// Extract the C type name from a type name tuple.
#define _S1TYPE_TYPENAME_CNAME(IMPLINFO)          _S1BOOSTPP_TUPLE_ELEM(2, 0, IMPLINFO)
// Extract the C type name from a type info.
#define _S1TYPE_TYPEINFO_CNAME(TYPEINFO)          _S1TYPE_TYPENAME_CNAME(_S1BOOSTPP_LIST_FIRST(TYPEINFO))
// Extract the C++ identifier tuple from a type name tuple.
#define _S1TYPE_TYPENAME_CXXINFO(IMPLINFO)        _S1BOOSTPP_TUPLE_ELEM(2, 1, IMPLINFO)
// Extract the C++ forward declaration from a type name tuple.
#define _S1TYPE_TYPENAME_CXXFWD(IMPLINFO)         _S1BOOSTPP_TUPLE_ELEM(2, 0, _S1TYPE_TYPENAME_CXXINFO(IMPLINFO))
// Extract the C++ forward declaration from a type info.
#define _S1TYPE_TYPEINFO_CXXFWD(TYPEINFO)         _S1TYPE_TYPENAME_CXXFWD(_S1BOOSTPP_LIST_FIRST(TYPEINFO))
// Extract the C++ type alias from a type name tuple.
#define _S1TYPE_TYPENAME_CXXALIAS(IMPLINFO)       _S1BOOSTPP_TUPLE_ELEM(2, 1, _S1TYPE_TYPENAME_CXXINFO(IMPLINFO))
// Extract the C++ type alias from a type info.
#define _S1TYPE_TYPEINFO_CXXALIAS(TYPEINFO)       _S1TYPE_TYPENAME_CXXALIAS(_S1BOOSTPP_LIST_FIRST(TYPEINFO))
#endif // #if !defined(DOXYGEN_RUN)

#if defined(__cplusplus)
/**\def S1TYPE_CAST(Type, x)
 * \hideinitializer
 * Downcast a pointer to a public API object to a (base) type.
 * Provides some compile-time type safety (i.e. a cast will not
 * compile if trying to cast to a type that's not a base class).
 * \param Type Type to cast down to.
 * \param x Pointer to cast down.
 */
#define S1TYPE_CAST(Type, x)    (static_cast<Type*> (x))
#define _S1TYPE_DECLARE_BODY1(TYPEINFO, T, Body)                \
  _S1TYPE_TYPEINFO_CXXFWD(TYPEINFO)                             \
  typedef _S1TYPE_TYPEINFO_CXXALIAS(TYPEINFO) T
/**\def S1_CXXTYPE_IMPL(Type)
 * \hideinitializer
 * \internal
 * Make a type name tuple for the Shader public API.
 */
#define S1_CXXTYPE_IMPL(Type)                           \
  (namespace s1 { namespace cxxapi { class Type; } }, s1::cxxapi::Type)

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

#define _S1TYPE_ANCESTRY_ENTRY1(state, T)       state _S1BOOSTPP_CAT(T, _Type) T;
#define _S1TYPE_ANCESTRY_ENTRY(d, state, T)     _S1TYPE_ANCESTRY_ENTRY1(state, _S1TYPE_TYPENAME_CNAME(T))
#define _S1TYPE_ANCESTRY(TYPEINFO)                              \
        _S1BOOSTPP_LIST_FOLD_RIGHT(_S1TYPE_ANCESTRY_ENTRY,        \
                                 _S1BOOSTPP_EMPTY(),              \
                                 TYPEINFO)

#define S1TYPE_CAST(Type, x)    ((Type*)(&((x)->bases.Type)))

#endif // defined(__cplusplus)

#if !defined(DOXYGEN_RUN)
#define _S1TYPE_DECLARE_BODY(TYPEINFO, Body)        \
  _S1TYPE_DECLARE_BODY1(TYPEINFO, _S1TYPE_TYPEINFO_CNAME(TYPEINFO), Body)

#define _S1TYPE_BODY_DUMMY()    void* reserved;
#define _S1TYPE_BODY_EMPTY()
#endif

#if defined(__cplusplus)
/**\def S1TYPE_DECLARE(TYPEINFO)
 * \hideinitializer
 * \internal
 * Emits type declaration for type described in \c TYPEINFO as well as
 * all necessary helpers to support casting/type-checking.
 * \param TYPEINFO Type description. Is a Boost.PP tuple, the first element
 *   is a type name t, the second element is the type info for the
 *   base class.
 */
#define S1TYPE_DECLARE(TYPEINFO)       _S1TYPE_DECLARE_BODY(TYPEINFO, _S1TYPE_BODY_EMPTY)
/**\def S1TYPE_DECLARE_FWD(IMPLINFO)
 * \hideinitializer
 * \internal
 * Forward-declare a type. Takes a &lsquo;raw&rsquo; type name tuple.
 */
#define S1TYPE_DECLARE_FWD_RAW(TYPENAME, IMPLINFO)      \
  _S1TYPE_TYPENAME_CXXFWD(IMPLINFO)                     \
  namespace s1 {                                        \
    S1_NS_CXXAPI_BEGIN                                  \
      template<> struct WellKnownForwardDecl<TYPENAME>; \
    S1_NS_CXXAPI_END                                    \
  }                                                     \
  typedef _S1TYPE_TYPENAME_CXXALIAS(IMPLINFO)           \
    _S1TYPE_TYPENAME_CNAME(IMPLINFO)
#else // defined(__cplusplus)
#define S1TYPE_DECLARE(TYPEINFO)       _S1TYPE_DECLARE_BODY(TYPEINFO, _S1TYPE_BODY_DUMMY)
#define _S1TYPE_DECLARE_FWD_RAW_1(TYPENAME)             \
  struct _S1BOOSTPP_CAT(TYPENAME, _s);                    \
  typedef struct _S1BOOSTPP_CAT(TYPENAME, _s) TYPENAME
#define S1TYPE_DECLARE_FWD_RAW(TYPENAME, TYPEINFO)      \
  _S1TYPE_DECLARE_FWD_RAW_1(_S1TYPE_TYPENAME_CNAME(TYPEINFO))
#endif

/**\def S1_TYPE_MAKE_NAME(Name)
 * \hideinitializer
 * \internal
 * Make a type name tuple for the Shader1 API.
 */
#define S1_TYPE_MAKE_NAME(Name)         (_S1BOOSTPP_CAT(s1_, Name), S1_CXXTYPE_IMPL(Name))
/**\def S1TYPE_DECLARE_FWD(TYPENAME)
 * \hideinitializer
 * \internal
 * Forward-declare a type. Uses #S1_TYPE_MAKE_NAME to generate type name tuple.
 */
#define S1TYPE_DECLARE_FWD(TYPENAME)    S1TYPE_DECLARE_FWD_RAW(TYPENAME, S1_TYPE_MAKE_NAME(TYPENAME))

#define S1TYPE_INFO_s1_Object	(S1_TYPE_MAKE_NAME(Object), _S1BOOSTPP_NIL)

#if defined(DOXYGEN_RUN)
  /**
   * Base "class" for public C API base objects.
   * Provides reference counting facilities.
   * All classes in the public API are derived from this class.
   * \sa #S1TYPE_CAST
   * \sa \ref refcounting
   */
  S1TYPE_DECLARE(S1TYPE_INFO_s1_Object);
#else
  #if defined(__cplusplus) && defined(S1_BUILD)
  // Library-internal, s1_Object is declared empty
  _S1TYPE_DECLARE_BODY(S1TYPE_INFO_s1_Object, _S1TYPE_BODY_EMPTY);
  #else
  /* Externally, s1_Object never appears empty.
  * (This is important so the empty base class optimization can be employed
  * to get the desired memory layout for the API classes.) */
  _S1TYPE_DECLARE_BODY(S1TYPE_INFO_s1_Object, _S1TYPE_BODY_DUMMY);
  #endif
#endif

/**
 * Add a reference to the object. Returns new reference count.
 * \param obj Object to add an reference to.
 * \remarks In code, you can actually pass in any public API object,
 *   not only those of type s1_Object, without a separate cast.
 * \sa \ref refcounting
 * \memberof s1_Object
 */
S1_API(int) s1_add_ref (s1_Object* obj);
/**
 * Release a reference to the object. Returns new reference count.
 * \param obj Object to release a reference from.
 * \remarks In code, you can actually pass in any public API object,
 *   not only those of type s1_Object, without a separate cast.
 * \sa \ref refcounting
 * \memberof s1_Object
 */
S1_API(int) s1_release (s1_Object* obj);
/**
 * Query reference count of an object. Useful for debugging purposes.
 * \param obj Object to query reference count from.
 * \remarks In code, you can actually pass in any public API object,
 *   not only those of type s1_Object, without a separate cast.
 * \sa \ref refcounting
 * \memberof s1_Object
 */
S1_API(int) s1_get_ref_count (s1_Object* obj);

#if !defined(DOXYGEN_RUN)
// MSVC bugginess, see http://lists.cairographics.org/archives/cairo/2008-January/012722.html
#ifdef _MSC_VER
#define inline __inline
#endif

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

#ifdef _MSC_VER
#undef inline
#endif

#define s1_add_ref(Obj)         _s1_add_ref_actual (S1TYPE_CAST(s1_Object, Obj))
#define s1_release(Obj)         _s1_release_actual (S1TYPE_CAST(s1_Object, Obj))
#define s1_get_ref_count(Obj)   _s1_get_ref_count_actual (S1TYPE_CAST(s1_Object, Obj))
#endif

#if defined(__cplusplus)
#include "s1/warn_off.h"

#if !defined(S1_BUILD)
#define S1_CXXAPI_EXTERNAL(T)                                     \
  namespace s1 { typedef cxxapi::T T; }
#else
#define S1_CXXAPI_EXTERNAL(T)
#endif // !defined(S1_BUILD)

namespace s1
{
  S1_NS_CXXAPI_BEGIN

#if !defined(DOXYGEN_RUN)
    /**
     * \internal Base class for Object to disallow instantiation, copying,
     * assignment, and manual destruction.
     */
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
#endif

    /**
     * Base class for public C++ API base objects.
     * Provides reference counting facilities.
     * All classes in the public API are derived from this class.
     * \sa \ref refcounting
     */
    class Object
#if !defined(S1_BUILD) && !defined(DOXYGEN_RUN)
      : public ObjectBase
#endif // !defined(S1_BUILD)
    {
    public:
      /**
       * Add a reference to the object. Returns new reference count.
       * \sa \ref refcounting
       */
      int AddRef()
      {
        return s1_add_ref (this);
      }
      /**
       * Release a reference to the object. Returns new reference count.
       * \sa \ref refcounting
       */
      int Release()
      {
        return s1_release (this);
      }
      /**
       * Query reference count of an object. Useful for debugging purposes.
       * \sa \ref refcounting
       */
      int GetRefCount()
      {
        return s1_get_ref_count (this);
      }
    };
  
  #if !defined(DOXYGEN_RUN)
    /* Note: If this class is _defined_, we know there is _no_ well-known forward
     * declaration!
     * A well-known forward declaration causes a specialization to be forward-
     * declared. */
    template<typename T>
    struct WellKnownForwardDecl
    {
      typedef int Sentinel;
    };
  #endif

  S1_NS_CXXAPI_END

#if !defined(S1_BUILD) && !defined(DOXYGEN_RUN)
  typedef cxxapi::Object Object;
#endif // !defined(S1_BUILD)
} // namespace s1

#include "s1/warn_on.h"
#endif // defined(__cplusplus)

/**\page refcounting Reference counting
 *
 * Reference counting controls the lifetime of public API objects.
 * These can only be created and destroyed through API
 * methods. The point of time when an object can be freed is determined
 * by reference counting - when you start using an object you &ldquo;take&rdquo;
 * (non-exclusive) ownership of an object by adding a reference. After you're
 * done using it, release the reference &mdash; after the last reference was
 * released, the object is automatically destroyed.
 * 
 * Some methods &ldquo;transfer&rdquo; ownership when returning an object,
 * that is, a reference has already been added for you. In this case,
 * you \em only need to release the reference.
 * 
 * Other methods, however, &ldquo;borrow&rdquo; you ownership when returning
 * an objects, meaning a reference was \em not added. You can use the object
 * as long as it's guaranteed that it is still &ldquo;alive&rdquo;. The
 * conditions may vary from case to case, but typically, it's fine to use
 * an object without adding a reference if you can ensure it's initially owning
 * object (the one which borrowed you the reference) is valid. However, if
 * you plan to keep a reference beyond the lifetime of the initially owning
 * object, or if you're not sure the initially owning object is alive long
 * enough, you should add a reference (and later release it) as usual.
 * 
 * As a rule of thumb, methods that &ldquo;<tt>create</tt>&rdquo; an object
 * transfer a reference, while methods that &ldquo;<tt>get</tt>&rdquo; some
 * object return a borrowed reference.
 * 
 * \par Reference counting in the C API
 * * Add a reference: s1_add_ref()
 * * Release a reference: s1_release()
 * * Query reference count: s1_get_ref_count()
 * 
 * \par Reference counting in the C++ API
 * \note The C++ API generally returns smart pointers to objects, so
 * proper reference counting is done automatically. The explicit
 * reference counting methods typically don't have to be used.
 * 
 * * Add a reference: s1::Object::AddRef()
 * * Release a reference: s1::Object::Release()
 * * Query reference count: s1::Object::GetRefCount()
 * 
 */

#endif // __S1_OBJECT_H__
