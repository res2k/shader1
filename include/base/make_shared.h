/**\file 
 * boost::make_shared<> replacement that is conveniently friendable
 */
#ifndef __BASE_MAKE_SHARED_H__
#define __BASE_MAKE_SHARED_H__

#include <boost/shared_ptr.hpp>

#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#ifndef MAX_MAKE_SHARED_ARGS
#define MAX_MAKE_SHARED_ARGS  10
#endif

#include <boost/make_shared.hpp> // we're using some boost::details from there...

/* Lifted from boost/make_shared.hpp */
#if !defined( BOOST_NO_FUNCTION_TEMPLATE_ORDERING )
# define BOOST_SP_MSD( T ) boost::detail::sp_inplace_tag< boost::detail::sp_ms_deleter< T > >()
#else
# define BOOST_SP_MSD( T ) boost::detail::sp_ms_deleter< T >()
#endif

namespace s1
{
  template<typename T>
  class make_shared
  {
    typedef boost::shared_ptr<T> PtrType;
    PtrType ptr;
  public:

#if defined( BOOST_HAS_RVALUE_REFS )
#define _MAKE_SHARED_CTOR_ARG_PREFIX
#define _MAKE_SHARED_CTOR_ARG_SUFFIX  &&
#else
#define _MAKE_SHARED_CTOR_ARG_PREFIX  const
#define _MAKE_SHARED_CTOR_ARG_SUFFIX  &
#endif

#define _MAKE_SHARED_CTOR_TEMPLDECL(N)				    \
    BOOST_PP_IF(N, BOOST_PP_IDENTITY(template<), BOOST_PP_EMPTY)()  \
    BOOST_PP_ENUM_PARAMS(N, typename A)				    \
    BOOST_PP_IF(N, BOOST_PP_IDENTITY(>), BOOST_PP_EMPTY)()
#define _MAKE_SHARED_CTOR_ARGS(N)     \
    BOOST_PP_ENUM_BINARY_PARAMS(N,    \
      _MAKE_SHARED_CTOR_ARG_PREFIX A, \
      _MAKE_SHARED_CTOR_ARG_SUFFIX a)
#define _MAKE_SHARED_CTOR_NEW_ARG(_z, Num, _data) \
    boost::detail::sp_forward<BOOST_PP_CAT(A, Num)>( BOOST_PP_CAT(a, Num) )
#define _MAKE_SHARED_CTOR_NEW_ARGS(N) \
    BOOST_PP_ENUM(N, _MAKE_SHARED_CTOR_NEW_ARG, _)
#define _MAKE_SHARED_CTOR(_z, N, _data)					      \
    _MAKE_SHARED_CTOR_TEMPLDECL(N)					      \
    make_shared (_MAKE_SHARED_CTOR_ARGS(N))				      \
    {									      \
      /* Lifted from boost::make_shared<> */				      \
      boost::shared_ptr< T > pt( static_cast< T* >( 0 ), BOOST_SP_MSD( T ) ); \
      boost::detail::sp_ms_deleter< T > * pd =				      \
	boost::get_deleter< boost::detail::sp_ms_deleter< T > >( pt );	      \
      void * pv = pd->address();					      \
      ::new( pv ) T(_MAKE_SHARED_CTOR_NEW_ARGS(N));			      \
      pd->set_initialized();						      \
      T * pt2 = static_cast< T* >( pv );				      \
      boost::detail::sp_enable_shared_from_this( &pt, pt2, pt2 );	      \
      ptr = boost::shared_ptr< T >( pt, pt2 );				      \
    }

    BOOST_PP_REPEAT(MAX_MAKE_SHARED_ARGS, _MAKE_SHARED_CTOR, _)

#undef _MAKE_SHARED_CTOR_ARG_PREFIX
#undef _MAKE_SHARED_CTOR_ARG_SUFFIX
#undef _MAKE_SHARED_CTOR_ARGS
#undef _MAKE_SHARED_CTOR_NEW_ARGS
#undef _MAKE_SHARED_CTOR_NEW_ARG
#undef _MAKE_SHARED_CTOR
#undef _MAKE_SHARED_CTOR_TEMPLDECL

    template<typename U>
    operator boost::shared_ptr<U> () const { return boost::shared_static_cast<U> (ptr); }
  };
} // namespace s1

#undef BOOST_SP_MSD

#endif // __BASE_MAKE_SHARED_H__
