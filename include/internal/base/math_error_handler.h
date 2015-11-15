/*
Shader1
Copyright (c) 2015 Frank Richter


This library is free software; you can redistribute it and/or
modify it under the terms of the wxWindows Library Licence.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Please see the wxWindows Library Licence for more details.
The licence texts can be found in the source distribution files
LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef __BASE_MATH_ERROR_HANDLER_H__
#define __BASE_MATH_ERROR_HANDLER_H__

/**\file
 * Declaractions for boost::math user-defined error handling policy
 */

#include <boost/convert.hpp>
#include <boost/convert/printf.hpp>
#include <boost/math/policies/policy.hpp>

namespace s1
{
  namespace detail
  {
    void RaiseRoundingError (const char* function, const char* message, const char* typeStr, const char* valStr);
  } // namespace detail
} // namespace s1

namespace boost
{
  namespace math
  {
    namespace policies
    {
      template <class T, class TargetType>
      T user_rounding_error (const char* function, const char* message, const T& val, const TargetType& t)
      {
        if (!function) function = "<unknown function> (%1%)";
        if (!message) message = "bad argument (%1%)";
        auto valStr = boost::convert<std::string> (val, boost::cnv::printf ());
        s1::detail::RaiseRoundingError (function, message, typeid(T).name (),
                                        valStr ? valStr->c_str () : "???");
        return t;
      }
    } // namespace policies
  } // namespace math
} // namespace boost

#endif // __BASE_MATH_ERROR_HANDLER_H__