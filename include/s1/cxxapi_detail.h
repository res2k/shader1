/*
    Shader1
    Copyright (c) 2017 Frank Richter


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
 * Shader1 Public C++ API internal helpers.
 */
#ifndef __S1_CXXAPI_DETAIL_H__
#define __S1_CXXAPI_DETAIL_H__

#if defined(__cplusplus)
namespace s1
{
  S1_NS_CXXAPI_BEGIN

  namespace detail
  {
    template<class T> struct remove_pointer                    { typedef T type; };
    template<class T> struct remove_pointer<T*>                { typedef T type; };
    template<class T> struct remove_pointer<T* const>          { typedef T type; };
    template<class T> struct remove_pointer<T* volatile>       { typedef T type; };
    template<class T> struct remove_pointer<T* const volatile> { typedef T type; };
  } // namespace detail

  S1_NS_CXXAPI_END
} // namespace s1

#endif // defined(__cplusplus)

#endif // __S1_CXXAPI_DETAIL_H__
