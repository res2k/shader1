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

#include "ResultCode_internal.h"

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
    /// Error info
    boost::optional<std::string> resultErrorInfo;
    /// Return value
    boost::optional<T> val;
  public:
    typedef T value_type;

    /**
     * Construct from error code.
     */
    Result (ResultCode resultCode, const char* errorInfo = nullptr) : resultCode (resultCode)
    {
      if (errorInfo) resultErrorInfo.emplace (errorInfo);
    }
    //@{
    /**
    * Default constructor with value and optional result code.
    * Result code default to #S1_SUCCESS.
    */
    template <class U>
    Result (U&& value, ResultCode resultCode = S1_SUCCESS)
      : resultCode (resultCode), val (std::forward<U> (value)) {}
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
    template <class U>
    Result& operator= (U&& value)
    {
      resultCode = S1_SUCCESS;
      val = std::forward<U> (value);
      return *this;
    }
    //@}

    /// Get result code.
    ResultCode code () const { return resultCode;  }
    /// Get optional error info.
    const char* errorInfo () const { return resultErrorInfo ? resultErrorInfo->c_str () : nullptr; }

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

    /// Apply a "filter" functor to this result's value, if it contains one
    template<typename F>
    Result<typename std::result_of<F (T)>::type> filter (F func)
    {
      typedef Result<typename std::result_of<F (T)>::type> result_type;
      if (val)
        return result_type (func (*val), resultCode);
      else if (resultErrorInfo)
        return result_type (resultCode, resultErrorInfo->c_str());
      else
        return result_type (resultCode);
    }
  };

} // namespace s1

#endif // __BASE_RESULT_H__
