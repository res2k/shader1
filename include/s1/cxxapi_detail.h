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

/**
 * \internal Helper function to print message about an unhandled exception
 * escaping from user code. Utilizes the message handler.
 * \param locationDescription Description of the place where the exception was caught.
 * \param function Name of the function where the exception occured.
 * \param what String returned by \c what(), if available.
 */
S1_API(void) _s1_print_unhandled_exception (const char* locationDescription,
                                            const char* function,
                                            const char* what);

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

    /**
     * \internal Helper function to print message about an unhandled exception
     * escaping from user code.
     * \param locationDescription Description of the place where the exception was caught.
     * \param function Name of the function where the exception occured.
     * \param what String returned by \c what(), if available.
     */
    static void PrintUnhandledException (const char* location, const char* function, const char* what)
    {
      _s1_print_unhandled_exception (location, function, what);
    }
  } // namespace detail

  S1_NS_CXXAPI_END
} // namespace s1

/**
 * \def _S1_DEFAULT_CATCH_UNHANDLED_EXCEPTIONS(Descr)
 * Macro to generate a default catch() block in functor wrappers
 */
#define _S1_DEFAULT_CATCH_UNHANDLED_EXCEPTIONS(Descr)                                             \
  catch (std::exception& e)                                                                       \
  {                                                                                               \
    ::s1::cxxapi::detail::PrintUnhandledException (Descr, S1_FUNCTION_NAME, e.what());            \
  }                                                                                               \
  catch (...)                                                                                     \
  {                                                                                               \
    ::s1::cxxapi::detail::PrintUnhandledException (Descr, S1_FUNCTION_NAME, 0);                   \
  }                                                                                               \
  assert (("Unhandled exception in " ## Descr) && false);

#endif // defined(__cplusplus)

#endif // __S1_CXXAPI_DETAIL_H__
