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

/**\file
 * Function result wrapper
 */
#ifndef __BASE_RESULT_H__
#define __BASE_RESULT_H__

#include "s1/Error.h"

#include <boost/call_traits.hpp>
#include <boost/optional.hpp>

namespace s1
{
  /**
   * Function result wrapper. Stores both a result code and an
   * (optional) function return value.
   */
  template<typename T>
  class Result
  {
    /// Result code
    ResultCode resultCode;
    /// Return value
    boost::optional<T> val;
  public:
    typedef T value_type;

    /**
     * Construct from error code.
     */
    Result (ResultCode resultCode) : resultCode (resultCode) {}
    //@{
    /**
    * Default constructor with value and optional result code.
    * Result code default to #S1_SUCCESS.
    */
    Result (const value_type& value, ResultCode resultCode = S1_SUCCESS)
      : resultCode (resultCode), val (value) {}
    Result (value_type&& value, ResultCode resultCode = S1_SUCCESS)
      : resultCode (resultCode), val (std::move (value)) {}
    //@}
    Result (const Result& other) : resultCode (other.resultCode), val (other.val) {}
    Result (Result&& other) : resultCode (other.resultCode), val (std::move (other.val)) {}

    Result& operator= (const Result& other)
    {
      resultCode = other.resultCode;
      val = other.val;
      return *this;
    }
    Result& operator= (Result&& other)
    {
      resultCode = other.resultCode;
      val = std::move (other.val);
      return *this;
    }
    /// Convenience: Assign a result code. Clears the value
    Result& operator= (ResultCode resultCode)
    {
      resultCode = resultCode;
      val = boost::none;
      return *this;
    }
    //@{
    /// Convenience: Assign a result value. Sets result code to #S1_SUCCESS.
    Result& operator= (typename boost::call_traits<value_type>::param_type value)
    {
      resultCode = S1_SUCCESS;
      val = value;
      return *this;
    }
    Result& operator= (value_type&& value)
    {
      resultCode = S1_SUCCESS;
      val = std::move (value);
      return *this;
    }
    //@}

    /// Get result code.
    ResultCode code () const { return resultCode;  }

    //@{
    /**
     * Get contained value or a default value.
     */
  #ifndef BOOST_NO_CXX11_REF_QUALIFIERS
    template <class U>
    value_type value_or (U&& v) const&
    { 
      return val.value_or (std::forward<U>(v));
    }

    template <class U>
    value_type value_or (U&& v) && 
    { 
      return val.value_or (std::forward<U>(v));
    }
  #else
    template <class U>
    value_type value_or ( U&& v ) const 
    {
      return val.value_or (std::forward<U>(v));
    }
  #endif
    const value_type& get_value_or (const value_type& v) const { return val.get_value_or (v); }
    value_type& get_value_or (value_type& v) { return val.get_value_or (v); }
    //@}
  };

} // namespace s1

#endif // __BASE_RESULT_H__
